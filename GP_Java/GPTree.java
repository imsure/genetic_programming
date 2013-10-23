
import acm.program.*;
import acm.util.RandomGenerator;
/**
 * 
 * @author Shuo Yang
 * 
 * The GPTree class represent a program tree in Genetic Programming System
 *
 */
public class GPTree extends Program {
	
	/**
	 * 
	 * @author Shuo Yang
	 * 
	 * The Node class represent a node of GPTree
	 *
	 */
	private static class Node {
		/**
		 * Reference to the left node
		 */
		Node left;
		/**
		 * Reference to the right node
		 */
		Node right;
		/**
		 * Data stored in the node which indicate what the node represent:
		 * 1. If it is a leaf node, then:
		 * 		0 ~ Dimension-1 : Variable;
		 *      >= Dimension : Constant;
		 * 2. If it is not a leaf, it represents an operator:
		 * 		0 : '+';
		 * 		1 : '-';
		 * 		2 : '*';
		 * 		3 : '/';
		 */
		int data;
		
		/**
		 * Default constructor for Node
		 * Just Sets left and right as null
		 */
		Node() {
			left = null;
			right = null;
		}
		
		/**
		 * 
		 * @param newData set a new data to the node
		 */
		public void setValue (int newData) {
			data = newData;
		}
	}
	
	/**
	 * 
	 * @return a random index of operator between 0 ~ 3
	 */
	private static int randomOperator() {
		return rgen.nextInt(4);	// [0, 4)
	}
	
	/**
	 * 
	 * @return a random index of operand, can be either variable or constant, chance is half and half 
	 */
	private static int randomOperand() {
		if (rgen.nextBoolean()) {
			return rgen.nextInt() % GPSettings.Dimension;
		} else {
			return rgen.nextInt(settings.getNumOfConstants()) + GPSettings.Dimension;
		}
	}
	
	/**
	 * Randomly generate a program tree (Public Method)
	 */
	public void randomGrow() {
		root = randomGrow (root, GPSettings.MaxDepth);
	}
	
	/**
	 * Randomly generate a program tree (Private Method)
	 * @param root top node of the tree
	 * @param depth depth of the tree
	 * @return new program tree
	 */
	private static Node randomGrow(Node root, int depth) {
		if (root == null) {
			root = new Node();
		}
		
		if (depth == GPSettings.MaxDepth || ((depth != 0) && rgen.nextBoolean())) {
			root.setValue(randomOperator());	// operator
			root.left = randomGrow(root.left, depth-1);
			root.right = randomGrow(root.right, depth-1);
		} else {
			root.setValue(randomOperand());	// operand
		} 
		return root;
	}
	
	/**
	 * Get the max depth of the programming tree (Public Method)
	 * @return 
	 */
	public int maxDepth() {
		return maxDepth(root);
	}
	
	/**
	 * Get the max depth of the programming tree (Private Method)
	 * @param root
	 * @return
	 */
	private static int maxDepth(Node root) {
		int leftDepth = 0;
		int rightDepth = 0;
		if (root.left == null) {
			return 0;
		} else {
			leftDepth = maxDepth(root.left) + 1;
			rightDepth = maxDepth(root.right) + 1;
			return Math.max(leftDepth, rightDepth);
		}
	}
	
	/**
	 * Print the Program tree on console (Public Method)
	 */
	public void printTree() {
		printTree(root);
		print("\n");
	}
	
	/**
	 * Print the Program tree on console (Private Method)
	 * @param root
	 */
	private static void printTree(Node root) {
		if (root.left != null) {
			System.out.print("(");
			printTree(root.left);
			System.out.print(settings.operator2char(root.data));
			printTree(root.right);
			System.out.print(")");
		} else {
			System.out.print(settings.operand2string(root.data));
		}
	}
	
	/**
	 * Evaluate the fitness of the tree
	 * @param root
	 * @param varSet index of input number 
	 * @return fitness value of the tree
	 */
	private double eval(Node root, int varSet) {
		if (root.left != null) {
			switch (root.data) {
			case GPSettings.ADD : 
				return eval(root.left, varSet) + eval(root.right, varSet);
			case GPSettings.SUB : 
				return eval(root.left, varSet) - eval(root.right, varSet);
			case GPSettings.MUL : 
				return eval(root.left, varSet) * eval(root.right, varSet);
			case GPSettings.DIV : {
				double leftNum = eval(root.left, varSet);
				double rightNum = eval(root.right, varSet);
				if (Math.abs(rightNum) <= 0.001) {
					return leftNum;
				} else {
					return leftNum / rightNum;
				}
			}
			}
			return 0.0;	// should never get here
		} else {
			if (root.data < GPSettings.Dimension) {
				return settings.getInputX(root.data + GPSettings.Dimension * varSet);
			} else {
				return settings.getConstant(root.data - GPSettings.Dimension);
			}
		}
	}
	
