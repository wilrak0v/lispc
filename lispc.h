#ifndef LISPC_H
#define LISPC_H

#include <stdio.h>

enum target_platforms {
    ARM32,
    ARM64,
    X86_64,
    C
};

enum LispcType {
    INT,
    I8,
    I16,
    I32,
    U8,
    U16,
    U32,
    F32,
    F64,
    BOOL,
    STR
};

typedef struct {
    char *sinput;
    char *soutput;
    int target;
    FILE *input;
    FILE *output;
} compiler;

#endif
