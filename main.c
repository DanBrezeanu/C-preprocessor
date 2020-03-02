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

Bool isstring(uint8t *value) {
    return (value[0] == '\"' && value[strlen(value) - 1] == '\"');
}

void find_key_value(HashMap *hm, uint8t *line, uint8t *key, uint8t *value, uint8t *directive) {
    if (strcmp(directive, "#define ") == 0)
        replace_existing_defines(hm, &line);
    uint8t *start = strstr(line, directive) + strlen(directive);
    
    //TODO: realloc if necessary
    int32t key_len = 0;
    int32t value_len = 0;

    while (!isspace(*start)) {
        key[key_len++] = *start;
        ++start;
    }

    while (*start && isspace(*start)) {
        ++start;
    }

    while (*start && *start != '\n') {
        value[value_len++] = *start;
        ++start;
    }
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
    uint8t *name, *value;
    uint8t directive[] = "#define ";

    name = calloc(MAX_BUFFER, sizeof(uint8t)); 
    value = calloc(MAX_BUFFER, sizeof(uint8t)); 

    find_key_value(hm, line, name, value, directive);

    if (strlen(value) == 0) {
        value[0] = value[1] = '\"';
        hm->addValue(hm, name, value);
    // } else if (!isstring(value)) {
    //     int32t real_value = evaluate_expression(value);
    //     uint8t *real_value_s = string_from_number(real_value);
    //     hm->addValue(hm, name, real_value_s);
    //     free(real_value_s);
        
    } else {
        hm->addValue(hm, name, value);
    }

    free(name);
    free(value);
}

void preprocess_undef_directive(HashMap *hm, uint8t *line) {
    uint8t *name, *value;
    uint8t directive[] = "#undef ";


    name = calloc(MAX_BUFFER, sizeof(uint8t)); 
    value = calloc(MAX_BUFFER, sizeof(uint8t)); 

    find_key_value(hm, line, name, value, directive);

    hm->remove(hm, name);

    free(name);
    free(value);
}

Bool preprocess_ifdef_directive(HashMap *hm, uint8t *line) {
    uint8t *name, *value;
    uint8t directive[] = "#ifdef ";

    name = calloc(MAX_BUFFER, sizeof(uint8t)); 
    value = calloc(MAX_BUFFER, sizeof(uint8t)); 

    printf("BEFORE FIND_K_V %s\n", line);
    find_key_value(hm, line, name, value, directive);
    printf("AFTER FIND_K_V %s\n", line);

    return hm->exists(hm, name) != -ENOEXISTS ? true : false;
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

uint8t** preprocess_file(HashMap *hm, int32t *final_lines_count) {
    int32t lines = 0;
    uint8t **file_content = read_file(hm, &lines);
    uint8t *tmp, *tmp_endif;
    int32t i = 0;
    
    int32t result_line = 0;
    uint8t **result_file = calloc(lines, sizeof(uint8t*));

    for (i = 0; i < lines; ++i) {
        if ((tmp = strstr(file_content[i], "#define")) && tmp != NULL && tmp == file_content[i]) {
            preprocess_define_directive(hm, file_content[i]);
            continue;
        }

        
        if ((tmp = strstr(file_content[i], "#undef")) && tmp != NULL && tmp == file_content[i]) {
            preprocess_undef_directive(hm, file_content[i]);
            continue;
        }

        if ((tmp = strstr(file_content[i], "#ifdef")) && tmp != NULL && tmp == file_content[i]) {
            Bool defined = preprocess_ifdef_directive(hm, file_content[i]);
            printf("%s: %d\n", file_content[i], defined);
            i++;

            while (strstr(file_content[i], "#endif") == NULL) {
                if (defined) {
                    result_file[result_line] = calloc(strlen(file_content[i]) + 1, sizeof(uint8t));
                    strcpy(result_file[result_line], file_content[i]);
                    ++result_line;
                }
                ++i;
            }
            continue;
        }

        result_file[result_line] = calloc(strlen(file_content[i]), sizeof(uint8t));
        strcpy(result_file[result_line++], file_content[i]);
    }

    for (i = 0; i < lines; ++i)
        free(file_content[i]);
    free(file_content);

    for (i = result_line; i < lines; ++i) {
        free(result_file[i]);
    }

    *final_lines_count = result_line;
    return result_file;
}


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