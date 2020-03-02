#include "preprocessor.h"


int main(int argc, char** argv) {
    int32t lines = 0, i = 0;

    HashMap* hm = _HashMap_new();
    
    parse_args(hm, argc - 1, argv + 1);

    uint8t **result = preprocess_file(hm, &lines);

    for (i = 0; i < lines; ++i) {
        printf("%s", result[i]);
    }

    hm->print(hm);
     

    hm->destroy(&hm);
    return 0;
}