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
void HandGenerator(struct gameState *state, int player, int size, int min, int max);
void DisplayHand(struct gameState *state, int player, char* msg);
void DisplayDiscard(struct gameState *state, int player, char* msg);
void DisplayDeck(struct gameState *state, int player, char* msg);
int HandCardCount(struct gameState *state, int player, int choice1);
int HandCardCount2(struct gameState *state, int player, int choice1, int handPos);




/* -- MAIN FUNCTION -- */
int main(int argc, char** argv){
  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // general iterators
  int i, j, r;

  // gameplay values
  int handPos = 0, choice1 = 0, num_bys = 0, bonus = 0, bonus_start = 0;
  
  // setup/initialize parameters
  srand(time(NULL));

  int seed = rand();
  int player = 0;
  int num_players, num_hand;
  
  // Counters
  //int counter_success, counter_failure;
  int count_tests = 500;

  // Kingdom Cards
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};


  // Create a Game State and Clear Memory
  struct gameState state;
  memset(&state, 0, sizeof(struct gameState));   // clear the game state

  /* -- Run count_tests number of tests -- */
  for(i=0; i<count_tests; i++)
  {

    printf("TEST #%d\n",i+1);
    // fail flag
    int failFlag = 0;

    // randomize number of playes: 2 to 4 playes 
    num_players = (rand()%(4-2+1))+2;

    // Initialize Game
    initializeGame(num_players, k, seed, &state);

    // Randomize hand size & Position 
    int hand_size = rand()% 10; //MAX_HAND;
    handPos = rand()%(hand_size+1)-1;

    // set game state (hand count, random generated hand, and place of playing card (baron))
    state.handCount[player] = hand_size;
    HandGenerator(&state, player, hand_size, 0, treasure_map);
    state.hand[player][handPos] = baron; 
    bonus = 0; // reset bonus

    // Randomize Estate Supply Count
    int estate_count = state.supplyCount[estate];
    state.supplyCount[estate] = rand()%(estate_count+1);  // may need to weight the zero value higher ...
    //estate_count = state.supplyCount[estate];

    // Display All Randomly Generated Stuffss....
    
    printf("Number of Players: %d\n", num_players);
    printf("Hand Size: %d & Hand Pos: %d\n", hand_size, handPos);
    DisplayHand(&state, player, "Player1");
    int hand_estate = HandCardCount(&state, player, estate);
    printf("Number of Estates in Hand: %d vs. Current Estate Supply Count: %d vs. Original Estate Supply Count: %d\n",hand_estate, state.supplyCount[estate], estate_count);
  

    // reset the state
   memset(&state, 0, sizeof(struct gameState));   // clear the game state

  } // end of primary for-loop
  
 
  return 0;

}


/* -- Helper Functions -- */

// Random Hand Generator 

void HandGenerator(struct gameState *state, int player, int size, int min, int max)
{
  int i;
  if(min > max)
  {
      int temp = min;
      min = max;
      max = temp;
  }

  for(i=0; i<size; i++)
  {
      state->hand[player][i] = (rand()%(max-min+1))+min;
  }


  return;
}


// Display Cards in Hand, Discard, and Deck Piles
void DisplayHand(struct gameState *state, int player, char* msg)
{
    int m;
    printf("%s Hand Pile: ", msg);
    for (m=0; m<state->handCount[player]; m++) {
        printf("(%d)", state->hand[player][m]);
    }
    printf("\n");
}

void DisplayDiscard(struct gameState *state, int player, char* msg)
{
    int m;
    printf("%s Discard Pile: ", msg);
    for (m=0; m<state->discardCount[player]; m++) {
        printf("(%d)", state->discard[player][m]);
    }
    printf("\n");
}

void DisplayDeck(struct gameState *state, int player, char* msg)
{
    int m;
    printf("%s Deck Pile: ", msg);
    for (m=0; m<state->deckCount[player]; m++) {
        printf("(%d)", state->deck[player][m]);
    }
    printf("\n");
}  

// Count of a specific card in hand.
int HandCardCount(struct gameState *state, int player, int card)
{
    int count = 0;
    int i;

    for (i = 0; i < state->handCount[player]; i++)
    {
        if (state->hand[player][i] == card) count++;
    }

    return count;
}

// count of duplicate cards matching specific card.
int HandCardCount2(struct gameState *state, int player, int choice1, int handPos)
{
    int count = 0;
    int i;
    for (i = 0; i < state->handCount[player]; i++)
    {
        if (i != handPos && state->hand[player][i] == state->hand[player][choice1] && i != choice1) 
        {
            count++;
        }
    }

    return count;
}
