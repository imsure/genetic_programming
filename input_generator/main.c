/*
 * MAIN
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "b_tree.h"
#include "postfix2tree.h"

#define TEST_CASES  19

int dimension = 1;
float *input;
float constants[100];
int const_cnt = 0;

int main (int argc, char *argv[])
{
	int i = 0;
	char *infix;
	char *postfix;
	FILE *input_file;
	struct t_node *root;
	float res;

	if (argc != 2) {
		printf ("Usage: %s <expression>\n", argv[0]);
		exit (EXIT_FAILURE);
	}

	postfix = (char *)malloc(strlen(argv[1])+1);
	if (NULL == postfix) {
		fatal_error ("Out of Space !!!");
	}
	infix = (char *)malloc(strlen(argv[1])+1);
	if (NULL == infix) {
		fatal_error ("Out of Space !!!");
	}
	strcpy (infix, argv[1]);
	printf ("input expression is:\n%s\n", infix);
	
	infix2postfix (infix, postfix);
	printf ("out expression:\n%s\n", postfix);

	input = (float *)malloc(dimension * TEST_CASES * sizeof(float));                                                                
	input_file = fopen ("test.dat", "r");
	while (!feof(input_file)) {
		fscanf (input_file, "%f", &input[i++]);
	} 
	//for (i=0; i<TEST_CASES; i++) printf ("%f\n", input[i]);

	//for (i=0; i<TEST_CASES; i++)
	//	eval (postfix, i);
	root = postfix2tree (postfix);
	printf ("Post Order:\n");
	print_tree_postorder (root);
	printf ("\n");
	for (i=0; i<TEST_CASES; i++) {
		printf ("%f %f\n", input[i], eval_tree(root, i));
	}
#if 1
	printf ("Post Order:\n");
	print_tree_postorder (root);
	printf ("\n");
	printf ("In Order: \n");
	print_tree_inorder (root);
	printf ("\n");
	printf ("Pre Order: \n");
	print_tree_preorder (root);
	printf ("\n");
#endif

	return 0;
}

