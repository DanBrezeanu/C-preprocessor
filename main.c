#include "preprocessor.h"


int main(int argc, char** argv) {
    int lines = 0, i = 0;

    HashMap* hm = _HashMap_new();

    char **include_dirs = calloc(MAX_BUFFER, sizeof(char*));
    char **result;
    char *out_file;
    int n_includes = 0;
    FILE *f_out;

    for (i = 0; i < 1 << 10; ++i)
        include_dirs[i] = calloc(MAX_BUFFER, sizeof(char));

    parse_args(hm, argc - 1, argv + 1, include_dirs, &n_includes);

    result = preprocess_file(hm, &lines, include_dirs, n_includes);

    out_file = hm->getValue(hm, "__OUTPUT_FILE__");

    f_out = (strcmp(out_file, "stdout") == 0) ? stdout : fopen(out_file, "w");

    for (i = 0; i < lines; ++i) {
        fprintf(f_out, "%s", result[i]);
    }

    // hm->print(hm);
     

    hm->destroy(&hm);

    fclose(f_out);
    return 0;
}