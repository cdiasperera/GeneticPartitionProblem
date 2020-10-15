// LIBRARY IMPORTS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>


// CONSTANTS
#define SIZE_ORIGINAL_SET 4 // The number of integers in the original set
#define CHROMOSOME_LENGTH SIZE_ORIGINAL_SET // The length of each chromosome
#define POP_SIZE 10 // The number of chromosomes in a generation

#define MAX_ITER_WITHOUT_IMPROVEMENT 10 // The number of iterations allowed without fitness improvement

#define NUM_CROSS_OVERS 1
#define NUM_MUTATIONS 1
#define NUM_CHROMOSOMES_REPLACED 2

// TYPES
typedef bool gene_t;
typedef gene_t chromo_t[CHROMOSOME_LENGTH];
typedef int *set_t;
// FUNCTIONS

// Functions for Genetic Algorithm
void  getInitialSet(set_t originalSet);
void  generateRandomChromosome(chromo_t chromosome);
void  mutateSingleGene(chromo_t chromosome);
void  mutateChromosomeCrossOver(chromo_t chromosome1, chromo_t chromosome2);
int   heightOfSet(chromo_t chromosome, bool chosenSet, set_t orignalSet);
int   measureFitness(chromo_t chromosome, set_t originalSet);
void  replaceChromosomes(int *idxStrongChromos, int *idxWeakChromos, chromo_t *generation);
void  copyChromosome(chromo_t dest, chromo_t src);
int   performSelection(set_t originalSet, chromo_t *generation);
int   checkForConvergence(set_t originalSet, chromo_t *generation, chromo_t solChromo, int *numIterNoImprov);
void  generateNewGeneration(chromo_t *generation);
int   simulateEvolution(set_t originalSet, chromo_t *generation);

// Utlity Functions
void printChromosome(bool *chromosome);
void printOriginalSet(int *originalSet);