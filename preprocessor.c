#include "preprocessor.h"

Bool isstring(char *value) {
    return (value[0] == '\"' && value[strlen(value) - 1] == '\"');
}

Bool define_in_string(char *line, int index) {
    Bool result1 = false, result2 = false;
    int i = 0;

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

void find_key_value(HashMap *hm, char **line, char *key, char *value, char *directive) {
    char *start;
    int key_len = 0;
    int value_len = 0;

    if (strcmp(directive, "#define ") == 0 
        || strcmp(directive, "#if ") == 0
        || strcmp(directive, "#elif ") == 0)
        replace_existing_defines(hm, line);
    
    start = strstr(*line, directive) + strlen(directive);
    
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

void clean_file(char ***content, int *lines) {
    char **new_content = calloc(*lines + 2, sizeof(char*));
    int i = 0, j = 0;
    int k = 0, p = 0;
    int result_lines = *lines;
    Bool new_line = false;
    int len;
    
    for (i = 0; i < *lines; ++i)
        new_content[i] = calloc(1024, sizeof(char));


    for (i = 0; i < *lines; ++i) {
        len = strlen((*content)[i]);
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

void replace_existing_defines(HashMap *hm, char **line) {
    char **keys = hm->getKeys(hm);
    char **values = hm->getValues(hm);
    char *substr_old = *line;
    char *substr_new = NULL;
    int i = 0;

    // TODO: find multiple defines on one line

    for (i = 0; i < hm->count_entries; ++i) {
        substr_new = strstr(substr_old, keys[i]);

        while (substr_new != NULL) {
            if (!define_in_string(*line, (int)(substr_new - *line)))
                strrep(line, keys[i], values[i], (int)(substr_new - *line));
            substr_old = substr_new + strlen(keys[i]);
            substr_new = strstr(substr_old, keys[i]);
        }
    } 
}

void preprocess_define_directive(HashMap *hm, char *line) {
    char *name, *value;
    char directive[] = "#define ";

    name = calloc(MAX_BUFFER, sizeof(char)); 
    value = calloc(MAX_BUFFER, sizeof(char)); 

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

void preprocess_undef_directive(HashMap *hm, char *line) {
    char *name, *value;
    char directive[] = "#undef ";

    name = calloc(MAX_BUFFER, sizeof(char)); 
    value = calloc(MAX_BUFFER, sizeof(char)); 

    find_key_value(hm, &line, name, value, directive);

    hm->remove(hm, name);

    free(name);
    free(value);
}

Bool preprocess_ifdef_directive(HashMap *hm, char *line, Bool is_ifndef) {
    char *name, *value;
    char *directive = calloc(10, sizeof(char));
    Bool result;

    if (is_ifndef)
        strcpy(directive, "#ifndef ");
    else
        strcpy(directive, "#ifdef ");
    name = calloc(MAX_BUFFER, sizeof(char)); 
    value = calloc(MAX_BUFFER, sizeof(char)); 

    find_key_value(hm, &line, name, value, directive);

    result = hm->exists(hm, name) != -ENOEXISTS;
    return (is_ifndef) ? (!result) : (result);
}

Bool preprocess_if_directive(HashMap *hm, char **line, Bool is_elif) {
    char *name, *value;
    char *directive = calloc(10, sizeof(char));
    Bool result;
    int real_value;

    if (is_elif)
        strcpy(directive, "#elif ");
    else
        strcpy(directive, "#if ");

    name = calloc(MAX_BUFFER, sizeof(char)); 
    value = calloc(MAX_BUFFER, sizeof(char)); 

    find_key_value(hm, line, name, value, directive);

    if (!isstring(name)) {
        real_value = evaluate_expression(name);
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

void insert_lines(char ***lines, int *total_lines, int current_line, char **to_insert, int n_to_insert) {
    int i = 0;

    char **result_lines = calloc(*total_lines + n_to_insert - 1, sizeof(char*));

    for (i = 0; i < *total_lines + n_to_insert - 1; ++i)
        result_lines[i] = calloc(MAX_BUFFER, sizeof(char*));

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

Bool preprocess_include_directive(HashMap *hm, char ***lines, int *total_lines, int current_line, char **include_dirs, int n_includes) {
    char *name, *value, *filename;
    char directive[] = "#include ";
    Bool result = true;
    int i = 0;

    FILE *fd = NULL;
    char **include_lines = NULL;
    int include_n_lines = 0;
    int tmp;

    name = calloc(MAX_BUFFER, sizeof(char)); 
    value = calloc(MAX_BUFFER, sizeof(char)); 
    filename = calloc(MAX_BUFFER, sizeof(char)); 

    find_key_value(hm, &((*lines)[current_line]), name, value, directive);

    tmp = strlen(name);
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

char **read_file(HashMap *hm, int *lines, char *filename) {
    char *in_file;
    FILE *fd;
    int buffer_max = 1 << 10;
    char **file_content;
    char *ret;
    int i = 0;

    if (filename == NULL) {
       in_file = hm->getValue(hm, "__INPUT_FILE__");
    } else {
        in_file = filename;
    }

    fd = (strcmp(in_file, "stdin") == 0) ? stdin : fopen(in_file, "rt");
    // DIE(fd == NULL, -1);

    //TODO: realloc if necessary
    file_content = calloc(buffer_max, sizeof(char*));
    ret = (char*)0x1; 

    while(ret) {
        file_content[*lines] = calloc(buffer_max, sizeof(char));
        ret = fgets(file_content[*lines], buffer_max, fd);
        (*lines)++;
    }

    for (i = *lines; i <= buffer_max; ++i)
        free(file_content[i]);

    return file_content;   
}

char** preprocess_file(HashMap *hm, int *final_lines_count, char **include_dirs, int n_includes) {
    int lines = 0;
    char **file_content;
    char *tmp, *tmp_endif;
    int i = 0;
    Bool scans_line = true;
    
    int result_line = 0;
    char **result_file = calloc(lines, sizeof(char*));
    
    file_content = read_file(hm, &lines, NULL);

    clean_file(&file_content, &lines);

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
                result_file[result_line] = calloc(strlen(file_content[i]), sizeof(char));
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