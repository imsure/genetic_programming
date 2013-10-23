#ifndef _UTILITY_H_
#define _UTILITY_H_

#define INVALID_STR	255
#define NUM_OPERATOR	4

enum {OPERATOR, OPERAND};

enum {
	PRIORITY_PLUS_MINUS = 0,
	PRIORITY_MULT_DIV,
	PRIORITY_LEFT_PAR
};

enum {
	PLUS = 0,
	MINUS, 
	MULT,  
	DIV,   
	LEFT_PAR,  
	RIGHT_PAR 
};

void check_priority ();
unsigned char get_sym_no (char symb); 
void eval (const char *postfix, int input_seq);
int check_symbols (char sym);
float calculater (float v1, float v2, char operator);
float get_val (unsigned char no, int cnt);
unsigned char str2no (const char *s);

#endif
