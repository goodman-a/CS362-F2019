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


int main(int argc, char** argv){

    /* -- Constant Parameters -- */
    int player1 = 0;



    /* -- Variables for Comparison, Checks, and Verifications -- */

    // General Variables
    int i;

    // Flags


	// Kingdom Cards
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};


    // Declare Game State
    struct gameState state;

    // Declare Arrays




    /* -- TESTING -- */














    return 1;
}