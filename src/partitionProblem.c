#include "partitionProblem.h"

/**
 * Gathers the initial set containing all the numbers.
 */
void getInitialSet(set_t set) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    scanf("%d", set + i);
  }
}

/* Simulates evolution of the chromosomes till a solution is found or we have
 * gone many iterations without improvement.
 */
int simulateEvolution(set_t set) {
  /*
   * The total number of iterations, the number of iterations without
   * improvement
   */
  int numIter = 0;
  int numIterNoImprov = 0;
  
  bool solutionFound = false;
  chromo_t solChromo;
  
  chromo_t generation[POP_SIZE];
  makeInitialGenration(generation, set);

  while (!solutionFound) {
    numIter++;
    int prevBestFitness = performSelection(set, generation);
    // Check if we are converging to or have found a solution
    solutionFound = converges(set, generation, &solChromo, &numIterNoImprov, prevBestFitness);

    if (solutionFound) {
      // Print output
      printOutput(solChromo);
      return numIter;
    } else {
      generateNewGeneration(generation, set);
    } 
  }

  return numIter;
}

/*
 * Makes the initial generation of chromosomes. 
 */
void makeInitialGenration(chromo_t *generation, set_t set) {
  for (int i = 0; i < POP_SIZE; i++){
    generateRandomChromosome(&generation[i], set);
  }
  return;
}

/* Performs selection.
 * Basically replaces the weakest two chromosomes with the strongest chromosomes
 */
int  performSelection(set_t set, chromo_t *generation) {
  /* Find the weakest and strongest chromosomes. The most extreme chromosomes
   * on either end appear earlier in the list.
   */
  int weakChromos[NUM_CHROMOSOMES_REPLACED];
  int strongChromos[NUM_CHROMOSOMES_REPLACED];

  /* Since we maintain the invariant that the generation would be sorted along
   * fitness, with the fittest at the front.
   */
  for (int i = 0; i < NUM_CHROMOSOMES_REPLACED; i++) {
    weakChromos[i]  = WORST_CHROMO-i;
    strongChromos[i] = i;
  }

  replaceChromosomes(strongChromos, weakChromos, generation, NUM_CHROMOSOMES_REPLACED);
  
  // We modified our population, so we need to sort it.
  sortChromos(generation);
  return generation[0].fitness;
}

