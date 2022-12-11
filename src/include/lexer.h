#ifndef LEXER_H
#define LEXER_H

bool isAlphabet(char currentChar);
bool isNumeric(char currentChar);
bool isOperator(char currentChar);
bool isDelimiter(char currentChar);
bool isBlankSpace();
void getClass();
void evalChar(char x);
void myFunction();

#endif