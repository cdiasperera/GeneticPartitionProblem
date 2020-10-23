/* file     : simconfig.c
 * author   : Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date     : October 2020
 * version  : 1.0
 */

/* Description:
 * Function relation to the simulation configuration.
 */
#include <stdio.h>
#include "simconfig.h"

// Prints out the coniguration information
void printConfig(simConfig_t simConfig) {
  printf("Simulation Configuration:\n");
  printf(
    "Cross-overs: %d, Mutations: %d, Selection Replacement: %d\n\n",
    simConfig.numCrossOvers,
    simConfig.numMutations,
    simConfig.numChromosReplaced
  );
}