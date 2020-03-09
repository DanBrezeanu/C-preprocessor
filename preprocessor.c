#include "preprocessor.h"

Bool isstring(uint8t *value) {
    return (value[0] == '\"' && value[strlen(value) - 1] == '\"');
}

Bool define_in_string(uint8t *line, int32t index) {
    Bool result1 = false, result2 = false;
    int32t i = 0;

    for (i = index; i >= 0; --i) {
        if (line[i] == '\"') {
            result1 = true;
            break;
        }
    }

    for (i = index; i < strlen(line); ++i) {
        if (line[i] == '\"') {
            result2 = true;
            break;
        }
    }

    return (result1 && result2);
}

void find_key_value(HashMap *hm, uint8t **line, uint8t *key, uint8t *value, uint8t *directive) {

    if (strcmp(directive, "#define ") == 0 
        || strcmp(directive, "#if ") == 0
        || strcmp(directive, "#elif ") == 0)
        replace_existing_defines(hm, line);
    
    uint8t *start = strstr(*line, directive) + strlen(directive);
    
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
    uint8t *substr_old = *line;
    uint8t *substr_new = NULL;
    int32t i = 0;

    // TODO: find multiple defines on one line

    for (i = 0; i < hm->count_entries; ++i) {
        substr_new = strstr(substr_old, keys[i]);

        while (substr_new != NULL) {
            if (!define_in_string(*line, (int32t)(substr_new - *line)))
                strrep(line, keys[i], values[i], (int32t)(substr_new - *line));
            substr_old = substr_new + strlen(keys[i]);
            substr_new = strstr(substr_old, keys[i]);
        }
    } 
}

