/* file     : testing.c
 * author   : Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date     : October 2020
 * version  : 1.0
 */

/* Description:
 * Function relation to the testing the program.
 */

#include "testing.h"
#include "string.h"
#include "output.h"

void findT2() {
  simConfig.numChromosReplaced = NUM_CHROMOS_REPLACED;
  simConfig.numCrossOvers = NUM_CROSS_OVERS;
  simConfig.numMutations = NUM_MUTATIONS;
  int t2 = 1;
  while (!sufficientT2(t2)) {
    t2 *= 10;
  }

  // Fine grain t2
  int fineGrainedT2;
  for (int i = 1; i <= T2_GRAIN; i++) {
    if (sufficientT2((i * t2) / T2_GRAIN)) {
      fineGrainedT2 = ((i * t2) / T2_GRAIN);
      break;
    }
  }

  printf("Use t_2 of %d\n", fineGrainedT2);

}

bool sufficientT2(int t2) {
  printf("t_2 = %d\n", t2);

  simConfig.t2 = t2;
  int results[T2_CONFIG_NUM_SIMS];
  set_t set;
  getInitialSet(set, true);
  // Fill results
  for (int i = 0; i < T2_CONFIG_NUM_SIMS; i++) {
    // See how far through this run of t_2 we are
    if (i % (T2_CONFIG_NUM_SIMS / 50) == 0) {
      printf("|");
      fflush(stdout);
    }

    simulateEvolution(set , results + i);
  }
  printf("|\n");

  // See how well this t2 did
  int smallestVal = results[0];
  int numSmallest = 1;
  for (int i = 1; i < T2_CONFIG_NUM_SIMS; i++) {
    if (smallestVal == results[i]) {
      numSmallest++;
    } else if (smallestVal > results[i]) {
      smallestVal = results[i];
      numSmallest = 1;
    } else {
      continue;
    }
  }
  printf(
    "Perfect similarity: %d%%\n\n",
    (int) (numSmallest * 100.0) / T2_CONFIG_NUM_SIMS
  );

  // Check if t2 is good enough. Floating point math here, but that's okay
  if (numSmallest >= 0.8 * T2_CONFIG_NUM_SIMS) {
    return true;
  } else {
    return false;
  }

}

// Determines the optimal configuration of parameters.
void determineOptimalSimConfig(set_t set) {
  int bestIter = INT_MAX;
  int bestConfigLongestIter = 0;
  simConfig_t bestConfig;

  /* Go upto and including max for each parameter. Need at least one mutation.
   */
  for (int i = 0; i <= MAX_NUM_CROSS_OVERS; i++) {
    for (int j = 1; j <= MAX_NUM_MUTATIONS; j++) {
      for (int k = 1; k <= MAX_NUM_CHROMOS_REPLACED; k++) {
        // The longest iteration for this set of parameters
        int longestIter = 0;

        // Define simulation configuration programatically
        simConfig.numCrossOvers = i;
        simConfig.numMutations = j;
        simConfig.numChromosReplaced = k;
        simConfig.t2 = MAX_T2;

        int averageIter = findAverageIterationLength(&longestIter);
        if (averageIter < bestIter) {
          bestConfig = simConfig;
          bestIter = averageIter;

          /* This is the set of parameters we will be using and thus, we are
           * interested in knowing the maximum amount of iterations required
           */
          bestConfigLongestIter = longestIter;
        }
        
        // Print out configuration results.
        printf("Average number of iterations: %d\n", averageIter);
        printConfig(simConfig);
      }
    }
  }

  // Print best config
  printBestConfig(bestConfig, bestIter, bestConfigLongestIter);
}

// Prints out the best configuration
void printBestConfig(simConfig_t bestConfig, int avgIter, int longestIter) {
  printf("Best config:\n");
  printf("Average number of iterations: %d\n", avgIter);
  printf("Longest Iteration for this config: %d\n", longestIter);
  printConfig(bestConfig);
}

// Finds the average amount of iterations a configuration takes
int findAverageIterationLength(int *longestIter) {
  set_t set;
  // Create set programatically
  int totalIter = 0;


  simConfig_t original = simConfig;
  // Set simConfig to configuration that finds correct result 90% of the time
  simConfig.numCrossOvers = NUM_CROSS_OVERS;
  simConfig.numMutations = NUM_MUTATIONS;
  simConfig.numChromosReplaced = NUM_CHROMOS_REPLACED;
  simConfig.t2 = MAX_T2;

  int correctDiff = 0;
  // Find set such that the a.d.s is not 0
  while (correctDiff == 0) {
    getInitialSet(set, true);
    simulateEvolution(set, &correctDiff);
  }

  simConfig = original;
  // Assume longest iteration is longest it can possibly be.
  *longestIter = INT_MAX;

  // Get average iteration
  int numPassingSimulations = 0;
  for (int i = 1; i <= CONFIG_NUM_TESTS; i++) {
    int solDifference;
    int simIter = simulateEvolution(set, &solDifference);
    // Only update the average and longest iteration if the answer is correct
    if (solDifference <= correctDiff) {
      numPassingSimulations++;
      *longestIter = (*longestIter > simIter ? *longestIter : simIter);
      totalIter += simIter;
    }
  }

  /* If we fail more than 10% of tests, return INT_MAX as the average iteration
   * as failure is equal to the worst performance.
   */
  if (numPassingSimulations > (CONFIG_NUM_TESTS / 10)) {
    return totalIter / CONFIG_NUM_TESTS;
  } else {
    return INT_MAX;
  }
}

