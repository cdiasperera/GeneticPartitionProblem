#include "partitionProblem.h"

void getInitialSet(set_t set) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    scanf("%d", set + i);
  }
}

/* Simulates evolution of the chromosomes till a solution is found or we have
 * gone many iterations without improvement.
 */
int simulateEvolution(set_t set, chromo_t *generation) {
  int numIter = 0;
  int numIterNoImprov = 0;
  
  bool solutionFound = false;
  chromo_t solChromo;
  
  while (!solutionFound) {
    numIter++;
    int prevBestFitness = performSelection(set, generation);
    solutionFound = converges(set, generation, &solChromo, &numIterNoImprov, prevBestFitness);

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
int  performSelection(set_t set, chromo_t *generation) {

  //Sort array by fitness, higher fitness, lower value, lower index
  sortChromos(generation);

  //  Indexes of chromosomes.
  // Weakest chromosome, 2nd weakest chromosome, ...
  int weakChromos[NUM_CHROMOSOMES_REPLACED];
  // Strongest chromosome, 2nd strongest chromosome, ...
  int strongChromos[NUM_CHROMOSOMES_REPLACED];

  for (int i = 0; i < NUM_CHROMOSOMES_REPLACED; i++) {
    weakChromos[i]  = WORST_CHROMO-i;
    strongChromos[i] = i;
  }

  replaceChromosomes(strongChromos, weakChromos, generation);

  return generation[0].fitness;
}

// Check if our current generation has converged to a solution
int converges(set_t set, chromo_t *generation, chromo_t *solChromo, int *numIterNoImprov, int prevBestFitness) {
  sortChromos(generation);

  // Check if best chromosome has made improvement. Best chromo = firt chromo
  if (generation[BEST_CHROMO]. fitness < prevBestFitness) {
    *numIterNoImprov = 0;
  } else {
    (*numIterNoImprov)++;
  }

  if (generation[BEST_CHROMO].fitness == 0 || *numIterNoImprov > MAX_ITER_WITHOUT_IMPROVEMENT) {
    // Either found solution, or we havne't made progress in a while
    copyChromosome(solChromo, generation[BEST_CHROMO]);
    return 1;
  }

  // No solution
  return 0;
}

// Randomly mutates previous generation. Makes sure best chromosome is unchanged
void  generateNewGeneration(chromo_t *generation) {
  return;
}

void sortChromos(chromo_t *generation) {
  /* Use selection sort since the array is so small that O(n log n) isn't that
   * useful
   */
  for (int i = 0; i < POP_SIZE; i++) {
    int min = i;
    for (int j = i; j < POP_SIZE; j++) {
      // Find smallest element
      if (generation[j].fitness < generation[min].fitness) {
        min = j;
      }
    }
    swap(i, min, generation);
  }
}

void swap(int i, int j, chromo_t *generation) {
  chromo_t temp;
  copyChromosome(&temp, generation[i]);
  copyChromosome(generation+i, generation[j]);
  copyChromosome(generation+j, temp);
}

void copyChromosome(chromo_t *dst, chromo_t src) {
  for (int i = 0; i < CHROMOSOME_LENGTH; i++) {
    dst->genes[i] = src.genes[i];
  }
  dst->fitness = src.fitness;
}

void replaceChromosomes(int *strongChromos, int *weakChromos, chromo_t *generation) {
  for (int i = 0; i < NUM_CHROMOSOMES_REPLACED; i++) {
    copyChromosome(generation + weakChromos[i], generation[strongChromos[i]]);
  }
}

void generateRandomChromosome(chromo_t *chromosome, set_t set) {
  for (int i = 0; i < CHROMOSOME_LENGTH; i++) {
    chromosome->genes[i] = rand() > (RAND_MAX / 2);
  }

  // Calculate fitness
  chromosome->fitness = 0;
  chromosome->fitness = measureFitness(*chromosome, set);
}

void mutateSinglegene(chromo_t chromosome, set_t set) {
  int geneToMutate = rand() % CHROMOSOME_LENGTH;

  chromosome.genes[geneToMutate] = (chromosome.genes[geneToMutate] + 1) % 2;
  chromosome.fitness = measureFitness(chromosome, set);
}

void chromoCrossOver(chromo_t chromo1, chromo_t chromo2, set_t set) {
  // There is a potential for simple gene swaps, as crossOverLocation could be 0
  int crossOverLocation = rand() % CHROMOSOME_LENGTH;

  // Put the tail of chromosome1 into temp, as it will be replaced first
  chromo_t tempChromo;
  for (int i = crossOverLocation; i < CHROMOSOME_LENGTH; i++) {
    tempChromo.genes[i] = chromo1.genes[i];
    chromo1.genes[i] = chromo2.genes[i];
    chromo2.genes[i] = tempChromo.genes[i];
  }

  chromo1.fitness = measureFitness(chromo1, set);
  chromo2.fitness = measureFitness(chromo2, set);

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
int heightOfSet(chromo_t chromosome, bool chosenSet, set_t set) {
  int height = 0;
  for (int i = 0; i < CHROMOSOME_LENGTH; i++) {
    if (chromosome.genes[i] == chosenSet) {
      height += set[i];
    }
  }
  return height;
}

int measureFitness(chromo_t chromosome, set_t set) {
  int set1Height = heightOfSet(chromosome, true, set);
  int set2Height = heightOfSet(chromosome, false, set);

  return abs(set1Height - set2Height);
}

void printChromosome(chromo_t chromosome) {
  for(int i = 0; i < CHROMOSOME_LENGTH; i++) {
    printf("%d ", chromosome.genes[i]);
  }
  printf("\n");
}

void printOriginalSet(set_t set) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    printf("%d ", set[i]);
  }
  printf("\n");
}