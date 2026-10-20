#ifndef LEXER_H
#define LEXER_H

enum TokenType {
    LPARENT,
    RPARENT,
    NUMBER,
    STRING,
    IDENTIFIER,
    EOF_TOKEN
};

struct StringSlice {
    char *string;
    int length;
};

struct Token {
    enum TokenType type;
    union {
        int number;
        struct StringSlice string;
        char identifier[64];
    };
};

typedef struct Token token_t;
typedef struct StringSlice string_t;

token_t get_next_token(FILE *fp);


#endif
