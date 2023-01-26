#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void fileChecker(char str[]);
void getLexeme();
void getToken();
void printToken();
void parse();
bool isSeparator(char ch);
bool isOperator(char ch);
void identifyOperator(char *subStr);
void identifyDelimiter(char ch);
bool isIdentifierElement(char ch);
bool isKeyword(char *subStr);
bool isReservedword(char *subStr);
bool isNoiseword(char *subStr);
bool isIntegerLiteral(char *str);
bool isFloatLiteral(char *str);
bool isIdentifier(char *str);

/* parser methods */

void stmt();
void scan_stmt();
void print_stmt();
void ident_symbol();
void declaration_stmt();
void assign_stmt();
void expr();
void assign_op();
void iterative_stmt();
void condition();
void increment();
void rel_expr();
void rel_op();
void bool_expr();
void bool_term();
void bool_factor();
void conditional_stmt();
void arith_expr();
void arith_term();
void arith_power();
void arith_factor();

void error();

char *token;
FILE *outputptr;
FILE *inputptr;
FILE *fptr;

int i;
char ch1 = ' ';
char ch2 = ' ';
char comment[1000];
char charLit[100];
char strLit[100];
char operators[100];
char string[1000];
int strIndex = 0;
char currentLexeme[100];
char currentToken[20];

int lineNo = 1;

int main()
{
    char filepath[100];
    printf("Input filepath: ");
    scanf("%s", filepath);
    fileChecker(filepath);

    if (inputptr == NULL)
        return 0;

    getLexeme();

    fclose(outputptr);
    fclose(inputptr);

    // parse
    fptr = fopen("SymbolTable.txt", "r");
    if (fptr == NULL)
        printf("Error! opening file");

    parse();

    fclose(fptr);

    return (0);
}

void fileChecker(char str[])
{
    int len = strlen(str);
    int checker = 0;

    // check if the file path have proper length
    if (len <= 1)
        printf("Invalid file path\n");
    // check file extension
    else if (str[len - 1] == 's' && str[len - 2] == '.')
    {
        inputptr = fopen(str, "r");
        outputptr = fopen("SymbolTable.txt", "w");
        checker = 1;
        if (inputptr == NULL)
            printf("\nFile doesn't exist\n");
    }
    else if (len > 1 && checker != 1)
        printf("Invalid file extension\n.\n.\nProgram will now close");
}

void getLexeme()
{

    do
    {
        // comment
        if (ch1 == '/' && ch2 == '*')
        {
            i = 0;
            ch1 = fgetc(inputptr);
            ch2 = fgetc(inputptr);

            while (!feof(inputptr))
            {
                if (ch1 == '*' && ch2 == '/')
                {
                    comment[i] = '\0';
                    fprintf(outputptr, "%s\t\t\t\t\t\t\t\tcomment\n", comment);
                    // printf("%s comment\n", comment);
                    //  next character
                    ch1 = fgetc(inputptr);
                    ch2 = fgetc(inputptr);
                    break;
                }
                else
                {
                    comment[i] = ch1;
                    i++;
                    ch1 = ch2;
                    ch2 = fgetc(inputptr);
                }
            }
        }

        // char lit
        else if (ch1 == '\'')
        {
            ch1 = ch2;
            ch2 = fgetc(inputptr);

            if (ch1 == '\'')
            {
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\tinvalid\n", comment);
                // printf("! invalid empty\n");
                ch1 = ch2;
                ch2 = fgetc(inputptr);
            }
            else if (ch1 == '\\' && ch2 == '\'')
            {
                fprintf(outputptr, "%c\t\t\t\t\t\t\t\tchar_lit\n", ch2);
                // printf("%c \tchar lit\n", ch2);
                fgetc(inputptr);
                ch1 = fgetc(inputptr);
                ch2 = fgetc(inputptr);
            }
            else if (ch1 == '\\' && ch2 == '\\')
            {
                fprintf(outputptr, "%c\t\t\t\t\t\t\t\tchar_lit\n", ch2);
                // printf("%c \tchar lit\n", ch2);
                fgetc(inputptr);
                ch1 = fgetc(inputptr);
                ch2 = fgetc(inputptr);
            }
            else if (ch2 != '\'')
            {
                i = 0;
                charLit[i] = ch1;
                i++;
                charLit[i] = ch2;
                i++;
                ch1 = fgetc(inputptr);

                while (!feof(inputptr))
                {
                    if (ch1 == '\'')
                    {
                        charLit[i] = '\0';
                        fprintf(outputptr, "%s\t\t\t\t\t\t\t\tinvalid\n", charLit);
                        // printf("%s \tinvalid char lit\n", charLit);
                        ch1 = fgetc(inputptr);
                        ch2 = fgetc(inputptr);
                        break;
                    }
                    else
                    {
                        charLit[i] = ch1;
                        i++;
                        ch1 = fgetc(inputptr);
                    }
                }
            }
            else
            {
                fprintf(outputptr, "%c\t\t\t\t\t\t\t\tchar_lit\n", ch1);
                // printf("%c \t char lit\n", ch1);
                ch1 = fgetc(inputptr);
                ch2 = fgetc(inputptr);
            }
        }

        // string lit
        else if (ch1 == '\"')
        {
            ch1 = ch2;
            i = 0;
            while (!feof(inputptr))
            {
                ch2 = fgetc(inputptr);

                if (ch1 == '\"')
                {
                    strLit[i] = '\0';
                    ch1 = ch2;
                    ch2 = fgetc(inputptr);
                    fprintf(outputptr, "%s\t\t\t\t\t\t\t\tstr_lit\n", strLit);
                    // printf("%s \tstring\n", strLit);
                    break;
                }
                /*
                else if (ch1 == '%' && (ch2 == 'd' || ch2 == 'c' || ch2 == 's' || ch2 == 'f')){
                    fprintf(outputptr, "\"\t\t\t\t\t\t\t\t\"\n");
                    fprintf(outputptr, "%c%c\t\t\t\t\t\t\t\tformat_specs\n", ch1,ch2);
                    ch1 = fgetc(inputptr);
                    fprintf(outputptr, "%c\t\t\t\t\t\t\t\t\"\n", ch1);
                    ch1 = fgetc(inputptr);
                    ch2 = fgetc(inputptr);
                    break;
                }
                */
                else if (ch1 == '\\' && ch2 == '\"')
                {
                    strLit[i] = '\"';
                    i++;
                    ch1 = fgetc(inputptr);
                }
                else if (ch1 == '\\' && ch2 == '\\')
                {
                    strLit[i] = '\\';
                    i++;
                    ch1 = fgetc(inputptr);
                }
                else
                {
                    strLit[i] = ch1;
                    i++;
                    ch1 = ch2;
                }
            }
        }

        // special character
        else if (ch1 == '&')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\tspecial_char\n", ch1);
            // printf("%c \tformat\n",ch1);
            ch1 = ch2;
            ch2 = fgetc(inputptr);
        }
        // format specifier
        else if (ch1 == '%' && (ch2 == 'd' || ch2 == 'c' || ch2 == 's' || ch2 == 'f'))
        {
            fprintf(outputptr, "%c%c\t\t\t\t\t\t\t\t%c%c\n", ch1, ch2, ch1, ch2);
            ch1 = fgetc(inputptr);
            ch2 = fgetc(inputptr);
        }
        else if (isSeparator(ch1) == false)
        {
            string[strIndex] = ch1;
            strIndex++;
            ch1 = ch2;
            ch2 = fgetc(inputptr);
        }
        else if (isSeparator(ch1) == true && strIndex == 0)
        {
            if (ch1 == '\n')
            {
                fprintf(outputptr, "\n");
            }
            if (isOperator(ch1))
            {
                i = 0;
                while (isOperator(ch1))
                {
                    operators[i] = ch1;
                    i++;
                    ch1 = ch2;
                    ch2 = fgetc(inputptr);
                }
                operators[i] = '\0';
                identifyOperator(operators);
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\t%s\n", operators, token);
                // printf("%s \t%s\n",operators,token);
            }
            else
            {
                if (ch1 != ' ' && ch1 != '\n')
                {
                    identifyDelimiter(ch1);
                    fprintf(outputptr, "%c\t\t\t\t\t\t\t\t%s\n", ch1, token);
                    // printf("%c \t%s\n",ch1,token);
                }
                ch1 = ch2;
                ch2 = fgetc(inputptr);
            }
        }
        else if (isSeparator(ch1) == true && strIndex > 0)
        {
            string[strIndex] = '\0';

            if (isKeyword(string) == true)
            {
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\t%s\n", string, token);
                // printf("%s \t%s\n", string, token);
            }
            else if (isReservedword(string) == true)
            {
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\t%s\n", string, token);
                // printf("%s \t\t%s\n", string, token);
            }
            else if (isNoiseword(string) == true)
            {
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\t%s\n", string, token);
                // printf("%s \t\t%s\n", string, token);
            }
            else if (isIntegerLiteral(string) == true)
            {
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\t%s\n", string, token);
                // printf("%s \t\t%s\n", string, token);
            }
            else if (isFloatLiteral(string) == true)
            {
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\t%s\n", string, token);
                // printf("%s \t\t%s\n", string, token);
            }
            else if (isIdentifier(string) == true)
            {
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\t%s\n", string, token);
                // printf("%s \t\t%s\n", string, token);
            }
            else
            {
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\t! unrecognized input\n", string);
                // printf("%s \t\t! unrecognized input\n", string);
            }

            strIndex = 0;
        }
        else
        {
            ch1 = ch2;
            ch2 = fgetc(inputptr);
        }

    } while (!feof(inputptr));
}

