#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include "utils.h"
#include <stdio.h>
#include <errno.h>

typedef struct HashMap {
    uint8_t** keys;
    uint8_t** values;
    uint32_t count_entries;
    uint32_t capacity;

    Bool (*addValue) (struct HashMap *self, uint8_t *key, uint8_t *value);
    void (*print) (struct HashMap *self);
    uint8_t *(*getValue) (struct HashMap *self, uint8_t *key);
    uint8_t **(*getKeys) (struct HashMap *self);
    uint8_t **(*getValues) (struct HashMap *self);
    int32_t (*exists) (struct HashMap *self, uint8_t *key);
    Bool (*remove) (struct HashMap *self, uint8_t *key);
    void (*destroy) (struct HashMap **self);

} HashMap;

HashMap *_HashMap_new();
Bool _HashMap_addValue(HashMap *self, uint8_t *key, uint8_t *value);
void _HashMap_print(HashMap *self);
uint8_t *_HashMap_getValue(HashMap *self, uint8_t *key);
uint8_t **_HashMap_getValues(HashMap *self);
uint8_t **_HashMap_getKeys(HashMap *self);
Bool _HashMap_remove(HashMap *self, uint8_t *key);
int32_t _HashMap_exists(HashMap *self, uint8_t *key);
void _HashMap_destroy(HashMap **self);


#endif