#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define MY_EOF -1

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
int inputIndex = 0;
char inputExpr[] = "( sum + 43 ) * total / total";

void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);
int lookup(char ch);
void printToken(int token, const char* lexeme);

int main() {
    printf("Lexical Analysis of: \"%s\"\n\n", inputExpr);

    getChar();

    do {
        lex();
    } while (nextToken != MY_EOF);

    printf("\nLexical analysis completed.\n");
    return 0;
}

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    } else {
        printf("Error - word is too long\n");
    }
}

void getChar() {
    if (inputIndex < strlen(inputExpr)) {
        nextChar = inputExpr[inputIndex++];

        if (isalpha(nextChar)) {
            charClass = LETTER;
        } else if (isdigit(nextChar)) {
            charClass = DIGIT;
        } else {
            charClass = UNKNOWN;
        }
    } else {
        charClass = MY_EOF;
        nextChar = '\0';
    }
}

void getNonBlank() {
    while (isspace(nextChar)) {
        getChar();
    }
}

int lookup(char ch) {
    int result;

    switch (ch) {
        case '(':
            addChar();
            result = LEFT_PAREN;
            break;
        case ')':
            addChar();
            result = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            result = ADD_OP;
            break;
        case '-':
            addChar();
            result = SUB_OP;
            break;
        case '*':
            addChar();
            result = MULT_OP;
            break;
        case '/':
            addChar();
            result = DIV_OP;
            break;
        case '=':
            addChar();
            result = ASSIGN_OP;
            break;
        default:
            addChar();
            result = MY_EOF;
            break;
    }

    return result;
}

void printToken(int token, const char* lexeme) {
    char* tokenType;

    switch (token) {
        case INT_LIT:
            tokenType = "INTEGER";
            break;
        case IDENT:
            tokenType = "IDENTIFIER";
            break;
        case ASSIGN_OP:
            tokenType = "ASSIGN_OP";
            break;
        case ADD_OP:
            tokenType = "ADD_OP";
            break;
        case SUB_OP:
            tokenType = "SUB_OP";
            break;
        case MULT_OP:
            tokenType = "MULT_OP";
            break;
        case DIV_OP:
            tokenType = "DIV_OP";
            break;
        case LEFT_PAREN:
            tokenType = "LEFT_PAREN";
            break;
        case RIGHT_PAREN:
            tokenType = "RIGHT_PAREN";
            break;
        case MY_EOF:
            tokenType = "EOF";
            break;
        default:
            tokenType = "UNKNOWN";
            break;
    }

    printf("Token: %-12s | Code: %-3d | Lexeme: %s\n", tokenType, token, lexeme);
}

int lex() {
    lexLen = 0;
    lexeme[0] = '\0';

    getNonBlank();

    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;

        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        case UNKNOWN:
            nextToken = lookup(nextChar);
            getChar();
            break;

        case MY_EOF:
            nextToken = MY_EOF;
            strcpy(lexeme, "EOF");
            break;
    }

    printToken(nextToken, lexeme);

    return nextToken;
}