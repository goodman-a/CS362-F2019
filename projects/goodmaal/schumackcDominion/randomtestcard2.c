/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 4 Learn How to Create a Random Tester
 * Author: Alexander Goodman
 * Due Date: 17 November 2019
 * 
 * File: randomtestcard2.c
 * 
 * File Description: Random Tester for the Minion Card Function:
 * OLD:  minionCard(int handPos, int currentPlayer, int choice1, int choice2, struct gameState* state, int* bonus);
 * NEW: minionCardEffect(choice1, choice2, &testState, handPos);
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
        printf("PASS: %s\n", msg);
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
int MinionTest(struct gameState *state, int player1, int handPos, int choice1, int choice2, int num_players);

/* -- MAIN FUNCTION -- */
int main(int argc, char** argv){
  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // general iterators
  int i, j;

  // gameplay values
  int handPos = 0, choice1 = 0, choice2 = 0;
  
  // setup/initialize parameters
  srand(time(0));

  int seed = rand();
  int player1 = 0;
  int num_players;
  
  // Counters
  int counter_success=0, counter_failure=0, miniontest = 0;
  int count_tests = 500;

  // Kingdom Cards
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

  // Create a Game State and Clear Memory
  struct gameState state;
  memset(&state, 0, sizeof(struct gameState));   // clear the game state

  /* -- Run count_tests number of tests -- */
  for(i=0; i<count_tests; i++)
  {
    // randomize number of playes: 2 to 4 players 
    num_players = (rand()%(4-2+1))+2;
    
    // Initialize Game
    initializeGame(num_players, k, seed, &state);

    // set game state for each player (hand count, random generated hand, and place of playing card (baron))
    for(j=0; j<num_players; j++)
    {
        state.handCount[j] = (rand()%10)+1;
        if(j == 0)
        {
          if(state.handCount[j] == 1){handPos=0;}
          else{handPos = (rand()%state.handCount[j]);}
          HandGenerator(&state, j, state.handCount[j], 0, treasure_map);
          state.hand[j][handPos] = minion; // ENUM Value 17
        }
        else
        {
          HandGenerator(&state, j, state.handCount[j], 0, treasure_map);
        }
        
    }

    // Randomly choose choice1 or choice2
    choice1 = (rand()%2);
    if(choice1){choice2 = 0;}
    else{choice2 = 1;}

    // Run the Minion Random Test Iteration
    printf("_____ TEST #%d _____\n",i+1);
    miniontest = MinionTest(&state, player1, handPos, choice1, choice2, num_players);

    // Record Stats and if Failure occurs then print additional game information. 
    if(miniontest){ counter_failure++; }
    else{counter_success++;}

  } // end of primary for-loop

  // Result Summary
  printf("Successful:%d  vs. Failures:%d\n", counter_success, counter_failure);  

  return 0;
}


/* -- Minion Random Test Function -- */
int MinionTest(struct gameState *state, int player1, int handPos, int choice1, int choice2, int num_players)
{
  // Variables
  int flagFail = 0;
  int bonus = 0;
  int bonus_start = 0;
  int assert_state, minion_return;
  int z;

  // Set-up Test Gamestate
  struct gameState testState;
  memcpy(&testState, state, sizeof(struct gameState));

  // call minion function ...
  minion_return = minionCardEffect(choice1, choice2, &testState, handPos);
  if(minion_return != 0){printf("Minion Failed Execution\n");}
  
  // +1 Action
  assert_state = AssertTest((testState.numActions == state->numActions + 1), "+1 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state->numActions +1); }

  // +0 Trash Count
  //assert_state = AssertTest((testState.trashedCardCount == state->trashedCardCount), "+0 Trashed Card Count");
  //if(assert_state){flagFail = 1; printf("\tTrash Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state->trashedCardCount);}
 
  // Player selected choice1 (+2 Bonus)
  if(choice1)
  {
    // +2 Bonus
    assert_state = AssertTest((testState.coins == state->coins+2), "+2 Bonus");
    if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state->coins+2);}

    // +1 Discard Count
    assert_state = AssertTest((testState.discardCount[player1] == state->discardCount[player1]+1), "Player1: +1 Discard Count");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state->discardCount[player1]+1);}

    //-1 Hand Count
    assert_state = AssertTest((testState.handCount[player1] == state->handCount[player1]-1), "Player1: -1 Hand Count");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state->handCount[player1]-1);}

  }
  // Player selected choice2 (Discard Hand & Redraw 4 cards)
  else if(choice2)
  {
    // +0 Bonus
    assert_state = AssertTest((testState.coins == state->coins), "+0 Bonus");
    if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", testState.coins, state->coins);}

    // Player 1: Test Hand Count <= 4 (could check more indepth with checking deck/discard sizes and if deck is empty)
    if(state->discardCount[player1] + state->deckCount[player1] +state->handCount[player1] < 4)
    {
      assert_state = AssertTest((testState.handCount[player1] == state->discardCount[player1] + state->deckCount[player1] +state->handCount[player1]), "Player1: Hand Count == Total Cards");
      if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state->discardCount[player1] + state->deckCount[player1] +state->handCount[player1]);}
    }
    else
    {
      assert_state = AssertTest((testState.handCount[player1] == 4), "Player1: Hand Count == 4");
      if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], 4);}

    }
    
    assert_state = AssertTest((testState.handCount[player1] <= 4), "Player1: Hand Count <= 4");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected <= %d\n", testState.handCount[player1], 4);}

    // Player 1: Test Discard Count = State Hand Count
    assert_state = AssertTest((testState.discardCount[player1] == state->handCount[player1]), "+handCount Discard Count");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state->handCount[player1]);}

    // Checks for Each Opponent.
    for(z = 0; z<num_players; z++)
    {
        if(z != player1)
        {
          // Opponent Hand Counts <= 4
          assert_state = AssertTest((testState.handCount[z] <= 4), "Opponent: Hand Count <= 4");
          if(assert_state){flagFail = 1; printf("\tPlayer #%d - Hand Count: Current = %d, Expected <= %d\n", z, testState.handCount[z], 4);}

          // Opponent Discard Counts Based on Minion Card Rules 
          if(state->handCount[z]>4)
          {
            assert_state = AssertTest((testState.discardCount[z] == state->handCount[z]), "+handCount Discard Count");
            if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[z], state->handCount[z]);}
          }
          else
          {
            assert_state = AssertTest((testState.discardCount[z] == state->discardCount[z]), "+0 Discard Count");
            if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[z], state->discardCount[z]);}  
          }

        }
    }
  }

  if(flagFail)
  {
    printf("  *More Information: \n");
    int r;
    for(r=0; r<num_players; r++)
    {
      printf("\tPlayer%d Piles: Hand Count: %d ; Discard Count: %d ; Deck Count: %d\n", r+1, state->handCount[r],state->discardCount[r],state->deckCount[r]);
      printf("\t");
      DisplayHand(state, r, "Player");
    }
    printf("\tChoice1: %d & Choice2: %d\n\n",choice1, choice2);
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

