#include "utils.h"

inline void DIE(Bool exit_condition, int32t error) {
    if (exit_condition)
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

uint8t* strrep(uint8t **string, uint8t *old_str, uint8t *new_str, int32t offset) {
    uint8t *start = strstr(*string + offset, old_str);

    if (start == NULL) {
        return NULL;
    }

    uint8t *result = calloc(1024, sizeof(uint8t));

    // printf("OLD: %s\n", *string);

    strncpy(result, *string, start - *string);
    strcat(result, new_str);
    strcat(result, start + strlen(old_str));

    // printf("NEW : %s\n", result);  /21 25


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

Bool is_empty_string(uint8t *string) {
    uint16t size = strlen(string);
    int32t i = 0;

    if (size == 0)
        return true;

    for (i = 0; i < size; ++i)
        if (string[i] != '\t' && string[i] != '\n' && string[i] != ' ' && string[i] != '\r')
            return false;

    return true;
}