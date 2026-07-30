#include <stdio.h>
#include <unistd.h>
// TODO: make it compatible with Windows

typedef struct {
    char input[16];
    char ouptut[16];
    // TODO: add target language
} compiler;

int parse_args(int argc, char *argv[])
{
    if (argc < 1)
    {
        printf("Usage: %s [ flags ] <input-file>\n", argv[0]);
        return -1;
    }
    char flags[2] = {'o', 't'};
    for (int i = 1; i < argc - 1; ++i)
    {
        int current_flag = getopt(argc, argv, flags);
        switch (current_flag)
        {
        case 'o':
            printf("Output is %s\n", argv[optind++]);
            break;
        case 't':
            printf("Target is %s\n", argv[optind++]);
            break;
        case '?':
            printf("Error: unknown arguments '-%c'", optopt);
            return -1;
        }
    }
    char *input = argv[optind];
    printf("Input file: %s\n", input);
    return 69;
}

int main(int argc, char *argv[])
{
    if (parse_args(argc, argv) != 69)
        return 69;
    return 0;
}
