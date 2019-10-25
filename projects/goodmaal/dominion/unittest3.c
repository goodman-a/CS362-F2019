/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 3 Learn How to Create Unit Tests
 * Author: Alexander Goodman
 * Due Date: 10 November 2019
 * 
 * File: unittesto3.c
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








/* -- MAIN FUNCTION -- */
int main(int argc, char** argv){
  /* -- Variables for Comparison, Checks, and Vericiations -- */

  // Passed in gameplay values
  int handPos = 0, choice1 = 0, choice2 = 0, bonus = 0, bonus_start = 0;

  // Set-up/Initialize Parameters
  int player1 = 0;
  int player2 = 1;
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
    // Note: All Tests Need to Check for +1 Actions & Discard/Trash Counts (BUG 01 HERE)
    // 1. choice1 & +2 Buys 
    // 2. choice2 & player2 hand count > 4 cards (BUG 02 HERE)
    // 3. choice2 & player2 hand count == 4 cards (BUG 02 HERE)
    // 4. choice2 & player2 hand count < 4 cards
    // 5. choice2 & player1 hand count < 4 & player2 hand cout > 4
    
    /* -- Bugs Introduced -- */
    // 1. discardCard trash bit high (1) instead of low (0). Trashing Card instead of Discarding
    // 2. state->handCount[i] >= 4 instead of state->handCount[i] > 4. 
    // note: other bugs may be present, but some have been fixed during refactoring

    printf("\n********** AMBASSADoR CARD TEST **********\n");

    /* -- TEST 1: choice1 = +2 bonus -- */
    printf("----- TEST 1: choice1 = +2 bonus -----\n");
    // handPos = 0;
    // state.hand[player1][handPos] = ambassador;

  return 0;
}






