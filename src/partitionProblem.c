/* file     : main.c
 * author   : Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date     : October 2020
 * version  : 1.0
 */

#include "partitionProblem.h"
#include "simconfig.h"

// Assignment 1: Gathers the initial set containing all the numbers.
void getInitialSet(set_t set) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    scanf("%d", set + i);
    if (set[i] < 0) {
      printf("You cannot have negative numbers in the set\n");
      exit(-1);
    }
  }
}

/* Assignment 7: Simulates evolution of the chromosomes till a solution is found
 * or we have gone many iterations without improvement.
 */
int simulateEvolution(set_t set) {
  /* The total number of iterations, the number of iterations without
   * improvement.
   */
  int numIter = 0;
  int numIterNoImprov = 0;
  
  int convergeStatus = CONVERGING;
  chromo_t solChromo;
  
  chromo_t generation[POP_SIZE];
  makeInitialGenration(set, generation);

  int prevBestFitness = WORST_FITNESS;

  while (convergeStatus == CONVERGING) {
    numIter++;
    performSelection(set, generation);
    // Check if we are converging to or have found a solution
    convergeStatus = converges(
      set,
      generation,
      &solChromo,
      &numIterNoImprov,
      prevBestFitness
    );

    if (convergeStatus != CONVERGING) {
      // Print output
      printOutput(set, convergeStatus, solChromo);
      return numIter;
    } else {
      // Update previous best fitness, beforem making the next generation
      prevBestFitness = generation[BEST_CHROMO].fitness;
      generateNewGeneration(set, generation);
    }
  }

  return numIter;
}


// Makes the initial generation of chromosomes. 
void makeInitialGenration(set_t set, chromo_t *generation) {
  for (int i = 0; i < POP_SIZE; i++){
    generateRandomChromo(set, &generation[i]);
  }
  return;
}

/* Performs selection. Basically replaces the weakest two chromosomes with the
 * strongest chromosomes.
 */
void performSelection(set_t set, chromo_t *generation) {
  /* Find the weakest and strongest chromosomes. The most extreme chromosomes
   * on either end appear earlier in the list.
   */
  int weakChromos[NUM_CHROMOS_REPLACED];
  int strongChromos[NUM_CHROMOS_REPLACED];

  /* Since we maintain the invariant that the generation would be sorted along
   * fitness, with the fittest at the front.
   */
  for (int i = 0; i < NUM_CHROMOS_REPLACED; i++) {
    weakChromos[i]  = WORST_CHROMO-i;
    strongChromos[i] = i;
  }

  replaceChromos(
    strongChromos,
    weakChromos,
    generation,
    NUM_CHROMOS_REPLACED
  );
  
  // We modified our population, so we need to sort it.
  sortChromos(generation);
}

