#include "stack.h"

#include <stdlib.h>

void stack_create(stack_s* const s) {
	s->size = 0;
	s->top = NULL;
}

int stack_is_empty(const stack_s* const s) {
	return s->top == NULL;
}

size_t stack_get_size(const stack_s* const s) {
	return s->size;
}

int stack_push(stack_s* const s, const stack_data_type_t value) {
	stack_item_s* const item = (stack_item_s *)malloc(sizeof(stack_item_s));

	if (!item) {
		return 0;
	}

	item->data = value;
	item->previous = s->top;
	s->top = item;
	s->size++;

	return 1;
}

int stack_pop(stack_s* const s) {
	if (!s->size)
		return 0;

	stack_item_s* const item = s->top;
	s->top = s->top->previous;
	s->size--;

	free(item);

	return 1;
}

stack_data_type_t stack_get_top(const stack_s* const s) {
	return s->top->data;
}

void stack_destroy(stack_s* const s) {
	while (s->top) {
		stack_item_s* const item = s->top;
		s->top = s->top->previous;

		free(item);
	}

	s->size = 0;
	s->top = NULL;
}
