#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lexer.c"
#include "include/main.h"

int main()
{
    printf("Program: ");
    fgets(program,300,stdin);
    printf("%s\n", program);
    myFunction();

    return 0;
}