void parse()
{

    getToken();
    do
    {
        stmt();

    } while (!feof(fptr));
}

void getToken()
{
    char ch = fgetc(fptr);
    int index = 0;

    if (ch == '\n')
    {
        lineNo++;
        // printf("Line %d\n", lineNo);
        ch = fgetc(fptr);
        while (ch == '\n')
        {
            lineNo++;
            ch = fgetc(fptr);
        }
    }
    // gets the lexeme
    while (ch != '\t')
    {
        currentLexeme[index] = ch;
        index++;
        ch = fgetc(fptr);
    }
    currentLexeme[index] = '\0';

    // skips all \t
    while (ch == '\t')
    {
        ch = fgetc(fptr);
    }

    // gets the token
    index = 0;
    while (ch != '\n')
    {
        currentToken[index] = ch;
        index++;
        ch = fgetc(fptr);
    }
    currentToken[index] = '\0';
    if (((currentToken[0] == 'c') && (currentToken[1] == 'o' && currentToken[2] == 'm' && currentToken[3] == 'm' && currentToken[4] == 'e' && currentToken[5] == 'n' && currentToken[6] == 't')) == true)
    {
        getToken();
    } /*
     else{
         printf("Lexeme: %s\t", currentLexeme);
         printf("Token: %s\n", currentToken);
     } */
}

void printToken()
{
    printf("%s ", currentToken);
}
void stmt()
{
    scan_stmt();
    print_stmt();
    declaration_stmt();
    assign_stmt();
    iterative_stmt();
    conditional_stmt();
}

