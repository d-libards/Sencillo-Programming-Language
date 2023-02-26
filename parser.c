#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// lexer

void fileChecker(char str[]);
void getLexeme();
void getString();
void storeChar();
void invalid();
bool isSeparator(char ch);
void identifyDelimiter(char ch);
bool isIdentifierElement(char ch);
bool isIdentifier(char *str);
void floatConst();

char *token;
FILE *outputptr;
FILE *inputptr;

int i;
char ch;
char ch1 = ' ';
char ch2 = ' ';
char comment[1000];
char charLit[100];
char strLit[100];
char string[1000];
int strIndex = 0;

int lineno = 0;
int commentno = 0;

// parser

void parse();
void getToken();
void stmt();
void scan_stmt();
void print_stmt();
void declaration_stmt();
void assignment_stmt();
void arith_expr();
void arith_term();
void arith_power();
void arith_factor();
void iterative_stmt();
void iterative_assign_stmt();
void declaration_assign_stmt();
void condition();
void increment();
void conditional_stmt();
void bool_expr();
void bool_term();
void bool_factor();
void rel_expr();
void error(char *token, char *delimiter);
void error_recovery(char *delimiter);

FILE *fptr;
char currentLexeme[100];
char currentToken[20];

int main()
{
    char filepath[100];
    printf("Input filepath: ");
    scanf("%s", filepath);
    fileChecker(filepath);

    // lexer

    if (inputptr == NULL)
        return 0;

    ch = fgetc(inputptr);
    do
    {
        getLexeme();
    } while (ch != EOF);

    fclose(outputptr);
    fclose(inputptr);

    // parser

    fptr = fopen("SymbolTable.txt", "r");
    if (fptr == NULL)
        printf("Error! opening file");
    else
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
    i = 0;

    // Delimiters
    if (ch == ':')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t:\n", ch);
        ch = fgetc(inputptr);
    }
    else if (ch == '(')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t(\n", ch);
        ch = fgetc(inputptr);
    }
    else if (ch == ')')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t)\n", ch);
        ch = fgetc(inputptr);
    }
    else if (ch == '[')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t[\n", ch);
        ch = fgetc(inputptr);
    }
    else if (ch == ']')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t]\n", ch);
        ch = fgetc(inputptr);
    }
    else if (ch == '{')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t{\n", ch);
        ch = fgetc(inputptr);
    }
    else if (ch == '}')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t}\n", ch);
        ch = fgetc(inputptr);
    }
    else if (ch == ',')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t,\n", ch);
        ch = fgetc(inputptr);
    }
    else if (ch == ';')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t;\n", ch);
        ch = fgetc(inputptr);
    }

    // operators

    else if (ch == '+')
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == '+')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t++\n", ch); // ++
            ch = fgetc(inputptr);
        }
        else if (ch == '=')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t+=\n", ch); // +=
            ch = fgetc(inputptr);
        }
        else
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\t+\n"); // +
        }
    }

    else if (ch == '-')
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == '-')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t--\n", ch); // --
            ch = fgetc(inputptr);
        }
        else if (ch == '=')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t-=\n", ch); // -=
            ch = fgetc(inputptr);
        }
        else
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\t-\n"); // -
        }
    }

    else if (ch == '*')
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == '*')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t**\n", ch); // **
            ch = fgetc(inputptr);
        }
        else if (ch == '=')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t*=\n", ch); // *=
            ch = fgetc(inputptr);
        }
        else
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\t*\n"); // *
        }
    }

    else if (ch == '/')
    {
        ch = fgetc(inputptr);

        if (ch == '/')
        {
            fprintf(outputptr, "/%c", ch);
            ch = fgetc(inputptr);

            if (ch == '=')
            {
                fprintf(outputptr, "%c\t\t\t\t\t\t\t\t//=\n", ch); // //=
                ch = fgetc(inputptr);
            }
            else
                fprintf(outputptr, "\t\t\t\t\t\t\t\t//\n"); // //
        }
        else if (ch == '=')
        {
            fprintf(outputptr, "/%c\t\t\t\t\t\t\t\t/=\n", ch); // /=
            ch = fgetc(inputptr);
        }
        else if (ch == '*')
        {
            // get two characters to determine the end comment: */
            ch1 = fgetc(inputptr);
            ch2 = fgetc(inputptr);
            i = 0;

            // store character in ch1 to comment array if ch1 and ch2 do not form end comment symbols
            while (!feof(inputptr))
            {
                if (ch1 == '*' && ch2 == '/')
                {
                    comment[i] = '\0';
                    fprintf(outputptr, "%s\t\t\t\t\t\t\t\tcomment\n", comment);
                    ch = fgetc(inputptr);
                    ;
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
        else
        {
            fprintf(outputptr, "/\t\t\t\t\t\t\t\t/\n"); // /
        }
    }

    else if (ch == '%')
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == '=')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t%%=\n", ch); // %=
            ch = fgetc(inputptr);
        }
        else if (ch == 'd')
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (isSeparator(ch) == true)
            {
                fprintf(outputptr, "\t\t\t\t\t\t\t\tid_symbol\n");
            }
            else
                getString();
        }
        else if (ch == 'c')
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (isSeparator(ch) == true)
            {
                fprintf(outputptr, "\t\t\t\t\t\t\t\tid_symbol\n");
            }
            else
                getString();
        }
        else if (ch == 'f')
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (isSeparator(ch) == true)
            {
                fprintf(outputptr, "\t\t\t\t\t\t\t\tid_symbol\n");
            }
            else
                getString();
        }
        else if (ch == 's')
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (isSeparator(ch) == true)
            {
                fprintf(outputptr, "\t\t\t\t\t\t\t\tid_symbol\n");
            }
            else
                getString();
        }
        else
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\t%%\n"); // /
        }
    }

    else if (ch == '=')
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == '=')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t==\n", ch);
            ch = fgetc(inputptr);
        }
        else
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\t=\n");
        }
    }

    else if (ch == '!')
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == '=')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t!=\n", ch);
            ch = fgetc(inputptr);
        }
        else
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\tinvalid\n");
        }
    }

    else if (ch == '<')
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == '=')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t<=\n", ch);
            ch = fgetc(inputptr);
        }
        else
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\t<\n");
        }
    }

    else if (ch == '>')
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == '=')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\t>=\n", ch);
            ch = fgetc(inputptr);
        }
        else
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\t>\n");
        }
    }

    // Character Constant
    else if (ch == '\'')
    {
        ch1 = fgetc(inputptr);
        ch2 = fgetc(inputptr);

        // if character is '' or empty character literal
        if (ch1 == '\'')
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\t\tinvalid\n");
            ch = ch2;
        }

        // if '\''
        else if (ch1 == '\\' && ch2 == '\'')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\tchar_const\n", ch2);
            fgetc(inputptr);
            ch = fgetc(inputptr);
        }

        // if '\\'
        else if (ch1 == '\\' && ch2 == '\\')
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\tchar_const\n", ch2);
            fgetc(inputptr);
            ch = fgetc(inputptr);
        }

        // if 'abc'
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
                    ch = fgetc(inputptr);
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

        // if valid character literal
        else
        {
            fprintf(outputptr, "%c\t\t\t\t\t\t\t\tchar_const\n", ch1);
            ch = fgetc(inputptr);
        }
    }

    // String Constant
    else if (ch == '\"')
    {
        ch1 = fgetc(inputptr);
        i = 0;

        // get all string literal inside " "
        while (!feof(inputptr))
        {
            ch2 = fgetc(inputptr);
            if (ch1 == '\"')
            {
                strLit[i] = '\0';
                ch = ch2;
                fprintf(outputptr, "%s\t\t\t\t\t\t\t\tstr_const\n", strLit);
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

    // Special Character
    else if (ch == '&')
    {
        fprintf(outputptr, "%c\t\t\t\t\t\t\t\t&\n", ch);
        ch = fgetc(inputptr);
    }

    // Keywords and Reserved words
    else if (ch == 'y') // and op
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (isSeparator(ch) == true)
        {
            fprintf(outputptr, "\t\t\t\t\t\t\t\tand_op\n");
        }
        else
            getString();
    }

    else if (ch == 'n') // ni, no
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'o') // no
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (isSeparator(ch) == true)
            {
                fprintf(outputptr, "\t\t\t\t\t\t\t\tnot_op\n");
            }
            else
                getString();
        }
        else if (ch == 'i') // ni
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (isSeparator(ch) == true)
            {
                fprintf(outputptr, "\t\t\t\t\t\t\t\tor_op\n");
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'b') // bool, booleano
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'o')
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'o')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'l')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (isSeparator(ch) == true)
                    {
                        fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                    }
                    else if (ch == 'e') // booleano
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (ch == 'a')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (ch == 'n')
                            {
                                storeChar();
                                fprintf(outputptr, "%c", ch);
                                ch = fgetc(inputptr);

                                if (ch == 'o')
                                {
                                    storeChar();
                                    fprintf(outputptr, "%c", ch);
                                    ch = fgetc(inputptr);

                                    if (isSeparator(ch) == true)
                                    {
                                        fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                                    }
                                    else
                                        getString();
                                }
                                else
                                    getString();
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'c') // cuer, cuerda
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'u') // cuer
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'e')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'r')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (isSeparator(ch) == true)
                    {
                        fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                    }
                    else if (ch == 'd') // cuerda
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (ch == 'a')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (isSeparator(ch) == true)
                            {
                                fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'd') // dec, decontra, descanso, doble
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'e') // dec
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'c')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (isSeparator(ch) == true)
                {
                    fprintf(outputptr, "\t\t\t\t\t\t\t\telif_keyword\n");
                }
                else if (ch == 'o') // decontra
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'n')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (ch == 't')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (ch == 'r')
                            {
                                storeChar();
                                fprintf(outputptr, "%c", ch);
                                ch = fgetc(inputptr);

                                if (ch == 'a')
                                {
                                    storeChar();
                                    fprintf(outputptr, "%c", ch);
                                    ch = fgetc(inputptr);

                                    if (isSeparator(ch) == true)
                                    {
                                        fprintf(outputptr, "\t\t\t\t\t\t\t\telif_noiseword\n");
                                    }
                                    else
                                        getString();
                                }
                                else
                                    getString();
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else if (ch == 's') // descanso
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'c')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'a')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);
                        if (ch == 'n')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (ch == 's')
                            {
                                storeChar();
                                fprintf(outputptr, "%c", ch);
                                ch = fgetc(inputptr);

                                if (ch == 'o')
                                {
                                    storeChar();
                                    fprintf(outputptr, "%c", ch);
                                    ch = fgetc(inputptr);

                                    if (isSeparator(ch) == true)
                                    {
                                        fprintf(outputptr, "\t\t\t\t\t\t\t\tbreak_keyword\n");
                                        ch = fgetc(inputptr);
                                    }
                                    else
                                        getString();
                                }
                                else
                                    getString();
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else if (ch == 'o') // doble
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'b')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'l')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'e')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (isSeparator(ch) == true)
                        {
                            fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                            ch = fgetc(inputptr);
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'e') // ent, entero, escan, escanear
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 's') // escan
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'c')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'a')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'n')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (isSeparator(ch) == true)
                        {
                            fprintf(outputptr, "\t\t\t\t\t\t\t\tscanf_keyword\n");
                        }
                        else if (ch == 'e') // escanear
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (ch == 'a')
                            {
                                storeChar();
                                fprintf(outputptr, "%c", ch);
                                ch = fgetc(inputptr);

                                if (ch == 'r')
                                {
                                    storeChar();
                                    fprintf(outputptr, "%c", ch);
                                    ch = fgetc(inputptr);

                                    if (isSeparator(ch) == true)
                                    {
                                        fprintf(outputptr, "\t\t\t\t\t\t\t\tscanf_noiseword\n");
                                    }
                                    else
                                        getString();
                                }
                                else
                                    getString();
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else if (ch == 'n') // ent
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 't')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (isSeparator(ch) == true)
                {
                    fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                }
                else if (ch == 'e') // entero
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'r')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (ch == 'o')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (isSeparator(ch) == true)
                            {
                                fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'f') // flot, flotar, falso
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'l') // flot
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'o')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 't')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (isSeparator(ch) == true)
                    {
                        fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                    }
                    else if (ch == 'a') // flotar
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (ch == 'r')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (isSeparator(ch) == true)
                            {
                                fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else if (ch == 'a') // falso
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'l')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 's')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'o')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (isSeparator(ch))
                        {
                            fprintf(outputptr, "\t\t\t\t\t\t\t\tbool_const\n");
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'h') // hacer
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'a')
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'c')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'e')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'r')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (isSeparator(ch) == true)
                        {
                            fprintf(outputptr, "\t\t\t\t\t\t\t\tdo_keyword\n");
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'i') // impri, imprimir
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'm') // impri
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'p')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'r')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'i')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (isSeparator(ch) == true)
                        {
                            fprintf(outputptr, "\t\t\t\t\t\t\t\tprintf_keyword\n");
                        }
                        else if (ch == 'm') // imprimir
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (ch == 'i')
                            {
                                storeChar();
                                fprintf(outputptr, "%c", ch);
                                ch = fgetc(inputptr);

                                if (ch == 'r')
                                {
                                    storeChar();
                                    fprintf(outputptr, "%c", ch);
                                    ch = fgetc(inputptr);

                                    if (isSeparator(ch))
                                    {
                                        fprintf(outputptr, "\t\t\t\t\t\t\t\tprintf_noiseword\n");
                                    }
                                    else
                                        getString();
                                }
                                else
                                    getString();
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'm') // mas, mientras
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'a') // mas
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 's')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (isSeparator(ch))
                {
                    fprintf(outputptr, "\t\t\t\t\t\t\t\telse_keyword\n");
                }
                else
                    getString();
            }
            else
                getString();
        }
        else if (ch == 'i') // mientras
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'e')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'n')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 't')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (ch == 'r')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (ch == 'a')
                            {
                                storeChar();
                                fprintf(outputptr, "%c", ch);
                                ch = fgetc(inputptr);

                                if (ch == 's')
                                {
                                    storeChar();
                                    fprintf(outputptr, "%c", ch);
                                    ch = fgetc(inputptr);

                                    if (isSeparator(ch))
                                    {
                                        fprintf(outputptr, "\t\t\t\t\t\t\t\twhile_keyword\n");
                                    }
                                    else
                                        getString();
                                }
                                else
                                    getString();
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'p') // perso, personaje, por
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'e') // perso
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'r')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 's')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'o')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (isSeparator(ch) == true)
                        {
                            fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                        }
                        else if (ch == 'n') // personaje
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (ch == 'a')
                            {
                                storeChar();
                                fprintf(outputptr, "%c", ch);
                                ch = fgetc(inputptr);

                                if (ch == 'j')
                                {
                                    storeChar();
                                    fprintf(outputptr, "%c", ch);
                                    ch = fgetc(inputptr);

                                    if (ch == 'e')
                                    {
                                        storeChar();
                                        fprintf(outputptr, "%c", ch);
                                        ch = fgetc(inputptr);

                                        if (isSeparator(ch))
                                        {
                                            fprintf(outputptr, "\t\t\t\t\t\t\t\tdata_type\n");
                                        }
                                        else
                                            getString();
                                    }
                                    else
                                        getString();
                                }
                                else
                                    getString();
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else if (ch == 'o') // por
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'r')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (isSeparator(ch))
                {
                    fprintf(outputptr, "\t\t\t\t\t\t\t\tfor_keyword\n");
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'r') // retorno
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'e')
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 't')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'o')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'r')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (ch == 'n')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (ch == 'o')
                            {
                                storeChar();
                                fprintf(outputptr, "%c", ch);
                                ch = fgetc(inputptr);

                                if (isSeparator(ch))
                                {
                                    fprintf(outputptr, "\t\t\t\t\t\t\t\treturn_keyword\n");
                                }
                                else
                                    getString();
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 's') // sequir, si
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'e') // sequir
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'q')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'u')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'i')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (ch == 'r')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (isSeparator(ch))
                            {
                                fprintf(outputptr, "\t\t\t\t\t\t\t\tcontinue_keyword\n");
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else if (ch == 'i') // si
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (isSeparator(ch))
            {
                fprintf(outputptr, "\t\t\t\t\t\t\t\tif_keyword\n");
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (ch == 'v') // vacio, vamos, verdad
    {
        storeChar();
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        if (ch == 'a') // vacio
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'c') // vacio
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'i')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'o')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (isSeparator(ch) == true)
                        {
                            fprintf(outputptr, "\t\t\t\t\t\t\t\tvoid_keyword\n");
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else if (ch == 'm') // vamos
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'o')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 's')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (isSeparator(ch))
                        {
                            fprintf(outputptr, "\t\t\t\t\t\t\t\tgoto_keyword\n");
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }

        else if (ch == 'e') // verdad
        {
            storeChar();
            fprintf(outputptr, "%c", ch);
            ch = fgetc(inputptr);

            if (ch == 'r')
            {
                storeChar();
                fprintf(outputptr, "%c", ch);
                ch = fgetc(inputptr);

                if (ch == 'd')
                {
                    storeChar();
                    fprintf(outputptr, "%c", ch);
                    ch = fgetc(inputptr);

                    if (ch == 'a')
                    {
                        storeChar();
                        fprintf(outputptr, "%c", ch);
                        ch = fgetc(inputptr);

                        if (ch == 'd')
                        {
                            storeChar();
                            fprintf(outputptr, "%c", ch);
                            ch = fgetc(inputptr);

                            if (isSeparator(ch))
                            {
                                fprintf(outputptr, "\t\t\t\t\t\t\t\tbool_const\n");
                            }
                            else
                                getString();
                        }
                        else
                            getString();
                    }
                    else
                        getString();
                }
                else
                    getString();
            }
            else
                getString();
        }
        else
            getString();
    }

    else if (isdigit(ch))
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);

        while (isSeparator(ch) == false)
        {
            if (isdigit(ch))
            {
                fprintf(outputptr, "%c", ch);
            }
            else if (ch == '.')
            {
                fprintf(outputptr, "%c", ch);
                floatConst();
                return;
            }
            else
            {
                invalid();
                return;
            }
            ch = fgetc(inputptr);
        }

        fprintf(outputptr, "\t\t\t\t\t\t\t\tint_const\n");
    }
    else
    {
        if (ch != ' ' && ch != '\n')
            getString();
        else
            ch = fgetc(inputptr);
    }
}

