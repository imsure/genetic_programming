/*
 * Stack storing tree node
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "stack4tree.h"

void 
T_fatal_error (const char *err_info)
{
	fprintf (stderr, "%s error %d: %s\n", err_info, errno, strerror(errno));
	exit( EXIT_FAILURE );
}

struct stack4tree * 
T_create_stack (void)
{
	struct stack4tree *stack = (struct stack4tree *)malloc (sizeof(struct stack4tree));
	if (NULL == stack) {
		fatal_error ("Out of Spack !!!");
	}
	stack->next = NULL;
	return stack;
}

int
T_is_empty (struct stack4tree *stack)
{
	return stack->next == NULL;
}

void
T_push (struct stack4tree *stack, struct t_node *x)
{
	struct stack4tree *tmp_cell = (struct stack4tree *)malloc (sizeof(struct stack4tree));
	if (NULL == tmp_cell) {
		fatal_error ("Out of Spack !!!");
	}
	tmp_cell->node = x;

	tmp_cell->next = stack->next;
	stack->next = tmp_cell;
}

struct t_node *
T_pop (struct stack4tree *stack)
{
	struct t_node *x;
	struct stack4tree *top_cell;

	if (!T_is_empty(stack)) {
		x = stack->next->node;
		top_cell = stack->next;
		stack->next = stack->next->next;
		free (top_cell);
		return x;
	} else {
		printf ("Empty Stack !!!\n");
	}
}

void
T_empty_stack (struct stack4tree *stack)
{
	struct t_node *tmp;

	if (NULL == stack) {
		printf ("You must create stack first !!!\n");
	} else {
		while (!T_is_empty(stack)) {
			tmp = T_pop (stack);
			free (tmp);
		}
	}
}

void 
T_delete_stack (struct stack4tree *stack)
{
	if (NULL == stack) {
		printf ("Stack is not existed !!!\n");
	} else {
		T_empty_stack (stack);
		// release header
		free (stack);
	}
}

