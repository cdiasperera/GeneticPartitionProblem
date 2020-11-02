/* file     : output.c
 * author   : Channa Dias Perera (c.dias.perera@student.rug.nl)
 *          : Ola Dybvadskog     (o.dybvadskog@student.rug.nl)
 * date     : October 2020
 * version  : 1.0
 */

/* Description:
 * Functions definitions to be used for writing to stdin.
 */
#include "output.h"
#include "partitionProblem.h"
#include "simconfig.h"
#include "testing.h"

// A debugging function, that prints the original set.
void printOriginalSet(set_t set) {
  for (int i = 0; i < SIZE_ORIGINAL_SET; i++) {
    printf("%d ", set[i]);
  }
  printf("\n");
}

// Prints the output for the program based on the solution.
void printOutput(set_t set, int convergeStatus, chromo_t chromo) {
#if !TESTING
  // Print type of solution
  switch(convergeStatus) {
    case SOLUTION_FOUND:
      printf("ZERO'D difference of sums!\n");
      break;
    case PAST_MAX_ITER:
    case NO_IMPROVEMENT:
      printf(
        "MINIMIZED absolute difference of sums: %d!\n",
        setDifference(set, chromo)
      );
      break;
    default:
      break;
  }
  
  // Print chromosome
  printf("CHROMOSOME: ");
  printChromo(chromo);
  printDivider(DIVIDER_LEN);

  // Print set results
  printSets(set, chromo);
  printDivider(DIVIDER_LEN);
#endif
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
  printf("COLLECTION 1: ");
  printSet(set, true, chromo);


  // Print false set
  printf("COLLECTION 2: ");
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
  printf("}\n");
  
  printf("sum = %d", sum);
  printf("\n");
}

void printDivider(int len) {
  for (int i = 0; i < len; i++) {
    printf("%c", DIVIDER_CHAR);
  }
  printf("\n");
}