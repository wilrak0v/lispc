#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

token_t
get_identifier(FILE *fp, char based_char)
{
    char c = '\0';
    int index = 1;
    char id_string[64];
    id_string[0] = based_char;
    token_t result = (token_t) {
        .type = IDENTIFIER,
        .identifier = "",
    };
    while (c != ' ' && c != EOF) {
        if (index > 63) {
            // TODO: print a better error message
            printf("Error: identifier too long\n");
            exit(69);
        }
        c = fgetc(fp);
        id_string[index] = c;
        ++index;
    }
    strncpy(result.identifier, id_string, 64);
    result.identifier[index - 1] = '\0';
    return result;
}

token_t
get_string(FILE *fp)
{
    // Find the string length
    long initial_position = ftell(fp);
    int size = 0;
    char next_char = 0;
    while (next_char != '"' && next_char != EOF) {
        next_char = fgetc(fp);
        ++size;
    }
    --size;
    // Save the string
    fseek(fp, initial_position, SEEK_SET);
    char *str = malloc(size + 1);
    fread(str, 1, size, fp);
    string_t str_struct = (string_t) {
        .string = str,
        .length = size,
    };
    fgetc(fp);
    return (token_t) {
        .type = STRING,
        .string = str_struct,
    };
}

token_t
get_number(FILE *fp, char current_char)
{
    char number_char = '0';
    char number_buffer[32];
    number_buffer[0] = current_char;
    int index = 1;
    while (number_char >= '0' && number_char <= '9' && number_char != EOF) {
        number_char = fgetc(fp);
        number_buffer[index] = number_char;
        ++index;
    }

    int number = atoi(number_buffer);
    return (token_t) {
        .type = NUMBER,
        .number = number,
    };
}

token_t
get_next_token(FILE *fp)
{
    char next_char = fgetc(fp);
    while ((next_char == ' ' || next_char == '\t' || next_char == '\r') && next_char != EOF) {
        next_char = fgetc(fp);
    }
    if (next_char == '(') {
        return (token_t) {
            .type = LPARENT,
            .number = 0,
        };
    }
    if (next_char == ')') {
        return (token_t) {
            .type = RPARENT,
            .number = 0,
        };
    }
    if (next_char == '"') {
        return get_string(fp);
    }
    if (next_char >= '0' && next_char <= '9') {
        return get_number(fp, next_char);
    }
    if (next_char == ';') {
        while (next_char != '\n') {
            next_char = fgetc(fp);
        }
        return get_next_token(fp);
    }
    if (next_char == '\n') {
        return get_next_token(fp);
    }
    if (next_char == EOF) {
        return (token_t) {
            .type = EOF_TOKEN,
            .number = -1,
        };
    }
    return get_identifier(fp, next_char);
}
