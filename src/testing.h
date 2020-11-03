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
#define TESTING 0

// How finely grained should T2 be
#define T2_GRAIN 10
// The minimum percentage similarity sougth
#define T2_MIN_PER_SIM 0.8

#define T2_CONFIG_NUM_SIMS 100

// Functions
void findT2();
bool sufficientT2(int t2);
int   findAverageIterationLength(int *longestIter);

#endif