void scan_stmt()
{
    if ((currentToken[0] == 's' && currentToken[1] == 'c' && currentToken[2] == 'a' && currentToken[3] == 'n' && currentToken[4] == 'f' && currentToken[5] == '_' && currentToken[6] == 'k' && currentToken[7] == 'e' && currentToken[8] == 'y' && currentToken[9] == 'w' && currentToken[10] == 'o' && currentToken[11] == 'r' && currentToken[12] == 'd') != true)
    { // escan
        return;
    }
    else
    {
        printf("\nEnter <scan_stmt>\n");
        printToken();
        getToken();
        if ((currentToken[0] == '(') != true) // (//escan(%d, &)
            error();
        else
        {
            printToken();
            getToken();

            if (((currentToken[0] == '%' && currentToken[1] == 'd') != true) &&
                ((currentToken[0] == '%' && currentToken[1] == 'f') != true) &&
                ((currentToken[0] == '%' && currentToken[1] == 'c') != true) &&
                ((currentToken[0] == '%' && currentToken[1] == 's') != true))
                error();
            else
            {
                printToken();
                getToken();
                if ((currentToken[0] == ',') != true) // ,
                {
                    error();
                }
                else
                {
                    printToken();
                    getToken();
                    if ((currentToken[0] == 's' && currentToken[1] == 'p' && currentToken[2] == 'e' && currentToken[3] == 'c' && currentToken[4] == 'i' && currentToken[5] == 'a' && currentToken[6] == 'l' && currentToken[7] == '_' && currentToken[8] == 'c' && currentToken[9] == 'h' && currentToken[10] == 'a' && currentToken[11] == 'r') != true) // &
                        error();
                    else
                    {
                        printToken();
                        getToken();
                        if ((currentToken[0] == 'i' && currentToken[1] == 'd') != true) // id
                            error();
                        else
                        {
                            printToken();
                            getToken();
                            if ((currentToken[0] == ')') != true)
                                error();
                            else
                            {
                                printToken();
                                getToken();
                                if ((currentToken[0] == ':') != true)
                                    error();
                                else
                                {
                                    printToken();
                                    printf("\nExit <scan_stmt>\n");
                                    getToken();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void print_stmt()
{

    if ((currentToken[0] == 'p' && currentToken[1] == 'r' && currentToken[2] == 'i' && currentToken[3] == 'n' && currentToken[4] == 't' && currentToken[5] == 'f' && currentToken[6] == '_' && currentToken[7] == 'k' && currentToken[8] == 'e' && currentToken[9] == 'y' && currentToken[10] == 'w' && currentToken[11] == 'o' && currentToken[12] == 'r' && currentToken[13] == 'd') != true) // impri
        return;
    else
    {
        printf("\nEnter <print_stmt>\n");
        printToken();
        getToken();
        if ((currentToken[0] == '(') == true)
        { // (
            printToken();
            getToken();
            if (((currentToken[0] == 'c' && currentToken[1] == 'h' && currentToken[2] == 'a' && currentToken[3] == 'r' && currentToken[4] == '_' && currentToken[5] == 'l' && currentToken[6] == 'i' && currentToken[7] == 't') == true) ||
                ((currentToken[0] == 's' && currentToken[1] == 't' && currentToken[2] == 'r' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true) ||
                ((currentToken[0] == 'i' && currentToken[1] == 'n' && currentToken[2] == 't' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true) ||
                ((currentToken[0] == 'f' && currentToken[1] == 'l' && currentToken[2] == 'o' && currentToken[3] == 'a' && currentToken[4] == 't' && currentToken[5] == '_' && currentToken[6] == 'l' && currentToken[7] == 'i' && currentToken[8] == 't') == true))
            { // const
                printToken();
                getToken();
                if ((currentToken[0] == ')') == true)
                { // )
                    printToken();
                    getToken();
                    if ((currentToken[0] == ':') == true)
                    { // :
                        printToken();
                        printf("\nExit <print_stmt>\n");
                        getToken();
                    }
                    else
                        error();
                }
                else if ((currentToken[0] == ',') == true)
                { // impri("hi",
                    while ((currentToken[0] == ',') == true)
                    {
                        printToken();
                        getToken();
                        if (((currentToken[0] == '%' && currentToken[1] == 'd') == true) || ((currentToken[0] == '%' && currentToken[1] == 'f') == true) ||
                            ((currentToken[0] == '%' && currentToken[1] == 'c') == true) || ((currentToken[0] == '%' && currentToken[1] == 's') == true))
                        { // %d
                            printToken();
                            getToken();
                        }
                        else
                            break;
                    }
                    if ((currentToken[0] == 'i' && currentToken[1] == 'd') == true)
                    { // id
                        printToken();
                        getToken();
                        while ((currentToken[0] == ',') == true)
                        {
                            printToken();
                            getToken();
                            if ((currentToken[0] == 'i' && currentToken[1] == 'd') == true)
                            { // id
                                printToken();
                                getToken();
                            }
                            else
                                error();
                        }
                        if ((currentToken[0] == ')') == true)
                        { // )
                            printToken();
                            getToken();
                            if ((currentToken[0] == ':') == true)
                            { // :
                                printToken();
                                printf("\nExit <print_stmt>\n");
                                getToken();
                            }
                            else
                                error();
                        }
                        else
                            error();
                    }
                    else
                        error();
                }
                else
                    error();
            }
            else if (((currentToken[0] == '%' && currentToken[1] == 'd') == true) ||
                     ((currentToken[0] == '%' && currentToken[1] == 'f') == true) ||
                     ((currentToken[0] == '%' && currentToken[1] == 'c') == true) ||
                     ((currentToken[0] == '%' && currentToken[1] == 's') == true))
            { // %d
                printToken();
                getToken();
                if ((currentToken[0] == ',') == true)
                { // ,
                    printToken();
                    getToken();
                    if ((currentToken[0] == 'i' && currentToken[1] == 'd') == true)
                    { // id
                        printToken();
                        getToken();
                        if ((currentToken[0] == ')') == true)
                        { // )
                            printToken();
                            getToken();
                            if ((currentToken[0] == ':') == true)
                            { // :
                                printToken();
                                printf("\nExit <print_stmt>\n");
                                getToken();
                            }
                            else
                                error();
                        }
                        else
                            error();
                    }
                    else
                        error();
                }
                else
                    error();
            }
            else
            {
                error();
            }
        }
        else
            error();
    }
}

void declaration_stmt()
{

    if (((currentToken[0] == 'i' && currentToken[1] == 'n' && currentToken[2] == 't' && currentToken[3] == 'e' && currentToken[4] == 'g' && currentToken[5] == 'e' && currentToken[6] == 'r' && currentToken[7] == '_' && currentToken[8] == 'k' && currentToken[9] == 'e' && currentToken[10] == 'y' && currentToken[11] == 'w' && currentToken[12] == 'o' && currentToken[13] == 'r' && currentToken[14] == 'd') != true) &&
        ((currentToken[0] == 'b' && currentToken[1] == 'o' && currentToken[2] == 'o' && currentToken[3] == 'l' && currentToken[4] == 'e' && currentToken[5] == 'a' && currentToken[6] == 'n' && currentToken[7] == '_' && currentToken[8] == 'k' && currentToken[9] == 'e' && currentToken[10] == 'y' && currentToken[11] == 'w' && currentToken[12] == 'o' && currentToken[13] == 'r' && currentToken[14] == 'd') != true) &&
        ((currentToken[0] == 'f' && currentToken[1] == 'l' && currentToken[2] == 'o' && currentToken[3] == 'a' && currentToken[4] == 't' && currentToken[5] == '_' && currentToken[6] == 'k' && currentToken[7] == 'e' && currentToken[8] == 'y' && currentToken[9] == 'w' && currentToken[10] == 'o' && currentToken[11] == 'r' && currentToken[12] == 'd') != true) &&
        ((currentToken[0] == 'd' && currentToken[1] == 'o' && currentToken[2] == 'u' && currentToken[3] == 'b' && currentToken[4] == 'l' && currentToken[5] == 'e' && currentToken[6] == '_' && currentToken[7] == 'k' && currentToken[8] == 'e' && currentToken[9] == 'y' && currentToken[10] == 'w' && currentToken[11] == 'o' && currentToken[12] == 'r' && currentToken[13] == 'd') != true) &&
        ((currentToken[0] == 's' && currentToken[1] == 't' && currentToken[2] == 'r' && currentToken[3] == 'i' && currentToken[4] == 'n' && currentToken[5] == 'g' && currentToken[6] == '_' && currentToken[7] == 'k' && currentToken[8] == 'e' && currentToken[9] == 'y' && currentToken[10] == 'w' && currentToken[11] == 'o' && currentToken[12] == 'r' && currentToken[13] == 'd') != true) &&
        ((currentToken[0] == 'c' && currentToken[1] == 'h' && currentToken[2] == 'a' && currentToken[3] == 'r' && currentToken[4] == '_' && currentToken[5] == 'k' && currentToken[6] == 'e' && currentToken[7] == 'y' && currentToken[8] == 'w' && currentToken[9] == 'o' && currentToken[10] == 'r' && currentToken[11] == 'd') != true))
    {
        return;
    }
    else
    {
        printf("\nEnter <declaration_stmt>\n");
        printToken();
        getToken();
        if ((currentToken[0] == 'i' && currentToken[1] == 'd') == true)
        { // id
            printToken();
            getToken(); // ,
            while ((currentToken[0] == ',') == true)
            { // if <data-type><id> , ... (comma)
                printToken();
                getToken(); // id
                if ((currentToken[0] == 'i' && currentToken[1] == 'd') == true)
                {
                    printToken();
                    getToken(); // , or ass op, if ass op go to while
                    while (((currentToken[0] == '=') == true) || ((currentToken[0] == '+' && currentToken[1] == '=') == true) || ((currentToken[0] == '-' && currentToken[1] == '=') == true) ||
                           ((currentToken[0] == '*' && currentToken[1] == '=') == true) || ((currentToken[0] == '/' && currentToken[1] == '=') == true) || ((currentToken[0] == '%' && currentToken[1] == '=') == true) ||
                           ((currentToken[0] == '/' && currentToken[1] == '/' && currentToken[2] == '=') == true))

                    {
                        printToken();
                        getToken();
                        expr(); // id or const
                        while (((currentToken[0] == '+') == true) || ((currentToken[0] == '-') == true) || ((currentToken[0] == '*') == true) || ((currentToken[0] == '/') == true) || ((currentToken[0] == '%') == true) ||
                               ((currentToken[0] == '*' && currentToken[1] == '*') == true) || ((currentToken[0] == '/' && currentToken[1] == '/') == true))
                        {
                            printToken();
                            getToken(); // id or const
                            expr();
                        } // while END
                    }     // while END
                }
                else
                    error();
            } // while END
            while (((currentToken[0] == '=') == true) || ((currentToken[0] == '+' && currentToken[1] == '=') == true) || ((currentToken[0] == '-' && currentToken[1] == '=') == true) ||
                   ((currentToken[0] == '*' && currentToken[1] == '=') == true) || ((currentToken[0] == '/' && currentToken[1] == '=') == true) || ((currentToken[0] == '%' && currentToken[1] == '=') == true) ||
                   ((currentToken[0] == '/' && currentToken[1] == '/' && currentToken[2] == '=') == true))
            { // if <data-type><id><assignop>...
                printToken();
                getToken();
                expr();
                while (((currentToken[0] == '+') == true) || ((currentToken[0] == '-') == true) || ((currentToken[0] == '*') == true) || ((currentToken[0] == '/') == true) || ((currentToken[0] == '%') == true) ||
                       ((currentToken[0] == '*' && currentToken[1] == '*') == true) || ((currentToken[0] == '/' && currentToken[1] == '/') == true))
                {
                    printToken();
                    getToken();
                    expr();
                }
            } // while END
            if ((currentToken[0] == ':') == true)
            {
                printToken();
                printf("\nExit <declaration_stmt>\n");
                getToken();
            }
            else
                error();
        }
        else
            error();
    }
}

void assign_stmt()
{

    if ((currentToken[0] == 'i' && currentToken[1] == 'd') != true) // id
        return;
    else
    {
        printf("\nEnter <assign_stmt>\n");
        printToken();
        getToken();
        assign_op(); // op
        arith_expr();
        if ((currentToken[0] == ':') == true)
        { // : ; or none
            printToken();
            printf("\nExit <assign_stmt>\n");
            getToken();
        }
        else
        {
            printToken();
            printf("\nExit <assign_stmt>\n");
        }
    }
}

void assign_op()
{
    if (((currentToken[0] == '=') == true) || ((currentToken[0] == '+' && currentToken[1] == '=') == true) || ((currentToken[0] == '-' && currentToken[1] == '=') == true) ||
        ((currentToken[0] == '*' && currentToken[1] == '=') == true) || ((currentToken[0] == '/' && currentToken[1] == '=') == true) || ((currentToken[0] == '%' && currentToken[1] == '=') == true) ||
        ((currentToken[0] == '/' && currentToken[1] == '/' && currentToken[2] == '=') == true))
    {
        printToken();
        getToken();
    }

    else
        error();
}

void expr()
{
    if (((currentToken[0] == 'i' && currentToken[1] == 'd') == true) || ((currentToken[0] == 'i' && currentToken[1] == 'n' && currentToken[2] == 't' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true) ||
        ((currentToken[0] == 'f' && currentToken[1] == 'l' && currentToken[2] == 'o' && currentToken[3] == 'a' && currentToken[4] == 't' && currentToken[5] == '_' && currentToken[6] == 'l' && currentToken[7] == 'i' && currentToken[8] == 't') == true) ||
        ((currentToken[0] == 'c' && currentToken[1] == 'h' && currentToken[2] == 'a' && currentToken[3] == 'r' && currentToken[4] == '_' && currentToken[5] == 'l' && currentToken[6] == 'i' && currentToken[7] == 't') == true) ||
        ((currentToken[0] == 's' && currentToken[1] == 't' && currentToken[2] == 'r' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true))

    { // id const
        printToken();
        getToken();
    }
    else
        error();
}

void iterative_stmt()
{
    // por
    if ((currentToken[0] == 'f' && currentToken[1] == 'o' && currentToken[2] == 'r' && currentToken[3] == '_' && currentToken[4] == 'k' && currentToken[5] == 'e' && currentToken[6] == 'y' && currentToken[7] == 'w' && currentToken[8] == 'o' && currentToken[9] == 'r' && currentToken[10] == 'd') != true)
        return;

    else
    {
        printf("\nEnter <iterative_stmt>\n");
        printToken();
        getToken();
        if ((currentToken[0] == '(') == true)
        { // (
            printToken();
            getToken();
            assign_stmt();
            if ((currentToken[0] == ';') == true)
            {
                getToken();
                condition();
                if ((currentToken[0] == ';') == true)
                {
                    getToken();
                    increment();
                    if ((currentToken[0] == ')') == true)
                    { // )
                        printToken();
                        getToken();
                        if ((currentToken[0] == '{') == true)
                        {
                            printToken();
                            getToken();
                            while ((currentToken[0] == '}') != true)
                            {
                                stmt();
                            }
                            if ((currentToken[0] == '}') == true)
                            {
                                printToken();
                                printf("\nExit <iterative_stmt>\n");
                                getToken();
                            }
                            else
                                error();
                        }
                        else
                            error();
                    }
                    else
                        error();
                }
                else
                    error();
            }
            else
                error();
        }
        else
            error();
    }
}

void condition()
{
    printf("Enter <condition>\n");
    if ((currentToken[0] == 'i' && currentToken[1] == 'd') == true)
    {
        printToken();
        getToken();
        rel_op();
        if (((currentToken[0] == 'i' && currentToken[1] == 'd') == true) || ((currentToken[0] == 'i' && currentToken[1] == 'n' && currentToken[2] == 't' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true))
        {
            printToken();
            getToken();
        }
        else
            error();
    }
    else
        error();
    printf("\nExit <condition>\n");
}

void increment()
{
    printf("Enter <increment>\n");
    if ((currentToken[0] == 'i' && currentToken[1] == 'd') == true)

    {
        printToken();
        getToken();
        if (((currentToken[0] == '+' && currentToken[1] == '+') == true) || ((currentToken[0] == '-' && currentToken[1] == '-') == true))
        {
            printToken();
            getToken();
        }
        else
            error();
    }
    else
        error();
    printf("\nExit <increment>\n");
}

void rel_expr()
{
    printf("\nEnter <rel_expr>\n");
    if (((currentToken[0] == 'i' && currentToken[1] == 'd') == true) || ((currentToken[0] == 'i' && currentToken[1] == 'n' && currentToken[2] == 't' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true) ||
        ((currentToken[0] == 'f' && currentToken[1] == 'l' && currentToken[2] == 'o' && currentToken[3] == 'a' && currentToken[4] == 't' && currentToken[5] == '_' && currentToken[6] == 'l' && currentToken[7] == 'i' && currentToken[8] == 't') == true) ||
        ((currentToken[0] == 'c' && currentToken[1] == 'h' && currentToken[2] == 'a' && currentToken[3] == 'r' && currentToken[4] == '_' && currentToken[5] == 'l' && currentToken[6] == 'i' && currentToken[7] == 't') == true) ||
        ((currentToken[0] == 's' && currentToken[1] == 't' && currentToken[2] == 'r' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true))
    { // id cnst
        printToken();
        getToken();
        rel_op(); // op
        if (((currentToken[0] == 'i' && currentToken[1] == 'd') == true) || ((currentToken[0] == 'i' && currentToken[1] == 'n' && currentToken[2] == 't' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true) ||
            ((currentToken[0] == 'f' && currentToken[1] == 'l' && currentToken[2] == 'o' && currentToken[3] == 'a' && currentToken[4] == 't' && currentToken[5] == '_' && currentToken[6] == 'l' && currentToken[7] == 'i' && currentToken[8] == 't') == true) ||
            ((currentToken[0] == 'c' && currentToken[1] == 'h' && currentToken[2] == 'a' && currentToken[3] == 'r' && currentToken[4] == '_' && currentToken[5] == 'l' && currentToken[6] == 'i' && currentToken[7] == 't') == true) ||
            ((currentToken[0] == 's' && currentToken[1] == 't' && currentToken[2] == 'r' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true))
        {
            printToken();
            getToken();
        }
        else
            error();
    }
    else
        error();
    printf("\nExit <rel_expr>\n");
}

void rel_op()
{
    // printf("Enter <rel_op>\n");
    if (((currentToken[0] == '<') == true) || ((currentToken[0] == '>') == true) || ((currentToken[0] == '>' && currentToken[1] == '=') == true) ||
        ((currentToken[0] == '<' && currentToken[1] == '=') == true) || ((currentToken[0] == '=' && currentToken[1] == '=') == true) || ((currentToken[0] == '!' && currentToken[1] == '=') == true))
    {
        printToken();
        getToken();
    }

    else
    {
        error();
    }

    // printf("Exit <rel_op>\n");
}

void conditional_stmt()
{
    if ((currentToken[0] == 'i' && currentToken[1] == 'f' && currentToken[2] == '_' && currentToken[3] == 'k' && currentToken[4] == 'e' && currentToken[5] == 'y' && currentToken[6] == 'w' && currentToken[7] == 'o' && currentToken[8] == 'r' && currentToken[9] == 'd') != true)
    { // if
        return;
    }
    else
    {

        printf("\nEnter <conditional_stmt>\n");
        printToken();
        getToken();
        if ((currentToken[0] == '(') != true) // (
            error();
        else
        {
            printToken();
            getToken();
            bool_expr();
            if ((currentToken[0] == ')') != true) // )
                error();
            else
            {
                printToken();
                getToken();
                if ((currentToken[0] == '{') != true) // {
                    error();
                else
                {
                    printToken();
                    getToken();
                    while ((currentToken[0] == '}') != true) //}
                    {
                        stmt();
                    }
                    printToken();
                    getToken();
                    if ((currentToken[0] == 'e' && currentToken[1] == 'l' && currentToken[2] == 's' && currentToken[3] == 'e' && currentToken[4] == '_' && currentToken[5] == 'k' && currentToken[6] == 'e' && currentToken[7] == 'y' && currentToken[8] == 'w' && currentToken[9] == 'o' && currentToken[10] == 'r' && currentToken[11] == 'd') == true)
                    {
                        printToken();
                        getToken();
                        if ((currentToken[0] == '{') == true)
                        {
                            printToken();
                            getToken();
                            while ((currentToken[0] == '}') != true)
                            {
                                stmt();
                            }
                            printToken();
                            printf("\nExit <conditional_stmt>\n");
                            getToken();
                        }
                    }
                }
            }
        }
    }
}

void arith_expr()
{
    arith_term();
    while (((currentToken[0] == '+') || (currentToken[0] == '-')) == true)
    {
        printToken();
        getToken();
        arith_term();
    }
}
void arith_term()
{
    arith_power();
    while (((currentToken[0] == '*') || (currentToken[0] == '/') || (currentToken[0] == '%') ||
            (currentToken[0] == '/' && currentToken[1] == '/')) == true)
    {
        printToken();
        getToken();
        arith_power();
    }
}
void arith_power()
{
    arith_factor();
    while ((currentToken[0] == '*' && currentToken[1] == '*') == true)
    {
        printToken();
        getToken();
        arith_factor();
    }
}
void arith_factor()
{
    if (((currentToken[0] == 'i' && currentToken[1] == 'd') == true) || ((currentToken[0] == 'i' && currentToken[1] == 'n' && currentToken[2] == 't' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true) ||
        ((currentToken[0] == 'f' && currentToken[1] == 'l' && currentToken[2] == 'o' && currentToken[3] == 'a' && currentToken[4] == 't' && currentToken[5] == '_' && currentToken[6] == 'l' && currentToken[7] == 'i' && currentToken[8] == 't') == true) ||
        ((currentToken[0] == 'c' && currentToken[1] == 'h' && currentToken[2] == 'a' && currentToken[3] == 'r' && currentToken[4] == '_' && currentToken[5] == 'l' && currentToken[6] == 'i' && currentToken[7] == 't') == true) ||
        ((currentToken[0] == 's' && currentToken[1] == 't' && currentToken[2] == 'r' && currentToken[3] == '_' && currentToken[4] == 'l' && currentToken[5] == 'i' && currentToken[6] == 't') == true))
    { // id const
        printToken();
        getToken();
    }

    if ((currentToken[0] == '(') == true)
    {
        printToken();
        getToken();
        arith_expr();
        if ((currentToken[0] == ')') == true)
        {
            printToken();
            getToken();
        }
    }
}

void bool_expr()
{
    bool_term();
    while (((currentToken[0] == 'o' && currentToken[1] == 'r' && currentToken[2] == '_' && currentToken[3] == 'o' && currentToken[4] == 'p') == true))
    {
        printToken();
        getToken();
        bool_term();
    }
}
void bool_term()
{
    bool_factor();
    while (((currentToken[0] == 'a' && currentToken[1] == 'n' && currentToken[2] == 'd' && currentToken[3] == '_' && currentToken[4] == 'o' && currentToken[5] == 'p') == true))
    {
        printToken();
        getToken();
        bool_factor();
    }
}
void bool_factor()
{
    if (((currentToken[0] == 'n' && currentToken[1] == 'o' && currentToken[2] == 't' && currentToken[3] == '_' && currentToken[4] == 'o' && currentToken[5] == 'p') == true))
    {
        printToken();
        getToken();
        if ((currentToken[0] == 'i' && currentToken[1] == 'd') == true)
        {
            printToken();
            getToken();
        }
        else if ((currentToken[0] == '(') == true)
        {
            printToken();
            getToken();
            bool_expr();
            if ((currentToken[0] == ')') == true)
            {
                printToken();
                getToken();
            }
        }
    }
    else if ((currentToken[0] == '(') == true)
    {
        printToken();
        getToken();
        bool_expr();
        if ((currentToken[0] == ')') == true)
        {
            printToken();
            getToken();
        }
    }
    else
    {
        rel_expr();
    }
}

void ident_symbol()
{
    printf("Enter <ident_symbol>\n");
    if (((currentToken[0] == '%' && currentToken[1] == 'd') == true) || ((currentToken[0] == '%' && currentToken[1] == 'f') == true) ||
        ((currentToken[0] == '%' && currentToken[1] == 'c') == true) || ((currentToken[0] == '%' && currentToken[1] == 's') == true))
    {
        getToken();
    }
    else
        error();
    printf("Exit <ident_symbol>\n");
}

void error()
{
    printf("\n    ! error token: %s\n", currentToken);
    printf("    ! error in line: %d\n", lineNo);

    char char1 = fgetc(fptr);
    char char2 = fgetc(fptr);

    while (true)
    {
        if (char1 == '\n' && char2 == '\n')
            break;
        char1 = fgetc(fptr);
        char2 = fgetc(fptr);
    }
    lineNo++;
    getToken();
}

bool isSeparator(char ch)
{
    char separatorList[] = {' ', '+', '-', '*', '/', '%', '>', '<', '!', '=',
                            '[', ']', '{', '}', '(', ')', ':', ';', ',', '\n', '\0'};

    for (int i = 0; i < strlen(separatorList); i++)
    {
        if (separatorList[i] == ch)
            return (true);
    }

    return (false);
}

void identifyDelimiter(char ch)
{
    if (ch == '(')
    {
        token = "(";
    }
    else if (ch == ')')
    {
        token = ")";
    }
    else if (ch == '[')
    {
        token = "[";
    }
    else if (ch == ']')
    {
        token = "]";
    }
    else if (ch == '{')
    {
        token = "{";
    }
    else if (ch == '}')
    {
        token = "}";
    }
    else if (ch == ';')
    {
        token = ";";
    }
    else if (ch == ',')
    {
        token = ",";
    }
    else if (ch == ':')
    {
        token = ":";
    }
}

bool isOperator(char ch)
{
    char operatorList[] = {'=', '+', '-', '*', '/', '%', '!', '>', '<', '\0'};
    int i = 0;
    while (i < strlen(operatorList))
    {
        if (ch == operatorList[i])
        {
            return (true);
        }
        i++;
    }

    return (false);
}

void identifyOperator(char *subStr)
{
    // arithmetic operator

    if (subStr[0] == '+' && subStr[1] == '\0')
    {
        token = "+";
    }
    else if (subStr[0] == '-' && subStr[1] == '\0')
    {
        token = "-";
    }
    else if (subStr[0] == '/' && subStr[1] == '\0')
    {
        token = "/";
    }
    else if (subStr[0] == '*' && subStr[1] == '\0')
    {
        token = "*";
    }
    else if (subStr[0] == '%' && subStr[1] == '\0')
    {
        token = "%";
    }
    else if (subStr[0] == '*' && subStr[1] == '*' && subStr[2] == '\0')
    {
        token = "**";
    }
    else if (subStr[0] == '/' && subStr[1] == '/' && subStr[2] == '\0')
    {
        token = "//";
    }

    // assignment operator

    else if (subStr[0] == '=' && subStr[1] == '\0')
    {
        token = "=";
    }
    else if (subStr[0] == '+' && subStr[1] == '=' && subStr[2] == '\0')
    {
        token = "+=";
    }
    else if (subStr[0] == '-' && subStr[1] == '=' && subStr[2] == '\0')
    {
        token = "-=";
    }
    else if (subStr[0] == '*' && subStr[1] == '=' && subStr[2] == '\0')
    {
        token = "*=";
    }
    else if (subStr[0] == '%' && subStr[1] == '=' && subStr[2] == '\0')
    {
        token = "%=";
    }
    else if (subStr[0] == '/' && subStr[1] == '=' && subStr[2] == '\0')
    {
        token = "/=";
    }
    else if (subStr[0] == '/' && subStr[1] == '/' && subStr[2] == '=' && subStr[3] == '\0')
    {
        token = "//=";
    }

    // increment & decrement

    else if (subStr[0] == '+' && subStr[1] == '+' && subStr[2] == '\0')
    {
        token = "++";
    }
    else if (subStr[0] == '-' && subStr[1] == '-' && subStr[2] == '\0')
    {
        token = "--";
    }

    // boolean relational

    else if (subStr[0] == '=' && subStr[1] == '=' && subStr[2] == '\0')
    {
        token = "==";
    }
    else if (subStr[0] == '!' && subStr[1] == '=' && subStr[2] == '\0')
    {
        token = "!=";
    }
    else if (subStr[0] == '>' && subStr[1] == '=' && subStr[2] == '\0')
    {
        token = ">=";
    }
    else if (subStr[0] == '<' && subStr[1] == '=' && subStr[2] == '\0')
    {
        token = "<=";
    }
    else if (subStr[0] == '>' && subStr[1] == '\0')
    {
        token = ">";
    }
    else if (subStr[0] == '<' && subStr[1] == '\0')
    {
        token = "<";
    }
    else
    {
        token = "invalid";
    }
}

bool isIdentifier(char *str)
{
    // first checkpoint
    // no alphabet = not qualified to be an identifier
    int alphacount = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (isalpha(str[i]) != 0)
        {
            alphacount++;
        }
    }

    if (alphacount == 0)
    {
        return (false);
    }

    // second checkpoint
    // checks if each character in the str is an identifier element (a-zA-z,0-9,_)
    // if even 1 element is not an identifier element, then return true and tag it as an invalid id
    for (int i = 0; i < strlen(str); i++)
    {
        if (isIdentifierElement(str[i]) == false)
        {
            token = "invalid";
            return (true);
        }
    }

    // third checkpoint
    // checks if the first character is an alphabet
    // if not, then tag the str as an invalid identifier

    if (isalpha(str[0]) != 0)
    {
        token = "id";
        return (true);
    }
    token = "invalid";
    return (true);
}

bool isIdentifierElement(char ch)
{
    if (isalpha(ch) || isdigit(ch) || ch == '_')
    {
        return (true);
    }

    return (false);
}

bool isKeyword(char *subStr)
{
    if (subStr[0] == 'b' && subStr[1] == 'o' && subStr[2] == 'o' && subStr[3] == 'l' && subStr[4] == '\0')
    {
        token = "boolean_keyword";
    }
    else if (subStr[0] == 'c' && subStr[1] == 'u' && subStr[2] == 'e' && subStr[3] == 'r' && subStr[4] == '\0')
    {
        token = "string_keyword";
    }
    else if (subStr[0] == 'd' && subStr[1] == 'e' && subStr[2] == 'c' && subStr[3] == '\0')
    {
        token = "elif_keyword";
    }
    else if (subStr[0] == 'd' && subStr[1] == 'e' && subStr[2] == 's' && subStr[3] == 'c' &&
             subStr[4] == 'a' && subStr[5] == 'n' && subStr[6] == 's' && subStr[7] == 'o' && subStr[8] == '\0')
    {
        token = "break_keyword";
    }
    else if (subStr[0] == 'd' && subStr[1] == 'o' && subStr[2] == 'b' && subStr[3] == 'l' &&
             subStr[4] == 'e' && subStr[5] == '\0')
    { // doble
        token = "double_keyword";
    }
    else if (subStr[0] == 'e' && subStr[1] == 'n' && subStr[2] == 't' && subStr[3] == '\0')
    {
        token = "integer_keyword";
    }
    else if (subStr[0] == 'e' && subStr[1] == 's' && subStr[2] == 'c' && subStr[3] == 'a' && subStr[4] == 'n' &&
             subStr[5] == '\0')
    {
        token = "scanf_keyword";
    }
    else if (subStr[0] == 'f' && subStr[1] == 'l' && subStr[2] == 'o' && subStr[3] == 't' && subStr[4] == '\0')
    {
        token = "float_keyword";
    }
    else if (subStr[0] == 'h' && subStr[1] == 'a' && subStr[2] == 'c' && subStr[3] == 'e' &&
             subStr[4] == 'r' && subStr[5] == '\0')
    {
        token = "do_keyword";
    }
    else if (subStr[0] == 'i' && subStr[1] == 'm' && subStr[2] == 'p' && subStr[3] == 'r' && subStr[4] == 'i' &&
             subStr[5] == '\0')
    {
        token = "printf_keyword";
    }
    else if (subStr[0] == 'm' && subStr[1] == 'a' && subStr[2] == 's' && subStr[3] == '\0')
    {
        token = "else_keyword";
    }
    else if (subStr[0] == 'm' && subStr[1] == 'i' && subStr[2] == 'e' && subStr[3] == 'n' &&
             subStr[4] == 't' && subStr[5] == 'r' && subStr[6] == 'a' && subStr[7] == 's' && subStr[8] == '\0')
    {
        token = "while_keyword";
    }
    else if (subStr[0] == 'p' && subStr[1] == 'e' && subStr[2] == 'r' && subStr[3] == 's' && subStr[4] == 'o' &&
             subStr[5] == '\0')
    {
        token = "char_keyword";
    }
    else if (subStr[0] == 'p' && subStr[1] == 'o' && subStr[2] == 'r' && subStr[3] == '\0')
    {
        token = "for_keyword";
    }
    else if (subStr[0] == 'r' && subStr[1] == 'e' && subStr[2] == 't' && subStr[3] == 'o' &&
             subStr[4] == 'r' && subStr[5] == 'n' && subStr[6] == 'o' && subStr[7] == '\0')
    {
        token = "return_keyword";
    }
    else if (subStr[0] == 's' && subStr[1] == 'e' && subStr[2] == 'q' && subStr[3] == 'u' && subStr[4] == 'i' &&
             subStr[5] == 'r' && subStr[6] == '\0')
    {
        token = "continue_keyword";
    }
    else if (subStr[0] == 's' && subStr[1] == 'i' && subStr[2] == '\0')
    {
        token = "if_keyword";
    }
    else if (subStr[0] == 'v' && subStr[1] == 'a' && subStr[2] == 'c' && subStr[3] == 'i' && subStr[4] == 'o' &&
             subStr[5] == '\0')
    {
        token = "void_keyword";
    }
    else if (subStr[0] == 'v' && subStr[1] == 'a' && subStr[2] == 'm' && subStr[3] == 'o' && subStr[4] == 's' &&
             subStr[5] == '\0')
    {
        token = "goto_keyword";
    }

    // boolean
    else if (subStr[0] == 'y' && subStr[1] == '\0')
    {
        token = "and_op";
    }
    else if (subStr[0] == 'n' && subStr[1] == 'i' && subStr[2] == '\0')
    {
        token = "or_op";
    }
    else if (subStr[0] == 'n' && subStr[1] == 'o' && subStr[2] == '\0')
    {
        token = "not_op";
    }
    else
        return (false);

    return (true);
}

bool isReservedword(char *subStr)
{
    if (subStr[0] == 'f' && subStr[1] == 'a' && subStr[2] == 'l' && subStr[3] == 's' && subStr[4] == 'o' &&
        subStr[5] == '\0')
    {
        token = "false_reserved";
    }
    else if (subStr[0] == 'v' && subStr[1] == 'e' && subStr[2] == 'r' && subStr[3] == 'd' && subStr[4] == 'a' &&
             subStr[5] == 'd' && subStr[6] == '\0')
    {
        token = "true_reserved";
    }
    else
        return (false);

    return (true);
}

bool isNoiseword(char *subStr)
{
    if (subStr[0] == 'b' && subStr[1] == 'o' && subStr[2] == 'o' && subStr[3] == 'l' && subStr[4] == 'e' &&
        subStr[5] == 'a' && subStr[6] == 'n' && subStr[7] == 'o' && subStr[8] == '\0')
    {
        token = "boolean_noiseword";
    }

    else if (subStr[0] == 'c' && subStr[1] == 'u' && subStr[2] == 'e' && subStr[3] == 'r' && subStr[4] == 'd' &&
             subStr[5] == 'a' && subStr[6] == '\0')
    {
        token = "string_noiseword";
    }
    else if (subStr[0] == 'd' && subStr[1] == 'e' && subStr[2] == 'c' && subStr[3] == 'o' && subStr[4] == 'n' &&
             subStr[5] == 't' && subStr[6] == 'r' && subStr[7] == 'a' && subStr[8] == '\0')
    {
        token = "elif_noiseword";
    }
    else if (subStr[0] == 'e' && subStr[1] == 'n' && subStr[2] == 't' && subStr[3] == 'e' &&
             subStr[4] == 'r' && subStr[5] == 'o' && subStr[6] == '\0')
    {
        token = "integer_noiseword";
    }
    else if (subStr[0] == 'e' && subStr[1] == 's' && subStr[2] == 'c' && subStr[3] == 'a' &&
             subStr[4] == 'n' && subStr[5] == 'e' && subStr[6] == 'a' && subStr[7] == 'r' && subStr[8] == '\0')
    {
        token = "scanf_noiseword";
    }
    else if (subStr[0] == 'f' && subStr[1] == 'l' && subStr[2] == 'o' && subStr[3] == 't' && subStr[4] == 'a' &&
             subStr[5] == 'r' && subStr[6] == '\0')
    {
        token = "float_noiseword";
    }
    else if (subStr[0] == 'i' && subStr[1] == 'm' && subStr[2] == 'p' && subStr[3] == 'r' &&
             subStr[4] == 'i' && subStr[5] == 'm' && subStr[6] == 'i' && subStr[7] == 'r' && subStr[8] == '\0')
    {
        token = "printf_noiseword";
    }
    else if (subStr[0] == 'p' && subStr[1] == 'e' && subStr[2] == 'r' &&
             subStr[3] == 's' && subStr[4] == 'o' && subStr[5] == 'n' && subStr[6] == 'a' &&
             subStr[7] == 'j' && subStr[8] == 'e' && subStr[9] == '\0')
    {
        token = "char_noiseword";
    }
    else
        return (false);

    return (true);
}

bool isIntegerLiteral(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (isdigit(str[i]) == 0)
            return (false);
    }

    token = "int_lit";
    return (true);
}

bool isFloatLiteral(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (isdigit(str[i]) == 0 && str[i] != '.')
            return (false);
    }

    int count = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '.')
            count++;
    }

    if (count > 1)
    {
        token = "invalid";
    }
    else
    {
        token = "float_lit";
    }

    return (true);
    /*
        if a character in the substring is neither a digit nor a '.', then return false
        if the first loop ends w/o returning, then the str contains only numeric values and decimal
    */
}
