#include "int_types.h"
#include <stdlib.h>
#include <ctype.h>

inline void DIE(int32t error) {
    exit(error);
}

int32t number_from_string(uint8t *string, int32t *offset) {
    int32t result = 0;
    
    do {
        result = result * 10 + *string - '0';
        ++string; ++(*offset);
    } while (*string != '\0' && isdigit(*string));

    return result;
}