/*	file		: partitionProblem.c
	*	author	: Channa Dias Perera (c.dias.perera@student.rug.nl)
	* date	  : date
	* version	: 1.0
	*/

/*	Description:
	* Given two sets of positive integers, we create a pair of disjoint sets.
  * The union of these two sets must be the original set, and the sum of the
  * integers in each set must equal each other.
  * 
  * We shall use a genetic algorithm to determine these two sets.
	*/

#include "partitionProblem.h"

#define DEBUG 0

int main(int argc, char *argv[]) {
  time_t t;
  // Set seed for random generator
  srand((unsigned int) time(&t));

  int originalSet[SIZE_ORIGINAL_SET];
  bool generation[POP_SIZE][CHROMOSOME_LENGTH];

  getInitialSet(originalSet);
#if DEBUG 
  for (int i = 0; i < POP_SIZE; i++){
    generateRandomChromosome(generation[i]);
  }

  int chosenChromosome = CHROMOSOME_LENGTH - 1;
  printOriginalSet(originalSet);
  printChromosome(generation[chosenChromosome]);
  printf("fitness: %d\n", measureFitness(generation[chosenChromosome], originalSet));
#endif

  int numIterations = simulateEvolution(originalSet, generation);

  printf("Number of Iterations: %d\n", numIterations);

  return 0;
}