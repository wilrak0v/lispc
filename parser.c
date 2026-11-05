#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

token_t current_token;
int indent_level = 0;

void
print_indentations(int nb_space)
{
    for (int i = 0; i < indent_level; i++) {
        for (int j = 0; j < nb_space; j++) {
            printf(" ");
        }
    }
}


void
emit_start(compiler *comp)
{
    switch (comp->target) {
    case ARM32:
        printf("ERROR: arch not supported yet\n");
        exit(69);
        break;
    case ARM64:
        printf("ERROR: arch not supported yet\n");
        exit(69);
        break;
    case X86_64:
        printf("ERROR: arch not supported yet\n");
        exit(69);
        break;
    case C:
        printf("/* Wilrak0v Lispc\n * \n * Thanks for using it!\n */\n");
        printf("#include <stdio.h>\n#include <stdint.h>\n#include <stdbool.h>\n\n");
        break;
    default:
        break;
    }
}


void
advance(compiler *comp)
{
    current_token = get_next_token(comp->input);
}

void
expect(compiler *comp, enum TokenType type)
{
    if (current_token.type != type) {
        // TODO: make a better syntax error message
        fprintf(stderr, "Syntax error\n");
        fprintf(stderr, "Token expected: %d\n", type);
        fprintf(stderr, "Token received: %d\n", current_token.type);
        exit(69);
    }
    advance(comp);
}

enum LispcType
get_type(const char *str_type)
{
    if (strcmp(str_type, "int") == 0) {
        return INT;
    }
    else if (strcmp(str_type, "i8") == 0) {
        return I8;
    }
    else if (strcmp(str_type, "i16") == 0) {
        return I16;
    }
    else if (strcmp(str_type, "i32") == 0) {
        return I32;
    }
    else if (strcmp(str_type, "u8") == 0) {
        return U8;
    }
    else if (strcmp(str_type, "u16") == 0) {
        return U16;
    }
    else if (strcmp(str_type, "u32") == 0) {
        return U32;
    }
    else if (strcmp(str_type, "f32") == 0) {
        return F32;
    }
    else if (strcmp(str_type, "f64") == 0) {
        return F64;
    }
    else if (strcmp(str_type, "bool") == 0) {
        return BOOL;
    }
    else if (strcmp(str_type, "str") == 0) {
        return STR;
    }
    else {
        fprintf(stderr, "ERROR: unknown type '%s'\n", str_type);
        exit(69);
    }
    return I8;
}

void
emit_type(compiler *comp, enum LispcType type)
{
    switch (type) {
    case INT:
        printf("int");
        break;
    case I8:
        printf("int8_t");
        break;
    case I16:
        printf("int16_t");
        break;
    case I32:
        printf("int32_t");
        break;
    case U8:
        printf("uint8_t");
        break;
    case U16:
        printf("uint16_t");
        break;
    case U32:
        printf("uint32_t");
        break;
    case F32:
        printf("float");
        break;
    case F64:
        printf("double");
        break;
    case BOOL:
        printf("bool");
        break;
    case STR:
        printf("char*");
        break;
    default:
        printf("\nNot implemented yet\n");
        break;
    }
}

void
emit_operations(compiler *comp, const char *op)
{
    printf("(");
    int first = 1;
    while (current_token.type != RPARENT && current_token.type != EOF_TOKEN) {
        if (!first) {
            printf(" %s ", op);
        }
        parse_expression(comp);
        first = 0;
    }
    printf(")");
}

void
emit_variable(compiler *comp)
{
    if (current_token.type != IDENTIFIER) {
        fprintf(stderr, "ERROR: identifier expected after var\n");
        exit(69);
    }
    emit_type(comp, get_type(current_token.identifier));
    advance(comp);

    if (current_token.type != IDENTIFIER) {
        fprintf(stderr, "ERROR: identifier expected after type in var\n");
        exit(69);
    }
    // TODO: push variables
    // TODO: verify that the variable is assigned to the right type value
    char var_name[64];
    strncpy(var_name, current_token.identifier, 64);
    advance(comp);

    printf(" %s = ", var_name);

    parse_expression(comp);
}

struct Result
emit_if(compiler *comp)
{
    printf("if (");
    parse_expression(comp);
    printf(") {\n");

    indent_level++;

    expect(comp, LPARENT);
    while (current_token.type != RPARENT) {
        print_indentations(4);
        if (parse_expression(comp).make_semicolon) {
            printf(";");
        }
        printf("\n");
    }
    expect(comp, RPARENT);

    indent_level--;
    print_indentations(4);
    printf("} else {\n");
    indent_level++;

    expect(comp, LPARENT);
    while (current_token.type != RPARENT) {
        print_indentations(4);
        if (parse_expression(comp).make_semicolon) {
            printf(";");
        }
        printf("\n");
    }
    expect(comp, RPARENT);

    indent_level--;
    print_indentations(4);
    printf("}\n");
    expect(comp, RPARENT);
    return (struct Result) {
        .expression_type = I8,
        .make_semicolon = 0
    };
}

