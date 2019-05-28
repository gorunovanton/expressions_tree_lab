#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct {
	char operation;
	double value;
} stack_value_s;

typedef stack_value_s stack_data_type_t;

typedef struct _stack_item {
	stack_data_type_t data;
	struct _stack_item* previous;
} stack_item_s;

typedef struct {
	size_t size;
	stack_item_s* top;
} stack_s;

void stack_create(stack_s* const s);
int stack_is_empty(const stack_s* const s);
size_t stack_get_size(const stack_s* const s);
int stack_push(stack_s* const s, const stack_data_type_t value);
int stack_pop(stack_s* const s);
stack_data_type_t stack_get_top(const stack_s* const s);
void stack_destroy(stack_s* const s);

#endif
