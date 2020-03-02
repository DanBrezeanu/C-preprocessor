#include "utils.h"

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

uint8t* strrep(uint8t **string, uint8t *old_str, uint8t *new_str) {
    uint8t *start = strstr(*string, old_str);

    if (start == NULL) {
        return NULL;
    }

    uint8t *result = calloc(strlen(*string) - strlen(old_str) + strlen(new_str) + 1, sizeof(uint8t));

    strncpy(result, *string, start - *string);
    strcat(result, new_str);
    strcat(result, start + strlen(old_str));

    free(*string);
    *string = result;

    return result;
}


uint8t* string_from_number(int32t number) {
    int32t number_digits = 0;
    int32t number_copy = number;
    int32t reversed_number =  0;

    while(number_copy) {
        number_digits += 1;
        number_copy /= 10;
    }

    uint8t *result_rev = calloc(number_digits + 3, sizeof(uint8t));
    int32t i = 0;
    
    while (number) {
        result_rev[i++] = '0' + number % 10;
        number /= 10;
    }

    uint8t *result = calloc(number_digits + 3, sizeof(uint8t));

    for (i = strlen(result_rev) - 1; i >= 0; --i) {
        result[strlen(result_rev) - i - 1] = result_rev[i];
    }

    free(result_rev);
    return result;
}