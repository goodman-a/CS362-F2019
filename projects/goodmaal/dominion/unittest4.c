/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 3 Learn How to Create Unit Tests
 * Author: Alexander Goodman
 * Due Date: 10 November 2019
 * 
 * File: unittest4.c
 * 
 * File Description: Tribute Card ...
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

/* tributerCard Function General info*/
/*
int tributeCard(int handPos, int currentPlayer, int nextPlayer, int tributeRevealedCards[], struct gameState* state, int* bonus){

  // if NextPlayer discard + deck count is <=1    (check discard and deck counts after entering this section)
    // if deck > 0 
        // reveal 1 card only and then discard 
        *Check Discard Count +1
        *Check Deck COunt -1
        *Only 1 Revealed Card (tributeRevealedCards[0])
        *Check the last card in players discard deck? --> state->discrd[nextPlayer][state-discardCount[nextPlayer]-1] ..

    // else if discard > 0
        // reveal 1 card only and then back to discard ... (deck is empty)
        *check discard Count Unchanged
        *Check discard pile id 1 and the card in there?

    // else
        // No Cards <-- Will only enter if no cards & then there is a worthless debug in there.

  // else There is > 1 cards in discard and deck!
    // if deckCount is empty then transfer discard pile to deck pile and then shuffle
        // for-loop that transfers cards from discard to deck pile
        //shuffle hand (BUG 01)
        *deckCount == original discardCount
        *discardCount == 0
        *that the cards actually were shuffled... first display the original discard against the new deck, but will need to add a for loop to check with a flag passed in.
    // store the top 2 cards in tributeRevealedCards[]
    *verify tributeRevealedCards[] are the actual cards
    *deckCount -2
    *discardCount +2

    // if the 2 cards that were on-top are the same set the second one to -1
    *duplicate card is in trash pile
    *trashedCardCount +1
    *tributeRevealedCards[1] == -1

    //for-loop through the two array positions in tributeRevealedCards[]
        //if tributeRevealedCards[i] == treasure card 
            // +2 bonus  (bonus issue?????)
            *check that only treasure cards get the bonus
            *check that only 2 treasure cards produce +4??
        //else if tributeRevealedCards[i] == Victory Card
            // Draw 2 Careds
            *check that handCount +2
            *check that deckCount -1
        //else if tributeRevealedCards[i] == -1
            // Nothing should happen, just if in debug print to screen.
        //else (tribueRevealedCareds[i] == Action Card)
            //numActions +2
            *check numActions +2

  // discard tribute card
  *discardCount +1 (need to add all the others together)
  *trashCount remains same unless during duplicate careds ...
  *handCount -1
  *card removed was actually the tribute card...


*/


/* Test Cases */

  // TEST 01 -


  // TEST ** - More than 3 revealed cards?

  // TEST ** - Wrap Around Next Player?
  
  // TESTS:
    //1. 1 in deck, 0 in discard -- reveal 1
    //2. 0 in deck, 1 in discard -- reveal 1
    //3. 0 in deck, 2+ in discard -- move, shuffle, reveal 2
    //4. ***1 in deck, 1+ in discard -- shuffle, reveal, 1 from each? --> Chung Went #1047
    //5. 0 in deck, 0 in discard -- reveal none
    //6. 2+ in deck w/ 2*action cards revealed
    //7. 2+ in deck w/ 1 action & 1 victory
    //8. 2+ in deck w/ 1 vicotry & 1 estate
    //9. 2+ in deck w/ 1 action & 1 estate
    //10. 2+ in deck w/ card that has both action & victory along with an estate
    //11. cna't hold three cards ....

/* -- List of Bugs -- */
  // Note: The code provided to the class had LOTS of bugs in this section. Most were fixed during refactoring, but I am sure some are lingering around
  // Bug 01: Commented out shuffle in the primary else statement
  // Bug 02: Commented out one of te draw card functions, so only drawing 1 instead of 2..


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
  //int handNum_player1 = 5;
  //int handNum_player2 = 5;

  // Store return baron value ... kinda worthless here..
  int tribute_return, assert_state;
  int flagFail = 0; 

  // Declare Game States
  struct gameState state, testState;

    /* __________ TESTING TIME __________ */

  /* -- Test Overview -- */
  /* -- 
      Note: All Tests Need....
      1. Discard + deck Count is <=1 (discard empty and deck == 1) and card is gold (treasure card)
        a. Verify tributeRevealedCards[0] == gold and tributeRevealedCards[1] == -1
        b. Player2: -1 deckCount & +1 discardCount
        c. Player2: discard[discardCount-1] == gold
        d. Player1: +2 bonus
        e. Player1: +1 discardCount
        f. Player1: +0 numActions
        g. Player1: +0 handCount
      2. Discard + Deck Count is <=1 (deck empty and discard ==1) and card is gold (treasure card) <-- maybe do victory card..
        a. Verify tributeRevealedCards[0] == gold and tributeRevealedCards[1] == -1
        b. Player2: +0 deckCount & +0 discardCount
        c. Player2: discard[discardCount-1] == gold
        d. Player1: +2 bonus
        e. Player1: +1 discardCount
        f. Player1: +0 numActions
        g. Player1: +0 handCount
      3. Discard + Deck Count is == 0 AND/OR -1
        a. Verify tributeRevealedCards[0] == -1 and tributeRevealedCards[1] == -1
        b. Player2: +0 deckCount & +0 discardCount
        c. Player1: +1 discardCount
        d. Player1: +0 bonus
        e. Player1: +0 numActions
        f. Player1: +0 handCount
      4. Discard = 1+ & Deck = 1 (should reveal one for each....)
        a. CheckShuffle  
      4b. Discard = 2+ and Deck = 0
      5. 2+ in Deck --> Action & Treasure (Minion & Gold)
      6. 2+ in Deck --> Action & Victory (Minion & Gardens)
      7. 2+ in Deck --> Treasure & Victory (Gold & Estate)
      8. 2+ in Deck --> Treasure & Treasure (Gold & Gold) 
        a. duplicate card is in trash pile, trashedCardCount +1, tributeRevealedCard[1] == -1 hmmm bug?
      9. 2+ in Deck --> Treasure & Treasure (Gold & Silver)
      10. REMEMBER THE CASE WHERE THERE ARE 3+ types and array can't hold...
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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+2);}

  // Player1: +0 numActions  
  assert_state = AssertTest((testState.numActions == state.numActions), "Player1: +0 Action");
  if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions); }

  // Player1: -1 Hand Count
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player1: -1 Hand Count");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // Player1: +1 Discard Count (Tribute Card)
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Player1: +1 Discard Count (Tribute Card)");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}


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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+0), "Player1: +0 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start);}

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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+0), "Player1: +0 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start);}

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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+2);}

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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+2);}

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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start), "Player1: +0 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start);}

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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+2);}

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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+2);}

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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+4), "Player1: +4 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+4);}

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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+2);}

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
  tribute_return = tributeCard(handPos, player1, player2, tributeRevealedCards, &testState, &bonus);

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
  assert_state = AssertTest((bonus == bonus_start+2), "Player1: +2 Bonus");
  if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+2);}

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

