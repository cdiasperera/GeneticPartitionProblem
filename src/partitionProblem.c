#include "partitionProblem.h"

void printChromosome(bool *chromosome) {
  for(int i = 0; i < CHROMOSOME_LENGTH; i++) {
    printf("%d ", chromosome[i]);
  }
  printf("\n");
}

void printOriginalSet(int *originalSet) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    printf("%d ", originalSet[i]);
  }
  printf("\n");
}

void getInitialSet(int *originalSet) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    scanf("%d", originalSet + i);
  }
}

void generateRandomChromosome(bool *chromosome) {
  for (int i = 0; i < CHROMOSOME_LENGTH; i++) {
    chromosome[i] = rand() > (RAND_MAX / 2);
  }
}

void mutateSinglegene(bool *chromosome) {
  int randNum = rand() % CHROMOSOME_LENGTH;

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

int *checkFitnessOfAllChromosomes(int *originalSet, bool **generation, int *allHeights) {
  for (int i = 0; i < POP_SIZE; i++) {
    allHeights[i] = measureFitness(generation[i], originalSet);
  }
  return allHeights;
}

void replaceChromosomes(indexStrongestChromosomes, indexWeakestChromosomes, generation) {
  for (int i = 0; i < NUM_CHROMOSOMES_REPLACED; i++) {
    generation[indexWeakestChromosomes[i]] = generation[indexStrongestChromosomes[i]];
  }
}

int  performSelection(int *originalSet, bool **generation) {
  int indexStrongestChromosomes[NUM_CHROMOSOMES_REPLACED];
  int indexWeakestChromosomes[NUM_CHROMOSOMES_REPLACED];

  // strongest, 2nd strongest, weakest, 2nd weakest
  int strongestChromosomeFitnesss[NUM_CHROMOSOMES_REPLACED];
  int weakestChromosomeFitness[NUM_CHROMOSOMES_REPLACED];

  // Initialize extremeHeights
  for (int i = 0; i < NUM_CHROMOSOMES_REPLACED; i++) {
    strongestChromosomeFitnesss[i] = __INT_MAX__;
    weakestChromosomeFitness[i] = 0;
  }

  // Find strongest/weakest chromosomes
  for (int i = 0; i < POP_SIZE; i++) {
    bool *chromosome = generation[i];
    int chromosomeFitness = measureFitness(chromosome, originalSet);
    if (chromosomeFitness > strongestChromosomeFitnesss[0]) {
      indexStrongestChromosomes[1] = indexStrongestChromosomes[0];
      indexStrongestChromosomes[0] = i;

      strongestChromosomeFitnesss[1] = strongestChromosomeFitnesss[0];
      strongestChromosomeFitnesss[0] = chromosomeFitness;
    } else if (chromosomeFitness > strongestChromosomeFitnesss[1]) {
      indexStrongestChromosomes[1] = i;

      strongestChromosomeFitnesss[1] = chromosomeFitness;
    } else if (chromosomeFitness < weakestChromosomeFitness[0]) {
      indexWeakestChromosomes[1] = weakestChromosomeFitness[0];
      indexWeakestChromosomes[0] = i;

      weakestChromosomeFitness[1] = weakestChromosomeFitness[0];
      weakestChromosomeFitness[0] = chromosomeFitness;
    } else if (chromosomeFitness < weakestChromosomeFitness[1]) {
      indexWeakestChromosomes[1] = i;

      weakestChromosomeFitness[1] = chromosomeFitness;
    }
  }

  replaceChromosomes(indexStrongestChromosomes, indexWeakestChromosomes, generation);

  return indexStrongestChromosomes[0];
}

void checkForConvergence(int *originalSet, bool **generation, bool *solutionChromosome) {
  // Get all heights in an array.
  // Check if heights are 0. If so, success! Put solution in solChromoome
  // If not, check if we're past MAX_ITER_WITH_NO_IMPROVEMNENT
  // If so, still fill in solutionChromosome
}

// Randomly mutates previous generation. Makes sure best chromosome is unchanged
void  generateNewGeneration(bool **generation, int bestChromosomeIndex) {

}

int simulateEvolution(int *originalSet, bool **generation) {
  int numIterations = 0;
  
  bool solutionFound = false;
  bool solutionChromosome[CHROMOSOME_LENGTH];
  
  while (!solutionFound) {
    numIterations++;
    int bestChromosomeIndex = performSelection(originalSet, generation);
    checkForConvergence(originalSet, generation, solutionChromosome);

    if (solutionFound) {
      printChromosome(solutionChromosome);
      return numIterations;
    } else {
      generateNewGeneration(generation, bestChromosomeIndex)
    }
  }

  return numIterations;
}