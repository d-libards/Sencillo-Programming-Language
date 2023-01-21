#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void fileChecker(char str[]);
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

char *token;
FILE *outputptr;
FILE *inputptr;

int main()
{
    char filepath[200];
    printf("Input filepath: ");
    scanf("%s", filepath);
    fileChecker(filepath);

    int i;

    char ch1 = ' ';
    char ch2 = ' ';
    char comment[1000];
    char charLit[100];
    char strLit[100];
    char operators[100];
    char string[1000];
    int strIndex = 0;

    if (inputptr == NULL)
        return 0;

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
                    fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\tcomment\n", comment);
                    printf("%s comment\n", comment);
                    // next character
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
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\tinvalid\n", comment);
                printf("! invalid empty\n");
                ch1 = ch2;
                ch2 = fgetc(inputptr);
            }
            else if (ch1 == '\\' && ch2 == '\'')
            {
                fprintf(outputptr, "Lexeme : %c\t\t\t\t\t\t\t\tchar_lit\n", ch2);
                printf("%c \tchar lit\n", ch2);
                fgetc(inputptr);
                ch1 = fgetc(inputptr);
                ch2 = fgetc(inputptr);
            }
            else if (ch1 == '\\' && ch2 == '\\')
            {
                fprintf(outputptr, "Lexeme : %c\t\t\t\t\t\t\t\tchar_lit\n", ch2);
                printf("%c \tchar lit\n", ch2);
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
                        fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\tinvalid\n", charLit);
                        printf("%s \tinvalid char lit\n", charLit);
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
                fprintf(outputptr, "Lexeme : %c\t\t\t\t\t\t\t\tchar_lit\n", ch1);
                printf("%c \t char lit\n", ch1);
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
                    fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\tstr_lit\n", strLit);
                    printf("%s \tstring\n", strLit);
                    break;
                }
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

        else if (isSeparator(ch1) == false)
        {
            string[strIndex] = ch1;
            strIndex++;
            ch1 = ch2;
            ch2 = fgetc(inputptr);
        }
        else if (isSeparator(ch1) == true && strIndex == 0)
        {
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
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", operators, token);
                printf("%s \t%s\n", operators, token);
            }
            else
            {
                if (ch1 != ' ' && ch1 != '\n')
                {
                    identifyDelimiter(ch1);
                    fprintf(outputptr, "Lexeme : %c\t\t\t\t\t\t\t\t%s\n", ch1, token);
                    printf("%c \t%s\n", ch1, token);
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
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", string, token);
                printf("%s \t%s\n", string, token);
            }
            else if (isReservedword(string) == true)
            {
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", string, token);
                printf("%s \t\t%s\n", string, token);
            }
            else if (isNoiseword(string) == true)
            {
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", string, token);
                printf("%s \t\t%s\n", string, token);
            }
            else if (isIntegerLiteral(string) == true)
            {
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", string, token);
                printf("%s \t\t%s\n", string, token);
            }
            else if (isFloatLiteral(string) == true)
            {
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", string, token);
                printf("%s \t\t%s\n", string, token);
            }
            else if (isIdentifier(string) == true)
            {
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", string, token);
                printf("%s \t\t%s\n", string, token);
            }
            else
            {
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t! unrecognized input\n", string);
                printf("%s \t\t! unrecognized input\n", string);
            }

            strIndex = 0;
        }
        else
        {
            ch1 = ch2;
            ch2 = fgetc(inputptr);
        }

    } while (!feof(inputptr));

    fclose(outputptr);
    fclose(inputptr);

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
        token = "identifier";
        return (true);
    }
    token = "invalid";
    return (true);
}

bool isIdentifierElement(char ch)
{
    char identifierElements[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                                 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                                 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                                 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_', '\0'};

    for (int i = 0; i < strlen(identifierElements); i++)
    {
        if (identifierElements[i] == ch)
        {
            return (true);
        }
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

    token = "int_literal";
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
        token = "float_literal";
    }

    return (true);
    /*
        if a character in the substring is neither a digit nor a '.', then return false
        if the first loop ends w/o returning, then the str contains only numeric values and decimal
    */
}