struct Result
emit_function(compiler *comp)
{
    if (current_token.type != IDENTIFIER) {
        fprintf(stderr, "ERROR: identifier expected after defn\n");
        exit(69);
    }
    emit_type(comp, get_type(current_token.identifier));
    advance(comp);

    if (current_token.type != IDENTIFIER) {
        fprintf(stderr, "ERROR: identifier expected after a function type\n");
        exit(69);
    }
    // TODO: check if there is no identifier(vars, etc) with the same name
    char func_name[64];
    strncpy(func_name, current_token.identifier, 64);
    advance(comp);
    expect(comp, LPARENT);
    printf("\n%s(", func_name);
    // TODO: make different types for the parameters
    int first_param = 1;
    while (current_token.type != RPARENT && current_token.type != EOF_TOKEN) {
        if (current_token.type != IDENTIFIER) {
            fprintf(stderr, "ERROR: identifier expected in a parameter list\n");
            exit(69);
        }
        if (!first_param) {
            printf(", ");
        }
        printf("int %s", current_token.identifier);
        advance(comp);
        first_param = 0;
    }
    expect(comp, RPARENT);
    printf(")\n{\n");
    indent_level++;
    while (current_token.type != RPARENT && current_token.type != EOF_TOKEN) {
        print_indentations(4);
        if (parse_expression(comp).make_semicolon) {
                printf(";");
        }
        printf("\n");
    }
    indent_level--;
    expect(comp, RPARENT);
    printf("}\n");
    return (struct Result) {
        .expression_type = I8,
        .make_semicolon = 0,
    };
}

struct Result
parse_list(compiler *comp)
{
    expect(comp, LPARENT);
    if (current_token.type != IDENTIFIER) {
        // TODO: make a better error message
        fprintf(stderr, "ERROR: identifier expected at the begining of a list\n");
        printf("Token Type: %d\n", current_token.type);
        exit(69);
    }
    char op[64];
    strncpy(op, current_token.identifier, 64);
    advance(comp);

    if (strcmp(op, "+") == 0 || strcmp(op, "*") == 0  || !strcmp(op, ">") || !strcmp(op, "<")) {
        emit_operations(comp, op);
    }
    else if (strcmp(op, "var") == 0) {
        emit_variable(comp);
    }
    else if (strcmp(op, "print") == 0) {
        printf("printf(");
        int first = 1;
        while (current_token.type != RPARENT && current_token.type != EOF_TOKEN) {
            if (!first) {
                printf(" ");
            }
            parse_expression(comp);
            first = 0;
        }
        printf(")");
    }
    else if (strcmp(op, "putint") == 0) {
        printf("printf(\"%%d\", ");
        parse_expression(comp);
        printf(")");
    }
    else if (strcmp(op, "return") == 0) {
        printf("return ");
        parse_expression(comp);
    }
    else if (strcmp(op, "if") == 0) {
        return emit_if(comp);
    }
    else if (strcmp(op, "defn") == 0) {
        return emit_function(comp);
    }
    else {
        printf("%s(", op);
        int first = 1;
        while (current_token.type != RPARENT && current_token.type != EOF_TOKEN) {
            if (!first) {
                printf(", ");
            }
            parse_expression(comp);
            first = 0;
        }
        printf(")");
    }
    expect(comp, RPARENT);
    return (struct Result) {
        .expression_type = I8,
        .make_semicolon = 1,
    };
}

void
parse_atom(compiler *comp)
{
    if (current_token.type == NUMBER) {
        printf("%d", current_token.number);
        advance(comp);
    }
    else if (current_token.type == STRING) {
        printf("\"%s\"", current_token.string.string);
        advance(comp);
    }
    else if (current_token.type == IDENTIFIER) {
        printf("%s", current_token.identifier);
        advance(comp);
    }
    else {
        printf("ERROR: unknown atom (type: %d)\n", current_token.type);
        exit(69);
    }
}

struct Result
parse_expression(compiler *comp)
{
    if (current_token.type == LPARENT) {
        return parse_list(comp);
    }
    else {
        parse_atom(comp);
    }
    return (struct Result) {
        .expression_type = I8,
        .make_semicolon = 0
    };
}
