#include <stdio.h>
#include <unistd.h>
#include <string.h>
// TODO: make it compatible with Windows

enum target_platforms {
    ARM32,
    ARM64,
    X86_64,
    C
};

typedef struct {
    char *sinput;
    char *soutput;
    int target;
    FILE *input;
    FILE *output;
} compiler;

int get_target(char *starget)
{
    if (strcmp(starget, "arm32") == 0) return ARM32;
    if (strcmp(starget, "arm64") == 0) return ARM64;
    if (strcmp(starget, "x86_64") == 0) return X86_64;
    if (strcmp(starget, "C") == 0) return C;
    return -1;
}

int parse_args(int argc, char *argv[], compiler *comp)
{
    if (argc < 2)
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
            comp->soutput = argv[optind++];
            break;
        case 't':
            comp->target = get_target(argv[optind++]);
            if (comp->target == -1)
            {
                printf("Error: unknown target '%s'\n", argv[optind - 1]);
                // TODO: print supported targets
                return -1;
            }
            break;
        case '?':
            printf("Error: unknown arguments '-%c'\n", optopt);
            return -1;
        }
    }
    comp->sinput = argv[optind];
    return 69;
}

int main(int argc, char *argv[])
{
    compiler comp = {0};
    comp.soutput = "a.out";
    comp.target = C;
    if (parse_args(argc, argv, &comp) != 69) return 69;
    comp.input = fopen(comp.sinput, "r");
    comp.output = fopen(comp.soutput, "w");
    if (comp.input == NULL || comp.output == NULL)
    {
        perror("Error");
        fclose(comp.input);
        fclose(comp.output);
        return 69;
    }
    fclose(comp.input);
    return 0;
}
