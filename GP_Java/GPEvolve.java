import acm.program.*;
import acm.util.RandomGenerator;

/**
 * 
 * @author Shuo Yang
 * 
 * The GPEvolve class performs the evolution of the whole Genetic Programming System
 *
 */
public class GPEvolve extends Program {
	
	/**
	 * Initialize the first generation
	 */
	private void initPop() {
		int i;
		for (i=0; i<GPSettings.POP_SIZE; i++) {
			pop[i] = new GPTree();
			pop[i].randomGrow();
		}
	}
	
	/**
	 * Evaluate the fitness of the current generation
	 * @return the best fitness value
	 */
	private double fitnessEval() {
		int i;
		double temp;
		fitness[0] = pop[0].getFitnessVal();
		double best = fitness[0];
		for (i=1; i<GPSettings.POP_SIZE; i++) {
			fitness[i] = pop[i].getFitnessVal();
			if (fitness[i] < best) {
				best = fitness[i];
				bestIndivdual = i;
			}
		}
		return best;
	}
	
	/**
	 * Tournament among a subset of the population
	 * Select the best individual of the subset
	 * @return the best individual we find in the subset
	 */
	private int tournament() {
		int i, index, bestOne;
		index = rgen.nextInt(GPSettings.POP_SIZE);
		bestOne = index;
		double localBestFitness = fitness[index];
		for (i=1; i<GPSettings.TournamentSize; i++) {
			index = rgen.nextInt(GPSettings.POP_SIZE);
			if (fitness[index] < localBestFitness) {
				localBestFitness = fitness[index];
				bestOne = index;
			}
		}
		return bestOne;
	}
	
	/**
	 * Perform selection to the current generation and generate the new generation
	 */
	private void Selection() {
		int i, index1, index2;
		GPTree indiv1, indiv2;
		for (i=0; i<GPSettings.POP_SIZE; i++) {
			if (rgen.nextDouble() <= GPSettings.CrossoverProb) {
				index1 = tournament();
				index2 = tournament();
				indiv1 = new GPTree(pop[index1]);
				indiv2 = new GPTree(pop[index2]);
				newPop[i] = GPTree.Crossover(indiv1, indiv2);
			} else {
				index1 = tournament();
				indiv1 = new GPTree(pop[index1]);
				newPop[i] = GPTree.Mutation(indiv1);
			}
		}
		
		for (i=0; i<GPSettings.POP_SIZE; i++) {
			pop[i] = null;
			pop[i] = newPop[i];
		}
	}
	
	private void CrossoverMutationTest() {
		int i, index1, index2, index3;
		GPTree indiv1, indiv2, indiv3;
		//for (i=0; i<GPSettings.POP_SIZE; i++) {
			index1 = tournament();
			index2 = tournament();
			indiv1 = new GPTree(pop[index1]);
			indiv2 = new GPTree(pop[index2]);
			print("indiv1: \n");
			indiv1.printTree();
			print("indiv2: \n");
			indiv2.printTree();
			indiv1 = GPTree.Crossover(indiv1, indiv2);
			print("CrossOver: \n");
			indiv1.printTree();
			index3 = tournament();
			indiv3 = new GPTree(pop[index3]);
			print("indiv3: \n");
			indiv3.printTree();
			GPTree.Mutation(indiv3);
			print("Mutation: \n");
			indiv3.printTree();
		//}
	}
	
	/**
	 * Default Constructor
	 */
	public GPEvolve() {
		int i;
		pop = new GPTree[GPSettings.POP_SIZE];
		newPop = new GPTree[GPSettings.POP_SIZE];
		fitness = new double[GPSettings.POP_SIZE];
		bestFitness = new double[GPSettings.POP_SIZE];
		problemSolved = 0;
		rgen.setSeed(123);
	}
	
	/**
	 * Run method in which :
	 * 
	 * current Generation ---> Fitness Evaluation ---> Selection ---> New Generation --->
	 *    ^                                                                             | 
	 *    |                                                                             |
	 *    -------------------------------------------------------------------------------
	 *    
	 *    Loop until we find the qualified individual or task fails
	 */
	public void run() {
		int i, fitnessGapCounter;
		double fitnessGap = 0.0;
		initPop();
		fitnessGapCounter = 0;
		//CrossoverMutationTest();
		for (i=0; i<GPSettings.MaxGeneration; i++){
			// fitness evaluation
			bestFitness[i] = fitnessEval();
			print("Generation " + i +" : Best Individual:\n");
			pop[bestIndivdual].printTree();
			print("Fitness: " + bestFitness[i] + "\n");
			
			if (bestFitness[i] <= GPSettings.terminationVal) {
				print("Problem Solved !!!\n");
				problemSolved = 1;
				break;
			} else {
				if (i == 0) {
					Selection();
				} else {
					fitnessGap += Math.abs(bestFitness[i] - bestFitness[i-1]);
					fitnessGapCounter++;
					if (fitnessGapCounter == 10) {
						if (fitnessGap >= 10.0) {
							Selection();
						} else {
							print("\nReinitialize Population\n");
							int j;
							for (j=0; j<GPSettings.POP_SIZE; j++) {
								pop[j] = null;
							}
							initPop();
						}
						fitnessGap = 0.0;
						fitnessGapCounter = 0;
					} else {
						Selection();
					}
				}
			}
		}
		if (problemSolved == 0) {
			print("Mission Failed...\n");
		}
	}
	
	/* Standard Java entry point */
	/* This method can be eliminated in most Java environments */
	public static void main(String[] args) {
		new GPEvolve().start(args);
	}

	/**
	 * Current Population
	 */
	private GPTree[] pop;
	
	/**
	 * New Population
	 */
	private GPTree[] newPop;
	
	/**
	 * Fitness value for the current population
	 */
	private double[] fitness;
	
	/**
	 * Best fitness of each generation
	 */
	private double[] bestFitness;
	
	/**
	 * Individual of the best fitness in current generation
	 */
	private int bestIndivdual;
	
	/**
	 * Indicate if problem is solved ?
	 * 0 : no
	 * 1 : yes
	 */
	private int problemSolved;
	
	/**
	 * Random number generator
	 */
	private static RandomGenerator rgen = RandomGenerator.getInstance();
}
