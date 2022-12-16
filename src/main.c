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
    scanf("%s",filepath);
    len = strlen(filepath);
    if (len < 1) {
        printf("Invalid file path");
        return 0;
    }
    if(filepath[len-1] != 's' && filepath[len-2] != '.'){
        printf("Invalid file extension\n.\n.\nProgram will now close");
        return 0;
    }else{
        inputptr = fopen(filepath, "r");
    }

    char contents [1000];

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

