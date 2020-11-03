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

#include <stdio.h>
#include <stdbool.h>

#define  SIZE_ORIGINAL_SET 20
#define CHROMO_LENGTH SIZE_ORIGINAL_SET

// Termination conditions
#define MAX_T2 500000
#define MAX_ITER (10 * MAX_T2)

#define POP_SIZE 10

// Program configuration
#define NUM_CROSS_OVERS 4
#define NUM_MUTATIONS 20
// Maximum = POP_SIZE / 2
#define NUM_CHROMOS_REPLACED 2

/* Global simulation configuration. Since it's related to all the functions in
 * we decided to make it a global function.
 * 
 * If we decide to test multiple configurations programatically, this struct can
 * be used.
 */
typedef struct {
  int numCrossOvers;
  int numMutations;
  int numChromosReplaced;
  int t2;
} simConfig_t;

simConfig_t simConfig;

// FUNCTIONS

void printConfig(simConfig_t simConfig);

#endif