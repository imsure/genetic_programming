/*
 * Commonly Used Functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

extern int dimension;
extern float constants[]; 
extern int const_cnt;
extern float *input;

const char tokens[] = "+-*/()";

int 
check_symbols (char sym)
{
	int i;
	for (i=0; i<=strlen(tokens); i++) {
		if (sym == tokens[i])
			return OPERATOR;
	}
	return OPERAND;
}

int 
get_priority (unsigned char no)
{
	if (no == PLUS || no == MINUS)
		return PRIORITY_PLUS_MINUS;
	else if (no == MULT || no == DIV)
		return PRIORITY_MULT_DIV;
	else if (no == LEFT_PAR)
		return PRIORITY_LEFT_PAR;
}

unsigned char 
get_sym_no (char symb) 
{
	switch (symb) {
	case '+' : return PLUS;
	case '-' : return MINUS;
	case '*' : return MULT;
	case '/' : return DIV;
	case '(' : return LEFT_PAR;
	}
}

float 
get_val (unsigned char no, int cnt)
{
	return (no < (dimension+NUM_OPERATOR)) ? input[no+cnt*dimension-NUM_OPERATOR] : constants[no-dimension-NUM_OPERATOR];
}

unsigned char 
str2no (const char *s)
{
	int i;
	char tmp[2];
	
	if (check_symbols(s[0]) == OPERATOR) {
		switch (s[0]) {
		case '+' : return PLUS;
		case '-' : return MINUS;
		case '*' : return MULT;
		case '/' : return DIV;
		}
	} else if (s[0] == 'x') {	// variable
		for (i=0; i<dimension; i++) {
			sprintf (tmp, "%i", i);
			if (tmp[0] == s[1])
				return i + NUM_OPERATOR;
		}
	} else {	// constant
		constants[const_cnt++] = strtof(s, NULL);
		return NUM_OPERATOR+dimension+const_cnt-1;
	}
	return INVALID_STR;
}

float 
calculater (float v1, float v2, char operator)
{
	switch (operator) {
	case '+' : return v1 + v2;
	case '-' : return v1 - v2;
	case '*' : return v1 * v2;
	case '/' : return (v2 == 0.0)? 0.0 : v1 / v2;
	}
}

