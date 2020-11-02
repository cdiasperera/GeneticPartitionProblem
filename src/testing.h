/* file     : testing.h
 * author   : Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date     : October 2020
 * version  : 1.0
 */

/* Description:
 * Constants, types and function declarations relation to testing the program
 */
#ifndef TESTING_H
#define TESTING_H

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "simconfig.h"
#include "partitionProblem.h"

// Constants

// Setting of program. If we're testing and what sort of testing.
#define TESTING 1
#define FINDT2 0
#define TESTPARAMS 1
#define FINDOPTIMALCONFIG 0

// Constants for finding t2
#define T2_GRAIN 10

// Constants for when testing trends
#define CROSSOVERS 1
#define MUTATIONS 2
#define REPLACEMENTS 3

#define CROSSOVERS_COL_NAME "C"
#define MUTATIONS_COL_NAME "M"
#define REPLACEMENTS_COL_NAME "R"

#define MAX_NUM_CROSS_OVERS (4 * POP_SIZE+1)
#define MAX_NUM_MUTATIONS (4 * POP_SIZE+1)
#define MAX_NUM_CHROMOS_REPLACED ((POP_SIZE / 2)+1)

#define NAME_MAX_LEN 20
#define NUM_PARAMS 3

/* Number of iterations to test a configuration. It's so low due to simulation
 * unfortunatey.
 */
#define CONFIG_NUM_TESTS 10
#define T2_CONFIG_NUM_SIMS 100

// T2 for testing, so as to not take too long
#define TESTING_T2 300000

// Functions
void findT2();
bool sufficientT2(int t2);

void  determineOptimalSimConfig(set_t set);
void  printBestConfig(simConfig_t bestConfig, int avgIter, int longestIter);

int   findAverageIterationLength(int *longestIter);

void  testParams(set_t set, int chosenParam);
#endif