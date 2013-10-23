#ifndef _STACK4TREE_H_
#define _STACK4TREE_H_

#include "b_tree.h"

struct stack4tree {
	struct t_node *node;
	struct stack4tree *next;
};

struct stack4tree * T_create_stack (void);
int T_is_empty (struct stack4tree *stack);
void T_push (struct stack4tree *stack, struct t_node *x);
struct t_node * T_pop (struct stack4tree *stack);
void T_empty_stack (struct stack4tree *stack);
void T_delete_stack (struct stack4tree *stack);
void T_fatal_error (const char *err_info);

#endif	
