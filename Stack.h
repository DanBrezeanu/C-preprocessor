#include "int_types.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct Stack {
    int32t size;
    int32t capacity;
    uint16t sizeofElement;

    void* array;

    void (*push) (struct Stack* st, void *e);
    void* (*top) (struct Stack* st);
    void* (*pop) (struct Stack *st);
    Bool (*empty) (struct Stack* st);
    void (*destroy) (struct Stack **st);
} Stack;

Stack* _Stack_new(uint16t sizeofElement);
void _Stack_push(Stack *st, void *e);
void* _Stack_top(Stack *st);
void* _Stack_pop(Stack *st);
Bool _Stack_empty(Stack *st);
void _Stack_destroy(Stack **st);