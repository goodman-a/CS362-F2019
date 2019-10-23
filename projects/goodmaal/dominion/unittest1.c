/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 3 Learn How to Create Unit Tests
 * Author: Alexander Goodman
 * Due Date: 10 November 2019
 * 
 * File: unittest1.c
 * 
 * File Description: Baron Card ...
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

/* baronCard Function General info*/
/*
    // int baronCard(int handPos, int choice1, int currentPlayer, struct gameState* state, int* bonus)

    // First Check if numBuys has increased by 1

    // if choice > 0
        // check for estate card in hand
        //if found estate card
            // increase bonus
            // discard card
            // discard count +1
            // hand count -1
        // else
            // gain an estate card to the discard card .
                //BUG 01 HERE! --> check to make sure the discard pile increases and not the hand pile
                // decrement supply and check supply count
                // check to make sure estate count is not 0 --> run a test case where it does go to zero
    // else
        // Gain Estate Card
            // check supply count
            // check discard pile count
            // BUG 02 HERE! --> check for a game-over is not commented out, so the estate count can go to zero and the game will continue
    
    // discardCard not using right flag
        // check the counts here as well. 

*/




int main(int argc, char** argv){

    /* -- Variables for Comparison, Checks, and Verifications -- */

    // iterators
    int i, r, m;

    // Total Counts
    //int count_hand, count_discard, count_deck, count_buys;

    // Passed in gameplay values
    int handPos = 0, choice1 = 0, bonus = 0, bonus_start = 0;

    // Setup/Initialize Parameters
    int seed = 1000;
    int player1 = 0;
    int num_players = 2;
    int num_hand = 5;

    // Store return baron value ... kinda worthless here..
    int baron_return, assert_state;
    //int failFlag = 0; 

	// Kingdom Cards
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

    // Declare Game State
    struct gameState state, testState;

    // Declare Arrays


	// Reset Game for a Controlled Test
	void ResetGame(struct gameState *state, int num_hand)
	{	
		// Initialize the game to populate decks.
		memset(state, 0, sizeof(struct gameState));   // clear the game state
		r = initializeGame(num_players, k, seed, state); // initialize a new game

        if (r<0){printf("ERROR DURING INITIALIZATION\n");}

        // Display Original Starting Hand
	    printf("Initialized Hand: ");
        for (m=0; m<state->handCount[player1]; m++) {
        printf("(%d)", state->hand[player1][m]);
        }
        printf("; \n");

		// Set-up Hand
        // WILL NEED TO DECREMENT SUPPLY COUNT TOO ... ugh and add to hand? hmm
		state->handCount[player1] = num_hand;
		state->hand[player1][0] = curse;
		state->hand[player1][1] = copper;
		state->hand[player1][2] = silver;
		state->hand[player1][3] = baron;
		state->hand[player1][4] = estate; //estate
	}


    /* -- TESTING -- */
    //printf("\n---------- BARON CARD TEST ----------\n");
    ResetGame(&state, num_hand);
    memcpy(&testState, &state, sizeof(struct gameState));


    void DisplayHand(struct gameState *state, char* msg)
    {
        printf("%s Hand: ", msg);
        for (m=0; m<state->handCount[player1]; m++) {
        printf("(%d)", state->hand[player1][m]);
        }
    }

    printf("Current Hand: ");
    for (m=0; m<testState.handCount[player1]; m++) {
        printf("(%d)", testState.hand[player1][m]);
    }
    printf("; \n");

/*     printf("Before - Number of Buys = %d\n", state.numBuys);    
    printf("Before - hand count = %d\n", state.handCount[player1]);
    printf("Before - deck count = %d\n", state.deckCount[player1]);
    printf("Before - discard count = %d\n", state.discardCount[player1]);
    printf("Before - supply count = %d\n", supplyCount(estate, &state)); 
    printf("Before - bonus = %d\n", bonus);
 */
    //baron_return = baronCard(handPos, choice1, player1, &testState, &bonus);


/*     printf("After - Number of Buys = %d\n", state.numBuys);    
    printf("After - hand count = %d\n", state.handCount[player1]);
    printf("After - deck count = %d\n", state.deckCount[player1]);
    printf("After - discard count = %d\n", state.discardCount[player1]);
    printf("After - supply count = %d\n", supplyCount(estate, &state)); 
    printf("After - bonus = %d\n", bonus);

    printf("Hand Count = %d, Original = %d\n", testState.handCount[player1], state.handCount[player1]);
    printf("Deck Count = %d, Original = %d\n", testState.deckCount[player1], state.deckCount[player1]);
    printf("Discard Count = %d, Original = %d\n", testState.discardCount[player1], state.discardCount[player1]);
    //need a bonus check ....

    printf("Supply Count = %d, Original = %d\n", supplyCount(estate, &testState), supplyCount(estate, &state));    

*/


    // Go Through Differnt Tests and Compare Current (testState) vs Expected (state +/- some value)

    /* -- Test Overview -- */
    // 1. choice1 > 0 & 1 Estate Card in Hand  (if-Statement)
    // 2. choice1 > 0 & 0 Estate Cards in Hand (else if-statement)
    // 3. choice1 > 0 & Multiple Estate Cards in Hand
    // 4. choice1 < 0 & 0 Estate Cards in Hand

    // NEED TO CHECK SUPPLY COUNT GOES DOWN FOR ESTATE WHEN DRAWING...

    // 4. Estate Card Supply Goes to Zero for choice1 > 0
    // 5. Estate Card Supply Goes to Zero for choice1 <=0

    // Introduced Bugs
        // 1. gainCard flag to 2 from 0 (hand count will increase and discard will not..)
        // 2. gainCard in else statement: receives copper instead of estate

  
    /* __________ TESTING TIME __________ */

    /* -- TEST 1: choice1 > 0 & 1 Estate Card in Hand -- */
    printf("----- TEST 1: choice1 > 0 & 1 Estate Card in Hand -----\n");
    // Set-up
    handPos = 3;
    choice1 = 1;
    bonus = 0;

    baron_return = baronCard(handPos, choice1, player1, &testState, &bonus);
    printf("Baron Value Returned: %d\n", baron_return);

    // Check numBuys has increased by 1 
    assert_state = AssertTest((testState.numBuys == state.numBuys + 1), "+1 Number of Buys");
    if(assert_state){printf("\tNumber of Buys: Current = %d vs. Expected = %d\n", testState.numBuys, state.numBuys +1); }

    // Check Bonus Number has increased to +4
    assert_state = AssertTest((bonus == bonus_start +4 ), "+4 Bonus");
    if(assert_state) {printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start+4);}

    // Check Hand Count has decreased by 2
    assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-2), "-2 Hand Count (Estate & Baron)");
    if(assert_state){printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-2);}

    // Check Discount Count has increased by 2
    assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+2), "+2 Discard Count (Estate & Baron)");
    if(assert_state){printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+2);}


     /* -- TEST 2: choice1 > 0 & 0 Estate Cards in Hand -- */
    // Note: Bug 01 is located in here
    printf("----- TEST 2: choice1 > 0 & 0 Estate Card in Hand -----\n");
    // Set-up
    ResetGame(&state, num_hand);
	state.hand[player1][4] = gold; // No Longer contains the Estate Card.
    memcpy(&testState, &state, sizeof(struct gameState));
    handPos = 3;
    choice1 = 1;
    bonus = 0;

    baron_return = baronCard(handPos, choice1, player1, &testState, &bonus);
    printf("Baron Value Returned: %d\n", baron_return);

    // Check numBuys has increased by 1 
    assert_state = AssertTest((testState.numBuys == state.numBuys + 1), "+1 Number of Buys");
    if(assert_state){printf("\tNumber of Buys: Current = %d vs. Expected = %d\n", testState.numBuys, state.numBuys +1); }

    // Check Bonus Number has NOT increased
    assert_state = AssertTest((bonus == bonus_start), "No Bonus");
    if(assert_state) {printf("\tBonus Count: Current = %d vs. Exepected = %d\n", bonus, bonus_start);}

    // Check Hand Count has decreased by 1
    assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "-1 Hand Count (Baron)");
    if(assert_state){printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

    // Check Discard Count has increased by 2
    assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+2), "+2 Discard Count (Estate & Baron)");
    if(assert_state){printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+2);}

    // Check Estate Supply has decreased by 1
    assert_state = AssertTest((supplyCount(estate, &testState) == supplyCount(estate, &state)-1), "-1 Estate Supply Count");
    if(assert_state){printf("\tSupply Count: Current= %d, Expected = %d\n", supplyCount(estate, &testState), supplyCount(estate, &state)-1);}    

    // Check to Make Sure No Estates in Hand
















    return 0;
}
