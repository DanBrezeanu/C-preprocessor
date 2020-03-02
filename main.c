#include <stdlib.h>
#include <stdio.h>
#include "HashMap.h"
#include <ctype.h>

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

uint8t* strrep(uint8t **string, uint8t *old_str, uint8t *new_str) {
    uint8t *start = strstr(*string, old_str);
    
    printf("OLD = %s\n", *string);

    if (start == NULL) {
        return NULL;
    }

    uint8t *result = calloc(strlen(*string) - strlen(old_str) + strlen(new_str) + 1, sizeof(uint8t));

    strncpy(result, *string, start - *string);
    strcat(result, new_str);
    strcat(result, start + strlen(old_str));

    printf("NEW = %s\n", result);

    free(*string);
    *string = result;

    return result;
}

Bool isstring(uint8t *value) {
    return (value[0] == '\"' && value[strlen(value) - 1] == '\"');
}

void clean_file(uint8t ***content, int32t *lines) {
    uint8t **new_content = calloc(*lines + 2, sizeof(uint8t*));
    int32t i = 0, j = 0;
    int32t k = 0, p = 0;
    int32t result_lines = *lines;
    Bool new_line = false;
    
    for (i = 0; i < *lines; ++i)
        new_content[i] = calloc(1024, sizeof(uint8t));


    for (i = 0; i < *lines; ++i) {
        uint16t len = strlen((*content)[i]);
        if (new_line) {
            ++k;
            p = 0;
        }
        new_line = true;

        for (j = 0; j < len; ++j, ++p) {
            if ((*content)[i][j] == '\\' && (*content)[i][j + 1] == '\n') {
                new_line = false;
                --result_lines;
                break;
            }

            new_content[k][p] = (*content)[i][j];
        }
    }

    for (i = 0; i < *lines; ++i)
        free((*content)[i]);
    free(*content);

    *content = new_content;
    *lines = result_lines;
}

void replace_existing_defines(HashMap *hm, uint8t **line) {
    uint8t **keys = hm->getKeys(hm);
    uint8t **values = hm->getValues(hm);
    uint8t *substr = NULL;
    int32t i = 0;

    for (i = 0; i < hm->count_entries; ++i) {
        if ((substr = strstr(*line, keys[i])) && substr != NULL) {
            strrep(line, keys[i], values[i]);
        }
    } 
}

void preprocess_define_directive(HashMap *hm, uint8t *line) {
    
    replace_existing_defines(hm, &line);
    uint8t *start = strstr(line, "#define ") + strlen("#define ");
    
    //TODO: realloc if necessary
    uint8t *name = calloc(1024, sizeof(uint8t));
    int32t name_len = 0;

    uint8t *value = calloc(1024, sizeof(uint8t));
    int32t value_len = 0;

    while (!isspace(*start)) {
        name[name_len++] = *start;
        ++start;
    }

    while (*start && isspace(*start)) {
        ++start;
    }

    while (*start && *start != '\n') {
        value[value_len++] = *start;
        ++start;
    }


    if (strlen(value) == 0) {
        value[0] = value[1] = '\"';
        hm->addValue(hm, name, value);
    } else if (!isstring(value)) {
        int32t real_value = evaluate_expression(value);
        uint8t *real_value_s = string_from_number(real_value);
        hm->addValue(hm, name, real_value_s);
        free(real_value_s);
        
    } else {
        hm->addValue(hm, name, value);
    }

    free(name);
    free(value);
}


uint8t **read_file(HashMap *hm, int32t *lines) {
    FILE *fd = fopen(hm->getValue(hm, "__INPUT_FILE__"), "rt");
    int32t buffer_max = 1 << 10;

    //TODO: realloc if necessary
    uint8t **file_content = calloc(buffer_max, sizeof(uint8t*));
    int32t ret = 1;

    while(ret) {
        file_content[*lines] = calloc(buffer_max, sizeof(uint8t));
        ret = fgets(file_content[*lines], buffer_max, fd);
        (*lines)++;
    }

    clean_file(&file_content, lines);

    return file_content;   
}

void preprocess_file(HashMap *hm) {
    int32t lines = 0;
    uint8t **file_content = read_file(hm, &lines);
    uint8t *tmp;
    int32t i = 0;

    for (i = 0; i < lines; ++i) {
        printf("%s", file_content[i]);
    }

    for (i = 0; i < lines; ++i) {
        if ((tmp = strstr(file_content[i], "#define")) && tmp != NULL && tmp == file_content[i]) {
            preprocess_define_directive(hm, file_content[i]);
        }

        
        if ((tmp = strstr(file_content[i], "#undef")) && tmp != NULL && tmp == file_content[i]) {
            preprocess_define_directive(hm, file_content[i]);
        }
    }
}


int main(int argc, char** argv) {
    HashMap* hm = _HashMap_new();
    
    parse_args(hm, argc - 1, argv + 1);

    preprocess_file(hm);

    hm->print(hm);
     

    hm->destroy(&hm);
    return 0;
}