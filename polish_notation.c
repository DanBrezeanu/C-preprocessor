#include "Stack.h"
#include <ctype.h>

Bool isoperation(char e) {
    return (strchr("()+-*/", e) != NULL);
}

char* clean_expression(char *exp) {
    int exp_len = strlen((int8t*)exp);
    char* new_exp = calloc(exp_len, sizeof(char));
    int k = 0;
    int i = 0;

    for (i = 0; i < exp_len; ++i) {
        if (isdigit(exp[i]) || isoperation(exp[i]))
            new_exp[k++] = exp[i];
    }

    return new_exp;
}

int operation_priority(char e) {
    if (e == '*' || e == '/')
        return 2;
    else if (e == '+' || e == '-')
        return 1;

    return 0;
}

void evaluate_operation(char operation, Stack *number_stack) {
    int number2 = *(int*)number_stack->pop(number_stack);
    int number1 = *(int*)number_stack->pop(number_stack);
    int result = 0;

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

void remove_operation(char replacement, Stack *op_stack, Stack *number_stack) {
    if (replacement == ')') {
        while (*(char*)op_stack->top(op_stack) != '(') {
            evaluate_operation(*(char*)op_stack->pop(op_stack), number_stack);
        }
    
        op_stack->pop(op_stack);
        return;
    }

    while (!op_stack->empty(op_stack)
           && operation_priority(replacement) <= operation_priority(*(char*)op_stack->top(op_stack)) 
           && *(char*)op_stack->top(op_stack) != '(' 
           ) {
        evaluate_operation(*(char*)op_stack->pop(op_stack), number_stack);
    }

    op_stack->push(op_stack, &replacement);
}

void add_operation(char operation, Stack *op_stack, Stack *number_stack) {
    switch (operation) {
    case '(':
        op_stack->push(op_stack, &operation);
        break;
    
    case ')':
        remove_operation(operation, op_stack, number_stack);
        break;

    default:
        if (!op_stack->empty(op_stack) && *(char*)op_stack->top(op_stack) == '(')
            op_stack->push(op_stack, &operation);
        else if (!op_stack->empty(op_stack) && operation_priority(operation) <= operation_priority(*(char*)op_stack->top(op_stack))) {
            remove_operation(operation, op_stack, number_stack);
        } else {
            op_stack->push(op_stack, &operation);
        }
    }
}

int evaluate_expression(char *expression) {

    Stack *op_stack = _Stack_new(sizeof(char));
    Stack *number_stack = _Stack_new(sizeof(int));
    int expression_length = strlen((int8t*)expression);
    int i = 0;
    int result = 0;
    int* popped;
    int offset = 0;
    int number;
    
    expression = clean_expression(expression);
    
    for (i = 0; i < expression_length; ++i) {
        if (isoperation(expression[i])) {
            add_operation(expression[i], op_stack, number_stack);
        } else if (isdigit(expression[i])) {
            offset = 0;
            number = number_from_string(expression + i, &offset);
            number_stack->push(number_stack, &number);
            i += offset - 1;
        }
    }

    while (!op_stack->empty(op_stack))
        evaluate_operation(*(char*)op_stack->pop(op_stack), number_stack);

    popped = (int*)number_stack->pop(number_stack);
    result = *popped;
    free(popped);

    free(expression);
    op_stack->destroy(&op_stack);
    number_stack->destroy(&number_stack);
    return result;
}   



// int main() {
//     char exp[] = "( (   5 +  3 ) * 2   + 5  * 7) /2\n\n*2";
//     int result = evaluate_expression(exp);

//     printf("%d\n", result);


//     return 0;
// }   