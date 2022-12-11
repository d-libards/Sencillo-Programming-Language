#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include/lexer.h"
#include "include/main.h"
//#include "lexer.c"

int main()
{
    char program[300];
    printf("Program: ");
    fgets(program,300,stdin);
    printf("%s\n", program);
    myFunction();
    return 0;
}
