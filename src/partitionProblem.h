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

// FUNCTIONS

// Functions for Genetic Algorithm
void  getInitialSet(int *originalSet);
void  generateRandomChromosome(bool *chromosome);
void  mutateSingleGene(bool *chromosome);
void  mutateChromosomeCrossOver(bool *chromosome1, bool *chromosome2);
int   heightOfSet(bool *chromosome, bool chosenSet, int *orignalSet);
int   measureFitness(bool *chromosome, int *originalSet);
void  replaceChromosomes(int *indexStrongestChromosomes, int *indexWeakestChromosomes, bool **generation);
int   performSelection(int *originalSet, bool **generation);
void  checkForConvergence(int *originalSet, bool **generation, bool *solutionChromosome);
void  generateNewGeneration(bool **generation, int bestChromosomeIndex);
int   simulateEvolution(int *originalSet, bool **generation);

// Utlity Functions
void printChromosome(bool *chromosome);
void printOriginalSet(int *originalSet);