void preprocess_define_directive(HashMap *hm, uint8t *line) {
    uint8t *name, *value;
    uint8t directive[] = "#define ";

    name = calloc(MAX_BUFFER, sizeof(uint8t)); 
    value = calloc(MAX_BUFFER, sizeof(uint8t)); 

    find_key_value(hm, &line, name, value, directive);

    if (strlen(value) == 0) {
        value[0] = value[1] = '\"';
        hm->addValue(hm, name, value);        
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

    find_key_value(hm, &line, name, value, directive);

    hm->remove(hm, name);

    free(name);
    free(value);
}

Bool preprocess_ifdef_directive(HashMap *hm, uint8t *line, Bool is_ifndef) {
    uint8t *name, *value;
    uint8t *directive = calloc(10, sizeof(uint8t));
    if (is_ifndef)
        strcpy(directive, "#ifndef ");
    else
        strcpy(directive, "#ifdef ");
    name = calloc(MAX_BUFFER, sizeof(uint8t)); 
    value = calloc(MAX_BUFFER, sizeof(uint8t)); 

    find_key_value(hm, &line, name, value, directive);

    Bool result = hm->exists(hm, name) != -ENOEXISTS;
    return (is_ifndef) ? (!result) : (result);
}

Bool preprocess_if_directive(HashMap *hm, uint8t **line, Bool is_elif) {
    uint8t *name, *value;
    uint8t *directive = calloc(10, sizeof(uint8t));
    if (is_elif)
        strcpy(directive, "#elif ");
    else
        strcpy(directive, "#if ");
    Bool result;

    name = calloc(MAX_BUFFER, sizeof(uint8t)); 
    value = calloc(MAX_BUFFER, sizeof(uint8t)); 

    find_key_value(hm, line, name, value, directive);

    if (!isstring(name)) {
        int32t real_value = evaluate_expression(name);
        result = (real_value != 0) ? true : false;
    } else if (strcmp(name, "\"\"") != 0) {
        result = true;
    } else {
        result = false;
    }

    free(name);
    free(value);
    free(directive);

    return result;
}

void insert_lines(uint8t ***lines, int32t *total_lines, int32t current_line, uint8t **to_insert, int32t n_to_insert) {
    int32t i = 0;

    uint8t **result_lines = calloc(*total_lines + n_to_insert - 1, sizeof(uint8t*));

    for (i = 0; i < *total_lines + n_to_insert - 1; ++i)
        result_lines[i] = calloc(MAX_BUFFER, sizeof(uint8t*));

    for (i = 0; i < current_line; ++i) {
        strcpy(result_lines[i], (*lines)[i]);
    }

    for (i = 0; i < n_to_insert; ++i) {
        strcpy(result_lines[i + current_line], to_insert[i]);
    }

    for (i = current_line + 1; i < *total_lines; ++i) {
        strcpy(result_lines[n_to_insert + i - 1], (*lines)[i]);
    }


    for (i = 0; i < *total_lines; ++i) {
        free((*lines)[i]);
    }
    free(*lines);

    *lines = result_lines;
    *total_lines = *total_lines + n_to_insert - 1;

}

Bool preprocess_include_directive(HashMap *hm, uint8t ***lines, int32t *total_lines, int32t current_line, uint8t **include_dirs, int32t n_includes) {
    uint8t *name, *value, *filename;
    uint8t directive[] = "#include ";
    Bool result = true;
    int32t i = 0;

    FILE *fd = NULL;
    uint8t **include_lines = NULL;
    int32t include_n_lines = 0;

    name = calloc(MAX_BUFFER, sizeof(uint8t)); 
    value = calloc(MAX_BUFFER, sizeof(uint8t)); 
    filename = calloc(MAX_BUFFER, sizeof(uint8t)); 


    find_key_value(hm, &((*lines)[current_line]), name, value, directive);

    int32t tmp = strlen(name);
    strncpy(name, name + 1, tmp - 1);
    name[tmp - 2] = '\0';

    fd = fopen(name, "r");
    if (fd != NULL) {
        strcpy(filename, name);
        include_lines = read_file(hm, &include_n_lines, filename);
    } else {
        for (i = 0; i < n_includes; ++i) {
            fd = fopen(strcat(include_dirs[i], name), "r");

            if (fd != NULL) {
                strcpy(filename, strcat(include_dirs[i], name));
                include_lines = read_file(hm, &include_n_lines, filename);
            }
        }
    }

    // DIE(include_dirs == NULL, -1);

    insert_lines(lines, total_lines, current_line, include_lines, include_n_lines);

    free(name);
    free(value);
}

uint8t **read_file(HashMap *hm, int32t *lines, uint8t *filename) {
    uint8t *in_file;
    if (filename == NULL) {
       in_file = hm->getValue(hm, "__INPUT_FILE__");
    } else {
        in_file = filename;
    }

    FILE *fd = (strcmp(in_file, "stdin") == 0) ? stdin : fopen(in_file, "rt");
    // DIE(fd == NULL, -1);
    int32t buffer_max = 1 << 10;

    //TODO: realloc if necessary
    uint8t **file_content = calloc(buffer_max, sizeof(uint8t*));
    uint8t *ret = (uint8t*)0x1; 
    int32t i = 0;

    while(ret) {
        file_content[*lines] = calloc(buffer_max, sizeof(uint8t));
        ret = fgets(file_content[*lines], buffer_max, fd);
        (*lines)++;
    }

    for (i = *lines; i <= buffer_max; ++i)
        free(file_content[i]);

    return file_content;   
}

uint8t** preprocess_file(HashMap *hm, int32t *final_lines_count, uint8t **include_dirs, int32t n_includes) {
    int32t lines = 0;
    uint8t **file_content = read_file(hm, &lines, NULL);
    clean_file(&file_content, &lines);
    uint8t *tmp, *tmp_endif;
    int32t i = 0;
    Bool scans_line = true;
    
    int32t result_line = 0;
    uint8t **result_file = calloc(lines, sizeof(uint8t*));
    

    for (i = 0; i < lines; ++i) {
        if ((tmp = strstr(file_content[i], "#include")) && tmp != NULL && tmp == file_content[i]) {
            preprocess_include_directive(hm, &file_content, &lines, i, include_dirs, n_includes);
            --i;
            goto next;
        }

        if ((tmp = strstr(file_content[i], "#endif")) && tmp != NULL && tmp == file_content[i]) {
            scans_line = true;
            goto next;
        }

        if ((tmp = strstr(file_content[i], "#elif")) && tmp != NULL && tmp == file_content[i]) {
            scans_line = preprocess_if_directive(hm, &file_content[i], true);
            goto next;
        }

        if ((tmp = strstr(file_content[i], "#else")) && tmp != NULL && tmp == file_content[i]) {
            scans_line = !scans_line;
            goto next;
        }

        if (scans_line) {
            if ((tmp = strstr(file_content[i], "#define")) && tmp != NULL && tmp == file_content[i]) {
                preprocess_define_directive(hm, file_content[i]);
                goto next;
            }
        
            if ((tmp = strstr(file_content[i], "#undef")) && tmp != NULL && tmp == file_content[i]) {
                preprocess_undef_directive(hm, file_content[i]);
                goto next;
            }

            if ((tmp = strstr(file_content[i], "#ifdef")) && tmp != NULL && tmp == file_content[i]) {
                scans_line = preprocess_ifdef_directive(hm, file_content[i], false);
                goto next;
            }

            if ((tmp = strstr(file_content[i], "#ifndef")) && tmp != NULL && tmp == file_content[i]) {
                scans_line = preprocess_ifdef_directive(hm, file_content[i], true);
                goto next;
            }
        

            if ((tmp = strstr(file_content[i], "#if ")) && tmp != NULL && tmp == file_content[i]) {
                scans_line = preprocess_if_directive(hm, &file_content[i], false);
                goto next;
            }

            replace_existing_defines(hm, &file_content[i]);
            if (!is_empty_string(file_content[i])) {
                result_file[result_line] = calloc(strlen(file_content[i]), sizeof(uint8t));
                strcpy(result_file[result_line++], file_content[i]);
            }
        }

next:
    continue;
    }

    // for (i = 0; i < lines; ++i) {
    //     if (file_content[i] != NULL) {
    //         free(file_content[i]);
    //         file_content[i] = NULL;
    //     }
    // }

    free(file_content);
    *final_lines_count = result_line;
    return result_file;
}