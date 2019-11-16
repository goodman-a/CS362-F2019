/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 4 Learn How to Create a Random Tester
 * Author: Alexander Goodman
 * Due Date: 17 November 2019
 * 
 * File: randomtestcard3.c
 * 
 * File Description: Random Tester for the Tribute Card Function:
 *   Orig: tributeCard(int handPos, int currentPlayer, int nextPlayer, int tributeRevealedCards[], struct gameState* state, int* bonus);
 *   New: int tributeCardEffect(struct gameState *state)
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
void DiscardGenerator(struct gameState *state, int player, int size, int min, int max);
void DeckGenerator(struct gameState *state, int player, int size, int min, int max);

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
  int maxHand = 10;
  int maxDiscard = 20;
  int maxDeck = 30;
  
  // Counters
  int counter_success=0, counter_failure=0, tributetest = 0;
  int count_tests = 5000;

  // Kingdom Cards
  int k[10] = {adventurer, council_room, tribute, gardens, mine, remodel, smithy, village, baron, great_hall};


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
        if(j == 0)
        {
          state.handCount[j] = (rand()%maxHand)+1;
          if(state.handCount[j] == 1){handPos=0;}
          else{handPos = (rand()%state.handCount[j]);}
          HandGenerator(&state, j, state.handCount[j], 0, treasure_map);
          state.hand[j][handPos] = tribute; // ENUM Value 19
        }
        else
        {
          state.handCount[j] = (rand()%maxHand);
          HandGenerator(&state, j, state.handCount[j], 0, treasure_map);
        }
        
    }
    
  for(j=0; j<num_players; j++)
  {
    state.discardCount[j] = (rand()%maxDiscard);
    DiscardGenerator(&state, j, state.discardCount[j], 0, treasure_map);
    state.deckCount[j] = (rand()%maxDeck);
    DeckGenerator(&state, j, state.deckCount[j], 0, treasure_map);
  }

    // Run the Tribute Random Test
    printf("\n_____ TEST #%d _____\n", i+1);
    tributetest = TributeTest(&state, player1, player2, handPos);

    // Record Stats and if Failure occurs then print additional game information. 
    if(tributetest){ counter_failure++; }
    else{counter_success++;}

  } // end of for-loop

  // Result Summary
  printf("Successful:%d  vs. Failures:%d\n", counter_success, counter_failure);  

  return 0;

  }

