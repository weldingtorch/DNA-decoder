#include <stdio.h>

char convertToAA(char *nuk)
{
    switch (nuk[0])
    {
    case 'A':
        switch (nuk[1])
        {
        case 'A':
            switch (nuk[2])
            {
            case 'A':
                return 'K';

            case 'C':
                return 'N';

            case 'G':
                return 'K';

            case 'T':
                return 'N';
            }

        case 'C':
            return 'T';

        case 'G':
            switch (nuk[2])
            {
            case 'A':
                return 'R';

            case 'C':
                return 'S';

            case 'G':
                return 'R';

            case 'T':
                return 'S';
            }

        case 'T':
            switch (nuk[2])
            {
            case 'A':
                return 'I';

            case 'C':
                return 'I';

            case 'G':
                return 'M';

            case 'T':
                return 'I';
            }
        }

    case 'C':
        switch (nuk[1])
        {
        case 'A':
            switch (nuk[2])
            {
            case 'A':
                return 'O';

            case 'C':
                return 'H';

            case 'G':
                return 'O';

            case 'T':
                return 'H';
            }

        case 'C':
            return 'P';

        case 'G':
            return 'R';

        case 'T':
            return 'L';
        }

    case 'G':
        switch (nuk[1])
        {
        case 'A':
            switch (nuk[2])
            {
            case 'A':
                return 'E';

            case 'C':
                return 'D';

            case 'G':
                return 'E';

            case 'T':
                return 'D';
            }

        case 'C':
            return 'A';

        case 'G':
            return 'G';

        case 'T':
            return 'V';
        }

    case 'T':
        switch (nuk[1])
        {
        case 'A':
            switch (nuk[2])
            {
            case 'A':
                return '-';

            case 'C':
                return 'Y';

            case 'G':
                return '-';

            case 'T':
                return 'Y';
            }

        case 'C':
            return 'S';

        case 'G':
            switch (nuk[2])
            {
            case 'A':
                return '-';

            case 'C':
                return 'C';

            case 'G':
                return 'W';

            case 'T':
                return 'C';
            }

        case 'T':
            switch (nuk[2])
            {
            case 'A':
                return 'L';

            case 'C':
                return 'F';

            case 'G':
                return 'L';

            case 'T':
                return 'F';
            }
        }

    default:
        return '?';
    }
}

void readNuk(char *nuk, FILE *inpfp)
{
    while (1)
    {
        nuk[0] = fgetc(inpfp);
        if (nuk[0] != '\n') // Reread if CR+LF
            break; 
    }
}

int main(void)
{
    FILE *inpfp;
    FILE *outfp;
    char nuk[3];
    char writingProtein = 0;
    int selector = 0;

    inpfp = fopen("./data/sequence.fasta", "r");
    outfp = fopen("./data/sequence.aa", "w");

    while (fgetc(inpfp) != '\n'); // Skip header

    while (1) // Search for ATG
    {   
        readNuk(nuk, inpfp);
        if (feof(inpfp)) break;
        
        if (nuk[0] == 'A')
        {
            readNuk(nuk+1, inpfp);
            if (nuk[1] == 'T')
            {
                readNuk(nuk+2, inpfp);
                if (nuk[2] == 'G')
                {
                    break;
                }
                else
                {
                    ungetc(nuk[2], inpfp);
                    ungetc(nuk[1], inpfp);
                }
            }
            else
            {
                ungetc(nuk[1], inpfp);
            } 
        }
        selector++;
    }

    printf("Start: %d\n", selector);

    while (!feof(inpfp))
    {
        // Triplet reading
        
        nuk[0] = convertToAA(nuk);

        if (nuk[0] == 'M')
            writingProtein = 1;
        else if (nuk[0] == '-')
        {
            if (writingProtein)
                fprintf(outfp, "\n");
                printf("End: %d\n", selector);
                break;
            writingProtein = 0;
        }
        if (writingProtein)
            fprintf(outfp, "%c", nuk[0]);
        
        for (int i = 0; i < 3; i++)
        {
            do {
                nuk[i] = fgetc(inpfp);
            } while (nuk[i] == '\n');  // Reread if CR+LF
        }
        
        selector += 3;  
    }
    
    fclose(inpfp);
    fclose(outfp);
    printf("Done!\n");
    
    return 0;
}