void floatConst()
{
    ch = fgetc(inputptr);

    // if the character after decimal is still a digit, proceed, else invalid
    if (isdigit(ch))
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);
    }
    else
    {
        invalid();
        return;
    }

    while (isSeparator(ch) == false)
    {
        if (isdigit(ch))
        {
            fprintf(outputptr, "%c", ch);
        }
        else
        {
            invalid();
            return;
        }
        ch = fgetc(inputptr);
    }

    fprintf(outputptr, "\t\t\t\t\t\t\t\tfloat_const\n");
}

void invalid()
{

    while (isSeparator(ch) == false)
    {
        fprintf(outputptr, "%c", ch);
        ch = fgetc(inputptr);
    }

    fprintf(outputptr, "\t\t\t\t\t\t\t\tinvalid\n");
}

void storeChar()
{
    string[i] = ch;
    i++;
}

void getString()
{
    while (isSeparator(ch) == false)
    {
        fprintf(outputptr, "%c", ch);
        string[i] = ch;
        i++;
        ch = fgetc(inputptr);
    }
    string[i] = '\0';

    if (isIdentifier(string) == true)
    {
        fprintf(outputptr, "\t\t\t\t\t\t\t\t%s\n", token);
    }
    else
    {
        fprintf(outputptr, "\t\t\t\t\t\t\t\tinvalid\n");
    }
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

    // gets the lexeme
    i = 0;
    while (ch != '\t')
    {
        currentLexeme[i] = ch;
        i++;
        ch = fgetc(fptr);
    }
    currentLexeme[i] = '\0';

    // skips all \t
    while (ch == '\t')
    {
        ch = fgetc(fptr);
    }

    // gets the token
    i = 0;
    while (ch != '\n')
    {
        currentToken[i] = ch;
        i++;
        ch = fgetc(fptr);
    }
    currentToken[i] = '\0';

    // skips comment
    if (strcmp(currentToken, "comment") == 0)
    {
        commentno++;
        getToken();
    }
    else
    {
        lineno++;
        printf("Lexeme: %s\t", currentLexeme);
        printf("Token: %s\n", currentToken);
    }
}