/* -- Tribute Random Test Function -- */
int TributeTest(struct gameState *state, int player1, int player2, int handPos)
{
  // Variables
  int flagFail = 0;
  int bonus = 0;
  int bonus_start = 0;
  int bonus_count = 0;
  int hand_count = 0;
  int action_count = 0;
  int assert_state, tribute_return = -1;
  int z;
  int tributeRevealedCards[2] = {-1, -1};

  // Set-up Test Gamestate
  struct gameState testState;
  memcpy(&testState, state, sizeof(struct gameState));

  // call tributeCard function ...
  tribute_return = tributeCardEffect(&testState);
  if(tribute_return != 0){printf("Tribute Failed Execution\n");}

  /* - Player2 Stats: - */
  // Player2 has 0 or 1 cards to reveal.
  if(state->discardCount[player2] + state->deckCount[player2] <= 1)
  {
    if(state->deckCount[player2] > 0)
    {
      // check revealed card
      assert_state = AssertTest((tributeRevealedCards[0] == state->deck[player2][state->deckCount[player2]-1]), "Revealed Card 1");
      if(assert_state){flagFail = 1; printf("\tRevealed Card 1: Current = %d, Expected = %d\n", tributeRevealedCards[0], state->deck[player2][state->deckCount[player2]-1]);}

      // +1 discard count
      assert_state = AssertTest((testState.discardCount[player2] == state->discardCount[player2]+1), "Player2: Discard Count");
      if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state->discardCount[player2]+1);}

      // -1 deck count
      assert_state = AssertTest((testState.deckCount[player2] == state->deckCount[player2]-1), "Player2: Deck Count");
      if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], state->deckCount[player2]-1);}
    }
    else if(state->discardCount[player2] > 0)
    {
      // +0 discard count
      assert_state = AssertTest((testState.discardCount[player2] == state->discardCount[player2]), "Player2: Discard Count");
      if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state->discardCount[player2]);}

      // +0 deck count
      assert_state = AssertTest((testState.deckCount[player2] == state->deckCount[player2]), "Player2: Deck Count");
      if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], state->deckCount[player2]);}
    }
    // No Cards to Reveal
    else
    {        
      // Revealed Cards == -1
      assert_state = AssertTest((tributeRevealedCards[0] == -1), "Revealed Card 1 == -1");
      if(assert_state){flagFail = 1; printf("\tRevealed Card 1: Current = %d Expected = %d\n", tributeRevealedCards[0], -1);}

      assert_state = AssertTest((tributeRevealedCards[1] == -1), "Revealed Card 2 == -1");
      if(assert_state){flagFail = 1; printf("\tRevealed Card 2: Current = %d Expected = %d\n", tributeRevealedCards[1], -1);}

      // +0 discard count
      assert_state = AssertTest((testState.discardCount[player2] == state->discardCount[player2]), "Player2: Discard Count");
      if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state->discardCount[player2]);}

      // +0 deck count
      assert_state = AssertTest((testState.deckCount[player2] == state->deckCount[player2]), "Player2: Deck Count");
      if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], state->deckCount[player2]);}
    } 
  }

  // Player2 has > 1 card to reveal.
  else
  {
    if(state->deckCount[player2] == 0)
    {
      //testState deck count== state discard count -2
      assert_state = AssertTest((testState.deckCount[player2] == state->discardCount[player2]-2), "Player2: Deck Count");
      if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], state->discardCount[player2]-2);}

      assert_state = AssertTest((testState.discardCount[player2] == 2), "Player2: Discard Count");
      if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], 2);}

      // check shuffle
      assert_state = AssertTest((CheckShuffle(state, &testState, player2) == 1), "Player2: Shuffle Works");
      if(assert_state){flagFail = 1; printf("\tWARNING: Cards May Not Have Been Shuffled. Order Remained The Same.\n");}

    }
    else
    {
      // +2 Discard Count
      assert_state = AssertTest((testState.discardCount[player2] == state->discardCount[player2])+2, "Player2: Discard Count");
      if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state->discardCount[player2]+2);}
    }

  }
  
  /* - tributeRevealedCards Stats: - */
  if(tributeRevealedCards[0] != -1)
  {
    assert_state = AssertTest((tributeRevealedCards[0] != tributeRevealedCards[1]), "Revealed Card 1 != Revealed Card 2");
    if(assert_state){flagFail = 1; printf("\tRevealed Card 1: %d != Revaled Card 2: %d\n", tributeRevealedCards[0], tributeRevealedCards[1]);}
  }

  /* - Player1 Stats: - */
  // Count/Tally Rewards Earned
  for(z=0; z<2; z++)
  {
    // Treasure Card
    if(tributeRevealedCards[z] == copper || tributeRevealedCards[z] == silver || tributeRevealedCards[z] == gold )
    {
        bonus_count += 2;
    }
    // Victory Card
    else if(tributeRevealedCards[z] == estate || tributeRevealedCards[z] == duchy || tributeRevealedCards[z] == province || tributeRevealedCards[z] == gardens || tributeRevealedCards[z] == great_hall)
    {
        hand_count += 2;
    }
    // Empty
    else if(tributeRevealedCards[z] == -1)
    {
        //Nothing
    }
    // Action Card
    else
    {
        action_count += 2;
    }
    
  }

  // Player1: Bonus Count
  assert_state = AssertTest((testState.coins == state->coins + bonus_count), "Player1: Bonus Count");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", testState.coins, state->coins + bonus_count);}

  // Player1: Action Count 
  assert_state = AssertTest((testState.numActions == state->numActions+action_count), "Player1: Action Count");
  if(assert_state){flagFail = 1; printf("\tAction Count: Current = %d vs. Expected = %d\n", testState.numActions, state->numActions+action_count); }


  // Player1: Hand Count - Not enough cards in discard and deck to draw
  if(hand_count != 0 && (state->deckCount[player1] + state->discardCount[player1] < hand_count)) 
  {
    assert_state = AssertTest((testState.handCount[player1] == state->handCount[player1]-1+state->deckCount[player1] + state->discardCount[player1]), "Player1: Hand Count");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state->handCount[player1]-1+state->deckCount[player1] + state->discardCount[player1]);}
  }
  // Player1: Hand Count - Have enough cards to draw
  else
  {
    assert_state = AssertTest((testState.handCount[player1] == state->handCount[player1]-1+hand_count), "Player1: Hand Count");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state->handCount[player1]-1+hand_count);}
  }
  
  // Discard Count if Deck needs to acquire discard pile
  if(hand_count != 0 && state->deckCount[player1] < (hand_count))
  {
    assert_state = AssertTest((testState.discardCount[player1] == 1), "Player1: Discard Count");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1],1);}
  }
  // Deck does not need to pull from discard.
  else
  {
    assert_state = AssertTest((testState.discardCount[player1] == state->discardCount[player1]+1), "Player1: Discard Count");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state->discardCount[player1]+1);}
  }


  if(flagFail)
  {
    printf("  *More Information: \n");
    printf("\tPlayer1 Piles: Hand Count: %d ; Discard Count: %d ; Deck Count: %d\n", state->handCount[player1],state->discardCount[player1],state->deckCount[player1]);
    printf("\tPlayer2 Piles: Hand Count: %d ; Discard Count: %d ; Deck Count: %d\n", state->handCount[player2],state->discardCount[player2],state->deckCount[player2]);
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

// Random Discard Generator 
void DiscardGenerator(struct gameState *state, int player, int size, int min, int max)
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
      state->discard[player][i] = (rand()%(max-min+1))+min;
  }

  return;
}

// Random Deck Generator 
void DeckGenerator(struct gameState *state, int player, int size, int min, int max)
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
      state->deck[player][i] = (rand()%(max-min+1))+min;
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

// Compare that Pile was Shuffled (careful with small sample sizes ...)
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


