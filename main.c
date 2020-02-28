#include <stdlib.h>
#include <stdio.h>
#include "HashMap.h"



int main(int argc, char** argv) {
    HashMap* hm = _HashMap_new();
    
    parse_args(hm, argc - 1, argv + 1);
    hm->print(hm);

    hm->destroy(&hm);
    return 0;
}