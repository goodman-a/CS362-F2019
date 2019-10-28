/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 3 Learn How to Create Unit Tests
 * Author: Alexander Goodman
 * Due Date: 10 November 2019
 * 
 * File: unittest3.c
 * 
 * File Description: Ambassador Card ...
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

/* ambassadorCard Function General info*/

/*
int ambassadorCard(int handPos, int currentPlayer, int choice1, int choice2, struct gameState* state){

    // check for return -1 or 0 with the ambassador_return parameter

    // check choice2 is in the range of [0-2]
    // check choice1 is not handpos (handpos == ambassador card)

    //Check Supply Count has increased by choice2

    //Check Opponents Hand Count has Increased by 1 (depending if enough cards to go around) 
    // Check Opponents Hand Now Contain That Specific Card

    // check supply count has changed by: SupplyCount = (SupplyCount + choice2 - numPlayers) while SupplyCount >=0 if supplycount+choice2-numplayers < 0 then supplycount = 0

    //Hand Count decreases by choice2 + 1 (choice1)
    //Discard Count increases by +1 (choice1)
    //Trash Count increases by choice2


    // NEED A SCENARIO FOR: number of players is greater than number of supply cards left. Nothing breaks, but should just not populate those players hands)



*/



/* -- Constants -- */
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

/* -- MAIN FUNCTION -- */
int main(int argc, char** argv){
  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // Passed in gameplay values
  int handPos = 0, choice1 = 0, choice2 = 0;

  // Set-up/Initialize Parameters
  int player1 = 0;
  int player2 = 1;
  int player3 = 2;
  int num_players = 2;

  //int handNum_player1 = 5;
  //int handNum_player2 = 5;

  // Store return baron value ... kinda worthless here..
  int ambassador_return, assert_state;
  int flagFail = 0; 

  // Declare Game States
  struct gameState state, testState;

    /* __________ TESTING TIME __________ */

    /* -- Test Overview -- */
    // Note: All Tests Need to Check for -1 return from ambassador_return meaning illegal/invalid move
    // 1. choice2 is less than 0
    // 2. choice2 is greater than 2
    // 3. choice1 is equal to handpos (should be:handPos == ambassador card)
    // 4. choice2 is > than number of selected card type in hand (BUG 01 HERE)
    // 5. valid choice1 & choice2 w/ playerCount < supplyCount
    // 6. valid choice1 & choice 2 w/ playerCount > supplyCount

    /* -- Bugs Introduced -- */
    // Note: other bugs may be present, but some have been fixed during refactoring
    // 1. changed && to || in the for-loop that verifies the hand contains at least the number(choice2) of choice1
    // 2. commented out code that increases the supply count by choice2 for choice1 

    printf("\n********** AMBASSADOR CARD TEST **********\n");

    /* -- TEST 1: choice2 < 0  -- */
    printf("----- TEST 1: choice2 < 0 (Illegal/Invalid Move)-----\n");
    // Set-Up
    num_players = 2;
    handPos = 0;
    choice1 = 1;
    choice2 = -1;
    flagFail = 0;

    ResetGame(&state, num_players);
    SetUpHand(&state, player2, 4);
    state.hand[player1][handPos] = ambassador;
    // DisplayHand(&state, player1, "Player1");
    // DisplayHand(&state, player2, "Player2");

    memcpy(&testState, &state, sizeof(struct gameState));

    ambassador_return = ambassadorCard(handPos,player1, choice1, choice2, &testState);

    // Check for -1 return when input is an illegal/invalid move (choice2 < 0)
    assert_state = AssertTest((ambassador_return == -1), "choice2 < 0");
    if(assert_state){flagFail = 1; printf("\tInvalid/Illegal input for choice2 not caught."); }


    /* -- TEST 2: choice2 > 2  -- */
    printf("----- TEST 2: choice2 > 2 (Illegal/Invalid Move)-----\n");
    // Set-Up
    num_players = 2;
    handPos = 0;
    choice1 = 1;
    choice2 = 3;
    flagFail = 0;

    ResetGame(&state, num_players);
    SetUpHand(&state, player2, 4);
    state.hand[player1][handPos] = ambassador;
    // DisplayHand(&state, player1, "Player1");
    // DisplayHand(&state, player2, "Player2");

    memcpy(&testState, &state, sizeof(struct gameState));

    ambassador_return = ambassadorCard(handPos,player1, choice1, choice2, &testState);
  
    // Check for -1 return when input is an illegal/invalid move (choice2 > 2)
    assert_state = AssertTest((ambassador_return == -1), "choice2 > 2");
    if(assert_state){flagFail = 1; printf("\tInvalid/Illegal input for choice2 not caught."); }


    /* -- TEST 3: handpos == choice1  -- */
    printf("----- TEST 3: choice1 == handpos (Illegal/Invalid Move)-----\n");
    // Set-Up
    num_players = 2;
    handPos = 0;
    choice1 = 0;
    choice2 = 1;
    flagFail = 0;

    ResetGame(&state, num_players);
    SetUpHand(&state, player2, 4);
    state.hand[player1][handPos] = ambassador;
    // DisplayHand(&state, player1, "Player1");
    // DisplayHand(&state, player2, "Player2");

    memcpy(&testState, &state, sizeof(struct gameState));

    ambassador_return = ambassadorCard(handPos,player1, choice1, choice2, &testState);

    // Check for -1 return when input is an illegal/invalid move (choice1 == handPos)
    assert_state = AssertTest((ambassador_return == -1), "choice1 == handPos");
    if(assert_state){flagFail = 1; printf("\tInvalid/Illegal input for choice1 not caught."); }

/* -- TEST 4: choice2 > choice1 count in hand (BUG HERE)  -- */
    printf("----- TEST 4: choice2 > choice1 count in hand-----\n");
    // Set-Up
    num_players = 2;
    handPos = 0;
    choice1 = 3;
    choice2 = 2;
    flagFail = 0;

    ResetGame(&state, num_players);
    SetUpHand(&state, player2, 4);
    state.hand[player1][handPos] = ambassador;
    state.hand[player1][handPos+1] = silver;  // make only 1 silver in hand, leaving 2 coppers

    memcpy(&testState, &state, sizeof(struct gameState));
    DisplayHand(&testState, player1, "Player1");
    DisplayHand(&testState, player2, "Player2");


    ambassador_return = ambassadorCard(handPos,player1, choice1, choice2, &testState);

    // Check for -1 return when input is an illegal/invalid move (choice2 > choice1 count in hand)
    assert_state = AssertTest((ambassador_return == -1), "choice2 > choice1 count in hand");
    if(assert_state){flagFail = 1; printf("\tInvalid/Illegal input for choice2 not caught.\n"); 
        DisplayHand(&testState, player1, "\tPlayer1");
        printf("choice1 in hand: %d vs. choice2: %d\n", HandCardCount(&testState, player1, copper), choice2);
    }

    // Player 1 Hand Count Unchanged.
    assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]), "Player 1 Hand Count Unchanged");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]);}

    // Player 2 Discard Count Unchanged.
    assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]), "Player 2 Discard Count Unchanged");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]);}

    // check supply count is unchanged

    // check trash count is unchanged

    //DisplayHand(&testState, player1, "Player1");
    //DisplayHand(&testState, player2, "Player2");
    //printf("choice1 in hand: %d vs. choice2: %d\n", HandCardCount(&testState, player1, copper), choice2);



