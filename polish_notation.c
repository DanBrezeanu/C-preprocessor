#include "Stack.h"
#include <ctype.h>

Bool isoperation(uint8t e) {
    return (strchr("()+-*/", e) != NULL);
}

uint32t operation_priority(uint8t e) {
    if (e == '*' || e == '/')
        return 2;
    else if (e == '+' || e == '-')
        return 1;

    return 0;
}

void evaluate_operation(uint8t operation, Stack *number_stack) {
    int32t number2 = *(int32t*)number_stack->pop(number_stack);
    int32t number1 = *(int32t*)number_stack->pop(number_stack);
    int32t result = 0;

    switch (operation) {
    case '+':
        result = number1 + number2;
        break;

    case '-':
        result = number1 - number2;
        break;

    case '*':
        result = number1 * number2;
        break;
    
    case '/':
        result = number1 / number2;
        break;
    }

    number_stack->push(number_stack, &result);
}

void remove_operation(uint8t replacement, Stack *op_stack, Stack *number_stack) {
    if (replacement == ')') {
        while (*(uint8t*)op_stack->top(op_stack) != '(') {
            evaluate_operation(*(uint8t*)op_stack->pop(op_stack), number_stack);
        }
    
        op_stack->pop(op_stack);
        return;
    }

    while (!op_stack->empty(op_stack)
           && operation_priority(replacement) <= operation_priority(*(uint8t*)op_stack->top(op_stack)) 
           && *(uint8t*)op_stack->top(op_stack) != '(' 
           ) {
        evaluate_operation(*(uint8t*)op_stack->pop(op_stack), number_stack);
    }

    op_stack->push(op_stack, &replacement);
}

void add_operation(uint8t operation, Stack *op_stack, Stack *number_stack) {
    switch (operation) {
    case '(':
        op_stack->push(op_stack, &operation);
        break;
    
    case ')':
        remove_operation(operation, op_stack, number_stack);
        break;

    default:
        if (!op_stack->empty(op_stack) && *(uint8t*)op_stack->top(op_stack) == '(')
            op_stack->push(op_stack, &operation);
        else if (!op_stack->empty(op_stack) && operation_priority(operation) <= operation_priority(*(uint8t*)op_stack->top(op_stack))) {
            remove_operation(operation, op_stack, number_stack);
        } else {
            op_stack->push(op_stack, &operation);
        }
    }
}

int32t evaluate_expression(uint8t *expression) {
    Stack *op_stack = _Stack_new(sizeof(uint8t));
    Stack *number_stack = _Stack_new(sizeof(int32t));
    uint32t expression_length = strlen(expression);
    int32t i = 0;
    int32t result = 0;
    
    for (i = 0; i < expression_length; ++i) {
        if (isoperation(expression[i])) {
            add_operation(expression[i], op_stack, number_stack);
        } else if (isdigit(expression[i])) {
            int32t offset = 0;
            int32t number = number_from_string(expression + i, &offset);
            number_stack->push(number_stack, &number);
            i += offset - 1;
        }
    }

    while (!op_stack->empty(op_stack))
        evaluate_operation(*(uint8t*)op_stack->pop(op_stack), number_stack);

    result = *(int32t*)number_stack->pop(number_stack);
    
    op_stack->destroy(&op_stack);
    number_stack->destroy(&number_stack);
    return result;
}   



int main() {
    char exp[] = "((5+3)*2+5*7)/2";
    printf("%d\n", evaluate_expression(exp));

    return 0;
}   