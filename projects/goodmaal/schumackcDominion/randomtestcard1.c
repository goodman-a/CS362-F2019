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
int BaronTest(struct gameState *state, int player, int handPos, int choice1);

/* -- MAIN FUNCTION -- */
int main(int argc, char** argv){
  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // general iterators
  int i;

  // gameplay values
  int handPos = 0, choice1 = 0;
  
  // setup/initialize parameters
  srand(time(0));

  int seed = rand();
  int player = 0;
  int num_players;
  
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
    // randomize number of playes: 2 to 4 playes 
    num_players = (rand()%(4-2+1))+2;
    
    // Initialize Game
    initializeGame(num_players, k, seed, &state);

    // Randomize hand size, hand position, choice1 
    int hand_size = (rand()%10)+1; 

    if(hand_size == 1){handPos = 0;}
    else{handPos = (rand()%hand_size);}
 
    choice1 = (rand()%2);
    
    // set game state (hand count, random generated hand, and place of playing card (baron))
    state.handCount[player] = hand_size;
    HandGenerator(&state, player, hand_size, 0, treasure_map);
    state.hand[player][handPos] = baron; 

    // Randomize Estate Supply Count
    int estate_count = state.supplyCount[estate];
    state.supplyCount[estate] = rand()%(estate_count+1);  // may need to weight the zero value higher ...
    //estate_count = state.supplyCount[estate];    

    // Run the Baron Random Test 
    printf("_____ TEST #%d _____\n",i+1);
    int barontest = BaronTest(&state, player, handPos, choice1);

    // Record Stats and if Failure occurs then print additional game information. 
    if(barontest){ counter_failure++; }
    else{counter_success++;}

  } // end of primary for-loop

  // Result Summary
  printf("Successful:%d  vs. Failures:%d\n", counter_success, counter_failure);  
 
  return 0;

}


