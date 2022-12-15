#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include/lexer.h"
#include "include/main.h"

int main()
{
    char contents [1000];
    inputptr = fopen("examples/example.c", "r");
    outputptr = fopen("examples/SymbolTable.txt","w");
    if (inputptr == NULL){
        perror("Unable to open the file");
    }
    while(fgets(contents,sizeof(contents), inputptr) != NULL){
        strcat(program,contents);
    }

    myFunction();
    fclose(inputptr);
    fclose(outputptr);
    return 0;
}

