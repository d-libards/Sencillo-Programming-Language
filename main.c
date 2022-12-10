#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// global variables
char program[300];
int inputIndex = 0;
char mainStr[100]; // consider string literal
char charClass[15] = "ALPHABET";

char *lexeme[100];
int lexIndex = 0;

char token[20];

bool isAlphabet(char currentChar){
    int i=0;
    bool result = false;

    char letterList[]={'a','b','c','d','e','f','g','h','i','j','k','l','m',
                        'n','o','p','q','r','s','t','u','v','w','x','y','z',
                        'A','B','C','D','E','F','G','H','I','J','K','L','M',
                        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z','_','\0'};
    while(i < strlen(letterList)){   //53
        if(currentChar == letterList[i]){
            result = true;
            break;
        }
        i++;
    }
    
    return result;    
}

bool isNumeric(char currentChar){
    int i=0;
    bool result = false;

    char digitList[]={'0','1','2','3','4','5','6','7','8','9','.','\0'};
    
    while(i < strlen(digitList)){   
        if(currentChar == digitList[i]){
            result = true;
            break;
        }
        i++;
    }
    return result; 
}

bool isOperator(char currentChar){
    int i=0;
    bool result = false;

    char operatorList[]={'=','+','-','*','/','%','!','>','<','\0'};
    
    while(i < strlen(operatorList)){   
        if(currentChar == operatorList[i]){
            result = true;
            break;
        }
        i++;
    }
    return result;     
}

bool isDelimiter(char currentChar){
    int i=0;
    bool result = false;

    char delimiterList[]={'(',')','{','}','[',']',',',':',';','\0'};
    
    while(i < strlen(delimiterList)){   
        if(currentChar == delimiterList[i]){
            result = true;
            break;
        }
        i++;
    }
    return result;     
}
bool isBlankSpace(){
    if(charClass[0]=='B' && charClass[1]=='L' && charClass[2]=='A' && charClass[3]=='N' && charClass[4]=='K' && 
        charClass[5]=='_' && charClass[6]=='S' && charClass[7]=='P' && charClass[8]=='A' && charClass[9]=='C' && 
        charClass[10]=='E' && charClass[11]=='\0'){
        return (true);  
    }
    return(false);
}

void getClass(){ 
    
    if(charClass[0]=='A' && charClass[1]=='L' && charClass[2]=='P' && charClass[3]=='H' && charClass[4]=='A' && 
            charClass[5]=='B' && charClass[6]=='E' && charClass[7]=='T' && charClass[8]=='\0'){
        if((mainStr[0] == 'b' && mainStr[1] == 'o' && mainStr[2] == 'o' && mainStr[3] == 'l' && mainStr[4] == '\0') ||
            (mainStr[0] == 'b' && mainStr[1] == 'o' && mainStr[2] == 'o' && mainStr[3] == 'l' && mainStr[4] == 'e' && 
            mainStr[5] == 'a' && mainStr[6] == 'n' && mainStr[7] == 'o' && mainStr[8] == '\0')){
            strcpy(token,"boolean_keyword");
        }  
        else if((mainStr[0] == 'c' && mainStr[1] == 'u' && mainStr[2] == 'e' && mainStr[3] == 'r' && mainStr[4] == '\0') ||
                (mainStr[0] == 'c' && mainStr[1] == 'u' && mainStr[2] == 'e' && mainStr[3] == 'r' && mainStr[4] == 'd' &&
                mainStr[5] == 'a' && mainStr[6] == '\0')){
            strcpy(token,"string_keyword");
        }  
        else if((mainStr[0] == 'd' && mainStr[1] == 'e' && mainStr[2] == 'c' && mainStr[3] == '\0') ||
            (mainStr[0] == 'd' && mainStr[1] == 'e' && mainStr[2] == 'c' && mainStr[3] == 'o' && mainStr[4] == 'n' &&
            mainStr[5] == 't' && mainStr[6] == 'r' && mainStr[7] == 'a' && mainStr[8] == '\0')){
            strcpy(token,"elif_keyword");
        }
        else if(mainStr[0] == 'd' && mainStr[1] == 'e' && mainStr[2] == 's' && mainStr[3] == 'c' &&
                mainStr[4] == 'a' && mainStr[5] == 'n' && mainStr[6] == 's' && mainStr[7] == 'o' && mainStr[8] == '\0'){
            strcpy(token,"break_keyword");
        }  
        else if(mainStr[0] == 'd' && mainStr[1] == 'o' && mainStr[2] == 'b' && mainStr[3] == 'l' &&
                mainStr[4] == 'e' && mainStr[5] == '\0'){
            strcpy(token,"double_keyword");
        } 
        else if((mainStr[0] == 'e' && mainStr[1] == 'n' && mainStr[2] == 't' && mainStr[3] == '\0') || 
                (mainStr[0] == 'e' && mainStr[1] == 'n' && mainStr[2] == 't' && mainStr[3] == 'e' && 
                mainStr[4] == 'r' && mainStr[5] == 'o' && mainStr[6] == '\0')){
            strcpy(token,"integer_keyword");
        }
        else if((mainStr[0] == 'e' && mainStr[1] == 's' && mainStr[2] == 'c' && mainStr[3] == 'a' && mainStr[4] == 'n' && 
                mainStr[5] == '\0') || (mainStr[0] == 'e' && mainStr[1] == 's' && mainStr[2] == 'c' && mainStr[3] == 'a' && 
                mainStr[4] == 'n' && mainStr[5] == 'e' && mainStr[6] == 'a' && mainStr[7] == 'r' && mainStr[8] == '\0')){
            strcpy(token,"scanf_keyword");
        }    
        else if((mainStr[0] == 'f' && mainStr[1] == 'l' && mainStr[2] == 'o' && mainStr[3] == 't' && mainStr[4] == '\0') ||
                (mainStr[0] == 'f' && mainStr[1] == 'l' && mainStr[2] == 'o' && mainStr[3] == 't' && mainStr[4] == 'a' &&
                mainStr[5] == 'r' && mainStr[6] == '\0')){
            strcpy(token,"float_keyword");
        }   
        else if(mainStr[0] == 'h' && mainStr[1] == 'a' && mainStr[2] == 'c' && mainStr[3] == 'e' &&
                mainStr[4] == 'r' && mainStr[5] == '\0'){
            strcpy(token,"do_keyword");
        } 
        else if((mainStr[0] == 'i' && mainStr[1] == 'm' && mainStr[2] == 'p' && mainStr[3] == 'r' && mainStr[4] == 'i' && 
                mainStr[5] == '\0') || (mainStr[0] == 'i' && mainStr[1] == 'm' && mainStr[2] == 'p' && mainStr[3] == 'r' && 
                mainStr[4] == 'i' && mainStr[5] == 'm' && mainStr[6] == 'i' && mainStr[7] == 'r' && mainStr[8] == '\0')){
            strcpy(token,"printf_keyword");
        }    
        else if(mainStr[0] == 'm' && mainStr[1] == 'a' && mainStr[2] == 's' && mainStr[3] == '\0'){
            strcpy(token,"else_keyword");
        }    
        else if(mainStr[0] == 'm' && mainStr[1] == 'i' && mainStr[2] == 'e' && mainStr[3] == 'n' && 
                mainStr[4] == 't' && mainStr[5] == 'r' && mainStr[6] == 'a' && mainStr[7] == 's' && mainStr[8] == '\0'){
            strcpy(token,"while_keyword");
        }    
        else if((mainStr[0] == 'p' && mainStr[1] == 'e' && mainStr[2] == 'r' && mainStr[3] == 's' && mainStr[4] == 'o' && 
                mainStr[5] == '\0') || (mainStr[0] == 'p' && mainStr[1] == 'e' && mainStr[2] == 'r' && 
                mainStr[3] == 's' && mainStr[4] == 'o' && mainStr[5] == 'n' && mainStr[6] == 'a' && 
                mainStr[7] == 'j' && mainStr[8] == 'e' && mainStr[9] == '\0')){
            strcpy(token,"char_keyword");
        }    
        else if(mainStr[0] == 'p' && mainStr[1] == 'o' && mainStr[2] == 'r' && mainStr[3] == '\0'){
            strcpy(token,"for_keyword");
        }    
        else if(mainStr[0] == 'r' && mainStr[1] == 'e' && mainStr[2] == 't' && mainStr[3] == 'o' &&
                mainStr[4] == 'r' && mainStr[5] == 'n' && mainStr[6] == 'o' && mainStr[7] == '\0'){
            strcpy(token,"return_keyword");
        }
        else if(mainStr[0] == 's' && mainStr[1] == 'e' && mainStr[2] == 'q' && mainStr[3] == 'u' && mainStr[4] == 'i' &&
                mainStr[5] == 'r' && mainStr[6] == '\0'){
            strcpy(token,"continue_keyword");
        }   
        else if(mainStr[0] == 's' && mainStr[1] == 'i' && mainStr[2] == '\0'){
            strcpy(token,"if_keyword");
        }  
        else if(mainStr[0] == 'v' && mainStr[1] == 'a' && mainStr[2] == 'c' && mainStr[3] == 'i' && mainStr[4] == 'o' &&
                mainStr[5] == '\0'){
            strcpy(token,"void_keyword");
        }    
        else if(mainStr[0] == 'v' && mainStr[1] == 'a' && mainStr[2] == 'm' && mainStr[3] == 'o' && mainStr[4] == 's' &&
                mainStr[5] == '\0'){
            strcpy(token,"goto_keyword");
        }  
        else if(mainStr[0] == 'f' && mainStr[1] == 'a' && mainStr[2] == 'l' && mainStr[3] == 's' && mainStr[4] == 'o' &&
                mainStr[5] == '\0'){
            strcpy(token,"boolean_literal");
        }
        else if(mainStr[0] == 'v' && mainStr[1] == 'e' && mainStr[2] == 'r' && mainStr[3] == 'd' && mainStr[4] == 'a' &&
                mainStr[5] == 'd' && mainStr[6] == '\0'){
            strcpy(token,"boolean_literal");
        }  
        
        // boolean
        
        else if(mainStr[0] == 'y' && mainStr[1] ==  '\0'){
            strcpy(token,"and_op");
        }     
        else if(mainStr[0] == 'n' && mainStr[1] == 'i' && mainStr[2] == '\0'){
            strcpy(token,"or_op");
        } 
        else if(mainStr[0] == 'n' && mainStr[1] == 'o' && mainStr[2] == '\0'){
            strcpy(token,"not_op");
        }  
        else{
            if(mainStr[0] == '_'){
                strcpy(token,"invalid identifier");
            }
            else{
                strcpy(token,"identifier");                
            }
        }
            
    }
    
    else if(charClass[0]=='O' && charClass[1]=='P' && charClass[2]=='E' && charClass[3]=='R' && charClass[4]=='A' && 
            charClass[5]=='T' && charClass[6]=='O' && charClass[7] =='R' && charClass[8] =='\0'){

        // arithmetic operator
    
        if(mainStr[0] == '+' && mainStr[1] == '\0'){
            strcpy(token,"add_op");
        }
        else if(mainStr[0] == '-' && mainStr[1] == '\0'){
            strcpy(token,"sub_op");
        }    
        else if(mainStr[0] == '/' && mainStr[1] == '\0'){
            strcpy(token,"div_op");
        }   
        else if(mainStr[0] == '*' && mainStr[1] == '\0'){
            strcpy(token,"mult_op");
        }  
        else if(mainStr[0] == '%' && mainStr[1] == '\0'){
            strcpy(token,"mod_op");
        }    
        else if(mainStr[0] == '*' && mainStr[1] == '*' && mainStr[2] == '\0'){
            strcpy(token,"expo_op");
        }
        else if(mainStr[0] == '/' && mainStr[1] == '/' && mainStr[2] == '\0'){
            strcpy(token,"intDiv_op");
        }  
       
        // assignment operator
        
        else if(mainStr[0] == '=' && mainStr[1] == '\0'){
            strcpy(token,"assign_op");
        }    
        else if(mainStr[0] == '+' && mainStr[1] == '=' && mainStr[2] == '\0'){
            strcpy(token,"addAssign_op");
        }  
        else if(mainStr[0] == '-' && mainStr[1] == '=' && mainStr[2] == '\0'){
            strcpy(token,"subAssign_op");
        }  
        else if(mainStr[0] == '*' && mainStr[1] == '=' && mainStr[2] == '\0'){
            strcpy(token,"multAssign_op");
        } 
        else if(mainStr[0] == '%' && mainStr[1] == '=' && mainStr[2] == '\0'){
            strcpy(token,"modAssign_op");
        }  
        else if(mainStr[0] == '/' && mainStr[1] == '=' && mainStr[2] == '\0'){
            strcpy(token,"divAssign_op");
        }  
        else if(mainStr[0] == '/' && mainStr[1] == '/' && mainStr[2] == '=' && mainStr[3] == '\0'){
            strcpy(token,"intDivAssign_op");
        }     
        
        // increment & decrement
        
        else if(mainStr[0] == '+' && mainStr[1] == '+' && mainStr[2] == '\0'){
            strcpy(token,"increment_op");
        }  
        else if(mainStr[0] == '-' && mainStr[1] == '-' && mainStr[2] == '\0'){
            strcpy(token,"decrement_op");
        }  
    
        // boolean relational       
        
        else if(mainStr[0] == '=' && mainStr[1] == '=' && mainStr[2] == '\0'){
            strcpy(token,"equal_op");
        } 
        else if(mainStr[0] == '!' && mainStr[1] == '=' && mainStr[2] == '\0'){
            strcpy(token,"notEqual_op");
        }   
        else if(mainStr[0] == '>' && mainStr[1] == '=' && mainStr[2] == '\0'){
            strcpy(token,"goe_op");
        } 
        else if(mainStr[0] == '<' && mainStr[1] == '=' && mainStr[2] == '\0'){
            strcpy(token,"loe_op");
        }       
        else if(mainStr[0] == '>' && mainStr[1] == '\0'){
            strcpy(token,"greater_op");
        } 
        else if(mainStr[0] == '<' && mainStr[1]  == '\0'){
            strcpy(token,"less_op");
        }    
        
        else{
            strcpy(token,"invalid operator");
        }
            
    }
    
    else if(charClass[0]=='D' && charClass[1]=='E' && charClass[2]=='L' && charClass[3]=='I' && charClass[4]=='M' && 
            charClass[5]=='I' && charClass[6]=='T' && charClass[7] =='E' && charClass[8] =='R' && charClass[9] =='\0'){
        strcpy(token,"delimiter");
    }
    
    else if(charClass[0]=='N' && charClass[1]=='U' && charClass[2]=='M' && charClass[3]=='E' && charClass[4]=='R' && 
            charClass[5]=='I' && charClass[6]=='C' && charClass[7] =='\0'){
        int found=0;
        for(int i = 0; i < strlen(mainStr); i++){
            if(mainStr[i] == '.'){
                found=1;
            }
        }
        if(found==1){
            strcpy(token,"float_literal");
        }
        else{
             strcpy(token,"int_literal");
        }
    }
    
    else if(charClass[0]=='S' && charClass[1]=='T' && charClass[2]=='R' && charClass[3]=='_' && charClass[4]=='L' && 
            charClass[5]=='I' && charClass[6]=='T' && charClass[7]=='E' && charClass[8]=='R' && charClass[9]=='A' && 
            charClass[10]=='L' && charClass[11]=='\0'){
        strcpy(token,"str_literal");
    }
    
    else if(charClass[0]=='C' && charClass[1]=='H' && charClass[2]=='A' && charClass[3]=='R' && charClass[4]=='_' && 
            charClass[5]=='L' && charClass[6]=='I' && charClass[7]=='T' && charClass[8]=='E' && charClass[9]=='R' && 
            charClass[10]=='A' && charClass[11]=='L' && charClass[12]=='\0'){
        strcpy(token,"char_literal");
    }
    
    else if(charClass[0]=='C' && charClass[1]=='O' && charClass[2]=='M' && charClass[3]=='M' && charClass[4]=='E' && 
            charClass[5]=='N' && charClass[6]=='T' && charClass[7] =='\0'){
        strcpy(token,"comment");
    }
}

void evalChar(char x){    // gets character class
    char currentChar[2];
    currentChar[0] = x;
    char str_c[2] = "";  
    char str_literal[100] = "";   
    
    char comment_c[2] = ""; 
    char comment_str[100] = "";  
    
    if(isAlphabet(currentChar[0])){
        //printf("alphabet %c\n", currentChar[0]);
        if(charClass[0]=='A' && charClass[1]=='L' && charClass[2]=='P' && charClass[3]=='H' && charClass[4]=='A' && 
            charClass[5]=='B' && charClass[6]=='E' && charClass[7]=='T' && charClass[8]=='\0'){
            strcat(mainStr, currentChar);
        }
        else if(isBlankSpace()){
            strcpy(mainStr,currentChar);
            strcpy(charClass,"ALPHABET");          
        }
        else{
            getClass();
            lexeme[lexIndex] = mainStr; // string to string array
            printf("lexeme %d: %s\t%s\n",lexIndex, lexeme[lexIndex], token);
            strcpy(token,"");
            lexIndex++;
            strcpy(mainStr,currentChar);
            strcpy(charClass,"ALPHABET");
        }
        
    }
    
    else if(isNumeric(currentChar[0])){
        // printf("int literal %c\n", currentChar[0]);
        
        if(charClass[0]=='N' && charClass[1]=='U' && charClass[2]=='M' && charClass[3]=='E' && charClass[4]=='R' && 
            charClass[5]=='I' && charClass[6]=='C' && charClass[7] =='\0'){
            strcat(mainStr, currentChar);
        }
        else if(isBlankSpace()){
            strcpy(mainStr,currentChar);
            strcpy(charClass,"NUMERIC");            
        }        
        else{
            getClass();
            lexeme[lexIndex] = mainStr; 
            printf("lexeme %d: %s\t%s\n",lexIndex, lexeme[lexIndex], token);
            strcpy(token,"");
            lexIndex++;
            strcpy(mainStr,currentChar);
            strcpy(charClass,"NUMERIC");
        }
    }
    
    else if(isOperator(currentChar[0])){
        // printf("operator %c\n", currentChar[0]);
        if(charClass[0]=='O' && charClass[1]=='P' && charClass[2]=='E' && charClass[3]=='R' && charClass[4]=='A' && 
            charClass[5]=='T' && charClass[6]=='O' && charClass[7] =='R' && charClass[8] =='\0'){
            strcat(mainStr, currentChar);
        }
        else if(isBlankSpace()){
            strcpy(mainStr,currentChar);
            strcpy(charClass,"OPERATOR");            
        }          
        else{
            getClass();
            lexeme[lexIndex] = mainStr; 
            printf("lexeme %d: %s\t%s\n",lexIndex, lexeme[lexIndex], token);
            strcpy(token,"");
            lexIndex++;
            strcpy(mainStr,currentChar);
            strcpy(charClass,"OPERATOR");
        }        
    }
    
    else if(isDelimiter(currentChar[0])){
        // printf("delimiter %c\n", currentChar[0]);
        if(isBlankSpace()){
            strcpy(mainStr,currentChar);
            strcpy(charClass,"DELIMITER");            
        } 
        else{
            getClass();
            lexeme[lexIndex] = mainStr; 
            printf("lexeme %d: %s\t%s\n",lexIndex, lexeme[lexIndex], token);
            strcpy(token,"");
            lexIndex++;
            strcpy(mainStr,currentChar);
            strcpy(charClass,"DELIMITER");
        }
    }
    
    else if(currentChar[0] == '\"'){
        if(isBlankSpace()){
            inputIndex++;
            if(program[inputIndex] == '\"'){
                // str ="":
                // remove if necessary
            }
            else{
                while(program[inputIndex] != '\"'){
                    str_c[0] = program[inputIndex];
                    strcat(str_literal,str_c); 
                    inputIndex++;
                }     
                strcpy(mainStr,str_literal);
                strcpy(charClass,"STR_LITERAL");
            }
        }  
        else{
            getClass();
            lexeme[lexIndex] = mainStr; 
            printf("lexeme %d: %s\t%s\n",lexIndex, lexeme[lexIndex], token);
            strcpy(token,"");
            lexIndex++;
            
            inputIndex++;
            
            if(program[inputIndex] == '\"'){
                // str ="":
                // remove if necessary
            }
            else{
                
                while(program[inputIndex] != '\"'){
                    str_c[0] = program[inputIndex];
                    strcat(str_literal,str_c); 
                    inputIndex++;
                }     
                strcpy(mainStr,str_literal);
                strcpy(charClass,"STR_LITERAL");
            }
        }
        // printf("string literal %s\n", str_literal);
    }
    
    else if(currentChar[0] == '\''){
        if(isBlankSpace()){
            inputIndex++;
            
            char c_literal[30] = "";
            char ch[2];
            c_literal[0] = program[inputIndex];
            
            inputIndex++;           
            
            if(program[inputIndex] != '\''){
                while(program[inputIndex] != '\''){
                    ch[0] = program[inputIndex];
                    strcat(c_literal,ch); 
                    inputIndex++;
                }
                printf("invalid character: %s\n",c_literal);
                strcpy(charClass,"BLANK_SPACE");
            }
            else{
                strcpy(mainStr, c_literal);
                strcpy(charClass,"CHAR_LITERAL");
            }         
        } 
        else{
            getClass();
            lexeme[lexIndex] = mainStr; 
            printf("lexeme %d: %s\t%s\n",lexIndex, lexeme[lexIndex], token);
            strcpy(token,"");
            lexIndex++;
            
            inputIndex++;
            
            char c_literal[30] = "";
            char ch[2];
            c_literal[0] = program[inputIndex];
            
            inputIndex++;           
            
            if(program[inputIndex] != '\''){
                while(program[inputIndex] != '\''){
                    ch[0] = program[inputIndex];
                    strcat(c_literal,ch); 
                    inputIndex++;
                }
                printf("invalid character: %s\n",c_literal);
                strcpy(charClass,"BLANK_SPACE");
            }
            else{
                strcpy(mainStr, c_literal);
                strcpy(charClass,"CHAR_LITERAL");
            }
        }
    }  
    
    else if(currentChar[0] == ' '){
        //printf("space \n");
        if(isBlankSpace()){}
        else{
            getClass();
            lexeme[lexIndex] = mainStr; 
            printf("lexeme %d: %s\t%s\n",lexIndex, lexeme[lexIndex], token);
            strcpy(token,"");
            lexIndex++;
            strcpy(mainStr,currentChar);
            strcpy(charClass,"BLANK_SPACE");
        }
        
    }
    
    else if(currentChar[0] == '#'){
        if(isBlankSpace()){
            inputIndex++;
            inputIndex++;
            inputIndex++;
            
            while(program[inputIndex] != '#'){
                comment_c[0] = program[inputIndex];
                strcat(comment_str,comment_c); 
                inputIndex++;
            }     
            
            inputIndex++;
            inputIndex++;

            // inputIndex+=2;       - Alternative
            
            strcpy(mainStr,comment_str);
            strcpy(charClass,"COMMENT");            
        }
        
        else{
            getClass();
            lexeme[lexIndex] = mainStr; 
            printf("lexeme %d: %s\t%s\n",lexIndex, lexeme[lexIndex], token);
            strcpy(token,"");
            lexIndex++;
            
            inputIndex++;
            inputIndex++;

            inputIndex++;
            
            while(program[inputIndex] != '#'){
                comment_c[0] = program[inputIndex];
                strcat(comment_str,comment_c); 
                inputIndex++;
            }     
            
            inputIndex++;
            inputIndex++;

            strcpy(mainStr,comment_str);
            strcpy(charClass,"COMMENT");
        }
    }
    
    else{
        printf("unknown character: %c\n", currentChar[0]);
    }

}

void myFunction(){   // gets input character
    while(inputIndex < strlen(program)-1){
        evalChar(program[inputIndex]); // if(x==c)
        inputIndex++;
    }
    
    if(inputIndex == strlen(program)-1){
        if(isBlankSpace()){}    
        else{
            getClass();
            lexeme[lexIndex] = mainStr; 
            printf("lexeme %d: %s\t%s\n",lexIndex, lexeme[lexIndex], token);
            strcpy(token,"");
        }
    }
    

}


int main()
{
    printf("Program: ");
    fgets(program,300,stdin);
    printf("%s\n", program);
    myFunction();

    return 0;
}