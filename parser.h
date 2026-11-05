#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdint.h>
#include "lexer.h"
#include "lispc.h"

extern token_t current_token;

struct Variable {
    char name[64];
    enum LispcType type;
};

struct Result {
    enum LispcType expression_type;
    int make_semicolon;
};

typedef struct Variable var_t;

void emit_start(compiler *comp);
void advance(compiler *comp);
void expect(compiler *comp, enum TokenType type);
struct Result parse_list(compiler *comp);
void parse_atom(compiler *comp);
struct Result parse_expression(compiler *comp);

#endif
