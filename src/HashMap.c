#include "HashMap.h"

HashMap* _HashMap_new() {
    HashMap *hm = calloc(1, sizeof(HashMap));

    hm->addValue = &_HashMap_addValue;
    hm->print = &_HashMap_print;
    hm->getValue = &_HashMap_getValue;
    hm->getKeys = &_HashMap_getKeys;
    hm->getValues = &_HashMap_getValues;
    hm->exists = &_HashMap_exists;
    hm->remove = &_HashMap_remove;
    hm->destroy = &_HashMap_destroy;

    hm->capacity = 32;

    hm->keys = calloc(hm->capacity, sizeof(uint8_t*));
    hm->values = calloc(hm->capacity, sizeof(uint8_t*));

    DIE(hm->keys == NULL || hm->values == NULL, ENOMEM);

    return hm;   
}

Bool _HashMap_addValue(HashMap *self, uint8_t *key, uint8_t *value) {
    int32_t existing_index;

    existing_index = self->exists(self, key);

    if (existing_index != -ENOEXISTS) {
        self->keys[existing_index] = realloc(self->keys[existing_index], (strlen(key) + 1) * sizeof(uint8_t));
        self->values[existing_index] = realloc(self->values[existing_index], (strlen(value) + 1) * sizeof(uint8_t));

        DIE(self->keys[existing_index] == NULL || self->values[existing_index] == NULL, ENOMEM);
        
        strcpy(self->keys[existing_index], key);
        strcpy(self->values[existing_index], value);

        return true;
    }

    if (self->count_entries == self->capacity) {
        self->capacity <<= 1;
        self->keys = realloc(self->keys, self->capacity * sizeof(uint8_t*));
        self->values = realloc(self->values, self->capacity * sizeof(uint8_t*));
        
        DIE(self->keys == NULL || self->values == NULL, ENOMEM);
    }

    self->keys[self->count_entries] = calloc(strlen(key) + 1, sizeof(uint8_t));
    self->values[self->count_entries] = calloc(strlen(value) + 1, sizeof(uint8_t));

    DIE(self->keys == NULL || self->values == NULL, ENOMEM);

    strcpy(self->keys[self->count_entries], key);
    strcpy(self->values[self->count_entries], value);

    ++self->count_entries;

    return true;
}

int32_t _HashMap_exists(HashMap *self, uint8_t *key) {
    int32_t i = 0;

    for (i = 0; i < self->count_entries; ++i) {
        if (strcmp(self->keys[i], key) == 0)
            return i;
    }

    return -ENOEXISTS;
}

uint8_t* _HashMap_getValue(HashMap *self, uint8_t *key) {
    int32_t i = 0;

    for (i = 0; i < self->count_entries; ++i) {
        if (strcmp(self->keys[i], key) == 0)
            return self->values[i];
    }

    return NULL;
}

uint8_t** _HashMap_getKeys(HashMap *self) {
    return self->keys;
}

uint8_t** _HashMap_getValues(HashMap *self) {
    return self->values;
}


Bool _HashMap_remove(HashMap *self, uint8_t *key) {
    int32_t i = 0;
    Bool found = false;

    if (self->exists(self, key) == -ENOEXISTS)
        return false;
    
    for (i = 0; i < self->count_entries; ++i) {
        if (!found && strcmp(self->keys[i], key) == 0) {
            found = true;
        } else if (found) {
            self->keys[i - 1]   = realloc(self->keys[i - 1], (strlen(self->keys[i]) + 1) * sizeof(uint8_t));
            self->values[i - 1] = realloc(self->values[i - 1], (strlen(self->values[i]) + 1) * sizeof(uint8_t));

            strcpy(self->keys[i - 1], self->keys[i]);
            strcpy(self->values[i - 1], self->values[i]);
        }
    }

    free(self->keys[self->count_entries - 1]);
    free(self->values[self->count_entries - 1]);
    --self->count_entries;

    return true;
}

void _HashMap_print(HashMap *self) {
    int32_t i = 0;
    
    for (i = 0; i < self->count_entries; ++i) {
        printf("%s: %s\n", self->keys[i], self->values[i]);
    }
}

void _HashMap_destroy(HashMap **self) {
    int32_t i = 0;

    for (i = 0; i < (*self)->count_entries; ++i) {
        free((*self)->keys[i]);
        free((*self)->values[i]);
    }

    free((*self)->keys);
    free((*self)->values);

    free(*self);
}