/* Given a parameter to vary, we determine the average amount of steps required
 * to find a solution, from when the parameter is 0, upto a given maximum,
 * keeping the other parameters constant.
 * 
 * In addition, we run this against 9 configurations of the other parameter
 * (setting the other two values to either: 0, mid or max).
 * 
 * Not my finest function, ngl.
 */
void testParams(set_t set, int chosenParam) {
  /* Determine maximum values for the symbolic parameters and their names
   * Also make sure that the parameter tracker for a mutation starts at 1.
   * index = 0 => Param we're testing
   */
  int paramMax[NUM_PARAMS];
  int paramStart[NUM_PARAMS];
  char paramNames[NUM_PARAMS][NAME_MAX_LEN];
  
  switch(chosenParam) {
    case CROSSOVERS:
      paramMax[0] = MAX_NUM_CROSS_OVERS;
      paramMax[1] = MAX_NUM_MUTATIONS;
      paramMax[2] = MAX_NUM_CHROMOS_REPLACED;
      strcpy(paramNames[0], CROSSOVERS_COL_NAME);
      strcpy(paramNames[1], MUTATIONS_COL_NAME);
      strcpy(paramNames[2], REPLACEMENTS_COL_NAME);
      paramStart[0] = 0;
      paramStart[1] = 1;
      paramStart[2] = 0;
      break;
    case MUTATIONS:
      paramMax[0] = MAX_NUM_MUTATIONS;
      paramMax[1] = MAX_NUM_CROSS_OVERS;
      paramMax[2] = MAX_NUM_CHROMOS_REPLACED;
      strcpy(paramNames[0], MUTATIONS_COL_NAME);
      strcpy(paramNames[1], CROSSOVERS_COL_NAME);
      strcpy(paramNames[2], REPLACEMENTS_COL_NAME);
      paramStart[0] = 1;
      paramStart[1] = 0;
      paramStart[2] = 0;
      break;
    case REPLACEMENTS:
      paramMax[0] = MAX_NUM_CHROMOS_REPLACED;
      paramMax[1] = MAX_NUM_CROSS_OVERS;
      paramMax[2] = MAX_NUM_MUTATIONS;
      strcpy(paramNames[0], REPLACEMENTS_COL_NAME);
      strcpy(paramNames[1], CROSSOVERS_COL_NAME);
      strcpy(paramNames[2], MUTATIONS_COL_NAME);
      paramStart[0] = 0;
      paramStart[1] = 0;
      paramStart[2] = 1;
      break;
    default:
      break;
  }
  // How much each parameter increments
  int paramInc[NUM_PARAMS] = {paramMax[0]/10, paramMax[1]/2, paramMax[2]/2};

  // Store results. +1 since we index by parameter value
  int results[paramMax[1]+1][paramMax[2]+1][paramMax[0]+1];

  // Iterate through configurations of parameters.
  printf("CHANGING PARAMETER: %s\n\n", paramNames[0]);
  for (int p1 = paramStart[1]; p1 <= paramMax[1]; p1 += paramInc[1]) {
    for (int p2 = paramStart[2]; p2 <= paramMax[2]; p2 += paramInc[2]) {
      for (int p0 = paramStart[0]; p0 <= paramMax[0]; p0 += paramInc[0]) {
        switch(chosenParam) {
          case CROSSOVERS:
            simConfig.numCrossOvers = p0;
            simConfig.numMutations = p1;
            simConfig.numChromosReplaced = p2;
            break;
          case MUTATIONS:
            simConfig.numMutations = p0;
            simConfig.numCrossOvers = p1;
            simConfig.numChromosReplaced = p2;
            break;
          case REPLACEMENTS:
            simConfig.numChromosReplaced = p0;
            simConfig.numCrossOvers = p1;
            simConfig.numMutations = p2;
            break;
          default:
            break;  
        }
        simConfig.t2 = TESTING_T2;

        int longestIter = 0;
        int averageIter = findAverageIterationLength(&longestIter);

        results[p1][p2][p0] = averageIter;
      }
    }
  }

  // Print results header
  for (int p1 = paramStart[1]; p1 <= paramMax[1]; p1 += paramInc[1]) {
    for (int p2 = paramStart[2]; p2 <= paramMax[2]; p2 += paramInc[2]) {
      printf("%s%d%s%d\t", paramNames[1], p1, paramNames[2], p2);
    }
  }
  printf("\n");
  printDivider(DIVIDER_LEN);

  // Print results
  for (int p0 = paramStart[0]; p0 <= paramMax[0]; p0 += paramInc[0]) {
    for (int p1 = paramStart[1]; p1 <= paramMax[1]; p1 += paramInc[1]) {
      for (int p2 = paramStart[2]; p2 <= paramMax[2]; p2 += paramInc[2]) {
        printf("%4d\t", results[p1][p2][p0]);
      }
    }
    printf("\n");
  }
}