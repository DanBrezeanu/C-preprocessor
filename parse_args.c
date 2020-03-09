#include "int_types.h"
#include "HashMap.h"
#include <string.h>

void _define_arg(HashMap* hm, int8t* arg) {
    int8t *key = strtok(arg, "=");
    int8t *value = strtok(NULL, "=");

    if (value == NULL) {
        value = calloc(3, sizeof(int8t));
        strcpy(value, "\"\"");
        hm->addValue(hm, key, value);
        free(value);
        return;
    }

    hm->addValue(hm, key, value);
    
}

void _include_arg(HashMap *hm, int8t* arg, uint8t **includes, int32t *n_includes) {
    strcpy(includes[*n_includes], arg);
    includes[*n_includes][strlen(includes[*n_includes]) + 1] = '\0';
    includes[*n_includes][strlen(includes[*n_includes])] = '/';
    ++n_includes;
}

Bool parse_args(HashMap* hm, int32t argc, int8t** argv, uint8t **includes, int32t *n_includes) {
    int32t i = 0;
    int8t *input_file = NULL;
    int8t *out_file = NULL;

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

    // printf("input_file = %s\n", (input_file == NULL) ? "stdin" : input_file);
    hm->addValue(hm, "__INPUT_FILE__", (input_file) ? input_file : "stdin");

    // printf("outfile_file = %s\n", (out_file == NULL) ? "stdout" : out_file);
    hm->addValue(hm, "__OUTPUT_FILE__", (out_file) ? out_file : "stdout");

    return true;
}

