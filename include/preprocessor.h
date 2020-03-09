#include "HashMap.h"

uint8t* string_from_number(int32t number);
uint8t* strrep(uint8t **string, uint8t *old_str, uint8t *new_str, int32t offset);
Bool isstring(uint8t *value);
void find_key_value(HashMap *hm, uint8t **line, uint8t *key, uint8t *value, uint8t *directive);
void clean_file(uint8t ***content, int32t *lines);
void replace_existing_defines(HashMap *hm, uint8t **line);
void preprocess_define_directive(HashMap *hm, uint8t *line);
void preprocess_undef_directive(HashMap *hm, uint8t *line);
Bool preprocess_ifdef_directive(HashMap *hm, uint8t *line, Bool is_ifndef);
Bool preprocess_ifndef_directive(HashMap *hm, uint8t *line);
Bool preprocess_if_directive(HashMap *hm, uint8t **line, Bool is_elif);
uint8t **read_file(HashMap *hm, int32t *lines, uint8t *filename);
uint8t** preprocess_file(HashMap *hm, int32t *final_lines_count, uint8t **include_dirs, int32t n_includes);