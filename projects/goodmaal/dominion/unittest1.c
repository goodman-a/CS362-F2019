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
        return 0;
    }
    else
    {
        // OR NOT SAY ANYTHING?
        printf("PASS: %s\n", msg);
        return 1;
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
            // discard count
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

    /* -- Constant Parameters -- */
    int player1 = 0;



    /* -- Variables for Comparison, Checks, and Verifications -- */

    // General Variables
    int i, r, count_hand, count_discard, count_deck;
    int handPos = 0, choice1 = 0, bonus = 0;
    int seed = 1000;
    int num_players = 2;

    // Flags


	// Kingdom Cards
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

    // Declare Game State
    struct gameState state;

    // Declare Arrays


	// Reset Game for a Controlled Test
	void ResetGame(struct gameState *state)
	{	
		// Initialize the game to populate decks.
		memset(&state, 0, sizeof(struct gameState));   // clear the game state
		r = initializeGame(num_players, k, seed, &state); // initialize a new game
		
		// Set-up Hand
/* 		state.handCount[player1] = hand_count;
		state.hand[player1][0] = curse;
		state.hand[player1][1] = copper;
		state.hand[player1[2] = silver;
		state.hand[player1][3] = gold;
		state.hand[player1][4] = estate;
		state.hand[player1][5] = treasure_map; */
	}


    /* -- TESTING -- */
    //printf("\n---------- BARON CARD TEST ----------\n");













    return 0;
}