/* -- TEST 5: valid choice1 and choice2 w/ playerCount < supplyCount -- */
    printf("----- TEST 5: valid choice1 & choice2 w/ playerCount < supplyCount -----\n");
    // Set-Up
    num_players = 2;
    handPos = 1;
    choice1 = 3;
    choice2 = 2;
    flagFail = 0;

    ResetGame(&state, num_players);
    SetUpHand(&state, player2, 4);
    state.hand[player1][handPos] = ambassador;
    //state.hand[player1][handPos+1] = silver;  // make only 1 silver in hand, leaving 2 coppers

    memcpy(&testState, &state, sizeof(struct gameState));
    DisplayHand(&testState, player1, "Player1");
    DisplayHand(&testState, player2, "Player2");


    ambassador_return = ambassadorCard(handPos,player1, choice1, choice2, &testState);


    // Player 1 Hand Count -3
    assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-3), "Player 1 Hand Count -3");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-3);}

    // Player 1 Hand Count -choice2 Copper
    assert_state = AssertTest((HandCardCount(&testState, player1, copper) == HandCardCount(&state, player1, copper)-choice2), "Player 1 Copper Hand Count: -2 choice2");
    if(assert_state){flagFail = 1; printf("\tCopper Hand Count: Current = %d, Expected = %d\n", HandCardCount(&testState, player1, copper), HandCardCount(&state, player1, copper)-choice2);}

    // Player 2 Discard Count +1. (gains copy of choice1)
    assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]+1), "Player 2 Discard Count Gain +1 Card");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]+1);}

    // Check that Trash Count +choice2
    assert_state = AssertTest((testState.trashedCardCount == state.trashedCardCount+choice2), "Trashed Card Count +choice2");
    if(assert_state){flagFail = 1; printf("\tTrash Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state.trashedCardCount+choice2);}

    // Check choice1 supply = 
    assert_state = AssertTest((supplyCount(copper, &testState) == supplyCount(copper, &state)+choice2-(num_players-1)) ||(supplyCount(copper, &testState) == 0), "Copper Supply Count Has Changed");
    if(assert_state){flagFail = 1; printf("\t Copper Supply Count: Current= %d, Expected = %d\n", supplyCount(copper, &testState), supplyCount(copper, &state)+choice2-(num_players-1));}    

    DisplayHand(&state, player1, "Player1 (Starting)");
    DisplayHand(&testState, player1, "Player1 (Ending)");


    /* -- TEST 6: valid choice1 & choice2 w/ playerCount > supplyCount -- */
    printf("----- TEST 6: valid choice1 & choice2 w/ playerCount > supplyCount -----\n");
    // Set-Up
    num_players = 3;
    handPos = 1;
    choice1 = 0;
    choice2 = 0;
    flagFail = 0;

    ResetGame(&state, num_players);
    SetUpHand(&state, player2, 4);
    SetUpHand(&state, player3, 4);
    state.hand[player1][handPos] = ambassador;
    state.hand[player1][0] = curse; // make only 1 curse in hand
    state.supplyCount[curse] = 1;   // Set supply to 1 leaving not enough for all opponents.

    memcpy(&testState, &state, sizeof(struct gameState));
    //DisplayHand(&testState, player1, "Player1");
    //DisplayHand(&testState, player2, "Player2");
    //DisplayHand(&testState, player3, "Player3");

    ambassador_return = ambassadorCard(handPos,player1, choice1, choice2, &testState);


    // Player 1 Hand Count -1
    assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Player 1 Hand Count -1");
    if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

    // Player 1 Hand Count -choice2 Copper
/*     assert_state = AssertTest((HandCardCount(&testState, player1, copper) == HandCardCount(&state, player1, copper)-choice2), "Player 1 Copper Hand Count: -2 choice2");
    if(assert_state){flagFail = 1; printf("\tCopper Hand Count: Current = %d, Expected = %d\n", HandCardCount(&testState, player1, copper), HandCardCount(&state, player1, copper)-choice2);} */

    // Player 2 Discard Count +1. (gains copy of choice1)
    assert_state = AssertTest((testState.discardCount[player2] == state.discardCount[player2]+1), "Player 2 Discard Count Gain +1 Card");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player2], state.discardCount[player2]+1);}

    // Player 3 Discard Count Unchanged because choice1 supply ran out.
    assert_state = AssertTest((testState.discardCount[player3] == state.discardCount[player3]), "Player 3 Discard Count Unchanged");
    if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player3], state.discardCount[player3]);}

    // Check that Trash Count +choice2
    assert_state = AssertTest((testState.trashedCardCount == state.trashedCardCount+choice2), "Trashed Card Count +choice2");
    if(assert_state){flagFail = 1; printf("\tTrash Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state.trashedCardCount+choice2);}

    // Check curse supply equals 0 
    assert_state = AssertTest((supplyCount(curse, &testState) == supplyCount(curse, &state)+choice2-(num_players-1)) ||(supplyCount(curse, &testState) == 0), "Curse Supply Count Has Changed");
    if(assert_state){flagFail = 1; printf("\t Curse Supply Count: Current= %d, Expected = %d\n", supplyCount(curse, &testState), 0);}    

    DisplayHand(&state, player1, "Player1 (Starting)");
    DisplayHand(&testState, player1, "Player1 (Ending)");







    /* FAILURE DETECTED - PRINT OUT HAND PILES */
/*     if(flagFail)
    {
        printf("---> *FAILURE DETECTED* <---\n");
        DisplayHand(&state, player1, "Player1 (Starting)");
        DisplayHand(&testState, player1, "Player1 (Ending)");
        DisplayHand(&state, player2, "Player2 (Starting)");
        DisplayHand(&testState, player2, "Player2 (Ending)");
    } */


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
