/*
 * Converse Infix Expression to Postfix Expression
 * An Application of Stack
 * Need to be improved to handle cases such as (x*(-2)-1)/2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linklist_stack.h"
#include "utility.h"

#define	OPERATOR	0
#define PRIORITY_LEFT_PAR	2
#define LEFT_PAR	4

extern const char tokens[]; 

void infix2postfix (const char *in, char *out);

void 
infix2postfix (const char *in, char *out)
{
	int i = 0, rec = 0, len = strlen(in);
	char symb;
	unsigned char no, top;
	char *expr, *str;
	struct stack_node * stack;

	stack = create_stack ();
	if (NULL == stack) {
		fatal_error ("Out of Space !!!");
	}

	str = (char *)malloc (strlen(in) + 1);
	strcpy (str, in);	// get a copy of input string because strtok() will destruct its first argument

	while (in[i++] == '(') {
		push (stack, LEFT_PAR); 
		rec++;
	}

	for (;; str = NULL) {
		expr = strtok (str, tokens);
		if (NULL == expr)	break;
		strcat (out, expr);
		strcat (out, " ");
		rec += strlen(expr);
		if (rec < len) {
			symb = in[rec++];
			do {
				no = get_sym_no (symb);
				if (symb == ')') {
					while ((top = pop(stack)) != LEFT_PAR) {
						strncat (out, &tokens[top], 1);
						strcat (out, " ");
					}
				} else {
					while (!is_empty(stack)) {
						top = pop (stack);
						if ((get_priority(no) > get_priority(top)) || (get_priority(top) == PRIORITY_LEFT_PAR)) {
							push (stack, top);
							push (stack, no);
							break;
						} else {
							strncat (out, &tokens[top], 1);
							strcat (out, " ");
						}
					}
					if (is_empty(stack)) {
						push (stack, no);
					}
				}
				symb = in[rec++];
			} while(check_symbols(symb) == OPERATOR);
			rec--;
		}
	}

	while (!is_empty(stack)) {
		top = pop (stack);
		strncat (out, &tokens[top], 1);
		strcat (out, " ");
	}

	delete_stack (stack);
}

