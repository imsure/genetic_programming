/*
 * Converse Postfix Expreesion to a Binary Tree Using stack
 *
 * We use 0~255(unsigned char type) to represent operators, variables and constants
 * 0,1,2,3 allocate to '+'-'*'/'
 * 4~4+dimension allocate to variables
 * surplus allocate to constants
 * 255 represents a invalid number.
 *
 * So we only store numbers in the tree node, then we use numbers to find out
 * what it is.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack4tree.h"
#include "b_tree.h"
#include "utility.h"
#include "global.h"
#include "postfix2tree.h"

#define MALLOC_TNODE(x)	{ \
		x = (struct t_node *)malloc(sizeof(struct t_node)); \
		x->left = NULL; \
		x->right = NULL; \
}

static const char *operators = "+-*/"; 

struct t_node * 
postfix2tree (const char *postfix)
{
	struct stack4tree *stack; 
	struct t_node *root, *top1, *top2;
	char *str, *tmp;

	stack = T_create_stack ();
	if (NULL == stack) {
		fatal_error ("Out of Space !!!");
	}

	str = (char *)malloc(strlen(postfix)+1);
	strcpy (str, postfix);

	for (;; str = NULL) {
		if ((tmp=strtok(str, " ")) == NULL)	break;
		MALLOC_TNODE(root); 
		if (check_symbols(tmp[0]) == OPERATOR && strlen(tmp) == 1) {
			top1 = T_pop (stack);	// right sub-tree
			top2 = T_pop (stack); // left sub-tree
			/* combine tow sub-tree to a whole */
			root->no = str2no (tmp); 
			root->left = top2;
			root->right = top1;
			T_push (stack, root);
		} else {
			root->no = str2no (tmp); 
			if (root->no == INVALID_STR) {
				printf ("%s is invalid !!!\n", tmp);
				exit (EXIT_FAILURE);
			}
			T_push (stack, root);
		}
	}

	root = T_pop (stack);
	free (stack);

	return root;
}

void print_tree_postorder (struct t_node *root)
{
	if (root != NULL) {
		print_tree_postorder (root->left);
		print_tree_postorder (root->right);
		parse_print_no (root->no);
	}
}

void print_tree_inorder (struct t_node *root)
{
	if (root != NULL) {
		print_tree_inorder (root->left);
		parse_print_no (root->no);
		print_tree_inorder (root->right);
	}
}

void print_tree_preorder (struct t_node *root)
{
	if (root != NULL) {
		parse_print_no (root->no);
		print_tree_preorder (root->left);
		print_tree_preorder (root->right);
	}
}

void
parse_print_no (unsigned char no)
{
	if (no < NUM_OPERATOR) 
		printf("%c ", operators[no]);
	else if (no >= NUM_OPERATOR + dimension)
		printf ("%f ", constants[no-dimension-NUM_OPERATOR]);
	else
		printf("x%i ", no-NUM_OPERATOR);
}

int 
is_leaf (struct t_node *root) 
{
	return (root->left == NULL) ? 1 : 0;
}

float 
eval_tree (struct t_node *root, int cnt)
{
	return is_leaf(root) ? get_val(root->no, cnt) : \
		calculater(eval_tree(root->left, cnt), eval_tree(root->right, cnt), operators[root->no]);
}

