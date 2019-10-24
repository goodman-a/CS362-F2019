/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 3 Learn How to Create Unit Tests
 * Author: Alexander Goodman
 * Due Date: 10 November 2019
 * 
 * File: unittest2.c
 * 
 * File Description: Minion Card ...
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
        printf("FAILURE: %s\n", msg);
        return 1;
    }
    else
    {
        // OR NOT SAY ANYTHING?
        printf("PASS: %s\n", msg);
        return 0;
    }
    
}

/* minionCard Function General info*/

/*
  // FOR ALL TESTS:
    - Check for +1 Actions (state.numActions)
    - Check Card properly gets Discarded and Not Trashed (BUG 01)

  // TEST 01 - choice1
    - Check for +2 bonus
    - Check that no other changes happened besides the Action and Discard

  // TEST 02 - choice2 & Other Players Have <= 4 cards (BUG 02 HERE)
    - Check that Current Hand was Discarded (discardCount should be original handCount)
    - Check that New handCount == 4

  // TEST 03 - choice2 & Another Player Has > 4 Cards (BUG 02 HERE)
    - Check that Current Hand was Discarded (discardCount should be original handCount)
    - Check that New handCount == 4
    - Check that player2 handCount == 4
    - Check that player2 hand is different?

  // The Tests should check player card totals of 3, 4, & 5. These are the border cases


int minionCard(int handPos, int currentPlayer, int choice1, int choice2, struct gameState* state, int* bonus)

*/


int main(int argc, char** argv){
  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // Iterators

  int i, r, m;

  // Passed in gameplay values
  int handPos = 0, choice1 = 0, choice2 = 0, bonus = 0, bonus_start = 0;

  // Set-up/Initialize Parameters
  int seed = 1000;
  int player1 = 0;
  int player2 = 1;
  int num_players = 2;
  int handNum_player1 = 5;
  int handNum_player2 = 5;

  // Store return baron value ... kinda worthless here..
  int minion_return, assert_state;
  int flagFail = 0; 

	// Kingdom Cards
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

  // Declare Game States
  struct gameState state, testState;


  /* -- Helper Functions -- */

	// Reset Game for a Controlled Test
	void ResetGame(struct gameState *state, int num_players)
	{	
		// Initialize the game to populate decks.
		memset(state, 0, sizeof(struct gameState));   // clear the game state
		r = initializeGame(num_players, k, seed, state); // initialize a new game

    if (r<0){printf("ERROR DURING INITIALIZATION\n");}

/* 		// Set-up Hand
    for(i=0, i<num_hand, i++)
    {
      state->handCount[j] = num_hand;
      state
    }
    // WILL NEED TO DECREMENT SUPPLY COUNT TOO ... ugh and add to hand? hmm
		state->handCount[player1] = num_hand;
		state->hand[player1][0] = curse;
		state->hand[player1][1] = copper;
		state->hand[player1][2] = silver;
		state->hand[player1][3] = baron;
		state->hand[player1][4] = estate; //estate */

	}
/*
        while(state->handCount[currentPlayer] > 0 )
        {
            discardCard(0, currentPlayer, state, 0);
        }

        //draw 4
        for (i = 0; i < 4; i++)
        {
            drawCard(currentPlayer, state);
        }
  */


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



    /* __________ TESTING TIME __________ */

    /* -- Test Overview -- */
    // Note: All Tests Need to Check for +1 Actions & Discard/Trash Counts (BUG 01 HERE)
    // 1. choice1 & +2 Buys 
    // 2. choice2 & player2 hand count > 4 cards (BUG 02 HERE)
    // 3. choice2 & player2 hand count <= 4 cards
    // 4. choice2
    
    /* -- Bugs Introduced -- */
        // 1. discardCard trash bit high (1) instead of low (0). Trashing Card instead of Discarding
        // 2. state->handCount[i] >= 4 instead of state->handCount[i] > 4. 
        // note: other bugs may be present, but some have been fixed during refactoring

    printf("\n********** MINION CARD TEST **********\n");

    /* -- TEST 1: choice1 = +2 bonus -- */
    printf("----- TEST 1: choice1 = +2 bonus -----\n");
    // Set-up
    ResetGame(&state, num_players);
    state.hand[player1][0] = minion;
    memcpy(&testState, &state, sizeof(struct gameState));
    handPos = 0;
    choice1 = 1;
    choice2 = 0;
    bonus = 0;
    flagFail = 0;

    minion_return = minionCard(handPos, player1, choice1, choice2, &testState, &bonus);
    printf("Minion Value Returned: %d\n", minion_return);

    // Check numActions has increased by +1 
    assert_state = AssertTest((testState.numActions == state.numActions + 1), "+1 Action");
    if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions +1); }
    
    // Check Bonus Number has increased by +2
    assert_state = AssertTest((bonus == bonus_start + 2 ), "+2 Bonus");
    if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+2);}

    // Check Discard Count has increased by +1
    assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "+1 Discard Count (Should Be Minion)");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}

    // Check Trash Count Remained the Same
    assert_state = AssertTest((testState.trashedCardCount == state.trashedCardCount), "Trashed Card Count Unchanged");
    if(assert_state){flagFail = 1; printf("\tTrash Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state.trashedCardCount);}

    // IF FAILURE FLAG IS HIGH PRINT CARD PILES
    if(flagFail)
    {
        printf("--> FAILED TEST 01 *\n");
/*      DisplayHand(&testState, player1, "\tCurrent");
        DisplayDiscard(&testState, player1, "\tCurrent");
        DisplayDeck(&testState, player1, "\tCurrent"); */
    }

   /* -- TEST 2: choice2 & player2 > 4 cards -- */
    printf("----- TEST 2: choice2 & player2 Hand Count > 4 cards -----\n");
    
    // Set-up
    ResetGame(&state, num_players);

    state.hand[player1][0] = minion;
    SetUpHand(&state, player2, 5);
    DisplayHand(&state, player1, "Player1");
    DisplayHand(&state, player2, "Player2");

    memcpy(&testState, &state, sizeof(struct gameState));
    handPos = 0;
    choice1 = 0;
    choice2 = 1;
    bonus = 0;
    flagFail = 0;

    minion_return = minionCard(handPos, player1, choice1, choice2, &testState, &bonus);
    printf("Minion Value Returned: %d\n", minion_return);

    // Check numActions has increased by +1 
    assert_state = AssertTest((testState.numActions == state.numActions + 1), "+1 Action");
    if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions +1); }

    // Check Bonus Number Remained the Same
    assert_state = AssertTest((bonus == bonus_start), "0 Bonus");
    if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start);}

    // Check Trash Count Remained the Same
    assert_state = AssertTest((testState.trashedCardCount == state.trashedCardCount), "Trashed Card Count Unchanged");
    if(assert_state){flagFail = 1; printf("\tTrash Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state.trashedCardCount);}
 
    // Player 1 Hand Count = 4.
    assert_state = AssertTest((testState.handCount[player1] == 4), "Current Hand Count = 4");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], 4);}

    // Player 1 Discard Count = Original handCount .
    assert_state = AssertTest((testState.discardCount[player1] == state.handCount[player1]), "Discard Count == Original Hand Count");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.handCount[player1]);}

    // Player 2 Hand Count = 4.
    assert_state = AssertTest((testState.handCount[player2] == 4), "Player 2 Current Hand Count = 4");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player2], 4);}

    // Player 2 Discard Count  = Original handCount.
    assert_state = AssertTest((testState.discardCount[player2] == state.handCount[player2]), "Player 2 Discard Count == Original Hand Count");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.handCount[player2]);}


    // IF FAILURE FLAG IS HIGH PRINT CARD PILES
    if(flagFail)
    {
        printf("--> FAILED TEST 02 *\n");
/*      DisplayHand(&testState, player1, "\tCurrent");
        DisplayDiscard(&testState, player1, "\tCurrent");
        DisplayDeck(&testState, player1, "\tCurrent"); */
    }

