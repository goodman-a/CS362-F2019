/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 3 Learn How to Create Unit Tests
 * Author: Alexander Goodman
 * Due Date: 10 November 2019
 * 
 * File: unittest4.c
 * 
 * File Description: Unit Test for the Tribute Card Function:
 *   Orig: tributeCard(int handPos, int currentPlayer, int nextPlayer, int tributeRevealedCards[], struct gameState* state, int* bonus);
 *   New: int tributeCardEffect(struct gameState *state)
 * 
 * DOES NOT CHECK FOR CASES WHERE 3+ Rewards with cards having 2 types
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


/* -- Globals -- */
// seed
int seed = 1000;
// Iterators
int i, r, m;
// Kingdom Cards
int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

/* -- Helper Function Prototypes -- */
void ResetGame(struct gameState *state, int num_players);
void SetUpHand(struct gameState *state, int player, int num_hand);
void DisplayHand(struct gameState *state, int player, char* msg);
void DisplayDiscard(struct gameState *state, int player, char* msg);
void DisplayDeck(struct gameState *state, int player, char* msg);
int HandCardCount(struct gameState *state, int player, int choice1);
int HandCardCount2(struct gameState *state, int player, int choice1, int handPos);
int CheckShuffle(struct gameState *state_old, struct gameState *state_new, int player);  // retVal == 1 good, retVal <1 no shuffle

/* -- MAIN FUNCTION -- */
int main(int argc, char** argv){

  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // Passed in gameplay values
  int handPos = 0, bonus = 0, bonus_start = 0;

  // Set-up/Initialize Parameters
  int player1 = 0;
  int player2 = 1;
  int num_players = 2;
  int tributeRevealedCards[2] = {-1, -1};
  int card1, card2;

  // Store return baron value ... kinda worthless here..
  int tribute_return, assert_state;
  int flagFail = 0; 

  // Declare Game States
  struct gameState state, testState;

    /* __________ TESTING TIME __________ */

  /* -- Test Overview -- */
  /* --
    //1. Deck Count == 1, Discard Count == 0 -- reveal 1
    //2. Deck Count == 1, Discard Count == 1 -- reveal 1
	//3. Deck Count == 0, Discard Count == 0
	//4. Deck Count == 0, Discard Count >= 2
	//5. Deck Count >= 2 (Action & Treasure)
	//6. Deck Count >= 2 (Action & Victory)
	//7. Deck Count >= 2 (Treasure & Victory)
	//8. Deck Count >= 2 (Treasure & Treasure (same))
	//9. Deck Count >= 2 (Treasure & Treasure(different))
	//10. Special Case I: Deck Count == 0, Discard Count >= 5 --shuffle check
	//11. Special Case II: Deck Count == 1, Discard Count >= 5
	//Future Test: hold 3+ actions in tributeRevealedCards array.
  -- */

  /* -- List of Bugs -- */
  /* --
    Note: The code provided to the class had LOTS of bugs in this section. Most were fixed during refactoring, but I am sure some are lingering around
    Bug 01: Commented out shuffle in the primary else statement
    Bug 02: Commented out one of te draw card functions, so only drawing 1 instead of 2..
  -- */


  printf("\n********** TRIBUTE CARD TEST **********\n");

  /* -- TEST 1: DiscardCount + DeckCount <=1 --> Discard=0 ; Deck = 1 -- */
  printf("----- TEST 1: Discard = 0 card ; Deck = 1 card-----\n");

  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  state.deckCount[player2] = 1;
  state.deck[player2][state.deckCount[player2]-1] = gold;
  state.discardCount[player2] = 0;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect(&testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == gold
  assert_state = AssertTest((tributeRevealedCards[0] == gold), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], gold);}

  // Check that tributeRevealedCards[1] == -1
  assert_state = AssertTest((tributeRevealedCards[1] == -1), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], -1);}

  // Player2 Discard Count +1. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]+1), "Player 2: Discard Count Gain +1 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]+1);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-1] == state.deck[player2][state.deckCount[player2]-1]), "Player 2: Discard Pile Holds Correct Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-1], state.deck[player2][state.deckCount[player2]-1]);}

  // Player1: +2 Bonus 
    assert_state = AssertTest((testState.coins == state.coins+2), "+2 Bonus");
    if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins+2);}

  // Player1: +0 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions), "Player1: +0 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}

  if(flagFail)
  {
      //printf("FAILURE...\n");

  }


