/* file		: partitionProblem.c
 * author	: Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date	    : October 
 * version	: 1.0
 */

/* Description:
 * Given two sets of positive integers, we create a pair of disjoint sets.
 * The union of these two sets must be the original set, and the sum of the
 * integers in each set must equal each other.
 * We shall use a genetic algorithm to determine these two sets.
 * Input: _Precisely_ SIZE_ORIGINAL_SET (20) integers, separated by whitespace
 * Output: The solution chromosome, and how the chromosome leads to the best
 * solution it comes to.
 */

#include "partitionProblem.h"


int main(int argc, char *argv[]) {
  time_t t;
  // Set seed for random generator
  srand((unsigned int) time(&t));

  set_t set;
  chromo_t generation[POP_SIZE];

  getInitialSet(set);
#if DEBUG && !SIM_DEBUG
  printOriginalSet(set);
  
  printf("\nGENERATION\n");
  for (int i = 0; i < POP_SIZE; i++){
    generateRandomChromosome(&generation[i], set);
    printf("Chromosome %d: ", i);
    printChromosome(generation[i]);
    printf("fitness: %d\n", generation[i].fitness);
  }
  printf("\nAFTER SORTING\n");
  sortChromos(generation);
  for (int i = 0; i < POP_SIZE; i++){
    printf("Chromosome %d: ", i);
    printChromosome(generation[i]);
    printf("fitness: %d\n", generation[i].fitness);
  }

  printf("\nAFTER SELECTION\n");
  performSelection(set, generation);
  for (int i = 0; i < POP_SIZE; i++){
    printf("Chromosome %d: ", i);
    printChromosome(generation[i]);
    printf("fitness: %d\n", generation[i].fitness);
  }
  
  printf("\nAFTER NEWGENERATION\n");
  generateNewGeneration(generation, set);
  for (int i = 0; i < POP_SIZE; i++){
    printf("Chromosome %d: ", i);
    printChromosome(generation[i]);
    printf("fitness: %d\n", generation[i].fitness);
  }
#endif

#if DEBUG && SIM_DEBUG
  int numIterations = simulateEvolution(set);

  printf("Number of Iterations: %d\n", numIterations);
#endif

#if !DEBUG
  int numIterations = simulateEvolution(set);

  printf("Number of Iterations: %d\n", numIterations);
#endif 

  return 0;
}