void stmt()
{
    print_stmt();
    scan_stmt();
    declaration_stmt();
    assignment_stmt();
    iterative_stmt();
    conditional_stmt();
}

void scan_stmt()
{
    if (strcmp(currentToken, "scanf_keyword") != 0) // escan
        return;
    else
    {
        printf("Enter <scan_stmt>\n");
        getToken();
        if (currentToken[0] != '(') // (
            error("(", ":");
        else
        {
            getToken();
            if (strcmp(currentToken, "id_symbol") != 0) // id_symbol
                error("id_symbol", ":");
            else
            {
                getToken();
                if (currentToken[0] != ',') // ,
                    error(",", ":");
                else
                {
                    getToken();
                    if (currentToken[0] != '&') // &
                        error("&", ":");
                    else
                    {
                        getToken();
                        if (strcmp(currentToken, "identifier") != 0) // id
                            error("identifier", ":");
                        else
                        {
                            getToken();
                            if (currentToken[0] != ')') // )
                                error(")", ":");
                            else
                            {
                                getToken();
                                if (currentToken[0] != ':') // :
                                    error(":", "proceed");
                                else
                                {
                                    printf("Exit <scan_stmt>\n");
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
    if (strcmp(currentToken, "printf_keyword") != 0) // print
        return;
    else
    {
        printf("Enter <print_stmt>\n");
        getToken();
        if (currentToken[0] != '(') // (
            error("(", ":");
        else
        {
            getToken();
            if (strcmp(currentToken, "str_const") == 0) // str_const
            {
                getToken();
                if (currentToken[0] == ',') // ,
                {
                    getToken();
                    if (strcmp(currentToken, "id_symbol") != 0) // id_symbol
                        error("id_symbol", ":");
                    else
                    {
                        getToken();
                        if (currentToken[0] != ',') // ,
                            error(",", ":");
                        else
                        {
                            getToken();
                            if (strcmp(currentToken, "identifier") != 0) // identifier
                                error("identifier", ":");
                            else
                            {
                                getToken();
                                if (currentToken[0] != ')') // )
                                    error(")", ":");
                                else
                                {
                                    getToken();
                                    if (currentToken[0] != ':') // :
                                        error(":", "proceed");
                                    else
                                    {
                                        printf("Exit <print_stmt>\n");
                                        getToken();
                                    }
                                }
                            }
                        }
                    }
                }
                else if (currentToken[0] == ')') // )
                {
                    getToken();
                    if (currentToken[0] != ':') // :
                        error(":", "proceed");
                    else
                    {
                        printf("Exit <print_stmt>\n");
                        getToken();
                    }
                }
                else
                    error(", or )", ":");
            }                                                // end str_const
            else if (strcmp(currentToken, "id_symbol") == 0) // id_symbol
            {
                getToken();
                if (currentToken[0] != ',') // ,
                    error(",", ":");
                else
                {
                    getToken();
                    if (strcmp(currentToken, "identifier") != 0) // identifier
                        error("identifier", ":");
                    else
                    {
                        getToken();
                        if (strcmp(currentToken, ")") != 0) // )
                            error(")", ":");
                        else
                        {
                            getToken();
                            if (strcmp(currentToken, ":") != 0) // :
                                error(":", "proceed");
                            else
                            {
                                printf("Exit <print_stmt>\n");
                                getToken();
                            }
                        }
                    }
                }
            }
            else
                error("str_const or id_symbol", ":");
        }
    }
}

void declaration_stmt()
{
    if (strcmp(currentToken, "data_type") != 0) // data type
        return;
    else
    {
        printf("Enter <declaration_stmt>\n");
        getToken();
        if (strcmp(currentToken, "identifier") != 0) // id
            error("identifier", ":");
        else
        {
            getToken();
            if (currentToken[0] == ':') // :
            {
                printf("Exit <declaration_stmt>\n");
                getToken();
            }
            else if (currentToken[0] == ',') // ,
            {
                while (currentToken[0] == ',')
                {
                    getToken();
                    if (strcmp(currentToken, "identifier") != 0)
                        error("identifier", ":");
                    else
                    {
                        getToken();
                        if (currentToken[0] == '=')
                        {
                            getToken();
                            if (strcmp(currentToken, "char_const") == 0 || strcmp(currentToken, "str_const") == 0 || strcmp(currentToken, "bool_const") == 0)
                            {
                                getToken();
                            }
                            else
                                arith_expr();
                        }
                    }
                }
                if (currentToken[0] != ':') // :
                    error(":", "proceed");
                else
                {
                    printf("Exit <declaration_stmt>\n");
                    getToken();
                }
            }
            else if (strcmp(currentToken, "=") == 0) // =
            {
                getToken();
                if (strcmp(currentToken, "char_const") == 0 || strcmp(currentToken, "str_const") == 0 || strcmp(currentToken, "bool_const") == 0)
                {
                    getToken();
                }
                else
                    arith_expr();
                while (currentToken[0] == ',')
                {
                    getToken();
                    if (strcmp(currentToken, "identifier") != 0)
                        error("identifier", ":");
                    else
                    {
                        getToken();
                        if (currentToken[0] == '=')
                        {
                            getToken();
                            if (strcmp(currentToken, "char_const") == 0 || strcmp(currentToken, "str_const") == 0 || strcmp(currentToken, "bool_const") == 0)
                            {
                                getToken();
                            }
                            else
                                arith_expr();
                        }
                    }
                }
                if (currentToken[0] != ':')
                    error(":", "proceed");
                else
                {
                    printf("Exit <declaration_stmt>\n");
                    getToken();
                }
            }
            else
                error(": or , or assign_op", ":");
        }
    }
}

void assignment_stmt()
{
    if (strcmp(currentToken, "identifier") != 0)
        return;
    else
    {
        printf("Enter <assignment_stmt>\n");
        getToken();
        if (currentToken[0] != '=')
            error("=", ":");
        else
        {
            getToken();
            if (strcmp(currentToken, "char_const") == 0 || strcmp(currentToken, "str_const") == 0 || strcmp(currentToken, "bool_const") == 0)
            {
                getToken();
            }
            else
                arith_expr();
            if (currentToken[0] != ':')
                error(":", "proceed");
            else
            {
                printf("Exit <assignment_stmt>\n");
                getToken();
            }
        }
    }
}

void arith_expr()
{
    arith_term();
    while (((currentToken[0] == '+') || (currentToken[0] == '-')) == true)
    {
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
        getToken();
        arith_power();
    }
}

void arith_power()
{
    arith_factor();
    while ((currentToken[0] == '*' && currentToken[1] == '*') == true)
    {
        getToken();
        arith_factor();
    }
}

void arith_factor()
{
    if (strcmp(currentToken, "identifier") == 0 || strcmp(currentToken, "int_const") == 0 || strcmp(currentToken, "float_const") == 0)
    {
        // id const
        getToken();
    }
    else if ((currentToken[0] == '(') == true)
    {
        getToken();
        arith_expr();

        if ((currentToken[0] == ')') == true)
        {
            getToken();
        }
        else
        {
            error(")", "proceed");
        }
    }
    else
        error("id or const", "proceed");
}

void iterative_stmt()
{
    if (strcmp(currentToken, "for_keyword") != 0)
        return;
    else
    {
        printf("Enter <iterative_stmt>\n");
        getToken();
        if (currentToken[0] != '(') // (
            error("(", "}");
        else
        {
            getToken();
            iterative_assign_stmt();    // assign_stmt
            if (currentToken[0] != ';') // ;
                error(";", "}");
            else
            {
                getToken();
                condition();                // condition
                if (currentToken[0] != ';') // ;
                    error(";", "}");
                else
                {
                    getToken();
                    increment(); // increment
                    if (currentToken[0] != ')')
                        error(")", "}"); // )
                    else
                    {
                        getToken();
                        if (currentToken[0] != '{') // {
                            error("{", "}");
                        else
                        {
                            getToken();
                            while (strcmp(currentToken, "scanf_keyword") == 0 || strcmp(currentToken, "printf_keyword") == 0 ||
                                   strcmp(currentToken, "data_type") == 0 || strcmp(currentToken, "identifier") == 0 ||
                                   strcmp(currentToken, "for_keyword") == 0 || strcmp(currentToken, "if_keyword") == 0)
                            {
                                stmt();
                            }
                            if (currentToken[0] != '}') // }
                                error("}", "proceed");
                            else
                            {
                                printf("Exit <iterative_stmt>\n");
                                getToken();
                            }
                        }
                    }
                }
            }
        }
    }
}

void iterative_assign_stmt()
{
    if (strcmp(currentToken, "data_type") != 0)
        error("data_type", ";");
    else
    {
        getToken();
        declaration_assign_stmt();
    }
}

void declaration_assign_stmt()
{
    if (strcmp(currentToken, "identifier") != 0)
        error("identifier", ";");
    else
    {
        getToken();
        if (currentToken[0] != '=')
            error("=", ";");
        else
        {
            getToken();
            arith_expr();
        }
    }
}

void condition()
{
    if (strcmp(currentToken, "identifier") != 0)
        error("identifier", ";");
    else
    {
        getToken();
        if (currentToken[0] == '>' || currentToken[0] == '<' || strcmp(currentToken, "!=") == 0 ||
            strcmp(currentToken, "==") == 0 || strcmp(currentToken, ">=") == 0 || strcmp(currentToken, "<=") == 0)
        {
            getToken();
            if (strcmp(currentToken, "identifier") == 0 || strcmp(currentToken, "int_const") == 0 || strcmp(currentToken, "float_const") == 0 ||
                strcmp(currentToken, "char_const") == 0 || strcmp(currentToken, "str_const") == 0)
            {
                getToken();
            }
            else
                error("id or const", ";");
        }
        else
            error("rel-op", ";");
    }
}

void increment()
{
    if (strcmp(currentToken, "identifier") == 0 || strcmp(currentToken, "++") == 0 || strcmp(currentToken, "--") == 0)
    {
        getToken();
        if (strcmp(currentToken, "identifier") == 0 || strcmp(currentToken, "++") == 0 || strcmp(currentToken, "--") == 0)
        {
            getToken();
        }
        else
            error("id or unary-op", ")");
    }
    else
        error("id or unary-op", ")");
}

void conditional_stmt()
{
    if (strcmp(currentToken, "if_keyword") != 0)
        return;
    else
    {
        printf("Enter <conditional_stmt>\n");
        getToken();
        if (currentToken[0] != '(') // (
            error("(", "}");
        else
        {
            getToken();
            bool_expr();                // boolean expr
            if (currentToken[0] != ')') // )
                error(")", "}");
            else
            {
                getToken();
                if (currentToken[0] != '{') // {
                    error("{", "}");
                else
                {
                    getToken();
                    while (strcmp(currentToken, "scanf_keyword") == 0 || strcmp(currentToken, "printf_keyword") == 0 ||
                           strcmp(currentToken, "data_type") == 0 || strcmp(currentToken, "identifier") == 0 ||
                           strcmp(currentToken, "for_keyword") == 0 || strcmp(currentToken, "if_keyword") == 0)
                    {
                        stmt();
                    }
                    if (currentToken[0] != '}') // }
                        error("}", "}");
                    else
                    {
                        printf("Exit <conditional_stmt>\n");
                        getToken();
                        if (strcmp(currentToken, "else_keyword") == 0) // else
                        {
                            printf("Enter <else_stmt>\n");
                            getToken();
                            if (currentToken[0] != '{') // {
                                error("{", "}");
                            else
                            {
                                getToken();
                                while (strcmp(currentToken, "scanf_keyword") == 0 || strcmp(currentToken, "printf_keyword") == 0 ||
                                       strcmp(currentToken, "data_type") == 0 || strcmp(currentToken, "identifier") == 0 ||
                                       strcmp(currentToken, "for_keyword") == 0 || strcmp(currentToken, "if_keyword") == 0)
                                {
                                    stmt();
                                }
                                if (currentToken[0] != '}')
                                    error("}", "proceed");
                                else
                                {
                                    printf("Exit <else_stmt>\n");
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

void bool_expr()
{
    bool_term();
    while (strcmp(currentToken, "or_op") == 0)
    {
        getToken();
        bool_term();
    }
}

void bool_term()
{
    bool_factor();
    while (strcmp(currentToken, "and_op") == 0)
    {
        getToken();
        bool_factor();
    }
}

void bool_factor()
{
    if (strcmp(currentToken, "not_op") == 0)
    {
        getToken();
        if (strcmp(currentToken, "identifier") == 0)
        {
            getToken();
        }
        else if ((currentToken[0] == '(') == true)
        {
            getToken();
            bool_expr();
            if ((currentToken[0] == ')') == true)
            {
                getToken();
            }
            else
                error(")", "proceed");
        }
    }
    else if ((currentToken[0] == '(') == true)
    {
        getToken();
        bool_expr();
        if ((currentToken[0] == ')') == true)
        {
            getToken();
        }
    }
    else
    {
        rel_expr();
    }
}

void rel_expr()
{
    if (strcmp(currentToken, "identifier") == 0 || strcmp(currentToken, "int_const") == 0 || strcmp(currentToken, "float_const") == 0 ||
        strcmp(currentToken, "char_const") == 0 || strcmp(currentToken, "str_const") == 0) // id cnst
    {
        getToken();
        if (strcmp(currentToken, "<") == 0 || strcmp(currentToken, ">") == 0 || strcmp(currentToken, ">=") == 0 ||
            strcmp(currentToken, "<=") == 0 || strcmp(currentToken, "==") == 0 || strcmp(currentToken, "!=") == 0)
        {
            getToken();
            if (strcmp(currentToken, "identifier") == 0 || strcmp(currentToken, "int_const") == 0 || strcmp(currentToken, "float_const") == 0 ||
                strcmp(currentToken, "char_const") == 0 || strcmp(currentToken, "str_const") == 0)
            {
                getToken();
            }
            else
                error("id or const", ")");
        }
        else
            error("rel-op", ")");
    }
    else
        error("id or const", ")");
}

void error(char *token, char *delimiter)
{
    printf("    ! error token: %s\n", currentToken);
    printf("    ! expected token: %s\n", token);
    printf("    ! error in line: %d\n", lineno + commentno);

    if (strcmp(delimiter, ";") == 0 || strcmp(delimiter, ")") == 0)
    {
        error_recovery(delimiter);
    }
    else if (strcmp(delimiter, "proceed") != 0)
    {
        error_recovery(delimiter);
        getToken();
    }

    // if delimiter=proceed, as is
    // no getToken, return to stmt() then makakahanap ng next stmt
}

void error_recovery(char *delimiter)
{
    while (strcmp(currentToken, delimiter) != 0)
    {
        getToken();
    }
}

bool isSeparator(char ch)
{
    char separatorList[] = {' ', '+', '-', '*', '/', '%', '>', '<', '!', '=',
                            '[', ']', '{', '}', '(', ')', ':', ';', ',', '\n', '\0', '\t'};

    for (int i = 0; i < strlen(separatorList); i++)
    {
        if (separatorList[i] == ch)
            return (true);
    }

    return (false);
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
    if (isalpha(ch) || isdigit(ch) || ch == '_')
    {
        return (true);
    }

    return (false);
}