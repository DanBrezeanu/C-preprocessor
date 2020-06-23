#include "HashMap.h"

uint8_t* strrep(uint8_t **string, uint8_t *old_str, uint8_t *new_str, int32_t offset);
Bool isstring(uint8_t *value);
void find_key_value(HashMap *hm, uint8_t **line, uint8_t *key, uint8_t *value, uint8_t *directive);
void clean_file(uint8_t ***content, int32_t *lines);
void replace_existing_defines(HashMap *hm, uint8_t **line);
void preprocess_define_directive(HashMap *hm, uint8_t *line);
void preprocess_undef_directive(HashMap *hm, uint8_t *line);
Bool preprocess_ifdef_directive(HashMap *hm, uint8_t *line, Bool is_ifndef);
Bool preprocess_ifndef_directive(HashMap *hm, uint8_t *line);
Bool preprocess_if_directive(HashMap *hm, uint8_t **line, Bool is_elif);
uint8_t **read_file(HashMap *hm, int32_t *lines, uint8_t *filename);
uint8_t** preprocess_file(HashMap *hm, int32_t *final_lines_count, uint8_t **include_dirs, int32_t n_includes);