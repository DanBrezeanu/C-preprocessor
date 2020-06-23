#include "preprocessor.h"

int main(int32_t argc, char **argv) {
    uint32_t lines = 0, i = 0;

    HashMap *hm = _HashMap_new();

    uint8_t **include_dirs = calloc(MAX_BUFFER, sizeof(uint8_t *));
    uint8_t **result;
    uint8_t *out_file;
    uint32_t n_includes = 0;
    FILE *f_out;

    for (i = 0; i < 1 << 10; ++i)
        include_dirs[i] = calloc(MAX_BUFFER, sizeof(uint8_t));

    parse_args(hm, argc - 1, argv + 1, include_dirs, &n_includes);

    result = preprocess_file(hm, &lines, include_dirs, n_includes);

    out_file = hm->getValue(hm, "__OUTPUT_FILE__");

    f_out = (strcmp(out_file, "stdout") == 0) ? stdout : fopen(out_file, "w");

    for (i = 0; i < lines; ++i) {
        fprintf(f_out, "%s", result[i]);
    }     

    hm->destroy(&hm);

    fclose(f_out);
    return 0;
}