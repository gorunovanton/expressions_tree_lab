#ifndef TREE_H
#define TREE_H

#include "stack.h"

typedef struct _node {
	char operation_or_variable;
	double number_value;
	struct _node* left;
	struct _node* right;
} node_s;

node_s* tree_copy(node_s** node);
void build_tree(node_s** node, stack_s* st);
void destroy_tree(node_s** node);
void print_tree(node_s** node, const int level);
void print_expression(node_s** node);

void put_expression_to_stack(const char* str, stack_s* st);
void tree_multiply_fractions(node_s** const node);

#endif