// Check if our current generation has converged to a solution
int converges(
  set_t set,
  chromo_t *generation,
  chromo_t *solChromo,
  int *numIterNoImprov,
  int prevBestFitness
) {

  // Check if best chromosome has made improvement. Best chromo = firt chromo
  if (generation[BEST_CHROMO].fitness < prevBestFitness) {
    *numIterNoImprov = 0;
  } else {
    (*numIterNoImprov)++;
  }

  if (
    generation[BEST_CHROMO].fitness == 0 ||
    *numIterNoImprov > MAX_ITER_WITHOUT_IMPROVEMENT
  ) {
    // Either found solution, or we havne't made progress in a while
    copyChromo(solChromo, generation[BEST_CHROMO]);
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
void  generateNewGeneration(set_t set, chromo_t *generation) {
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

	  chromoCrossOver(set, &generation[mutator1], &generation[mutator2]);
  }

  // Perform random mutations.
  for(int i = 0; i < NUM_MUTATIONS; i++){
    // As before, avoid mutating best chromosome.
    int mutator = randInt(1, POP_SIZE);

	  mutateSingleGene(set, &generation[mutator]);
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
  copyChromo(&temp, generation[i]);
  copyChromo(generation+i, generation[j]);
  copyChromo(generation+j, temp);
}

// Copy chromosome src to dst.
void copyChromo(chromo_t *dst, chromo_t src) {
  for (int i = 0; i < CHROMO_LENGTH; i++) {
    dst->genes[i] = src.genes[i];
  }
  dst->fitness = src.fitness;
}

// Replace two chromosomes, indicated by their index.
void replaceChromos(
  int *strongChromos,
  int *weakChromos,
  chromo_t *generation,
  int numReplaced
) {
  for (int i = 0; i < numReplaced; i++) {
    copyChromo(generation + weakChromos[i], generation[strongChromos[i]]);
  }
}

// Assignment 2: Generates a random chromosome.
void generateRandomChromo(set_t set, chromo_t *chromo) {
  for (int i = 0; i < CHROMO_LENGTH; i++) {
    chromo->genes[i] = randInt(0, 2);
  }

  // Calculate fitness
  chromo->fitness = 0;
  chromo->fitness = measureFitness(set, *chromo);
}

// Assignment 3: Mutate a single gene in a chromosome.
void mutateSingleGene(set_t set, chromo_t *chromo) {
  int geneToMutate = randInt(0, CHROMO_LENGTH);

  chromo->genes[geneToMutate] = (chromo->genes[geneToMutate] + 1) % 2;
  chromo->fitness = measureFitness(set, *chromo);
}

// Perform a cross-over muation between two chromosomes.
void chromoCrossOver(set_t set, chromo_t *chromo1, chromo_t *chromo2) {
  // There is a potential for simple gene swaps, as crossOverLocation could be 0
  int crossOverLocation = randInt(0, CHROMO_LENGTH);

  // Put the tail of chromosome1 into temp, as it will be replaced first
  chromo_t tempChromo;
  for (int i = crossOverLocation; i < CHROMO_LENGTH; i++) {
    tempChromo.genes[i] = chromo1->genes[i];
    chromo1->genes[i] = chromo2->genes[i];
    chromo2->genes[i] = tempChromo.genes[i];
  }

  chromo1->fitness = measureFitness(set, *chromo1);
  chromo2->fitness = measureFitness(set, *chromo2);

}

/* Assignment 4: We are using the metaphor of "height" to mean the sum of one
 * the disjoint sets.
 * 
 * We create a set as such. For a given chromosome, each gene corresponds to
 * a single integer in the original set. Then, the two disjoint sets are 
 * constructed as such:
 * Set 1: All the integers that correspond to "true" genes in the chromosome
 * Set 2: All the integers that correspond to "false" genes in the chromosome
 */
int heightOfSet(set_t set, bool chosenSet, chromo_t chromo) {
  int height = 0;
  for (int i = 0; i < CHROMO_LENGTH; i++) {
    if (chromo.genes[i] == chosenSet) {
      height += set[i];
    }
  }
  return height;
}

/* Assignment 6: Generally, we want to have it such that our fitness increases
 * as our difference approaches zero. However, this isn't necessary, since
 * there's a strictly inverse relationship.
 * 
 * Thus, if we keep this in mind, we simply need to flip our inequalities and
 * remember that the stronger chromosomes occur earlier in a list when sorted.
 * 
 * Really, it's better this way since we don't have to process imprecise floats,
 * which we result from the inverse relationship. So while we _can_ write a 
 * function, we chose not to.
 */
int measureFitness(set_t set, chromo_t chromo) {
  return setDifference(set, chromo);
}

/* Assignment 5: Instead of using heighOfSet twice, we can simply add the
 * differences as we go through the set once. Then, we take the absolute value
 * of this accumulated difference .
 * 
 * Thus, instead of taking 2 * CHROMOSOME_LENGTH iterations, we can do it in
 * one iteration. Since O(n) however.
 */
int setDifference(set_t set, chromo_t chromo) {
  int diff = 0;
  for (int i = 0; i < CHROMO_LENGTH; i++) {
    /* We are finding the difference with respect to the true set. That is:
     * sum(trueSet) + diff = sum(falseSet)
     *
     */
    diff += (chromo.genes[i] ? -1 : 1) * (set[i]);
  }

  return abs(diff);
}

// A debugging function, that prints the original set.
void printOriginalSet(set_t set) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    printf("%d ", set[i]);
  }
  printf("\n");
}

// Prints the output for the program based on the solution.
void printOutput(set_t set, int convergeStatus, chromo_t chromo) {
  if (convergeStatus == SOLUTION_FOUND) {
    printf("SOLUTION CHROMOSOME: ");
    printChromo(chromo);
    printDivider(DIVIDER_LEN);

    printSets(set, chromo);
    printDivider(DIVIDER_LEN);
  } else {
    printf("NO SOLUTION");
  }

}

// Prints a specific chromosome to stdin.
void printChromo(chromo_t chromo) {
  for(int i = 0; i < CHROMO_LENGTH; i++) {
    printf("%d ", chromo.genes[i]);
  }
  printf("\n");
}

// Prints out the sets from a chromosome
void printSets(set_t set, chromo_t chromo) {
  // Print true set
  printf("SET 1: ");
  printSet(set, true, chromo);


  // Print false set
  printf("SET 2: ");
  printSet(set, false, chromo);
}

// Prints out a specific set from a chromosome
void printSet(set_t set, bool chosenSet, chromo_t chromo) {
  bool prevNumberPresent = false;

  int sum = 0;
  
  printf("{");
  for (int i = 0; i < CHROMO_LENGTH; i++) {
    if (chromo.genes[i] == chosenSet) {
      // Add the preceding comma, if there was a number before the current num
      if (prevNumberPresent) {
        printf(", ");
      }
      prevNumberPresent = true;
      printf("%d", set[i]);

      sum += set[i];
    }
  }
  printf("}");
  
  printf("\t\t || sum = %d", sum);
  printf("\n");
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

void printDivider(int len) {
  for (int i = 0; i < len; i++) {
    printf("%c", DIVIDER_CHAR);
  }
  printf("\n");
}