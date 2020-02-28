#include "int_types.h"
#include "HashMap.h"
#include <string.h>

void _define_arg(HashMap* hm, int8t* arg) {
    int8t *key = strtok(arg, "=");
    int8t *value = strtok(NULL, "=");

    if (value == NULL) {
        value = calloc(3, sizeof(int8t));
        strcpy(value, "\"\"");
    }

    hm->addValue(hm, key, value);
}

void _include_arg(HashMap *hm, int8t* arg) {
    return;
}

Bool parse_args(HashMap* hm, int32t argc, int8t** argv) {
    int32t i = 0;
    int8t *input_file = NULL;

    for (i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-D") == 0) {
            _define_arg(hm, argv[i + 1]);
            ++i;
            continue;
        }

        if (strcmp(argv[i], "-I") == 0) {
            _include_arg(hm, argv[i + 1]);
            ++i;
            continue;
        }

        if (strcmp(argv[i], "-o") == 0) {
            printf("output_file = %s\n", argv[i + 1]);
            ++i;
            continue;
        }

        input_file = argv[i];
    }

    printf("input_file = %s\n", (input_file == NULL) ? "stdin" : input_file);


    return true;
}

