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
    "Percent similarity: %d%%\n\n",
    (int) (numSmallest * 100.0) / T2_CONFIG_NUM_SIMS
  );

  // Check if t2 is good enough. Floating point math here, but that's okay
  if (numSmallest >= T2_MIN_PER_SIM * T2_CONFIG_NUM_SIMS) {
    return true;
  } else {
    return false;
  }

}