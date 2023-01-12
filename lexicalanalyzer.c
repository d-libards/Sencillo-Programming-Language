#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void fileChecker(char str[]);
void getLexemes(char *str);
bool isSeparator(char ch);
bool isOperator(char ch);
bool isIdentifier(char *str);
bool isKeyword(char *subStr);
bool isIntegerLiteral(char *str);
bool isFloatLiteral(char *str);
char *getSubString(char *str, int lowerbound, int upperbound);

bool isReservedword(char *subStr);
bool isNoiseword(char *subStr);
int identifyOperator(char *str, int upperbound);
void identifyDelimiter(char ch);
bool isIdentifierElement(char ch);

char *token;
FILE *outputptr;
FILE *inputptr;

int main()
{
    char filepath[100] = "";
    char string[2000] = "";
    char content[2000] = "";
    printf("Input filepath: ");
    scanf("%s", filepath);
    fileChecker(filepath);

    while (fgets(content, sizeof(content), inputptr))
    {
        strcat(string, content);
    }
    getLexemes(string);
    fclose(outputptr);
    fclose(inputptr);

    return (0);
}

void fileChecker(char str[])
{
    int len = strlen(str);
    int checker = 0;

    // check if the file path have proper length
    if (len <= 2)
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

void getLexemes(char *str)
{
    int lowerbound = 0, upperbound = 0;
    int length = strlen(str);

    while (upperbound <= length && lowerbound <= upperbound)
    {
        // COMMENT

        if (str[upperbound] == '/' && str[upperbound + 1] == '*')
        {
            upperbound += 2;

            char subs[500] = "";
            int i = 0;

            // string inside the /* */ will be stored in subs variable one by one
            while (upperbound <= length)
            {
                if (str[upperbound] == '*' && str[upperbound + 1] == '/')
                    break;
                else
                {
                    subs[i] = str[upperbound];
                    upperbound++;
                    i++;
                }
            }

            subs[i] = '\0';
            token = "comment";

            fprintf(outputptr, "Lexeme : %s\t\t\t\t\t%s\n", subs, token);

            upperbound += 2;
            lowerbound = upperbound;
            // extracted a lexeme, then update the lowerbound value
        }

        // CHARACTER LITERAL
        if (str[upperbound] == '\'')
        {
            upperbound++;
            char subs[300] = ""; // stores the char literal
            int charIndex = 0;

            // if '' (empty character literal)
            if (str[upperbound] == '\'')
            {
                token = "invalid char_literal";
            }
            // if '\''
            else if (str[upperbound] == '\\' && str[upperbound + 1] == '\'' && str[upperbound + 2] == '\'')
            {
                subs[charIndex] = '\'';
                token = "char_literal";
                upperbound += 2;
            }
            // if '\\'
            else if (str[upperbound] == '\\' && str[upperbound + 1] == '\\' && str[upperbound + 2] == '\'')
            {
                subs[charIndex] = '\\';
                token = "char_literal";
                upperbound += 2;
            }
            // if '\'a', output: 'a
            else if (str[upperbound] == '\\' && str[upperbound + 1] == '\'' && str[upperbound + 2] != '\'')
            {
                while (upperbound <= length)
                {
                    if (str[upperbound] == '\\' && str[upperbound + 1] == '\'')
                    {
                        subs[charIndex] = '\'';
                        charIndex++;
                        upperbound++;
                    }
                    else if (str[upperbound] == '\\' && str[upperbound + 1] == '\\')
                    {
                        subs[charIndex] = '\\';
                        charIndex++;
                        upperbound++;
                    }
                    else if (str[upperbound] == '\'')
                    {
                        break;
                    }
                    else
                    {
                        subs[charIndex] = str[upperbound];
                        charIndex++;
                    }
                    upperbound++;
                }
                token = "invalid char_literal";
            }
            // if '\\a', output: \a
            else if (str[upperbound] == '\\' && str[upperbound + 1] == '\\' && str[upperbound + 2] != '\'')
            {
                while (upperbound <= length)
                {
                    if (str[upperbound] == '\\' && str[upperbound + 1] == '\'')
                    {
                        subs[charIndex] = '\'';
                        charIndex++;
                        upperbound++;
                    }
                    else if (str[upperbound] == '\\' && str[upperbound + 1] == '\\')
                    {
                        subs[charIndex] = '\\';
                        charIndex++;
                        upperbound++;
                    }
                    else if (str[upperbound] == '\'')
                    {
                        break;
                    }
                    else
                    {
                        subs[charIndex] = str[upperbound];
                        charIndex++;
                    }
                    upperbound++;
                }
                token = "invalid char_literal";
            }

            // if the third character is not ' ex: 'aaa'
            else if (str[upperbound + 1] != '\'')
            {
                while (upperbound <= length)
                {
                    if (str[upperbound] == '\\' && str[upperbound + 1] == '\'')
                    {
                        subs[charIndex] = '\'';
                        charIndex++;
                        upperbound++;
                    }
                    else if (str[upperbound] == '\\' && str[upperbound + 1] == '\\')
                    {
                        subs[charIndex] = '\\';
                        charIndex++;
                        upperbound++;
                    }
                    else if (str[upperbound] == '\'')
                    {
                        break;
                    }
                    else
                    {
                        subs[charIndex] = str[upperbound];
                        charIndex++;
                    }
                    upperbound++;
                }
                token = "invalid char_literal";
            }
            else
            {
                subs[charIndex] = str[upperbound];
                token = "char_literal";
                upperbound++;
            }

            fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", subs, token);

            upperbound++;
            lowerbound = upperbound;
        }

        // STRING LITERAL
        if (str[upperbound] == '\"')
        {
            upperbound++;
            char c[2] = "";
            char subs[300] = ""; // stores the str literal

            // gets values between " "
            while (upperbound <= length)
            {
                if (str[upperbound] == '\"')
                {
                    break;
                }
                else if (str[upperbound] == '\\' && str[upperbound + 1] == '\"')
                {
                    c[0] = '\"';
                    strcat(subs, c);
                    upperbound += 2;
                }
                else if (str[upperbound] == '\\' && str[upperbound + 1] == '\\')
                {
                    c[0] = '\\';
                    strcat(subs, c);
                    upperbound += 2;
                }
                else
                {
                    c[0] = str[upperbound];
                    strcat(subs, c);
                    upperbound++;
                }
            }
            token = "str_literal";
            fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", subs, token);

            upperbound++;
            lowerbound = upperbound;
        }

        // checks if character is NOT a delimiter
        // if not a delimiter then proceed to evaluating the next character
        if (isSeparator(str[upperbound]) == false)
        {
            upperbound++;
        }

        // if char is a delimiter AND lowerbound == upperbound
        // if char is a delimiter AND lowerbound == upperbound
        if (isSeparator(str[upperbound]) == true && lowerbound == upperbound)
        {
            // OPERATOR
            if (isOperator(str[upperbound]) == true)
            {
                // gives new upperbound value after extracting the operator
                int new_upperbound = identifyOperator(str, upperbound);
                upperbound = new_upperbound; // upperbound
            }
            // IDENTIFY DELIMITER
            else
            {
                if (str[upperbound] != ' ' && str[upperbound] != '\n')
                {
                    identifyDelimiter(str[upperbound]);
                }
                upperbound++;
            }

            lowerbound = upperbound;
        }

        // if char is delimiter AND lowerbound != upperbound
        // or if upperbound == length AND lowerbound != upperbound
        else if ((isSeparator(str[upperbound]) == true && lowerbound != upperbound) || (upperbound == length && lowerbound != upperbound))
        {
            char *subStr = getSubString(str, lowerbound, upperbound - 1);

            if (isKeyword(subStr) == true)
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t%s\n", subStr, token);
            else if (isReservedword(subStr) == true)
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t%s\n", subStr, token);
            else if (isNoiseword(subStr) == true)
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t%s\n", subStr, token);
            else if (isIntegerLiteral(subStr) == true)
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t%s\n", subStr, token);
            else if (isFloatLiteral(subStr) == true)
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t%s\n", subStr, token);
            else if (isIdentifier(subStr) == true)
                fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t%s\n", subStr, token);
            else
                fprintf(outputptr, "Unknown input : \t\t\t\t\t\t\t%s\n", subStr);
            lowerbound = upperbound;
        }
    }
    return;
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

    fprintf(outputptr, "Lexeme : %c\t\t\t\t\t\t\t\t%s\n", ch, token);
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

int identifyOperator(char *str, int upperbound)
{
    char c[2] = "";
    char subStr[100] = "";

    // checks katabing operator
    // until may sunod sunod na operator, store it in subStr
    while (isOperator(str[upperbound]))
    {
        c[0] = str[upperbound];
        strcat(subStr, c);
        upperbound++;
    }

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
        token = "invalid operator";
    }
    fprintf(outputptr, "Lexeme : %s\t\t\t\t\t\t\t\t%s\n", subStr, token);

    return upperbound;
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
            token = "invalid identifier";
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
    token = "invalid identifier";
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
        token = "invalid float_literal";
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

char *getSubString(char *str, int lowerbound, int upperbound)
{
    char subs[300] = "";
    int i = 0;

    while (lowerbound <= upperbound)
    {
        subs[i] = str[lowerbound];
        lowerbound++;
        i++;
    }
    subs[i] = '\0';
    char *subStr = subs;

    return (subStr);
}
