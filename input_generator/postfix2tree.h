#ifndef _POSTFIX2TREE_H_
#define _POSTFIX2TREE_H_

void infix2postfix (const char *in, char *out);
void eval (const char *postfix, int input_seq);
struct t_node * postfix2tree (const char *postfix);
float eval_tree (struct t_node *root, int cnt);
int is_leaf (struct t_node *root);
void parse_print_no (unsigned char no);
void print_tree_inorder (struct t_node *root);
void print_tree_preorder (struct t_node *root);
void print_tree_postorder (struct t_node *root);

#endif
