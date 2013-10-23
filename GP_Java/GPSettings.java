/**
 * @author Shuo Yang
 * 
 * The GPSettings class contains all the settings of Genetic Programming System
 */

public class GPSettings {
	/**
	 *  Max Depth of Program Trees(Individuals) 
	 */
	public static final int MaxDepth = 5;
	/**
	 * All the constants we need in the GP system
	 */
	private static final int[] constants = {-5,-4,-3,-2,-1,1,2,3,4,5};	// 10 constants
	/**
	 * Number of Variable
	 */
	public static final int Dimension = 1;	// we need only one variable 'x', maybe 2 ?
	/**
	 * Function Set
	 */
	private static final char[] operators = {'+', '-', '*', '/'};
	/**
	 * Training Data (X)
	 */
	private static final int[] inputX = {-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9};
	/**
	 * Training Data (Y)
	 */
	private static final double[] inputY = {40, 31.5, 24, 17.5, 12, 7.5, 4, 1.5, 0, -0.5, 
	                                        0, 1.5, 4, 7.5, 12, 17.5, 24, 31.5, 40};
	
	public static final int ADD = 0; 
	public static final int SUB = 1; 
	public static final int MUL = 2; 
	public static final int DIV = 3; 
	
	/**
	 * Number of Training data
	 */
	public static final int INPUT_NUM = 19;
	
	/**
	 * Population Size
	 */
	public static final int POP_SIZE = 1000;
	
	/**
	 * Tournament Size (How many individuals we choose to select at one time)
	 */
	public static final int TournamentSize = 50;
	
	/**
	 * Number of Generation 
	 */
	public static final int MaxGeneration = 1000;
	
	/**
	 * Probability of Crossover
	 */
	public static final double CrossoverProb = 0.8;
	/**
	 * Probability of Mutation
	 */
	public static final double MutationProb = 0.2;
	
	/**
	 * Termination Condition (When we stop Evolution)
	 */
	public static final double terminationVal = 0.001;
	
	/**
	 * 
	 * @param randomNum
	 * @return a random constant number
	 */
	public int getRandomConstant(int randomNum) {
		return constants[randomNum];
	}
	
	/**
	 * 
	 * @return The total number of constants we have
	 */
	public int getNumOfConstants() {
		return constants.length;
	}
	
	/**
	 * 
	 * @param num indicate which operator we want (should be 0~3)
	 * 0 : +
	 * 1 : -
	 * 2 : *
	 * 3 : /
	 * @return operator in char format
	 */
	public char operator2char(int num) {
		return operators[num];
	}
	
	/**
	 * 
	 * @param num indicate which operand we want
	 * 0 ~ Dimension-1 : variable
	 * >= Dimension : constant
	 * @return operand in string format
	 */
	public String operand2string(int num) {
		if (num < Dimension) {
			String var = "x" + num;
			return var;
		} else {
			String con = "" + constants[num - Dimension];
			return con;
		}
	}
	
	/**
	 * 
	 * @param count indicate which training data(X) we want
	 * @return X-value
	 */
	public int getInputX(int count) {
		return inputX[count];
	}
	
	/**
	 * 
	 * @param count indicate which training data(Y) we want
	 * @return Y-value
	 */
	public double getInputY(int count) {
		return inputY[count];
	}
	
	/**
	 * 
	 * @param count indicate which constant we want
	 * @return Constant Value
	 */
	public int getConstant(int count) {
		return constants[count];
	}
}