/* -- TEST 2: DiscardCount + DeckCount <=1 --> Discard=1 ; Deck = 0 -- */
  printf("----- TEST 2: Discard = 1 card ; Deck = 0 card-----\n");

  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  state.deckCount[player2] = 0;
  state.discardCount[player2] = 1;
  state.discard[player2][state.discardCount[player2]-1] = minion;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect(&testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == minion
  assert_state = AssertTest((tributeRevealedCards[0] == minion), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], minion);}

  // Check that tributeRevealedCards[1] == -1
  assert_state = AssertTest((tributeRevealedCards[1] == -1), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], -1);}

  // Player2 Discard Count +0. 
  assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]), "Player 2: Discard Count +0 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-1] == minion), "Player 2: Discard Pile Holds Correct Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-1], minion);}

  // Player1: +0 Bonus 
  assert_state = AssertTest((testState.coins == state.coins), "Player1: +0 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins);}

  // Player1: +2 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions+2), "Player1: +2 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions+2); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}


/* -- TEST 3: DiscardCount + DeckCount == 0 --> Discard=0 ; Deck = 0 -- */
  printf("----- TEST 3: Discard = 0 card ; Deck = 0 card-----\n");

  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  state.deckCount[player2] = 0;
  state.discardCount[player2] = 0;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect(&testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == -1
  assert_state = AssertTest((tributeRevealedCards[0] == -1), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], -1);}

  // Check that tributeRevealedCards[1] == -1
  assert_state = AssertTest((tributeRevealedCards[1] == -1), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], -1);}

  // Player2 Discard Count +0. 
  assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]), "Player 2: Discard Count +0 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]);}

  // Player1: +0 Bonus 
  assert_state = AssertTest((testState.coins == state.coins), "Player1: +0 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins);}

  // Player1: +0 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions), "Player1: +0 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}


  /* -- TEST 4: DiscardCount + DeckCount > 1 --> Discard = 2+ cards  ; Deck = 0 Cards-- */
  printf("----- TEST 4: Discard = 2+ cards ; Deck = 0 card-----\n");

  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  memset(state.deck[player2], -1, sizeof(state.deck[player2]));
  state.deckCount[player2] = 0;
  state.discardCount[player2] = 2;
  state.discard[player2][state.discardCount[player2]-1] = minion;
  state.discard[player2][state.discardCount[player2]-2] = gold;
  card1 = minion;
  card2 = gold;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect(&testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == card1
  assert_state = AssertTest((tributeRevealedCards[0] == card1), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], card1);}

  // Check that tributeRevealedCards[1] == card2
  assert_state = AssertTest((tributeRevealedCards[1] == card2), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], card2);}

  // Player2 Discard Count +0. 
  assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]), "Player 2: Discard Count +0 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-1] == card2), "Player 2: Discard Pile Holds Correct Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-1], card2);}
  
  // Player2 Deck Count +0. 
  assert_state = AssertTest((testState.deckCount[player2] == 0), "Player 2: Deck Count +0 Card");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], 0);}

  // Player1: +2 Bonus 
  assert_state = AssertTest((testState.coins == state.coins+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins+2);}

  // Player1: +2 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions+2), "Player1: +2 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions+2); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}

  /* -- TEST 5: DeckCount >=2 --> Action & Treasure (Minion & Gold) -- */
  printf("----- TEST 5:DeckCount >= 2 ; Action & Treasure -----\n");
  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  state.deck[player2][state.deckCount[player2]-1] = minion;
  state.deck[player2][state.deckCount[player2]-2] = gold;
  card1 = minion;
  card2 = gold;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect(&testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == card1
  assert_state = AssertTest((tributeRevealedCards[0] == card1), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], card1);}

  // Check that tributeRevealedCards[1] == card2
  assert_state = AssertTest((tributeRevealedCards[1] == card2), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], card2);}

  // Player2 Discard Count +2. 
  assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]+2), "Player 2: Discard Count +2 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]+2);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-1] == card2), "Player 2: Discard Pile Holds Correct Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-1], card2);}
  
  // Player2 Deck Count -2. 
  assert_state = AssertTest((testState.deckCount[player2] == state.deckCount[player2]-2), "Player 2: Deck Count -2 Card");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], state.deckCount[player2]-2);}

  // Player1: +2 Bonus 
  assert_state = AssertTest((testState.coins == state.coins+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins+2);}

  // Player1: +2 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions+2), "Player1: +2 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions+2); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}


  /* -- TEST 6: DeckCount >=2 --> Action & Victory (Minion & Gardens) -- */
  printf("----- TEST 6:DeckCount >= 2 ; Action & Victory -----\n");
  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  card1 = minion;
  card2 = gardens;
  state.deck[player2][state.deckCount[player2]-1] = card1;
  state.deck[player2][state.deckCount[player2]-2] = card2;


  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect(&testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == card1
  assert_state = AssertTest((tributeRevealedCards[0] == card1), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], card1);}

  // Check that tributeRevealedCards[1] == card2
  assert_state = AssertTest((tributeRevealedCards[1] == card2), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], card2);}

  // Player2 Discard Count +2. 
  assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]+2), "Player 2: Discard Count +2 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]+2);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-1] == card2), "Player 2: Discard Pile Holds Correct Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-1], card2);}
  
  // Player2 Deck Count -2. 
  assert_state = AssertTest((testState.deckCount[player2] == state.deckCount[player2]-2), "Player 2: Deck Count -2 Card");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], state.deckCount[player2]-2);}

  // Player1: +0 Bonus 
  assert_state = AssertTest((testState.coins == state.coins), "Player1: +0 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins);}

  // Player1: +2 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions+2), "Player1: +2 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions+2); }

  // Player1: +1 Hand Count (draw 2 (+2) & discard 1 (-1) = +1)
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]+1), "Player1: +1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]+1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}


  /* -- TEST 7: DeckCount >=2 --> Treasure & Victory (Gold & Estate) -- */
  printf("----- TEST 7:DeckCount >= 2 ; Treasure & Victory -----\n");
  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  card1 = gold;
  card2 = gardens;
  state.deck[player2][state.deckCount[player2]-1] = card1;
  state.deck[player2][state.deckCount[player2]-2] = card2;


  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect(&testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == card1
  assert_state = AssertTest((tributeRevealedCards[0] == card1), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], card1);}

  // Check that tributeRevealedCards[1] == card2
  assert_state = AssertTest((tributeRevealedCards[1] == card2), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], card2);}

  // Player2 Discard Count +2. 
  assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]+2), "Player 2: Discard Count +2 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]+2);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-1] == card2), "Player 2: Discard Pile Holds Correct Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-1], card2);}
  
  // Player2 Deck Count -2. 
  assert_state = AssertTest((testState.deckCount[player2] == state.deckCount[player2]-2), "Player 2: Deck Count -2 Card");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], state.deckCount[player2]-2);}

  // Player1: +2 Bonus 
  assert_state = AssertTest((testState.coins == state.coins+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins+2);}

  // Player1: +0 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions), "Player1: +0 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions); }

  // Player1: +1 Hand Count (draw 2 (+2) & discard 1 (-1) = +1)
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]+1), "Player1: +1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]+1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}


  /* -- TEST 8: DeckCount >=2 --> Treasure & Treasure (Gold & Gold) -- */
  printf("----- TEST 8:DeckCount >= 2 ; Treasure & Treasure (Same) -----\n");
  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  card1 = gold;
  card2 = gold;
  state.deck[player2][state.deckCount[player2]-1] = card1;
  state.deck[player2][state.deckCount[player2]-2] = card2;


  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect(&testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == card1
  assert_state = AssertTest((tributeRevealedCards[0] == card1), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], card1);}

  // Check that tributeRevealedCards[1] == -1
  assert_state = AssertTest((tributeRevealedCards[1] == -1), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], -1);}

  // Player2 Discard Count +2. 
  assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]+2), "Player 2: Discard Count +2 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]+2);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-2] == card1), "Player 2: Discard Pile Holds Correct Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-2], card1);}
  
  // Player2 Deck Count -2. 
  assert_state = AssertTest((testState.deckCount[player2] == state.deckCount[player2]-2), "Player 2: Deck Count -2 Card");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], state.deckCount[player2]-2);}

  // Player1: +2 Bonus 
  assert_state = AssertTest((testState.coins == state.coins+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins+2);}

  // Player1: +0 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions), "Player1: +0 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-11);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}


  /* -- TEST 9: DeckCount >=2 --> Treasure & Treasure (Silver & Gold) -- */
  printf("----- TEST 9:DeckCount >= 2 ; Treasure & Treasure (Different) -----\n");
  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  card1 = gold;
  card2 = silver;
  state.deck[player2][state.deckCount[player2]-1] = card1;
  state.deck[player2][state.deckCount[player2]-2] = card2;


  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect( &testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == card1
  assert_state = AssertTest((tributeRevealedCards[0] == card1), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], card1);}

  // Check that tributeRevealedCards[1] == card2
  assert_state = AssertTest((tributeRevealedCards[1] == card2), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], card2);}

  // Player2 Discard Count +2. 
  assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]+2), "Player 2: Discard Count +2 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]+2);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-2] == card1), "Player 2: Discard Pile Holds Correct Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-2], card1);}
  
  // Player2 Deck Count -2. 
  assert_state = AssertTest((testState.deckCount[player2] == state.deckCount[player2]-2), "Player 2: Deck Count -2 Card");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], state.deckCount[player2]-2);}

  // Player1: +4 Bonus 
  assert_state = AssertTest((testState.coins == state.coins+4), "Player1: +4 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins+4);}

  // Player1: +0 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions), "Player1: +0 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-11);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}


  /* -- TEST SPECIAL: DiscardCount = 5+card & DeckCount == 0 --> Check Shuffle-- */
  printf("----- TEST SPECIAL CASE I: Discard >= 5 cards ; Deck = 0 card (shuffle) -----\n");  // should reveal 1 for each... 

  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  memset(state.deck[player2], -1, sizeof(state.deck[player2]));
  state.deckCount[player2] = 0;
  state.discardCount[player2] = 5;
  card1 = minion;
  card2 = gold;
  state.discard[player2][state.discardCount[player2]-1] = card1;
  state.discard[player2][state.discardCount[player2]-2] = card2;
  state.discard[player2][state.discardCount[player2]-3] = silver;
  state.discard[player2][state.discardCount[player2]-4] = copper;
  state.discard[player2][state.discardCount[player2]-5] = estate;

    // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect( &testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

  // Check that tributeRevealedCards[0] == card1
  assert_state = AssertTest((tributeRevealedCards[0] == card1), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], card1);}

  // Check that tributeRevealedCards[1] == card2
  assert_state = AssertTest((tributeRevealedCards[1] == card2), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], card2);}

  // Player2 Discard Count +2. 
  assert_state = AssertTest((testState.discardCount[player2] == 2), "Player 2: Discard Count +2 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], 2);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-1] == card2), "Player 2: Discard Pile Holds Correct Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-1], card2);}
  
  // Player2 Deck Count +3. 
  assert_state = AssertTest((testState.deckCount[player2] == 3), "Player 2: Deck Count +3 Card");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], 3);}

  // Player1: +2 Bonus 
  assert_state = AssertTest((testState.coins == state.coins+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins+2);}

  // Player1: +2 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions+2), "Player1: +2 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions+2); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}

  // Player2: Cards Shuffled
  assert_state = AssertTest((CheckShuffle(&state, &testState, player2) == 1), "Player2: Shuffe Works");
  if(assert_state){flagFail = 1; printf("\tWARNING: Cards May Not Have Been Shuffled. Order Remained The Same.\n");}
    // DEBUG:DisplayDeck(&testState, player2, "Player2");DisplayDiscard(&state, player2,"Player2");}


  /* -- TEST SPECIAL: DiscardCount + DeckCount > 1 --> Discard >= 5 cards  ; Deck = 1 Cards-- */
  printf("----- TEST SPECIAL CASE II: Discard >= 5 cards ; Deck = 1 card -----\n");  // should reveal 1 for each...

  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  bonus = 0;
  bonus_start = 0;
  memset(tributeRevealedCards, -1, sizeof(tributeRevealedCards));
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = tribute;
  memset(state.deck[player2], -1, sizeof(state.deck[player2]));
  card1 = gold;
  card2 = minion;
  state.deckCount[player2] = 1;
  state.deck[player2][0] = card1;
  state.discardCount[player2] = 1;
  state.discard[player2][state.discardCount[player2]-1] = card2;


  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function
  tribute_return = tributeCardEffect( &testState);

  // Check for return value
  assert_state = AssertTest((tribute_return == 0), "Tribute Card Returned Properly");
  if(assert_state){flagFail = 1; printf("\tERROR - Tribute Card Did Not Return Properliy.\n"); }

    // Check that tributeRevealedCards[0] == card1
  assert_state = AssertTest((tributeRevealedCards[0] == card1), "tributeRevealedCards[0] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[0]: Current = %d, Expected = %d\n", tributeRevealedCards[0], card1);}

  // Check that tributeRevealedCards[1] == card2
  assert_state = AssertTest((tributeRevealedCards[1] == card2), "tributeRevealedCards[1] Holds Correct Value");
  if(assert_state){flagFail = 1; printf("\ttributeRevealedCards[1]: Current = %d, Expected = %d\n", tributeRevealedCards[1], card2);}

  // Player2 Discard Count +2. 
  assert_state = AssertTest((testState.discardCount[player2] == 2), "Player 2: Discard Count +2 Card");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], 2);}

  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-2] == card1), "Player 2: Discard Pile Holds Correct Card1");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-2], card1);}
 
  // Player2: Discard Holds Actual Card. (Now Holds the Revealed Card From Deck.)
  assert_state = AssertTest((testState.discard[player2][testState.discardCount[player2]-1] == card2), "Player 2: Discard Pile Holds Correct Card2");
  if(assert_state){flagFail = 1; printf("\tDiscard Pile Last Card: Current = %d, Expected = %d\n", testState.discard[player2][testState.discardCount[player2]-1], card2);}
  
  // Player2 Deck Count +0. 
  assert_state = AssertTest((testState.deckCount[player2] == 0), "Player 2: Deck Count +0 Card");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player2], 0);}

  // Player1: +2 Bonus 
  assert_state = AssertTest((testState.coins == state.coins+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Expected = %d\n", testState.coins, state.coins+2);}

  // Player1: +2 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions+2), "Player1: +2 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions+2); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}

  // Too small a sample for shuffle to be ran.



  return 0;
}



/* -- Helper Functions -- */

// Reset Game for a Controlled Test
void ResetGame(struct gameState *state, int num_players)
{	
    // Initialize the game to populate decks.
    memset(state, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(num_players, k, seed, state); // initialize a new game

    if (r<0){printf("ERROR DURING INITIALIZATION\n");}
}

// discard all current cards and reset hand with all copper
void SetUpHand(struct gameState *state, int player, int num_hand)
{
int i;
while(state->handCount[player] > 0)
{
    discardCard(0, player, state, 0);
}

state->handCount[player] = num_hand;
for(i=0; i<num_hand; i++)
{
    state->hand[player][i] = copper;
}
}

// Display Cards in Hand, Discard, and Deck Piles
void DisplayHand(struct gameState *state, int player, char* msg)
{
    printf("%s Hand Pile: ", msg);
    for (m=0; m<state->handCount[player]; m++) {
        printf("(%d)", state->hand[player][m]);
    }
    printf("\n");
}

void DisplayDiscard(struct gameState *state, int player, char* msg)
{
    printf("%s Discard Pile: ", msg);
    for (m=0; m<state->discardCount[player]; m++) {
        printf("(%d)", state->discard[player][m]);
    }
    printf("\n");
}

void DisplayDeck(struct gameState *state, int player, char* msg)
{
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

