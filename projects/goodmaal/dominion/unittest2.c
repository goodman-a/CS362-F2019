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
    - Check that Current Hand was Discarded (discardCount should be handCount + 1)
    - Check that New handCount == 4

  // TEST 03 - choice2 & Another Player Has > 4 Cards (BUG 02 HERE)
    - Check that Current Hand was Discarded (discardCount should be handCount + 1)
    - Check that New handCount == 4
    - Check that player2 handCount == 4
    - Check that player2 hand is different?

  // The Tests should check player card totals of 3, 4, & 5. These are the border cases


*/




int main(int argc, char** argv){





    return 0;
}





