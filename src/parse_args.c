#include "HashMap.h"
#include <string.h>

void _define_arg(HashMap *hm, char* arg) {
    char *key = strtok(arg, "=");
    char *value = strtok(NULL, "=");

    if (value == NULL) {
        value = calloc(3, sizeof(char));
        strcpy(value, "\"\"");
        hm->addValue(hm, key, value);
        free(value);
        return;
    }

    hm->addValue(hm, key, value);
    
}

void _include_arg(HashMap *hm, char *arg, char **includes, int *n_includes) {
    strcpy(includes[*n_includes], arg);
    includes[*n_includes][strlen(includes[*n_includes]) + 1] = '\0';
    includes[*n_includes][strlen(includes[*n_includes])] = '/';
    ++n_includes;
}

Bool parse_args(HashMap *hm, int argc, char **argv, char **includes, int *n_includes) {
    int i = 0;
    char *input_file = NULL;
    char *out_file = NULL;

    for (i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-D") == 0) {
            _define_arg(hm, argv[i + 1]);
            ++i;
            continue;
        }

        if (strcmp(argv[i], "-I") == 0) {
            _include_arg(hm, argv[i + 1], includes, n_includes);
            ++i;
            continue;
        }

        if (strcmp(argv[i], "-o") == 0) {
            out_file = argv[i + 1];
            ++i;
            continue;
        }

        if (argv[i][0] == '-' && argv[i][1] == 'D') {
            _define_arg(hm, argv[i] + 2);
            continue;
        }

        if (argv[i][0] == '-' && argv[i][1] == 'I') {
            _include_arg(hm, argv[i] + 2, includes, n_includes);
            continue;
        }

        if (argv[i][0] == '-' && argv[i][1] == 'o') {
            out_file = argv[i] + 2;
            continue;
        }

        input_file = argv[i];
    }
    
    hm->addValue(hm, "__INPUT_FILE__", (input_file) ? input_file : "stdin");
    hm->addValue(hm, "__OUTPUT_FILE__", (out_file) ? out_file : "stdout");

    return true;
}

