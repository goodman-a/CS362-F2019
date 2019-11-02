/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 4 Learn How to Create a Random Tester
 * Author: Alexander Goodman
 * Due Date: 17 November 2019
 * 
 * File: randomtestcard1.c
 * 
 * File Description: Random Tester for the Baron Card Function:
 *   int baronCard(int handPos, int choice1, int currentPlayer, struct gameState* state, int* bonus);
 * 
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>


/* -- ASSERT FUNCTION -- */
int AssertTest(int pass, char* msg)
{
    if(!pass)
    {
        printf("FAIL: %s\n", msg);
        return 1;
    }
    else
    {
        // OR NOT SAY ANYTHING?
        printf("PASS: %s\n", msg);
        return 0;
    }
    
}



/* -- What to look for each iteration -- */
/*
  1. +1 buy everytime
  2. bonus value
  3. discard value
  4. hand count
  5. number of estates in hand
  6. estate supply count  (set up a large  number or a predefined array, but need it check 0 and 1 more often ....)
  7. 



*/


// Hmmm What to randomize?
/*
  1. If to discard an estate or not (a 0 or 1 rand value)
  2. What is in the player's hand ...
  3. Supply Count ..







*/



/* -- Helper Function Prototypes -- */




/* -- MAIN FUNCTION -- */
int main(int argc, char** argv){
  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // general iterators
  int i, r;

  // gameplay values
  int handPos = 0, choice1 = 0, num_bys = 0, bonus = 0, bonus_start = 0;
  
  // setup/initialize parameters
  srand(time(NULL));

  int seed = rand();
  int player = 0;
  int num_players, num_hand;
  

  // MAX_DECK ; MAX_HAND ; 

  // Counters
  //int counter_success, counter_failure;
  int count_tests = 50;

  // Kingdom Cards
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};


  // don't forget to free gamestate ... ooopsy

  /* -- For Loop -- */

    // randomize number of playes
    num_players = rand() % 4;

    // Randomize hand size
    int hand_size = rand()% MAX_HAND;

    // Randomize what is in hand?

    // Randomize Estate Supply Count

    // Randomize Discard count?



  return 0;

}


/* -- Helper Functions -- */




