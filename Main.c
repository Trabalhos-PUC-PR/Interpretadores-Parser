
// André Luiz Kovalski
#include <stdio.h>
#include <string.h>

#define stringMaxSize 512
static char *operators[9] = {"T", "F", "\\rightarrow", "\\land", "\\lor", "\\rightarrow", "\\leftrightarrow", "(", ")"};
static int opQuantity = sizeof(operators) / sizeof(operators[0]);

typedef enum
{
    false = 0,
    true = 1
} bool;

typedef enum
{
    constant,
    unary,
    binary,
    parentheses
} state;

int selectFile();

int main()
{
    FILE *inputFile;
    int LineNumber;
    char readString[stringMaxSize];

    char fileName[50];

    printf("a) input1.txt\n");
    printf("b) input2.txt\n");
    printf("c) input3.txt\n");
    printf("d) custom.txt (Teacher's file)\n");

    int selection = selectFile();

    if (selection == -1)
    {
        printf("Invalid selection!\n");
        return -1;
    }

    if (selection == 4)
    {
        strcpy(fileName, "custom.txt");
    }
    else
    {
        char strSelect[10];
        strcpy(fileName, "input");
        sprintf(strSelect, "%d", selection);
        strcat(fileName, strSelect);
        strcat(fileName, ".txt");
    }

    inputFile = fopen(fileName, "r");

    if (inputFile == NULL)
    {
        printf("No file names \"%s\" found!", fileName);
        return -1;
    }

    fgets(readString, stringMaxSize, inputFile);
    sscanf(readString, "%d", &LineNumber);

    for (int i = -1; i < LineNumber; i++)
    {
        if (i == -1)
        {
            fgets(readString, stringMaxSize, inputFile);
            continue;
        }
        // split, splits strings using provided token
        char *token = strtok(readString, " ");
        while (token != NULL)
        {
            int iterator, contains = 0;
            token[strcspn(token, "\n")] = 0;
            for (iterator = 0; iterator < opQuantity; iterator++)
            {
                // char caracter[2] = "\0";
                // caracter[0] = operators[iterator];
                contains = strcmp(token, operators[iterator]);
                if (contains == 0)
                {
                    printf("token '%s' | op[%d] '%s' contains: %d ", token, iterator, operators[iterator], contains);
                    break;
                }
            }
            printf("is inside? %d\n", contains);
            token = strtok(NULL, " ");
        }
        printf("NOVA LINHA\n\n\n");
        fgets(readString, stringMaxSize, inputFile);
    }

    fclose(inputFile);
    printf("\n");
    return 0;
}

int selectFile()
{
    char option;
    scanf("%c", &option);
    int selection = option - 96;

    if (selection > 4 || selection < 1)
    {
        return -1;
    }
    return selection;
}