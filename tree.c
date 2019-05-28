#include "tree.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int is_letter(const char ch);
int is_number(const char ch);
int is_operation(const char ch);
int compare_operation_priority(const char first, const char second);
int is_unary_minus_node(const node_s* const* const node);
node_s* tree_create_node();


node_s* tree_create_node() {
	node_s* const tmpNode = (node_s *)malloc(sizeof(node_s));

	tmpNode->operation_or_variable = '\0';
	tmpNode->number_value = 0.0;
	tmpNode->left = NULL;
	tmpNode->right = NULL;

	return tmpNode;
}

node_s* tree_copy(node_s** node) {
	if (*node == NULL){
		return NULL;
	}

	node_s* const tmpNode = tree_create_node();
	tmpNode->operation_or_variable = (*node)->operation_or_variable;
	tmpNode->number_value = (*node)->number_value;
	tmpNode->left = tree_copy(&(*node)->left);
	tmpNode->right = tree_copy(&(*node)->right);

	return tmpNode;
}


int is_unary_minus_node(const node_s* const* const node) {
	if (*node == NULL || (*node)->left == NULL || (*node)->right == NULL) {
		return 0;
	}

	return ((*node)->operation_or_variable == '-' &&
		(*node)->left->operation_or_variable == '\0' &&
		(*node)->left->number_value == 0.0);
}

void build_tree(node_s** node, stack_s* st) {
	if (stack_is_empty(st)){
		return;
	}

	const stack_value_s token = stack_get_top(st);

	stack_pop(st);

	(*node) = tree_create_node();
	(*node)->operation_or_variable = token.operation;
	(*node)->number_value = token.value;

	if (is_operation((*node)->operation_or_variable)) {
		build_tree(&(*node)->right, st);
		build_tree(&(*node)->left, st);
	}
}

void destroy_tree(node_s** node) {
	if (*node == NULL){
		return;
	}

	if ((*node)->left != NULL){
		destroy_tree(&(*node)->left);
	}

	if ((*node)->right != NULL){
		destroy_tree(&(*node)->right);
	}

	free(*node);

	*node = NULL;
}

void print_tree(node_s** node, const int level) {
	if (*node == NULL) {
		return;
	}

	if ((*node)->right != NULL) {
		print_tree(&(*node)->right, level + 1);
	}

	if ((*node)->operation_or_variable != '\0') {
		printf("%*s%c\n", level * 4, "", (*node)->operation_or_variable);
	}
	else {
		printf("%*s%f\n", level * 4, "", (*node)->number_value);
	}

	if ((*node)->left != NULL) {
		print_tree(&(*node)->left, level + 1);
	}
}

void print_expression(node_s** node) {
	if (*node == NULL) {
		return;
	}

	if ((*node)->left != NULL && !is_unary_minus_node(node)) {
		if ((*node)->left->left != NULL) {
			printf("(");
		}

		print_expression(&(*node)->left);

		if ((*node)->left->left != NULL) {
			printf(")");
		}
	}

	if ((*node)->operation_or_variable != '\0') {
		printf("%c", (*node)->operation_or_variable);
	}
	else {
		printf("%f", (*node)->number_value);
	}

	if ((*node)->right != NULL) {
		if ((*node)->right->left != NULL) {
			printf("(");
		}

		print_expression(&(*node)->right);

		if ((*node)->right->left != NULL) {
			printf(")");
		}
	}
}

int is_letter(const char ch) {
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

int is_number(const char ch) {
	return (ch >= '0' && ch <= '9');
}

int is_operation(const char ch) {
	return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')');
}

int get_operation_priority(const char operation) {
	if (operation == '^'){
		return 4;
	}

	if (operation == '*' || operation == '/'){
		return 3;
	}

	if (operation == '+' || operation == '-'){
		return 2;
	}

	return 1;
}

int compare_operation_priority(const char first, const char second) {
	if (first == '(' || second == '(' || second == ')'){
		return 0;
	}

	if (first == second && second == '^'){
		return 0;
	}

	return get_operation_priority(first) >= get_operation_priority(second);
}

void put_expression_to_stack(const char* str, stack_s* st) {
	int current_position = 0;
	int step = -1;
	int is_bracket = 0;
	int is_dot = 0;
	stack_s operations_stack;
	stack_create(&operations_stack);

	stack_value_s tk;
	tk.operation = '\0';
	tk.value = 0.0;

	while (str[current_position] != '\0') {
		if (str[current_position] == '.'){
			is_dot = 1;
		}
		else if (is_letter(str[current_position])) {
			tk.operation = str[current_position];

			stack_push(st, tk);
		}
		else if (is_number(str[current_position])) {
			tk.operation = '\0';

			if (!is_dot){
				tk.value = tk.value * 10.0 + str[current_position] - '0';
			}
			else {
				tk.value = tk.value + pow(10.0, step) * (str[current_position] - '0');
				step--;
			}

			if (str[current_position + 1] != '.' && !is_number(str[current_position + 1])) {
				stack_push(st, tk);

				tk.value = 0.0;
				step = -1;
				is_dot = 0;
			}
		}
		else if (is_operation(str[current_position])) {
			tk.operation = str[current_position];

			if (str[current_position] == ')'){
				is_bracket = 1;
			}
			else if (str[current_position] == '-' && (current_position == 0 || str[current_position - 1] == '(')) {
				const char tmp_ch = tk.operation;
				tk.operation = '\0';
				tk.value = 0.0;

				stack_push(st, tk);

				tk.operation = tmp_ch;
			}

			while (!stack_is_empty(&operations_stack) &&
				(compare_operation_priority(stack_get_top(&operations_stack).operation, str[current_position]) || is_bracket)) {
				if (stack_get_top(&operations_stack).operation == '(') {
					is_bracket = 0;
				}
				else {
					stack_push(st, stack_get_top(&operations_stack));
				}

				stack_pop(&operations_stack);
			}

			if (str[current_position] != ')'){
				stack_push(&operations_stack, tk);
			}
		}

		++current_position;
	}

	while (!stack_is_empty(&operations_stack)) {
		stack_push(st, stack_get_top(&operations_stack));
		stack_pop(&operations_stack);
	}

	stack_destroy(&operations_stack);
}

void tree_multiply_fractions(node_s** const node) {
	if (node == NULL || *node == NULL) {
		return;
	}

	node_s** const left = &(*node)->left;
	node_s** const right = &(*node)->right;

	tree_multiply_fractions(left);
	tree_multiply_fractions(right);

	if ((*node)->operation_or_variable == '*' && left != NULL && (*left)->operation_or_variable == '/' && right != NULL && (*right)->operation_or_variable == '/') {
		node_s* old_tree = *node;
		(*node) = tree_create_node();
		(*node)->operation_or_variable = '/';

		node_s* const firstNumerator = tree_copy(&(*left)->left);
		node_s* const secondNumerator = tree_copy(&(*right)->left);

		node_s* const firstDenominator = tree_copy(&(*left)->right);
		node_s* const secondDenominator = tree_copy(&(*right)->right);

		(*node)->left = tree_create_node();
		(*node)->left->operation_or_variable = '*';
		(*node)->left->left = firstNumerator;
		(*node)->left->right = secondNumerator;

		(*node)->right = tree_create_node();
		(*node)->right->operation_or_variable = '*';
		(*node)->right->left = firstDenominator;
		(*node)->right->right = secondDenominator;

		destroy_tree(&old_tree);
	}
}