#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include/lexer.h"
#include "include/main.h"

int main()
{
    inputptr = fopen("examples/example.c", "r");
    outputptr = fopen("examples/SymbolTable.txt","w");
    if (inputptr == NULL){
        perror("Unable to open the file");
    }
   while(fgets(program,sizeof(program), inputptr) != NULL){
        myFunction();
   }

    fclose(inputptr);
    fclose(outputptr);
    return 0;
}
