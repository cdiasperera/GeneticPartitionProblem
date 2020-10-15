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

typedef struct {
  gene_t genes[CHROMOSOME_LENGTH];
  int fitness;
} chromo_t;

typedef int set_t[SIZE_ORIGINAL_SET];
// FUNCTIONS

// Functions for Genetic Algorithm
int   simulateEvolution(set_t set, chromo_t *generation);
int   performSelection(set_t set, chromo_t *generation);

void  sortChromos(chromo_t *generation);
void  swap(int i, int j, chromo_t *generation);

void  getInitialSet(set_t set);
void  generateRandomChromosome(chromo_t *chromosome, set_t set);
void  mutateSingleGene(chromo_t chromosome);
void  chromoCrossOver(chromo_t chromo1, chromo_t chromo2, set_t set);
int   heightOfSet(chromo_t chromosome, bool chosenSet, set_t orignalSet);
int   measureFitness(chromo_t chromosome, set_t set);
void  replaceChromosomes(int *idxStrongChromos, int *idxWeakChromos, chromo_t *generation);
void  copyChromosome(chromo_t dest, chromo_t src);
int   converges(set_t set, chromo_t *generation, chromo_t solChromo, int *numIterNoImprov);
void  generateNewGeneration(chromo_t *generation);
// Utlity Functions
void printChromosome(chromo_t chromosome);
void printOriginalSet(set_t set);