/* André Luiz Kovalski

# TUDO NO ARQUIVO DE INPUT DEVE SER SEPARADO POR ESPAÇOS

Para obter os pontos relativos a este trabalho, você deverá fazer um programa,
usando a linguagem de programação que desejar, que seja capaz de validar
expressões de lógica propisicional escritas em latex e definir se são expressões
gramaticalmente corretas. Você validará apenas a forma da expressão (sintaxe).

A entrada será fornecida por um arquivo de textos que será carregado em linha de
comando, com a seguinte formatação:

    1. Na primeira linha deste arquivo existe um número inteiro que informa
quantas expressões lógicas estão no arquivo.

    2. Cada uma das linhas seguintes contém uma expressão lógica que deve ser
validada. A saída do seu programa será no terminal padrão do sistema e
constituirá de uma linha de saída para cada expressão lógica de entrada contendo
ou a palavra valida ou a palavra inválida e nada mais.

Gramática:
Formula=Constante|Proposicao|FormulaUnaria|FormulaBinaria.
Constante="T"|"F".
Proposicao=[a−z0−9]+
FormulaUnaria=AbreParen OperadorUnario Formula FechaParen
FormulaBinaria=AbreParen OperatorBinario Formula Formula FechaParen
AbreParen="("
FechaParen=")"
OperatorUnario="¬"
OperatorBinario="∨"|"∧"|"→"|"↔"

Cada expressão lógica avaliada pode ter qualquer combinação das operações de
negação, conjunção, disjunção, implicação e bi-implicação sem limites na
combiação de preposições e operações.

Os valores lógicos True e False estão representados na gramática e, como tal,
podem ser usados em qualquer expressão de entrada.

Para validar seu trabalho, você deve incluir no repl.it, no mínimo três arquivos
contendo números diferentes de expressões proposicionais. O professor irá
incluir um arquivo de testes extra para validar seu trabalho. Para isso, caberá
ao professor incluir o arquivo no seu repl.it e rodar o seu programa carregando
o arquivo de testes.
 */

#include <stdio.h>
#include <string.h>

#define stringMaxSize 512
static char *operators[9] = {
    "T", "F", "\\neg", "\\land", "\\lor", "\\rightarrow", "\\leftrightarrow",
    "(", ")"};
static int opQuantity = sizeof(operators) / sizeof(operators[0]);

typedef enum
{
    false = 0,
    true = 1
} bool;

typedef enum
{
    invalid = -1,
    null = 0,
    constant = 1,
    proposition = 2,
    unary = 3,
    binary = 4,
    lParentheses = 5,
    rParentheses = 6
} parseState;

int selectFile();
bool isSimpleFormula(parseState state);
bool checkValidation(parseState stateHistory[], int index, int parCount);

int main()
{
    FILE *inputFile;
    int LineNumber;
    char readString[stringMaxSize];

    char fileName[50];
    parseState state = null;

    printf("a) input1.txt\n");
    printf("b) custom.txt (Teacher's file)\n");

    int selection = selectFile();

    if (selection == -1)
    {
        printf("Invalid selection!\n");
        return -1;
    }

    if (selection == 2)
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
        parseState *stateHistory = malloc(100 * sizeof(strlen(readString)));
        int historyCounter = 0;
        char *token = strtok(readString, " ");
        while (token != NULL)
        {

            int iterator, contains = 0;
            token[strcspn(token, "\n")] = 0;
            for (iterator = 0; iterator < opQuantity; iterator++)
            {
                contains = strcmp(token, operators[iterator]);
                if (contains == 0)
                {
                    switch (iterator)
                    {
                    case (0):
                    case (1):
                        state = constant;
                        break;

                    case (2):
                        state = unary;
                        break;

                    case (3):
                    case (4):
                    case (5):
                    case (6):
                        state = binary;
                        break;

                    case (7):
                        state = lParentheses;
                        break;

                    case (8):
                        state = rParentheses;
                        break;

                    default:
                        state = invalid;
                        break;
                    }
                    break;
                }
            }
            if (contains != 0)
            {
                state = proposition;
            }
            stateHistory[historyCounter] = state;
            historyCounter++;
            token = strtok(NULL, " ");
        }

        // for (int i = 0; i < historyCounter; i++)
        // {
        //     printf("%d ", stateHistory[i]);
        // }

        if (checkValidation(stateHistory, 0, 0))
        {
            printf("- válido!");
        }
        else
        {
            printf("- inválido!");
        }

        printf("\n");
        fgets(readString, stringMaxSize, inputFile);
    }

    fclose(inputFile);
    printf("\n");
    return 0;
}

bool checkValidation(parseState stateHistory[], int index, int parCount)
{
    if (stateHistory[index] == 0)
    {
        // printf("error on %d (i:%d, par:%d)! ", stateHistory[index], index,
        // parCount);
        return false;
    }
    if (stateHistory[index] == 1 && stateHistory[index + 1] == 0 && index == 0)
    {
        return true;
    }
    if ((stateHistory[index] == 1 || stateHistory[index] == 2) && parCount > 0)
    {
        if (stateHistory[index - 1] == 1 || stateHistory[index - 1] == 2)
        {
            return false;
        }
        return checkValidation(stateHistory, index + 1, parCount);
    }
    if (stateHistory[index] == 3)
    {
        if (stateHistory[index + 1] == 5)
        {
            return checkValidation(stateHistory, index + 1, parCount);
        }
        if (stateHistory[index + 1] == 1 || stateHistory[index + 1] == 2)
        {
            return checkValidation(stateHistory, index + 2, parCount);
        }
    }
    if (stateHistory[index] == 4)
    {
        if (stateHistory[index + 1] == 5)
        {
            return checkValidation(stateHistory, index + 1, parCount);
        }
        if (stateHistory[index + 2] == 5)
        {
            return checkValidation(stateHistory, index + 2, parCount);
        }
        if (isSimpleFormula(stateHistory[index + 1]) &&
            isSimpleFormula(stateHistory[index + 2]))
        {
            return checkValidation(stateHistory, index + 3, parCount);
        }
    }
    if (stateHistory[index] == 5 && stateHistory[index + 1] != 6)
    {
        parCount++;
        return checkValidation(stateHistory, index + 1, parCount);
    }
    if (stateHistory[index] == 6)
    {
        parCount--;
        if (stateHistory[index + 1] == 0 && parCount == 0)
        {
            return true;
        }
        return checkValidation(stateHistory, index + 1, parCount);
    }
    // printf("not on %d (i:%d, par:%d)! ", stateHistory[index], index, parCount);
    return false;
}

bool isSimpleFormula(parseState state)
{
    if (state >= 1 || state <= 4)
    {
        return true;
    }
    return false;
}

int selectFile()
{
    char option;
    scanf("%c", &option);
    int selection = option - 96;

    if (selection > 2 || selection < 1 && (selection != 2 || selection != 1))
    {
        return -1;
    }
    return selection;
}