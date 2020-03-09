#include "utils.h"
// inline void DIE(Bool exit_condition, int error) {
//     if (exit_condition)
//         exit(error);
// }

int number_from_string(char *string, int *offset) {
    int result = 0;
    
    do {
        result = result * 10 + *string - '0';
        ++string; ++(*offset);
    } while (*string != '\0' && isdigit(*string));

    return result;
}

char* strrep(char **string, char *old_str, char *new_str, int offset) {
    char *start = strstr(*string + offset, old_str);
    char *result = calloc(1024, sizeof(char));

    if (start == NULL) {
        return NULL;
    }


    // printf("OLD: %s\n", *string);

    strncpy(result, *string, start - *string);
    strcat(result, new_str);
    strcat(result, start + strlen(old_str));

    // printf("NEW : %s\n", result);  /21 25


    free(*string);
    *string = result;

    return result;
}


char* string_from_number(int number) {
    int number_digits = 0;
    int number_copy = number;
    int reversed_number =  0;
    char *result_rev, *result;
    int i = 0;

    while(number_copy) {
        number_digits += 1;
        number_copy /= 10;
    }

    result_rev = calloc(number_digits + 3, sizeof(char));
    i = 0;
    
    while (number) {
        result_rev[i++] = '0' + number % 10;
        number /= 10;
    }

    result = calloc(number_digits + 3, sizeof(char));

    for (i = strlen(result_rev) - 1; i >= 0; --i) {
        result[strlen(result_rev) - i - 1] = result_rev[i];
    }

    free(result_rev);
    return result;
}

Bool is_empty_string(char *string) {
    int size = strlen(string);
    int i = 0;

    if (size == 0)
        return true;

    for (i = 0; i < size; ++i)
        if (string[i] != '\t' && string[i] != '\n' && string[i] != ' ' && string[i] != '\r')
            return false;

    return true;
}