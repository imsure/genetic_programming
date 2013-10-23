#ifndef _B_TREE_H_
#define _B_TREE_H_

struct t_node {
	unsigned char no;
	struct t_node *left;
	struct t_node *right;
};

#endif	// _B_TREE_H_
