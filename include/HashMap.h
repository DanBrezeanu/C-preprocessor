#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include "int_types.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

typedef struct HashMap {
    unschart** keys;
    unschart** values;
    uint32t count_entries;
    uint32t capacity;

    Bool (*addValue) (struct HashMap *self, unschart *key, unschart *value);
    void (*print) (struct HashMap *self);
    unschart* (*getValue) (struct HashMap *self, unschart *key);
    unschart** (*getKeys) (struct HashMap *self);
    unschart** (*getValues) (struct HashMap *self);
    int32t (*exists) (struct HashMap *self, unschart *key);
    Bool (*remove) (struct HashMap *self, unschart *key);
    void (*destroy) (struct HashMap **self);

} HashMap;

HashMap* _HashMap_new();
Bool _HashMap_addValue(HashMap *self, unschart *key, unschart *value);
void _HashMap_print(HashMap *self);
unschart* _HashMap_getValue(HashMap *self, unschart *key);
unschart** _HashMap_getValues(HashMap *self);
unschart** _HashMap_getKeys(HashMap *self);
Bool _HashMap_remove(HashMap *self, unschart *key);
int32t _HashMap_exists(HashMap *self, unschart *key);
void _HashMap_destroy(HashMap **self);



#endif