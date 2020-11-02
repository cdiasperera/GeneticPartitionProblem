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
#include "testing.h"
#include "output.h"

int main(int argc, char *argv[]) {
  time_t t;
  // Set seed for random generator
  srand((unsigned int) time(&t));

  set_t set;
  chromo_t generation[POP_SIZE];

#if TESTING && FINDT2
  findT2();
#endif

#if TESTING && FINDOPTIMALCONFIG
  determineOptimalSimConfig(set);
#endif

#if TESTING && TESTPARAMS
  for (int chosenParam = 1; chosenParam <= NUM_PARAMS; chosenParam++) {
    testParams(set, chosenParam);
    printDivider(DIVIDER_LEN);
  }
#endif

#if !TESTING
  // Grab predefined simulation configuration
  simConfig.numCrossOvers = NUM_CROSS_OVERS;
  simConfig.numMutations = NUM_MUTATIONS;
  simConfig.numChromosReplaced = NUM_CHROMOS_REPLACED;
  simConfig.t2 = MAX_T2;

  // Don't create set programatically, read from input
  getInitialSet(set, false);
  int solDifference;
  int numIterations = simulateEvolution(set, &solDifference);

  printf("Number of Iterations: %d\n", numIterations);
#endif 

  return 0;
}
