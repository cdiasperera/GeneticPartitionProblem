/* file     : main.c
 * author   : Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date     : October 2020
 * version  : 1.0
 */

// LIBRARY IMPORTS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

// CONSTANTS

#define DEBUG 1
#define SIM_DEBUG 2

#if DEBUG
#define  SIZE_ORIGINAL_SET 10
#else
#define SIZE_ORIGINAL_SET 20 
#endif

#define CHROMO_LENGTH SIZE_ORIGINAL_SET
#define POP_SIZE 10

// Converge statuses
#define CONVERGING 0
#define SOLUTION_FOUND 1
#define NO_IMPROVEMENT -1

// Indexes of the best/worst chromosome
#define BEST_CHROMO 0
#define WORST_CHROMO (POP_SIZE - 1)

// The worst possible fitness
#define WORST_FITNESS INT_MAX

// Output constants
#define DIVIDER_CHAR '='
#define DIVIDER_LEN 80

// TYPES
typedef bool gene_t;

typedef struct {
  gene_t genes[CHROMO_LENGTH];
  int fitness;
} chromo_t;

typedef int set_t[SIZE_ORIGINAL_SET];
// FUNCTIONS

// Functions to initialize program
void  getInitialSet(set_t set);

// Main functions for Genetic Algorithm
int   simulateEvolution(set_t set);
void  makeInitialGenration(set_t set, chromo_t *generation);
void  performSelection(set_t set, chromo_t *generation);
int   converges(
  set_t set,
  chromo_t *generation,
  chromo_t *solChromo,
  int *numIterNoImprov,
  int prevBestFitness
);
void  generateNewGeneration(set_t set, chromo_t *generation);

// Functions to manipulate choromosomes
void  sortChromos(chromo_t *generation);
void  swap(int i, int j, chromo_t *generation);
void  copyChromo(chromo_t *dst, chromo_t src);
void  replaceChromos(
  int *strongChromos,
  int *weakChromos,
  chromo_t *generation,
  int numReplaced
);

// Functions to modify genes
void  generateRandomChromo(set_t set, chromo_t *chromo);
void  mutateSingleGene(set_t set, chromo_t *chromo);
void  chromoCrossOver(set_t set, chromo_t *chromo1, chromo_t *chromo2);

// Functions related to fitness
int   heightOfSet(set_t set, bool chosenSet, chromo_t chromo);
int   measureFitness(set_t set, chromo_t chromo);
int   setDifference(set_t set, chromo_t chromo);

// Utlity Functions - Program specific
void  printOriginalSet(set_t set);
void  printOutput(set_t set, int convergeStatus, chromo_t chromo);
void  printChromo(chromo_t chromo);
void  printSets(set_t set, chromo_t chromo);
void  printSet(set_t set, bool chosenSet, chromo_t chromo);

// Utility Functions - Program agnostic
int   randInt(int lowerBound, int upperBound);
void printDivider(int len);