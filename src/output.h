/* file     : output.h
 * author   : Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date     : October 2020
 * version  : 1.0
 */

/* Description:
 * Function declarations for writing to stdin.
 */
#ifndef OUTPUT_H
#define OUTPUT_H

#include "partitionProblem.h"
#include "simconfig.h"

void printOriginalSet(set_t set);

void printOutput(set_t set, int convergeStatus, chromo_t chromo);

void printChromo(chromo_t chromo);

void printSets(set_t set, chromo_t chromo);
void printSet(set_t set, bool chosenSet, chromo_t chromo);

void printDivider(int len);

#endif
