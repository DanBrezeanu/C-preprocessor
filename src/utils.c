#include "utils.h"

inline void DIE(Bool exit_condition, int32_t error) {
    if (exit_condition)
        exit(error);
}

uint32_t number_from_string(uint8_t *string, int32_t *offset) {
    uint32_t result = 0;
    
    do {
        result = result * 10 + *string - '0';
        ++string; ++(*offset);
    } while (*string != '\0' && isdigit(*string));

    return result;
}

uint8_t *strrep(uint8_t **string, uint8_t *old_str, uint8_t *new_str, uint32_t offset) {
    uint8_t *start = strstr(*string + offset, old_str);
    uint8_t *result = calloc(1024, sizeof(uint8_t));

    if (start == NULL) {
        return NULL;
    }

    strncpy(result, *string, start - *string);
    strcat(result, new_str);
    strcat(result, start + strlen(old_str));

    free(*string);
    *string = result;

    return result;
}


uint8_t *string_from_number(int32_t number) {
    uint32_t number_digits = 0;
    uint32_t number_copy = number;
    uint8_t *result_rev, *result;
    int32_t i = 0;

    while(number_copy) {
        number_digits += 1;
        number_copy /= 10;
    }

    result_rev = calloc(number_digits + 3, sizeof(uint8_t));
    i = 0;
    
    while (number) {
        result_rev[i++] = '0' + number % 10;
        number /= 10;
    }

    result = calloc(number_digits + 3, sizeof(uint8_t));

    for (i = strlen(result_rev) - 1; i >= 0; --i) {
        result[strlen(result_rev) - i - 1] = result_rev[i];
    }

    free(result_rev);
    return result;
}

Bool is_empty_string(uint8_t *string) {
    uint32_t size = strlen(string);
    uint32_t i = 0;

    if (size == 0)
        return true;

    for (i = 0; i < size; ++i)
        if (string[i] != '\t' && string[i] != '\n' && string[i] != ' ' && string[i] != '\r')
            return false;

    return true;
}