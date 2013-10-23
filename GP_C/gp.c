#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FUNCTION_SET_NUM	4	
#define INPUT_NUM			19
#define POP_SIZE			1000
#define GENERATION			500
#define CRO_PRO				0.6
#define MUT_PRO				0.4
#define TOURNAMENT_SZ		50
#define T_SCORE_PARM		0.02

#define SUCCESS	1
#define FAIL	0

#define MAX(a, b)	(a > b ? a : b)

//#define PROP_SEL

struct node {
	unsigned char no;
	struct node *left;
	struct node *right;
	int con_val;	// only useful when the node is a terminal node represent a constant value
};

const char *function_set = "+-*/";
int min_depth = 2;
int max_depth = 6;
int current_depth;
int dimension = 1;
int initial_constant[] = {-5, -4, -3, -2, -1, 1, 2, 3, 4, 5};
int constant_range = 6;
float *input;
float *fitness;
float *normalized_fitness;
float *standardized_fitness;
float termination = 0.01;
float old_fitness;
int temperature_factor;
int seed = 5;

typedef float (*calculate) (float x, float y);

float plus (float x, float y) { return (x + y); }
float sub (float x, float y)  { return (x - y); }
float mult (float x, float y) { return (x * y); }
float division (float x, float y)  { return (y == 0) ? 0 : (x / y); }

calculate functions[4] = {plus, sub, mult, division}; 

unsigned char 
get_random_function(void)
{
	return rand() % FUNCTION_SET_NUM;
}

unsigned char
get_random_terminal(void)
{
	return (rand()%2) ? (rand()%dimension) : dimension;
}

int 
get_random_constant (void)
{
	int val = rand() % constant_range;
	//return initial_constant[(rand() % 10)];
	return ((rand()%2) ? val : -val);
}

struct node *
create_a_full_tree (struct node *root, int depth)
{
	root = (struct node *)malloc(sizeof(struct node));
	root->left = root->right = NULL;
	if (depth == 0) {   // means reach to the maximum depth
		root->no = get_random_terminal();
		if (root->no == dimension) {
			root->con_val = get_random_constant();
		}
	} else {    // means we have not reached the maximum depth
		root->no = get_random_function();   
		root->left = create_a_full_tree (root->left, depth-1);
		root->right = create_a_full_tree (root->right, depth-1);
	}   
	return root;
}

struct node *
create_a_grow_tree (struct node *root, int depth)
{
	root = (struct node *)malloc(sizeof(struct node));
	root->left = root->right = NULL;
	if ((depth != 0 && rand()%2) || (depth == max_depth)) { // to make sure not generate a tree with depth = 0
		root->no = get_random_function();
		root->left = create_a_grow_tree (root->left, depth-1);
		root->right = create_a_grow_tree (root->right, depth-1);
	} else {
		root->no = get_random_terminal();
		if (root->no == dimension) {
			root->con_val = get_random_constant();
		}   
	}   
	return root;
}

int 
is_leaf (struct node *node)
{
	return (node->left == NULL);
}

void 
print_tree_postorder (struct node *root)
{
	if (!is_leaf(root)) {
		print_tree_postorder (root->left);
		print_tree_postorder (root->right);
		printf ("%c ", function_set[root->no]);
	} else {
		(root->no > (dimension-1)) ? printf("%i ", root->con_val) : printf("x%i ", root->no);
	}
}

void 
print_tree_inorder (struct node *root)
{
	if (!is_leaf(root)) {
		printf ("(");
		print_tree_inorder (root->left);
		printf ("%c ", function_set[root->no]);
		print_tree_inorder (root->right);
		printf (")");
	} else {
		(root->no > (dimension-1)) ? printf("%i ", root->con_val) : printf("x%i ", root->no);
	}
}

int 
depth (struct node *root)
{
	int l_depth = 0, r_depth = 0;

	if (is_leaf(root))	return 0;
	else {
		l_depth = depth (root->left) + 1;
		r_depth = depth (root->right) + 1;
		return MAX(l_depth, r_depth);
	}
}

int 
size (struct node *root)
{
	if (is_leaf(root))
		return 1;
	else 
		return size(root->left) + size(root->right) + 1;
}

