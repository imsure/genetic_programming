#ifndef _LINKLIST_STACK_H_
#define _LINKLIST_STACK_H_

typedef unsigned char Stack_Data_Type;

struct stack_node {
	Stack_Data_Type data;
	struct stack_node *next;
};

struct stack_node * create_stack (void);
int is_empty (struct stack_node *stack);
void push (struct stack_node *stack, Stack_Data_Type x);
Stack_Data_Type pop (struct stack_node *stack);
void empty_stack (struct stack_node *stack);
void delete_stack (struct stack_node *stack);
void fatal_error (const char *err_info);

#endif