/* -- Baron Random Test Function -- */
int BaronTest(struct gameState *state, int player, int handPos, int choice1)
{
  // Set-up test game state
  struct gameState testState;
  memcpy(&testState, state, sizeof(struct gameState));

  // Variables
  int bonus = 0;
  int bonus_start = 0;
  int flagFail = 0;
  int assert_state, baron_return;

  // call baron function ...
  baron_return = baronCardEffect(choice1, &testState); 
  if(baron_return != 0){printf("Baron Failed Execution\n");}
  
  // check action
  assert_state = AssertTest((testState.numBuys == state->numBuys+1), "+1 Number of Buys");
  if(assert_state){flagFail = 1; printf("\tNumber of Buys: Current = %d vs. Expected = %d\n", testState.numBuys, state->numBuys +1); return flagFail;}

  if(choice1 > 0)
  {
      // Player has an estate card in hand
      if(HandCardCount(state, player, estate) >= 1)
      {
        // +4 Bonus
        assert_state = AssertTest((testState.coins == state->coins+4), "+4 Bonus");
        if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state->coins+4);}

        // -2 Hand Count
        assert_state = AssertTest((testState.handCount[player] == state->handCount[player]-2), "-2 Hand Count (Estate & Baron)");
        if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player], state->handCount[player]-2);}

        // +2 Discard Count
        assert_state = AssertTest((testState.discardCount[player] == state->discardCount[player]+2), "+2 Discard Count (Estate & Baron)");
        if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player], state->discardCount[player]+2);} 
        
        // -1 Estate Hand Count
        assert_state = AssertTest((HandCardCount(&testState, player, estate) == HandCardCount(state, player, estate)-1), "-1 Estate Hand Count");
        if(assert_state){flagFail = 1; printf("\tCopper Hand Count: Current = %d, Expected = %d\n", HandCardCount(&testState, player, estate), HandCardCount(state, player, estate)-1);}

        // +0 Estate Supply Count
        assert_state = AssertTest((supplyCount(estate, &testState) == supplyCount(estate, state)), "+0 Estate Supply Count");
        if(assert_state){flagFail = 1; printf("\tEstate Supply Count: Current= %d, Expected = %d\n", supplyCount(estate, &testState), supplyCount(estate, state));}    

      }

      // Player does Not have an estate card in hand
      else
      {
        // +0 Bonus
        assert_state = AssertTest((testState.coins == state->coins), "+0 Bonus");
        if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", testState.coins, state->coins);}

        // -1 Hand Count
        assert_state = AssertTest((testState.handCount[player] == state->handCount[player]-1), "-1 Hand Count (Baron Discarded)");
        if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player], state->handCount[player]-1);}

        // +0 Estate Hand Count
        assert_state = AssertTest((HandCardCount(&testState, player, estate) == HandCardCount(state, player, estate)), "+0 Estate Hand Count");
        if(assert_state){flagFail = 1; printf("\tCopper Hand Count: Current = %d, Expected = %d\n", HandCardCount(&testState, player, estate), HandCardCount(state, player, estate));}

        // Estate Supply Count is Empty
        if(testState.supplyCount[estate] <=0  && state->supplyCount[estate] == 0) 
        {
          // +1 Discard Count
          assert_state = AssertTest((testState.discardCount[player] == state->discardCount[player]+1), "+1 Discard Count (Baron)");
          if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player], state->discardCount[player]+1);}

          // +0 Estate Supply Count
          assert_state = AssertTest((supplyCount(estate, &testState) == supplyCount(estate, state)), "+0 Estate Supply Count");
          if(assert_state){flagFail = 1; printf("\tEstate Supply Count: Current= %d, Expected = %d\n", supplyCount(estate, &testState), supplyCount(estate, state));}    
        }
        else
        {
          // +2 Discard Count
          assert_state = AssertTest((testState.discardCount[player] == state->discardCount[player]+2), "+2 Discard Count (Estate & Baron)");
          if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player], state->discardCount[player]+2);} 

          // -1 Estate Supply Count
          assert_state = AssertTest((supplyCount(estate, &testState) == supplyCount(estate, state)-1), "-1 Estate Supply Count");
          if(assert_state){flagFail = 1; printf("\tEstate Supply Count: Current= %d, Expected = %d\n", supplyCount(estate, &testState), supplyCount(estate, state)-1);}    
        }
      }
  }

  else
  {
    // Estate Supply Count is Empty
    if(testState.supplyCount[estate] <=0  && state->supplyCount[estate] == 0) 
    {
      // +1 Discard Count
      assert_state = AssertTest((testState.discardCount[player] == state->discardCount[player]+1), "+1 Discard Count (Baron)");
      if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player], state->discardCount[player]+1);}

      // +0 Estate Supply Count
      assert_state = AssertTest((supplyCount(estate, &testState) == supplyCount(estate, state)), "+0 Estate Supply Count");
      if(assert_state){flagFail = 1; printf("\tEstate Supply Count: Current= %d, Expected = %d\n", supplyCount(estate, &testState), supplyCount(estate, state));}    
    }
    else
    {
      // +2 Discard Count
      assert_state = AssertTest((testState.discardCount[player] == state->discardCount[player]+2), "+2 Discard Count (Estate & Baron)");
      if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player], state->discardCount[player]+2);} 

      // -1 Estate Supply Count
      assert_state = AssertTest((supplyCount(estate, &testState) == supplyCount(estate, state)-1), "-1 Estate Supply Count");
      if(assert_state){flagFail = 1; printf("\tEstate Supply Count: Current= %d, Expected = %d\n", supplyCount(estate, &testState), supplyCount(estate, state)-1);}    
    }

  }
  
  if(flagFail){
    printf("  *More Information:\n");
    printf("\tPlayer1 Piles: Hand Count: %d ; Discard Count: %d ; Deck Count: %d\n", state->handCount[player],state->discardCount[player],state->deckCount[player]);
    int hand_estate = HandCardCount(&state, player, estate);
    printf("\tNumber of Estates in Hand: %d vs. Current Estate Supply Count: %d\n",hand_estate, state->supplyCount[estate]);
    printf("\tChoice: %d\n\n",choice1);
  }



  return flagFail;
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