float 
eval_tree (struct node *root, int var_set)
{
	return !is_leaf(root) ?\
		functions[root->no] (eval_tree(root->left, var_set), eval_tree(root->right, var_set)) :\
		(root->no)<dimension ? input[root->no + (dimension+1)*var_set] : root->con_val;
}

void
random_position_old (struct node *root, struct node **sub_tree, int pos)
{
	int i;

	if (pos == 0) {
		*sub_tree = root;
		return;
	}

	for (i=1; i<=pos; i++) {
		if (is_leaf(root->left))	*sub_tree = root->right;
		else if (is_leaf(root->right))	*sub_tree = root->left;
		else {
			if (rand() % 2 )	*sub_tree = root->right;
			else	*sub_tree = root->left;
		}
	}
}

void
random_position (struct node *root, struct node **sub_tree, int pos)
{
	if (pos == 0) {
		*sub_tree = root;
		return;
	} else {
		if (depth(root->left) < pos) {
			random_position(root->right, sub_tree, pos-1);
		} else if (depth(root->right) < pos) {
			random_position(root->left, sub_tree, pos-1);
		} else {
			if (rand() % 2) {
				random_position(root->left, sub_tree, pos-1);
			} else {
				random_position(root->right, sub_tree, pos-1);
			}
		}
		return;
	}
}

struct node *
get_a_copy (struct node *orgi, struct node *dst)
{
	dst = (struct node *)malloc(sizeof(struct node));
	dst->no = orgi->no;
	dst->left = dst->right = NULL;
	if (is_leaf(orgi)) {
		if (dst->no == dimension)	dst->con_val = orgi->con_val;
		return dst;
	} else {
		//dst->no = orgi->no;
		dst->left = get_a_copy (orgi->left, dst->left);
		dst->right = get_a_copy (orgi->right, dst->right);
		return dst;
	}
}

void 
destroy_tree (struct node *root)
{
	if (root != NULL) {
		destroy_tree (root->left);
		destroy_tree (root->right);
		free (root);
	}
}

struct node *
crossover (struct node *father, struct node *mother)
{
	int fpos, mpos;
	struct node *fsub_tree;
	struct node *msub_tree;
	struct node *chromosome;
	struct node *offspring;
	struct node *tmp_node;

	// choose a subtree of father at random
	fpos = rand() % depth(father);
	random_position (father, &fsub_tree, fpos);

	// choose a subtree of mother at random
	mpos = rand() % depth(mother);
	random_position (mother, &msub_tree, mpos);
	if (rand() % 2) {	
		chromosome = msub_tree->left;	
		msub_tree->left = NULL;
	} else {  
		chromosome = msub_tree->right;	
		msub_tree->right = NULL;
	}
	destroy_tree (mother);

	// combine subtree from mother with father
	if (rand() % 2) {
		tmp_node = fsub_tree->left;
		fsub_tree->left = chromosome;
	} else {
		tmp_node = fsub_tree->right;
		fsub_tree->right = chromosome;
	}
	destroy_tree (tmp_node);

	if (depth(father) > (max_depth + 4))	return NULL;
	else return father;
}

struct node *
mutation (struct node *root) 
{
	int mutation_pos, mutation_size;
	struct node *sub_tree;
	struct node *mutation;
	struct node *tmp_node;

	mutation_pos = rand() % depth(root);
	random_position (root, &sub_tree, mutation_pos);
	mutation_size = rand() % (max_depth - 1);
	mutation = create_a_grow_tree (mutation, mutation_size);
	
	if (rand() % 2) {
		tmp_node = sub_tree->left;
		sub_tree->left = mutation;
	} else {
		tmp_node = sub_tree->right;
		sub_tree->right = mutation;
	}
	destroy_tree (tmp_node);

	if (depth(root) > (max_depth + 4)) return NULL;
	else return root;
}

int 
proportional_selection (int best_individual)
{
	int i;
	float sum = 0.0;
	float decision = ((float)rand()) / RAND_MAX;

	for (i=0; i<POP_SIZE; i++) {
		sum += normalized_fitness[i];
		if (decision <= sum)
			return i;
	}

	// sometime may accidently come to here 
	// because of the definition of float calculattion
	// like decision = 0.999999996, but sum = 0.999999994
	// in this case we return the best individual of current population
	return best_individual;
}

