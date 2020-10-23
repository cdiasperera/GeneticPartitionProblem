/* file     : simConfig.h
 * author   : Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date     : October 2020
 * version  : 1.0
 */

/* Description:
 * Constants, types and function declarations relation to the simulation
 * configuration.
 */
#ifndef SIMCONFIG_H
#define SIMCONFIG_H

#include <stdbool.h>
#include "partitionProblem.h"

#define TESTING 1

// Termination conditions
#define MAX_ITER 1000
#define MAX_ITER_WITHOUT_IMPROVEMENT  (MAX_ITER / 10)

#define POP_SIZE 10

// Mutation configuration
#define NUM_CROSS_OVERS 1
#define NUM_MUTATIONS 1
// Maximum = POP_SIZE / 2
#define NUM_CHROMOS_REPLACED 2

#define MAX_NUM_CROSS_OVERS (POP_SIZE / 2)
#define MAX_NUM_MUTATIONS POP_SIZE
// Maximum = POP_SIZE / 2
#define MAX_NUM_CHROMOS_REPLACED ((POP_SIZE / 2) - 1)

// Global simulation configuration
typedef struct {
  int numCrossOvers;
  int numMutations;
  int numChromosReplaced;
} simConfig_t;

simConfig_t simConfig;

// Number of iterations to test a configuration
#define CONFIG_NUM_TESTS 500

// FUNCTIONS

void printConfig(simConfig_t simConfig);

#endif