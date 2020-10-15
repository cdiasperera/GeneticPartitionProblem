#include "partitionProblem.h"

/* Simulates evolution of the chromosomes till a solution is found or we have
 * gone many iterations without improvement.
 */
int simulateEvolution(set_t originalSet, chromo_t *generation) {
  int numIter = 0;
  int numIterNoImprov = 0;
  
  bool solutionFound = false;
  gene_t solChromo[CHROMOSOME_LENGTH];
  
  while (!solutionFound) {
    numIter++;
    performSelection(originalSet, generation);
    solutionFound = checkForConvergence(originalSet, generation, solChromo, &numIterNoImprov);

    if (solutionFound) {
      printChromosome(solChromo);
      return numIter;
    } else {
      generateNewGeneration(generation);
    }
  }

  return numIter;
}

// Replace the weakest two chromosomes with the strongest chromosomes
int  performSelection(set_t originalSet, chromo_t *generation) {
  int weakChromos[NUM_CHROMOSOMES_REPLACED];
  int strongChromos[NUM_CHROMOSOMES_REPLACED];

  // strongest, 2nd strongest, weakest, 2nd weakest
  int strongChromoFitness[NUM_CHROMOSOMES_REPLACED];
  int weakChromoFitness[NUM_CHROMOSOMES_REPLACED];

  // Initialize extremeHeights
  for (int i = 0; i < NUM_CHROMOSOMES_REPLACED; i++) {
    strongChromoFitness[i] = __INT_MAX__;
    weakChromoFitness[i] = 0;
  }

  // Find strongest/weakest chromosomes
  for (int i = 0; i < POP_SIZE; i++) {
    bool *chromosome = generation[i];
    int chromosomeFitness = measureFitness(chromosome, originalSet);
    if (chromosomeFitness > strongChromoFitness[0]) {
      strongChromos[1] = strongChromos[0];
      strongChromos[0] = i;

      strongChromoFitness[1] = strongChromoFitness[0];
      strongChromoFitness[0] = chromosomeFitness;
    } else if (chromosomeFitness > strongChromoFitness[1]) {
      strongChromos[1] = i;

      strongChromoFitness[1] = chromosomeFitness;
    } else if (chromosomeFitness < weakChromoFitness[0]) {
      weakChromos[1] = weakChromoFitness[0];
      weakChromos[0] = i;

      weakChromoFitness[1] = weakChromoFitness[0];
      weakChromoFitness[0] = chromosomeFitness;
    } else if (chromosomeFitness < weakChromoFitness[1]) {
      weakChromos[1] = i;

      weakChromoFitness[1] = chromosomeFitness;
    }
  }

  replaceChromosomes(strongChromos, weakChromos, generation);

  return strongChromos[0];
}

// Randomly mutates previous generation. Makes sure best chromosome is unchanged
void  generateNewGeneration(chromo_t *generation) {
  return;
}

void printChromosome(chromo_t chromosome) {
  for(int i = 0; i < CHROMOSOME_LENGTH; i++) {
    printf("%d ", chromosome[i]);
  }
  printf("\n");
}

void printOriginalSet(set_t originalSet) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    printf("%d ", originalSet[i]);
  }
  printf("\n");
}

void getInitialSet(set_t originalSet) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    scanf("%d", originalSet + i);
  }
}

void generateRandomChromosome(chromo_t chromosome) {
  for (int i = 0; i < CHROMOSOME_LENGTH; i++) {
    chromosome[i] = rand() > (RAND_MAX / 2);
  }
}

void mutateSinglegene(chromo_t chromosome) {
  int geneToMutate = rand() % CHROMOSOME_LENGTH;

  chromosome[geneToMutate] = (chromosome[geneToMutate] + 1) % 2;
}

void mutateChromosomeCrossOver(bool *chromosome1, bool *chromosome2) {
  // There is a potential for simple gene swaps, as crossOverLocation could be 0
  int crossOverLocation = rand() % CHROMOSOME_LENGTH;

  // Put the tail of chromosome1 into temp, as it will be replaced first
  bool tempChromosome[CHROMOSOME_LENGTH];
  for (int i = crossOverLocation; i < CHROMOSOME_LENGTH; i++) {
    tempChromosome[i] = chromosome1[i];
    chromosome1[i] = chromosome2[i];
    chromosome2[i] = tempChromosome[i];
  }

}

/*
  * We are using the metaphor of "height" to mean the sum of one the disjoint
  * sets.
  * 
  * We create a set as such. For a given chromosome, each gene corresponds to
  * a single integer in the original set. Then, the two disjoint sets are 
  * constructed as such:
  * Set 1: All the integers that correspond to "true" genes in the chromosome
  * Set 2: All the integers that correspond to "false" genes in the chromosome
  */
int heightOfSet(bool *chromosome, bool chosenSet, int *originalSet) {
  int height = 0;
  for (int i = 0; i < CHROMOSOME_LENGTH; i++) {
    if (chromosome[i] == chosenSet) {
      height += originalSet[i];
    }
  }
  return height;
}

int measureFitness(bool *chromosome, int *originalSet) {
  int set1Height = heightOfSet(chromosome, true, originalSet);
  int set2Height = heightOfSet(chromosome, false, originalSet);

  return abs(set1Height - set2Height);
}

int *checkFitnessOfAllChromosomes(set_t originalSet, chromo_t *generation, int *allHeights) {
  for (int i = 0; i < POP_SIZE; i++) {
    allHeights[i] = measureFitness(generation[i], originalSet);
  }
  return allHeights;
}

void replaceChromosomes(int *strongChromos, int *weakChromos, chromo_t *generation) {
  for (int i = 0; i < NUM_CHROMOSOMES_REPLACED; i++) {
    copyChromosome(generation[weakChromos[i]], generation[strongChromos[i]]);
  }
}

void copyChromosome(chromo_t dst, chromo_t src) {
  return;
}

int checkForConvergence(set_t originalSet, chromo_t *generation, chromo_t solChromo, int *numIterNoImprov) {
  int allHeights[POP_SIZE];
  checkFitnessOfAllChromosomes(originalSet, generation, allHeights);

  // Check if we have a height of 0, i.e: solved problem
  for (int i = 1; i < POP_SIZE; i++) {
    if (allHeights[i] == 0) {
      copyChromosome(solChromo, generation[i]);
      return 1;
    }
  }

  return 0;
}
