/*
 * Evaluate the Postfix Expression (Reverse Polish Notation)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include "linklist_stack.h"
#include "global.h"

void 
eval (const char *postfix, int input_seq)
{
	int i = 0, len = strlen(postfix), rec = 0;
	char *str, *expr;
	unsigned char no, top1, top2;
	struct stack_node * stack;
	float val1, val2, res;

	stack = create_stack ();
	if (NULL == stack) {
		fatal_error ("Out of Space !!!");
	}

	str = (char *)malloc (strlen(postfix) + 1);
	strcpy (str, postfix);	

	strcpy (str, postfix);	
	for (;; str = NULL) {
		expr = strtok (str, " ");
		if (NULL == expr)	break;
		if (check_symbols(expr[0]) == 0 && strlen(expr) == 1) {
			top1 = pop (stack);
			top2 = pop (stack);
			val1 = get_val (top1, input_seq);
			val2 = get_val (top2, input_seq);
			res = calculater (val1, val2, expr[0]);
			constants[const_cnt++] = res;
			push (stack, NUM_OPERATOR+dimension+const_cnt-1);
		} else {
			no = str2no (expr);
			if (no == INVALID_STR) {
				printf ("%s is invalid.\n", expr);
				exit (EXIT_FAILURE);
			}
			push (stack, no);
		}
	}

	no = pop (stack);
	printf ("%f\n", constants[no-dimension-NUM_OPERATOR]);
}