/* -- TEST 3: choice2 & player2 Hand Count == 4 cards -- */
    printf("----- TEST 3: choice2 & player2 Hand Count == 4 cards -----\n");
    
    // Set-up
    ResetGame(&state, num_players);

    state.hand[player1][0] = minion;
    SetUpHand(&state, player2, 4);
    DisplayHand(&state, player1, "Player1");
    DisplayHand(&state, player2, "Player2");

    memcpy(&testState, &state, sizeof(struct gameState));
    handPos = 0;
    choice1 = 0;
    choice2 = 1;
    bonus = 0;
    flagFail = 0;

    minion_return = minionCard(handPos, player1, choice1, choice2, &testState, &bonus);
    printf("Minion Value Returned: %d\n", minion_return);

    // Check numActions has increased by +1 
    assert_state = AssertTest((testState.numActions == state.numActions + 1), "+1 Action");
    if(assert_state){flagFail = 1; printf("\tNumber of Actions: Current = %d vs. Expected = %d\n", testState.numActions, state.numActions +1); }

    // Check Bonus Number Remained the Same
    assert_state = AssertTest((bonus == bonus_start), "0 Bonus");
    if(assert_state) {flagFail = 1; printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start);}

    // Check Trash Count Remained the Same
    assert_state = AssertTest((testState.trashedCardCount == state.trashedCardCount), "Trashed Card Count Unchanged");
    if(assert_state){flagFail = 1; printf("\tTrash Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state.trashedCardCount);}
 
    // Player 1 Hand Count = 4.
    assert_state = AssertTest((testState.handCount[player1] == 4), "Current Hand Count = 4");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], 4);}

    // Player 1 Discard Count = Original handCount .
    assert_state = AssertTest((testState.discardCount[player1] == state.handCount[player1]), "Discard Count == Original Hand Count");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.handCount[player1]);}

    // Player 2 Hand Count = 4.
    assert_state = AssertTest((testState.handCount[player2] == 4), "Player 2 Current Hand Count = 4");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player2], 4);}

    // Player 2 Discard Count Unchanged.
    assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]), "Player 2 Discard Count == Original Discard Count");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]);}

    // IF FAILURE FLAG IS HIGH PRINT CARD PILES
    if(flagFail)
    {
        printf("--> FAILED TEST 03 *\n");
/*      DisplayHand(&testState, player1, "\tCurrent");
        DisplayDiscard(&testState, player1, "\tCurrent");
        DisplayDeck(&testState, player1, "\tCurrent"); */
    }

/* -- TEST 4: choice2 & player2 Hand Count == 4 cards -- */
    printf("----- TEST 4: choice2 & player2 Hand Count < 4 cards -----\n");
    
    // Set-up
    ResetGame(&state, num_players);

    state.hand[player1][0] = minion;
    SetUpHand(&state, player2, 3);
    DisplayHand(&state, player1, "Player1");
    DisplayHand(&state, player2, "Player2");

    memcpy(&testState, &state, sizeof(struct gameState));
    handPos = 0;
    choice1 = 0;
    choice2 = 1;
    bonus = 0;
    flagFail = 0;

    minion_return = minionCard(handPos, player1, choice1, choice2, &testState, &bonus);
    printf("Minion Value Returned: %d\n", minion_return);





/* -- TEST 5: choice2 & player1 Hand Count < 4 cards and player2 Hand Count > 4 cards -- */


  return 0;
}





