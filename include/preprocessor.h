#include "HashMap.h"

uint8t* string_from_number(int32t number);
uint8t* strrep(uint8t **string, uint8t *old_str, uint8t *new_str);
Bool isstring(uint8t *value);
void find_key_value(HashMap *hm, uint8t *line, uint8t *key, uint8t *value, uint8t *directive);
void clean_file(uint8t ***content, int32t *lines);
void replace_existing_defines(HashMap *hm, uint8t **line);
void preprocess_define_directive(HashMap *hm, uint8t *line);
void preprocess_undef_directive(HashMap *hm, uint8t *line);
Bool preprocess_ifdef_directive(HashMap *hm, uint8t *line);
Bool preprocess_ifndef_directive(HashMap *hm, uint8t *line);
uint8t **read_file(HashMap *hm, int32t *lines);
uint8t** preprocess_file(HashMap *hm, int32t *final_lines_count);