int 
tournament_selection (void)                                                                                                               
{
	int i, index, best_one;
	float local_best_fitness; 
	best_one = index = rand() % POP_SIZE;
	local_best_fitness = fitness[best_one];
	for (i=1; i<TOURNAMENT_SZ; i++) {
		index = rand() % POP_SIZE;
		if (fitness[index] < local_best_fitness) {
			local_best_fitness = fitness[index];
			best_one = index;
		}   
	}   
	return best_one;
}

void
initialize_pop (struct node *pop[], int size) 
{
	int i;
	for (i = 0; i < 800; i++) {
		//current_depth = min_depth + rand() % (max_depth - min_depth + 1);
		pop[i] = create_a_grow_tree (pop[i], max_depth); 
		//print_tree_inorder (pop[i]);
		//printf ("\n");
	}
	for (i = 800; i < POP_SIZE; i++) {
		current_depth = 2 + rand() % (max_depth - min_depth - 1);
		pop[i] = create_a_full_tree (pop[i], current_depth); 
		//print_tree_inorder (pop[i]);
		//printf ("\n");
	}
	//exit (EXIT_FAILURE);
}

int 
fitness_evaluation (struct node *pop[], float *best_fitness, float *std_fitness_sum)
{
	int i, j, best_individual;

	for (i=0; i<POP_SIZE; i++) {
		fitness[i] = 0.0;
		for (j=0; j<INPUT_NUM; j++) {
			fitness[i] += fabs(eval_tree(pop[i], j) - input[(dimension+1)*j + dimension]); 
		}
#ifdef PROP_SEL
		standardized_fitness[i] = 1 / (1 + fitness[i]);
		*std_fitness_sum += standardized_fitness[i];
#endif
		if (i == 0) {	
			*best_fitness = fitness[i];
		}
		else if (fitness[i] < *best_fitness) {
			*best_fitness = fitness[i];
			best_individual = i;
		}
	}

	return best_individual;
}

void 
selection (struct node *pop[], struct node *limbo_pop[], int best_individual)
{
	int i, index, new_pop_cnt = 0;
	struct node *father, *mother, *offspring;
	float decision;

#if 0
	// Reproduction
	for (i = 0; i < 10; i++) {
		 limbo_pop[i] = get_a_copy (pop[best_individual], limbo_pop[i]);
		 new_pop_cnt++;
	}
#endif

	while (new_pop_cnt < POP_SIZE) {
		decision = ((float)rand()) / RAND_MAX;
		if (decision < CRO_PRO) {
		#ifdef PROP_SEL
			father = get_a_copy (pop[proportional_selection(best_individual)], father);
			mother = get_a_copy (pop[proportional_selection(best_individual)], mother);
		#else // Tournament selection
			father = get_a_copy (pop[tournament_selection()], father);
			mother = get_a_copy (pop[tournament_selection()], mother);
		#endif
			offspring = crossover (father, mother);
			if (offspring == NULL) continue;
			else limbo_pop[new_pop_cnt++] = offspring;
		} else {
		#ifdef PROP_SEL
		//	father = get_a_copy (pop[proportional_selection(best_individual)], father);
		#else // Tournament selection
		//	father = get_a_copy (pop[tournament_selection()], father);
		#endif
			index = rand() % POP_SIZE;
			father = get_a_copy (pop[index], father);
			offspring = mutation (father);
			if (offspring == NULL) continue;
			else limbo_pop[new_pop_cnt++] = offspring;
		}
	}
}

void
adjust_constant (struct node *root) 
{
	int range;
	int pro = rand() % 2;
	if (is_leaf(root)) {
		if (root->no == dimension) { // constant terminal 
			//(pro) ? (pro = pro) : \
				(pro) ? (root->con_val += temperature_factor) : (root->con_val -= temperature_factor);
			(pro) ? (root->con_val += rand() % temperature_factor) : (root->con_val -= rand() % temperature_factor);
		}
		return;
	} else {
		adjust_constant (root->left);	
		adjust_constant (root->right);	
	}
}

void 
numeric_mutation (struct node *pop[])
{
	int i, index, boundary;
	boundary = rand() % 400 + 100;

	for (i = 0; i < 600; i++) {
		index = tournament_selection ();
		//index = rand() % POP_SIZE;
		adjust_constant (pop[i]);
		//print_tree_inorder (pop[index]);
		//printf ("\n");
	}
	//exit (EXIT_FAILURE);
}

