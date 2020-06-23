#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>

#define ENOEXISTS 0x1
#define MAX_BUFFER 1 << 10

typedef enum Bool {
    false = 0,
    true = 1
} Bool;

void DIE(Bool exit_condition, int32_t error);
uint32_t number_from_string(uint8_t *string, int32_t* offset);
uint8_t* string_from_number(int32_t number);
Bool is_empty_string(uint8_t *string);



