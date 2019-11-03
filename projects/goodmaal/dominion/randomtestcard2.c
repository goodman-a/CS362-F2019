/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 4 Learn How to Create a Random Tester
 * Author: Alexander Goodman
 * Due Date: 17 November 2019
 * 
 * File: randomtestcard2.c
 * 
 * File Description: Random Tester for the Baron Card Function:
 *  minionCard(int handPos, int currentPlayer, int choice1, int choice2, struct gameState* state, int* bonus);
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
        //printf("PASS: %s\n", msg);
        return 0;
    }
    
}



/* -- Helper Function Prototypes -- */
void HandGenerator(struct gameState *state, int player, int size, int min, int max);
void DisplayHand(struct gameState *state, int player, char* msg);
void DisplayDiscard(struct gameState *state, int player, char* msg);
void DisplayDeck(struct gameState *state, int player, char* msg);
int HandCardCount(struct gameState *state, int player, int choice1);
int HandCardCount2(struct gameState *state, int player, int choice1, int handPos);
int MinionTest(struct gameState *state, int player1, int handPos, int choice1, int choice2);


/* -- MAIN FUNCTION -- */
int main(int argc, char** argv){
  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // general iterators
  int i, j, r;

  // gameplay values
  int handPos = 0, choice1 = 0, num_buys = 0, bonus = 0, bonus_start = 0;
  
  // setup/initialize parameters
  srand(time(0));

  int seed = rand();
  int player = 0;
  int num_players, num_hand;
  
  // Counters
  int counter_success=0, counter_failure=0;
  int count_tests = 500;

  // Kingdom Cards
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};


  // Create a Game State and Clear Memory
  struct gameState state;
  memset(&state, 0, sizeof(struct gameState));   // clear the game state

  /* -- Run count_tests number of tests -- */
  for(i=0; i<count_tests; i++)
  {
    //printf("TEST #%d\n",i+1);
   
    // fail flag
    int failFlag = 0;

    // randomize number of playes: 2 to 4 playes 
    num_players = (rand()%(4-2+1))+2;
    
    // Initialize Game
    //initializeGame(num_players, k, seed, &state);

  }





  return 0;
}


// Minion Random Test
int MinionTest(struct gameState *state, int player1, int handPos, int choice1, int choice2)
{
  int failFlag = 0;

  // +1 Action

  // +0 trash count


  // if(choice1)
    // +2 Bonus

    // +1 Discard Count

  // else if(choice2)
    // +0 Bonus

    // Player 1: Test Hand Count <= 4 (could check more indepth with checking deck/discard sizes and if deck is empty)

    // Player 1: Test Discard Count = State Hand Count

    // ALL Players Hand Count <= 4 & check their discard piles... --> Loop through each player and then print out player number if issues?
        // Loop through each player and if State Playeri Hand Count > 4 then Test Playeri Discard Count == State Playeri Hand Count




  return failFlag;
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

