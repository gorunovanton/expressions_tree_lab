#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "tree.h"

int main() {
	int action;
	node_s* source_expression_tree_root = NULL;
	node_s* converted_expression_tree_root = NULL;

	while (1) {
		printf("Menu:\n");
		printf("1) Enter expression\n");
		printf("2) Print source expression\n");
		printf("3) Print transformed expression\n");
		printf("4) Print source tree\n");
		printf("5) Print transformed tree\n");
		printf("6) exit\n");
		printf("Choose action: ");
		fseek(stdin, 0, SEEK_END);
		scanf("%d", &action);

		switch (action) {
			case 1: {
				char expression[255];
				printf("Enter expression: ");
				
				fseek(stdin, 0, SEEK_END);
				scanf("%s", expression);

				destroy_tree(&source_expression_tree_root);
				destroy_tree(&converted_expression_tree_root);

				stack_s operations_stack;
				stack_create(&operations_stack);

				put_expression_to_stack(expression, &operations_stack);
				build_tree(&source_expression_tree_root, &operations_stack);

				stack_destroy(&operations_stack);

				converted_expression_tree_root = tree_copy(&source_expression_tree_root);

				tree_multiply_fractions(&converted_expression_tree_root);

				break;
			}

			case 2: {
				printf("Source expression: ");
				print_expression(&source_expression_tree_root);
				printf("\n");
				break;
			}

			case 3: {
				printf("Converted expression: ");
				print_expression(&converted_expression_tree_root);
				printf("\n");
				break;
			}

			case 4: {
				if (source_expression_tree_root != NULL) {
					printf("Tree of source expression\n");
					print_tree(&source_expression_tree_root, 0);
				}
				else {
					printf("Tree for source expression is empty\n");
				}

				break;
			}

			case 5: {
				if (converted_expression_tree_root != NULL) {
					printf("Tree of converted expression\n");
					print_tree(&converted_expression_tree_root, 0);
				}
				else {
					printf("Tree of converted expression is empty\n");
				}

				break;
			}

			case 6: { break; }

			default: {
				printf("Error. Invalid option\n");

				break;
			}
		}

		if (action == 6) {
			break;
		}
	}

	destroy_tree(&source_expression_tree_root);
	destroy_tree(&converted_expression_tree_root);

	return 0;
}