	/**
	 * Return the fitness value of the tree
	 * @return
	 */
	public double getFitnessVal() {
		int i;
		for (i=0; i<GPSettings.INPUT_NUM; i++) {
			fitness += Math.abs(eval(root, i) - settings.getInputY(i));
			//print("Temp Fitness : " + fitness + "\n");
		}
		return fitness;
	}
	
	/**
	 * Get a copy of a specific node
	 * @param dst copy of node
	 * @param src original node
	 * @return new node
	 */
	private Node getCopyOfNode(Node dst, Node src) {
		dst = new Node();
		dst.data = src.data;
		if (src.left == null) {
			return dst;
		} else {
			dst.left = getCopyOfNode(dst.left, src.left);
			dst.right = getCopyOfNode(dst.right, src.right);
			return dst;
		}
	}
	
	/**
	 * Default Constructor, do nothing
	 */
	public GPTree() {
	}
	
	/**
	 * Another Constructor, get a copy of a specific tree
	 * @param another the tree we want copy
	 */
	public GPTree(GPTree another) {
		root = getCopyOfNode(root, another.root);
	}
	
	/**
	 * Find a specific location of the tree
	 * @param orig original tree
	 * @param pos position we want to locate
	 * @return the node point to the location we find
	 */
	private static Node locateOld(Node orig, int pos) {
		int i;
		Node loc = new Node();
		if (pos == 0) {
			loc = orig;
		} else {
			for (i=0; i<pos; i++) {
				if (orig.left.left == null) {
					loc = orig.right;
				} else if (orig.right.left == null) {
					loc = orig.left;
				} else {
					if (rgen.nextBoolean()) {
						loc = orig.left;
					} else {
						loc = orig.right;
					}
				}
			}
		}
		return loc;
	}
	
	private static Node locate(Node orig, int pos) {
		Node loc = new Node();
		if (pos == 0) {
			loc = orig;
		} else {
			if (maxDepth(orig.left) < pos) {
				loc = locate(orig.right, pos-1);
			} else if (maxDepth(orig.right) < pos) {
				loc = locate(orig.left, pos-1);
			} else {
				if (rgen.nextBoolean()) {
					loc = locate(orig.right, pos-1);
				} else {
					loc = locate(orig.left, pos-1);
				}
			}
		}
		return loc;
	}
	
	/**
	 * Perform Crossover of two trees
	 * @param n1 First tree
	 * @param n2 Second tree
	 */
	private static void Crossover(Node n1, Node n2) {
		Node sub1 = new Node();
		Node sub2 = new Node();
		Node tmp = new Node();
		int pos1, pos2;
		pos1 = rgen.nextInt(maxDepth(n1));
		pos2 = rgen.nextInt(maxDepth(n2));
		sub1 = locate(n1, pos1);
		sub2 = locate(n2, pos2);
		if (rgen.nextBoolean()) {
			tmp = sub2.left;
			sub2.left = null;
		} else {
			tmp = sub2.right;
			sub2.right = null;
		}
		if (rgen.nextBoolean()) {
			sub1.left = null;
			sub1.left = tmp;
		} else {
			sub1.right = null;
			sub1.right = tmp;
		}
	}
	
	/**
	 * Public Method of Crossover
	 * @param t1 First Tree
	 * @param t2 Second Tree
	 * @return Offspring
	 */
	public static GPTree Crossover(GPTree t1, GPTree t2) {
		Crossover(t1.root, t2.root);
		t2 = null;
		return t1;
	}
	
	/**
	 * Perform Mutation of a tree
	 * @param old
	 */
	private static void Mutation(Node old) {
		Node mutatePoint = new Node();
		Node branch = new Node();
		int pos, branchSize;
		pos = rgen.nextInt(maxDepth(old));
		mutatePoint = locate(old, pos);
		branchSize = rgen.nextInt(GPSettings.MaxDepth - 1);
		branch = randomGrow(branch, branchSize);
		if (rgen.nextBoolean()) {
			mutatePoint.right = null;
			mutatePoint.right = branch;
		} else {
			mutatePoint.left = null;
			mutatePoint.left = branch;
		}
	}

	/**
	 * Public Method of Mutation
	 * @param tree
	 * @return the tree after mutation
	 */
	public static GPTree Mutation(GPTree tree) {
		Mutation(tree.root);
		return tree;
	}
	
	/**
	 * Represent the root node of the tree
	 */
	private Node root = null;
	
	/**
	 * Instance of class GPSettings
	 */
	private static GPSettings settings = new GPSettings();
	
	/**
	 * Fitness value of the tree
	 */
	private double fitness = 0.0;
	
	/**
	 * Random number generator
	 */
	private static RandomGenerator rgen = RandomGenerator.getInstance();
	
}
