#include "Stack.h"

Stack *_Stack_new(uint16_t sizeofElement) {
    Stack *st = calloc(1, sizeof(Stack));

    st->capacity = 32;
    st->sizeofElement = sizeofElement;
    st->array = calloc(st->capacity, sizeof(st->sizeofElement));

    st->pop = &_Stack_pop;
    st->push = &_Stack_push;
    st->empty = &_Stack_empty;
    st->top = &_Stack_top;
    st->destroy = &_Stack_destroy;

    return st;
}

void _Stack_push(Stack *st, void *e) {
    void *dest;

    if (st->size == st->capacity) {
        st->capacity <<= 1;
        st->array = realloc(st->array, st->capacity * sizeof(st->sizeofElement));
    }
    
    dest = (int8_t *)st->array + (st->size * st->sizeofElement);
    memcpy(dest, e, st->sizeofElement);

    ++st->size;
}

void *_Stack_top(Stack *st) {
    int32_t source;
    int8_t *returnValue;
    int32_t i = 0;

    if (st->size == 0) {
        return NULL;
    }

    source = (st->size - 1) * st->sizeofElement;
    returnValue = malloc(st->sizeofElement);

    for (i = 0; i < st->sizeofElement; ++i) {
       *(returnValue + i) = *((int8_t *)st->array + source + i);
    }

    return (void *)returnValue;
}

void *_Stack_pop(Stack *st) {
    void *returnValue;

    if (st->size == 0) {
        return NULL;
    }

    returnValue = st->top(st);
    --st->size;

    return returnValue;
}

Bool _Stack_empty(Stack *st) {
    return (st->size == 0);
}

void _Stack_destroy(Stack **st) {
    int32_t i = 0;
    
    for (i = 0; i < (*st)->size; ++i) {
        free((int8_t *)(*st)->array + i * (*st)->sizeofElement);
    }
    
    free((*st)->array);
    free(*st);
}