int 
evolution (void) 
{
	int n = 100, i, j, count = 0, zero_cnt = 0;
	struct node *pop[POP_SIZE];
	struct node *limbo_pop[POP_SIZE];
	float best_fitness = 0.0;
	int best_individual;
	float std_fitness_sum;
	float fitness_gap = 0.0;
	float raw_tf;

	srand (time(NULL));
	// Initialize population
	initialize_pop (pop, POP_SIZE);

	fitness = (float *)malloc(POP_SIZE * sizeof(float));

#ifdef PROP_SEL
	normalized_fitness = (float *)malloc(POP_SIZE * sizeof(float));
	standardized_fitness = (float *)malloc(POP_SIZE * sizeof(float));
#endif

	for (j = 0; j < GENERATION; j++) {
		//srand (seed++);
		std_fitness_sum = 0.0;

		//Numeric mutation
		if (j >= 1) {
			raw_tf = best_fitness * T_SCORE_PARM;
			temperature_factor = (int)raw_tf;
			if (temperature_factor >= 2)	numeric_mutation(pop);
			//if (temperature_factor <= 1) { temperature_factor = 2 + rand()%5; numeric_mutation(pop); }
			//else if (temperature_factor >= 5) numeric_mutation(pop);
			//else { temperature_factor += rand() % 5; numeric_mutation(pop); }
		}

		// fitness evaluation
		best_individual = fitness_evaluation (pop, &best_fitness, &std_fitness_sum);

		printf ("Generation %i: Best Fitness: %f\n", j, best_fitness);
		//printf ("Best Individual: %i\n", best_individual);
		//print_tree_inorder (pop[best_individual]);
		//printf ("\n");
	
		if (best_fitness <= termination) {
			printf ("Congratulations!!! You Have Found Best Solution !!! Which is:\n");
			print_tree_inorder (pop[best_individual]);
			printf ("\n");
			return SUCCESS;
		}

		// Reset Mechanism
		if (j >= 1) {
			fitness_gap += fabs (old_fitness - best_fitness); 
			if (fitness_gap < 0.1) { count++; }
			else { count = 0; fitness_gap = 0.0; }
			//printf ("fitness gap = %f, count = %d\n", fitness_gap, count);
			old_fitness = best_fitness;
			if (count == 100) {
				fitness_gap = 0.0;
				printf ("Whole population need to be re-initialized\n");
				// Re-initialize population 
				for (i=0; i<POP_SIZE; i++) {
					destroy_tree (pop[i]);
					initialize_pop (pop, POP_SIZE);
				}
				count = 0;
				continue;
			} 
		} else {	// Record current best fitness value
			old_fitness = best_fitness;
		}

#ifdef PROP_SEL
		// calculate normalized fitness 
		for (i=0; i<POP_SIZE; i++) {
			normalized_fitness[i] = standardized_fitness[i] / std_fitness_sum; 
		}
#endif

		// generate new population
		// selection ---> crossover or mutation
		selection (pop, limbo_pop, best_individual);

		// replace old population with new
		for (i=0; i<POP_SIZE; i++) {
			destroy_tree (pop[i]);
			pop[i] = limbo_pop[i];
			limbo_pop[i] = NULL;
		}
	}

	return FAIL;
}

void 
read_dataset (const char *file_name)
{
	int i = 0;
	FILE *fp;

	input = (float *)malloc(INPUT_NUM * (dimension + 1) * sizeof(float)); 
	// Read file to get the trainning data
	fp = fopen (file_name, "r");
	if (fp == NULL) {
		perror ("Open file failed");
		exit (EXIT_FAILURE);
	}

	while (!feof(fp)) {
		fscanf (fp, "%f", &input[i++]);
	}
}

int main(int argc, char *argv[]) 
{
	FILE *fp;
	int succ_num;

	if (argc == 2) {
		read_dataset (argv[1]);
	} else {
		read_dataset ("problem.dat");
	}

	fp = fopen ("result", "r+");
	if (fp == NULL) {
		perror ("Open file failed");
		exit (EXIT_FAILURE);
	}   
	fscanf (fp, "%d", &succ_num);
	rewind (fp);

	if (evolution() == FAIL) {
		//printf ("GP system unable to find a suitable solution.\n");
	} else {
		succ_num++;
		fprintf (fp, "%d", succ_num);
	}   

	return 0;
}

