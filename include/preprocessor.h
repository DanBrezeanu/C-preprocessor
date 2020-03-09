#include "HashMap.h"

unschart* string_from_number(int32t number);
unschart* strrep(unschart **string, unschart *old_str, unschart *new_str, int32t offset);
Bool isstring(unschart *value);
void find_key_value(HashMap *hm, unschart **line, unschart *key, unschart *value, unschart *directive);
void clean_file(unschart ***content, int32t *lines);
void replace_existing_defines(HashMap *hm, unschart **line);
void preprocess_define_directive(HashMap *hm, unschart *line);
void preprocess_undef_directive(HashMap *hm, unschart *line);
Bool preprocess_ifdef_directive(HashMap *hm, unschart *line, Bool is_ifndef);
Bool preprocess_ifndef_directive(HashMap *hm, unschart *line);
Bool preprocess_if_directive(HashMap *hm, unschart **line, Bool is_elif);
unschart **read_file(HashMap *hm, int32t *lines, unschart *filename);
unschart** preprocess_file(HashMap *hm, int32t *final_lines_count, unschart **include_dirs, int32t n_includes);