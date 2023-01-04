#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include/lexer.h"
#include "include/main.h"

int main()
{
    char filepath[100];
    int len;
    printf("Input file path: ");
    scanf("%s", filepath);
    len = strlen(filepath);
    int checker = 0;

    if (len <= 1)
    {
        printf("Invalid file path\n");
    }
    if (filepath[len - 1] == 's' && filepath[len - 2] == '.')
    { // check file extension
        inputptr = fopen(filepath, "r");
        checker = 1;
    }
    else if (len > 1 && checker != 1)
    {
        printf("Invalid file extension\n.\n.\nProgram will now close");
    }

    if (inputptr == NULL)
    {
        printf("\nThe file is empty");
    }

    char contents[1000];
    outputptr = fopen("examples/SymbolTable.txt", "w");
    while (fgets(contents, sizeof(contents), inputptr) != NULL)
    {
        strcat(program, contents);
    }

    myFunction();
    fclose(inputptr);
    fclose(outputptr);
    return 0;
}
