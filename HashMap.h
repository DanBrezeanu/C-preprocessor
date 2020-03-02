#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include "int_types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct HashMap {
    uint8t** keys;
    uint8t** values;
    uint32t count_entries;
    uint32t capacity;

    Bool (*addValue) (struct HashMap *self, uint8t *key, uint8t *value);
    void (*print) (struct HashMap *self);
    uint8t* (*getValue) (struct HashMap *self, uint8t *key);
    uint8t** (*getKeys) (struct HashMap *self);
    uint8t** (*getValues) (struct HashMap *self);
    int32t (*exists) (struct HashMap *self, uint8t *key);
    Bool (*remove) (struct HashMap *self, uint8t *key);
    void (*destroy) (struct HashMap **self);

} HashMap;

HashMap* _HashMap_new();
Bool _HashMap_addValue(HashMap *self, uint8t *key, uint8t *value);
void _HashMap_print(HashMap *self);
uint8t* _HashMap_getValue(HashMap *self, uint8t *key);
uint8t** _HashMap_getValues(HashMap *self);
uint8t** _HashMap_getKeys(HashMap *self);
Bool _HashMap_remove(HashMap *self, uint8t *key);
int32t _HashMap_exists(HashMap *self, uint8t *key);
void _HashMap_destroy(HashMap **self);



#endif