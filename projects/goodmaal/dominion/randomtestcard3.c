/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 4 Learn How to Create a Random Tester
 * Author: Alexander Goodman
 * Due Date: 17 November 2019
 * 
 * File: randomtestcard3.c
 * 
 * File Description: Random Tester for the Tribute Card Function:
 *   tributeCard(int handPos, int currentPlayer, int nextPlayer, int tributeRevealedCards[], struct gameState* state, int* bonus);
 * 
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
int CheckShuffle(struct gameState *state_old, struct gameState *state_new, int player);  // retVal == 1 good, retVal <1 no shuffle
int TributeTest(struct gameState *state, int player1, int player2, int handPos);


/* -- MAIN FUNCTION -- */
int main(int argc, char** argv){
  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // general iterators
  int i, j, r;

  // gameplay values
  int handPos = 0;
  
  // setup/initialize parameters
  srand(time(0));

  int seed = rand();
  int player1 = 0;
  int player2 = 1;
  int num_players;
  
  // Counters
  int counter_success=0, counter_failure=0, tributetest = 0;
  int count_tests = 5;

  // Kingdom Cards
  int k[10] = {adventurer, council_room, tribute, gardens, mine, remodel, smithy, village, baron, great_hall};


  // Create a Game State and Clear Memory
  struct gameState state;
  memset(&state, 0, sizeof(struct gameState));   // clear the game state

  /* -- Run count_tests number of tests -- */
  for(i=0; i<count_tests; i++)
  {
    //printf("TEST #%d\n",i+1);
   
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
          state.hand[j][handPos] = tribute; // ENUM Value 19
        }
        else
        {
          HandGenerator(&state, j, state.handCount[j], 0, treasure_map);
        }
        
    }
/* -- TEST CHECKS -- */
/*
    // Number of Players:
    printf("Number of Players: %d\n",num_players);
    printf("Choice1: %d\n",choice1);
    printf("Choice2: %d\n",choice2);

    // Display Each Player's Hand
    for(r=0 ; r<num_players; r++)
    {
      DisplayHand(&state, r, "Player");
    }

*/
/* -- END OF TEST CHECKS -- */

    // call TributeTest and record if test was success or failure.. update stats.
    tributetest = TributeTest(&state, player1, player2, handPos);

    if(tributetest){counter_failure++;}
    else{counter_success++;}

  } // end of for-loop

  // Result Summary
  printf("Successful:%d  vs. Failures:%d\n", counter_success, counter_failure);  

  return 0;

  }


// Tribute Random Test Function
int TributeTest(struct gameState *state, int player1, int player2, int handPos)
{
  // Variables
  int flagFail = 0;
  int bonus = 0;
  int bonus_start = 0;
  int assert_state, tribute_return = -1;
  int z;
  int tributeRevealedCards[2] = {-1, -1};

  // Set-up Test Gamestate
  struct gameState testState;
  memcpy(&testState, state, sizeof(struct gameState));

  // call tributeCard function ...
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);
  if(tribute_return != 0){printf("Tribute Failed Execution\n");}

  /* - Player2 Stats: - */
  if(state->discardCount[player2] + state->deckCount[player2] <= 1)
  {
    if(state->deckCount[player2] > 0)
    {
        // check revealed card

        // +1 discard count

        // -1 deck count
    }

    else if(state->discardCount[player2] >0)
    {
        // check revealed card

        // +0 discard count

        // +0 deck count
    }

    else
    {
        // NO Cards to Reveal
        
        // Revealed Cards == -1
        // +0 discard count
        // +0 deck count
    } 
  }

  else
  {
    // if(state->deckCount[player2] == 0)
      //testState deck count -2 == state discard count

      // check shuffle

    // check revealed cards ...

    // +2 discard Count

  }
  

  /* - tributeRevealedCards Stats: - */
  if(tributeRevealedCards[0] != -1){
    assert_state = AssertTest((tributeRevealedCards[0] != tributeRevealedCards[1]), "Revealed Card 1 != Revealed Card 2");
    if(assert_state){flagFail = 1; printf("\tRevealed Card 1: %d != Revaled Card 2: %d\n", tributeRevealedCards[0], tributeRevealedCards[1]);}
  }


  /* - Player1 Stats: - */

  // +1 Discard Count
  assert_state = AssertTest((testState.discardCount[player1] == state->discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state->discardCount[player1]+1);}

  // if revealed card is Treasure: +2 bonus

  // else if revealed card is Victory: Draw 2

  // else if revealed card is -1: NOTHING

  // else: +2 action 





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

// Compare that Pile was Shuffled
int CheckShuffle(struct gameState *state_old, struct gameState *state_new, int player)
{
  int retVal = -1;
  int i;
  // Only compare against the current deck count (since old discard count will be higher after cards are played)
  for(i=0; i<state_new->deckCount[player]; i++)
  {
    if(state_new->deck[player][i] != state_old->discard[player][i])
    {
        retVal = 1;
        break;
    }
  }

    return retVal;
}