// Check if our current generation has converged to a solution
int converges(set_t set, chromo_t *generation, chromo_t *solChromo, int *numIterNoImprov, int prevBestFitness) {

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

/* Randomly mutates previous generation. Makes sure best chromosome is unchanged
 * Makes sure the resulting generation is sorted as well.
 * Note that the multiple mutations can occur on the same chromosome. Sometimes
 * this would result in an original mutation being reverted. This is intended.
 */
void  generateNewGeneration(chromo_t *generation, set_t set) {
  // Perform cross-overs, i.e: sexual reproduction.
  for(int i = 0; i < NUM_CROSS_OVERS; i++){
    int mutator1;
    int mutator2;

    /* Determine which chromosomes to cross over. Do not cross over best one
     * which starts at index 0, so we will generate a random number greater than
     * 0.
     * 
     * Equally fit solutions might exist at these indexes, but we only need to
     * preserve the best one.
     */
		mutator1 = randInt(1, POP_SIZE);
    mutator2 = randInt(1, POP_SIZE);

	  chromoCrossOver(&generation[mutator1], &generation[mutator2], set);
  }

  // Perform random mutations.
  for(int i = 0; i < NUM_MUTATIONS; i++){
    // As before, avoid mutating best chromosome.
    int mutator = randInt(1, POP_SIZE);

	  mutateSingleGene(&generation[mutator], set);
  }

  // Sort this generation by fitness
  sortChromos(generation);
}

/* Sort chromosomes by fitness, with the fittest occuring earlier in the aray.
 * Use selection sort since the array is so small that O(n log n) isn't that
 * useful.
 */
void sortChromos(chromo_t *generation) {
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

// Swap two chromosomes in the generation pool.
void swap(int i, int j, chromo_t *generation) {
  chromo_t temp;
  copyChromosome(&temp, generation[i]);
  copyChromosome(generation+i, generation[j]);
  copyChromosome(generation+j, temp);
}

// Copy chromosome src to dst.
void copyChromosome(chromo_t *dst, chromo_t src) {
  for (int i = 0; i < CHROMOSOME_LENGTH; i++) {
    dst->genes[i] = src.genes[i];
  }
  dst->fitness = src.fitness;
}

// Replace
void replaceChromosomes(int *strongChromos, int *weakChromos, chromo_t *generation, int numReplaced) {
  for (int i = 0; i < numReplaced; i++) {
    copyChromosome(generation + weakChromos[i], generation[strongChromos[i]]);
  }
}

// Generates a random chromosome
void generateRandomChromosome(chromo_t *chromosome, set_t set) {
  for (int i = 0; i < CHROMOSOME_LENGTH; i++) {
    chromosome->genes[i] = randInt(0, 2);
  }

  // Calculate fitness
  chromosome->fitness = 0;
  chromosome->fitness = measureFitness(*chromosome, set);
}

// Mutate a single gene in a chromosome.
void mutateSingleGene(chromo_t *chromosome, set_t set) {
  int geneToMutate = randInt(0, CHROMOSOME_LENGTH);

  chromosome->genes[geneToMutate] = (chromosome->genes[geneToMutate] + 1) % 2;
  chromosome->fitness = measureFitness(*chromosome, set);
}

// Perform a cross-over muation between two chromosomes.
void chromoCrossOver(chromo_t *chromo1, chromo_t *chromo2, set_t set) {
  // There is a potential for simple gene swaps, as crossOverLocation could be 0
  int crossOverLocation = randInt(0, CHROMOSOME_LENGTH);

  // Put the tail of chromosome1 into temp, as it will be replaced first
  chromo_t tempChromo;
  for (int i = crossOverLocation; i < CHROMOSOME_LENGTH; i++) {
    tempChromo.genes[i] = chromo1->genes[i];
    chromo1->genes[i] = chromo2->genes[i];
    chromo2->genes[i] = tempChromo.genes[i];
  }

  chromo1->fitness = measureFitness(*chromo1, set);
  chromo2->fitness = measureFitness(*chromo2, set);

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

/* 
 * Instead of using heighOfSet twice, we can simply add the differences as we
 * go through the set once. Then, we take the absolute value of this sum.
 * Thus, instead of taking 2 * CHROMOSOME_LENGTH iterations, we can do it in
 * one iteration.
 */
int setDifference(chromo_t chromosome, set_t set) {
  int diff = 0;
  for (int i = 0; i < CHROMOSOME_LENGTH; i++) {
    diff += (chromosome.genes[i] ? -1 : 1) * (set[i]);
  }

  return abs(diff);
}


/*
 * Generally, we want to have it such that our fitness increases as our 
 * difference approaches zero. However, this isn't necessary, since there's a
 * strictly inverse relationship.
 * 
 * Thus, if we keep this in mind, we simply need to flip our inequalities and
 * remember that the stronger chromosomes occur earlier in a list when sorted.
 * 
 * Really, it's better this way since we don't have to process imprecise floats,
 * which we result from the inverse relationship. So while we _can_ write a 
 * function, we chose not to.
 */
int measureFitness(chromo_t chromosome, set_t set) {
  return setDifference(chromosome, set);
}

// A debugging function, that prints the original set.
void printOriginalSet(set_t set) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    printf("%d ", set[i]);
  }
  printf("\n");
}

// Prints a specific chromosome to stdin.
void printChromosome(chromo_t chromosome) {
  for(int i = 0; i < CHROMOSOME_LENGTH; i++) {
    printf("%d ", chromosome.genes[i]);
  }
  printf("\n");
}

void printOutput(chromo_t chromo) {
  printChromosome(chromo);
}

// Generate random int between (incl) lower and (non-incl) upper bounds.
int randInt(int lowerBound, int upperBound) {
  int r = rand();

  int range = upperBound - lowerBound;
  // Can use simple modulus technique
  if (range >= 10) {
    return lowerBound  + (r % range);
  } else {
    // Need to do it baesd on distribution
    for (int i = 0; i < range; i++) {
      if (r >= i*(RAND_MAX / range) && r < (i+1)*(RAND_MAX)/range) {
        return i;
      }
    }
    return range-1;
  }
}