/*
 * Linklist Implementation of Stack
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "linklist_stack.h"

void 
fatal_error (const char *err_info)
{
	fprintf (stderr, "%s : %d: %s\n", err_info, errno, strerror(errno));
	exit( EXIT_FAILURE );
}

struct stack_node * 
create_stack (void)
{
	struct stack_node *stack = (struct stack_node *)malloc (sizeof(struct stack_node));
	if (NULL == stack) {
		fatal_error ("Out of Spack !!!");
	}
	stack->next = NULL;
	return stack;
}

int
is_empty (struct stack_node *stack)
{
	return stack->next == NULL;
}

void
push (struct stack_node *stack, Stack_Data_Type x)
{
	struct stack_node *tmp_cell = (struct stack_node *)malloc (sizeof(struct stack_node));
	if (NULL == tmp_cell) {
		fatal_error ("Out of Spack !!!");
	}
	tmp_cell->data = x;

	tmp_cell->next = stack->next;
	stack->next = tmp_cell;
}

Stack_Data_Type
pop (struct stack_node *stack)
{
	Stack_Data_Type x;
	struct stack_node *top_cell;

	if (!is_empty(stack)) {
		x = stack->next->data;
		top_cell = stack->next;
		stack->next = stack->next->next;
		free (top_cell);
		return x;
	} else {
		printf ("Empty Stack !!!\n");
	}
}

void
empty_stack (struct stack_node *stack)
{
	if (NULL == stack) {
		printf ("You must create stack first !!!\n");
	} else {
		while (!is_empty(stack)) {
			pop (stack);
		}
	}
}

void 
delete_stack (struct stack_node *stack)
{
	if (NULL == stack) {
		printf ("Stack is not existed !!!\n");
	} else {
		empty_stack (stack);
		// release header
		free (stack);
	}
}

