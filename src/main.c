/* file     : main.c
 * author   : Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date     : October 2020
 * version  : 1.0
 */

/* Description:
 * Given two sets of positive integers, we create a pair of disjoint sets.
 * The union of these two sets must be the original set, and the sum of the
 * integers in each set must equal each other.
 * We shall use a genetic algorithm to determine these two sets.
 * 
 * Input: _Precisely_ SIZE_ORIGINAL_SET (20) integers, separated by whitespace
 * 
 * Output: The solution chromosome, and how the chromosome leads to the best
 * solution it comes to.
 * 
 * Note: This program was written as part of the assessment for the course
 * "Introduction to Computer Science".
 */

#include "partitionProblem.h"
#include "simconfig.h"


int main(int argc, char *argv[]) {
  time_t t;
  // Set seed for random generator
  srand((unsigned int) time(&t));

  set_t set;
  chromo_t generation[POP_SIZE];


#if TESTING
  int bestIter = INT_MAX;
  simConfig_t bestConfig;

  // Go upto and including max. Need at least one mutation.
  // No selection = Very bad performance
  for (int i = 0; i <= MAX_NUM_CROSS_OVERS; i++) {
    for (int j = 1; j <= MAX_NUM_MUTATIONS; j++) {
      for (int k = 1; k <= MAX_NUM_CHROMOS_REPLACED; k++) {
        // Define simulation configuration programatically
        simConfig.numCrossOvers = i;
        simConfig.numMutations = j;
        simConfig.numChromosReplaced = k;

        // Create set programatically
        int totalIter = 0;
        for (int i = 1; i <= CONFIG_NUM_TESTS; i++) {
          getInitialSet(set, true);
          totalIter += simulateEvolution(set);
        }

        int averageIter = totalIter / CONFIG_NUM_TESTS;
        if (averageIter < bestIter) {
          bestConfig = simConfig;
          bestIter = averageIter;
        }
        
        // Print out configuration results.
        printf("Running average number of iterations: %d\n", averageIter);
        printConfig(simConfig);
      }
    }
  }

  // Print best config
  printf("Best config:\n");
  printf("Average number of iterations: %d\n", bestIter);
  printConfig(bestConfig);

#endif

#if !TESTING
  // Grab predefined simulation configuration
  simConfig.numCrossOvers = NUM_CROSS_OVERS;
  simConfig.numMutations = NUM_MUTATIONS;
  simConfig.numChromosReplaced = NUM_CHROMOS_REPLACED;

  // Don't create set programatically, read from input
  getInitialSet(set, false);
  int numIterations = simulateEvolution(set);

  printf("Number of Iterations: %d\n", numIterations);
#endif 

  return 0;
}
