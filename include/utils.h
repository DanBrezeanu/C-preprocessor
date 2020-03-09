#include "int_types.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void DIE(Bool exit_condition, int32t error);
int32t number_from_string(uint8t *string, int32t *offset);
int32t number_from_string(uint8t *string, int32t *offset);
uint8t* string_from_number(int32t number);
Bool is_empty_string(uint8t *string);
