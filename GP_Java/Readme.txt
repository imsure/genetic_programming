First version of GP in java. (Convert from C)

Has Three classes: 
* GPTree
* GPSetting
* GPEvolve

Has Fixed Input (Embedded in the GPSetting class)
Constants only range from -5 to 5 (need to improve, like automatic adjusted constants range)
Don't have GUI, print output on the console

Baselines:

1. Implement GPTree and GPSetting class, test the operations on trees, like generating tree(random), print tree in-order, evalate the tree, get tree's depth, etc. 
2. Implement part of GPEvolve, get first generation, evlate first generation and get the best fitness value and best individual.
3. Implement tournament slection in GPEvolve.
4. Improve GPTree, add a constructor used to get a copy of tree, add static methods crossover and mutation.
5. Finish GPEvolve.
