/* 
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Assignment 3 Learn How to Create Unit Tests
 * Author: Alexander Goodman
 * Due Date: 10 November 2019
 * 
 * File: unittest5.c
 * 
 * File Description: Unit Test for the Mine Card Function:
 *   mineCard(int handPos, int currentPlayer, int choice1, int choice2, struct gameState* state);
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
  int handPos = 0, choice1 = 0, choice2 = 0;

  // Set-up/Initialize Parameters
  int player1 = 0;
  //int player2 = 1;
  int num_players = 2;

  // Store return baron value ... kinda worthless here..
  int mine_return, assert_state;
  int flagFail = 0; 

  // Declare Game States
  struct gameState state, testState;

    /* __________ TESTING TIME __________ */

  /* -- Test Overview -- */
  /* -- 
    1. choice1 < copper 
    2. choice1 > gold 
    3. choice1 = curse (edge case)
    4. choice1 = treasure_map (edge case)
    5. choice2 < copper
    6. choice2 > gold
    7. choice2 = curse (edge case)
    8. choice2 = treasure_map (edge case)
    9. choice1 = copper & choice2 = gold
    10. choice1 = copper & choice2 = silver
    11. choice1 = silver & choice2 = copper
    12. choice1 = copper & choice2 = copper
    13. Special Case: Hand has 2+ cards of choice1 (current bug will remove all the cards matching)

  -- */


  printf("\n********** MINE CARD TEST **********\n");

  /* -- TEST 1:choice1 < copper -- */
  printf("----- TEST 1: choice1 < copper -----\n");

  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = silver;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = copper - 1;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == -1), "Mine Card Correctly Handled Illegal/Invalid Move");
  if(assert_state){flagFail = 1; printf("\tGAME VIOLATION - Mine Card Allowed Illegal/Invalid Move.\n"); }

  // choice2 SupplyCount +0
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)), "choice2 Supply Count +0");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state));}    

  /* -- TEST 2:choice1 > gold -- */
  printf("----- TEST 2: choice1 > gold -----\n");
  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = silver;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = gold + 1;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == -1), "Mine Card Correctly Handled Illegal/Invalid Move");
  if(assert_state){flagFail = 1; printf("\tGAME VIOLATION - Mine Card Allowed Illegal/Invalid Move.\n"); }

  // choice2 SupplyCount +0
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)), "choice2 Supply Count +0");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state));}    

  /* -- TEST 3: choice1 = curse (Edge Case) -- */
  printf("----- TEST 3: choice1 = curse (edge case)-----\n");
  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = silver;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = curse;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == -1), "Mine Card Correctly Handled Illegal/Invalid Move");
  if(assert_state){flagFail = 1; printf("\tGAME VIOLATION - Mine Card Allowed Illegal/Invalid Move.\n"); }

  // choice2 SupplyCount +0
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)), "choice2 Supply Count +0");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state));}      

  /* -- TEST 4: choice1 = treasure_map (Edge Case) -- */
  printf("----- TEST 4: choice1 = treasure_map (edge case)-----\n");
  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = silver;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = treasure_map;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == -1), "Mine Card Correctly Handled Illegal/Invalid Move");
  if(assert_state){flagFail = 1; printf("\tGAME VIOLATION - Mine Card Allowed Illegal/Invalid Move.\n"); }

  // choice2 SupplyCount +0
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)), "choice2 Supply Count +0");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state));}    

  /* -- TEST 5:choice2 < copper -- */
  printf("----- TEST 5: choice2 < copper -----\n");
// Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = copper - 1;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = copper;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == -1), "Mine Card Correctly Handled Illegal/Invalid Move");
  if(assert_state){flagFail = 1; printf("\tGAME VIOLATION - Mine Card Allowed Illegal/Invalid Move.\n"); }

  // choice2 SupplyCount +0
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)), "choice2 Supply Count +0");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state));}      

  /* -- TEST 6:choice2 > gold -- */
  printf("----- TEST 6: choice2 > gold -----\n");
  // Set-Up
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = gold + 1;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = copper;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == -1), "Mine Card Correctly Handled Illegal/Invalid Move");
  if(assert_state){flagFail = 1; printf("\tGAME VIOLATION - Mine Card Allowed Illegal/Invalid Move.\n"); }

  // choice2 SupplyCount +0
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)), "choice2 Supply Count +0");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state));}    

  /* -- TEST 7: choice2 = curse (Edge Case) -- */
  printf("----- TEST 7: choice2 = curse (edge case)-----\n");
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = curse;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = copper;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == -1), "Mine Card Correctly Handled Illegal/Invalid Move");
  if(assert_state){flagFail = 1; printf("\tGAME VIOLATION - Mine Card Allowed Illegal/Invalid Move.\n"); }

  // choice2 SupplyCount +0
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)), "choice2 Supply Count +0");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state));}    

  /* -- TEST 8: choice2 = treasure_map (Edge Case) -- */
  printf("----- TEST 8: choice2 = treasure_map (edge case)-----\n");
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = treasure_map;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = copper;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == -1), "Mine Card Correctly Handled Illegal/Invalid Move");
  if(assert_state){flagFail = 1; printf("\tGAME VIOLATION - Mine Card Allowed Illegal/Invalid Move.\n"); }

  // choice2 SupplyCount +0
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)), "choice2 Supply Count +0");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state));}      

  /* -- TEST 9: choice1 = copper & choice2 = gold -- */
  printf("----- TEST 9: choice1 = copper & choice2 = gold)-----\n");
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = gold;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = copper;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == -1), "Mine Card Correctly Handled Illegal/Invalid Move");
  if(assert_state){flagFail = 1; printf("\tGAME VIOLATION - Mine Card Allowed Illegal/Invalid Move.\n"); }

  // Hand Count +0
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]), "Hand Count +0");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]);}

  // choice2 SupplyCount +0
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)), "choice2 Supply Count +0");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state));}    

  // choice1 Hand Count +0 
  assert_state = AssertTest((HandCardCount(&testState, player1, state.hand[player1][choice1]) == HandCardCount(&state, player1, state.hand[player1][choice1])), "choice1 Hand Count +0");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice1: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, state.hand[player1][choice1]), HandCardCount(&state, player1, state.hand[player1][choice1]));}

  // choice2 Hand Count +0. 
  assert_state = AssertTest((HandCardCount(&testState, player1, choice2) == HandCardCount(&state, player1, choice2)), "choice2 Hand Count +0");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice2: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, choice2), HandCardCount(&state, player1, choice2));}   

  /* -- TEST 10: choice1 = copper & choice2 = silver -- */
  printf("----- TEST 10: choice1 = copper & choice2 = silver)-----\n");
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = silver;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = copper;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == 0), "Mine Card Successfully Returned");
  if(assert_state){flagFail = 1; printf("\tERROR - Mine Card has Failed.\n"); }

  // -1 handCount --> choice2 - mine - choice1
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Hand Count -1");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // +1 discardCount --> mine
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Discard Count +1");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}

  // +0 deckCount
  assert_state = AssertTest((testState.deckCount[player1] == state.deckCount[player1]), "Deck Count +0");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player1], state.deckCount[player1]);}

  // +1 trashedCardCount --> choice1
  assert_state = AssertTest((testState.trashedCardCount == state.trashedCardCount+1), "Trashed Card Count +1");
  if(assert_state){flagFail = 1; printf("\tTrashed Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state.trashedCardCount+1);}

  // choice2 SupplyCount -1
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)-1), "choice2 Supply Count -1");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state)-1);}    

  // choice1 Hand Count -1 
  assert_state = AssertTest((HandCardCount(&testState, player1, state.hand[player1][choice1]) == HandCardCount(&state, player1, state.hand[player1][choice1])-1), "choice1 Hand Count -1");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice1: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, state.hand[player1][choice1]), HandCardCount(&state, player1, state.hand[player1][choice1])-1);}

  // choice2 Hand Count +1. 
  assert_state = AssertTest((HandCardCount(&testState, player1, choice2) == HandCardCount(&state, player1, choice2)+1), "choice2 Hand Count +1");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice2: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, choice2), HandCardCount(&state, player1, choice2)+1);}   


  /* -- TEST 11: choice1 = silver & choice2 = copper -- */
  printf("----- TEST 11: choice1 = silver & choice2 = copper)-----\n");
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = copper;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = silver;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == 0), "Mine Card Successfully Returned");
  if(assert_state){flagFail = 1; printf("\tERROR - Mine Card has Failed.\n"); }

  // -1 handCount --> choice2 - mine - choice1
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Hand Count -1");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // +1 discardCount --> mine
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Discard Count +1");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}

  // +0 deckCount
  assert_state = AssertTest((testState.deckCount[player1] == state.deckCount[player1]), "Deck Count +0");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player1], state.deckCount[player1]);}

  // +1 trashedCardCount --> choice1
  assert_state = AssertTest((testState.trashedCardCount == state.trashedCardCount+1), "Trashed Card Count +1");
  if(assert_state){flagFail = 1; printf("\tTrashed Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state.trashedCardCount+1);}

  // choice2 SupplyCount -1
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)-1), "choice2 Supply Count -1");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state)-1);}    

  // choice1 Hand Count -1 
  assert_state = AssertTest((HandCardCount(&testState, player1, state.hand[player1][choice1]) == HandCardCount(&state, player1, state.hand[player1][choice1])-1), "choice1 Hand Count -1");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice1: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, state.hand[player1][choice1]), HandCardCount(&state, player1, state.hand[player1][choice1])-1);}

  // choice2 Hand Count +1. 
  assert_state = AssertTest((HandCardCount(&testState, player1, choice2) == HandCardCount(&state, player1, choice2)+1), "choice2 Hand Count +1");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice2: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, choice2), HandCardCount(&state, player1, choice2)+1);}  


  /* -- TEST 12: choice1 = copper & choice2 = copper -- */
  printf("----- TEST 12: choice1 = copper & choice2 = copper)-----\n");
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = copper;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = copper;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == 0), "Mine Card Successfully Returned");
  if(assert_state){flagFail = 1; printf("\tERROR - Mine Card has Failed.\n"); }

  // -1 handCount --> choice2 - mine - choice1
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Hand Count -1");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // +1 discardCount --> mine
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Discard Count +1");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}

  // +0 deckCount
  assert_state = AssertTest((testState.deckCount[player1] == state.deckCount[player1]), "Deck Count +0");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player1], state.deckCount[player1]);}

  // +1 trashedCardCount --> choice1
  assert_state = AssertTest((testState.trashedCardCount == state.trashedCardCount+1), "Trashed Card Count +1");
  if(assert_state){flagFail = 1; printf("\tTrashed Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state.trashedCardCount+1);}

  // choice2 SupplyCount -1
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)-1), "choice2 Supply Count -1");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state)-1);}    

 
  // choice1 Hand Count +0
  assert_state = AssertTest((HandCardCount(&testState, player1, state.hand[player1][choice1]) == HandCardCount(&state, player1, state.hand[player1][choice1])), "choice1 Hand Count +0");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice1: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, state.hand[player1][choice1]), HandCardCount(&state, player1, state.hand[player1][choice1])-1);}

  // choice2 Hand Count +10. 
  assert_state = AssertTest((HandCardCount(&testState, player1, choice2) == HandCardCount(&state, player1, choice2)), "choice2 Hand Count +0");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice2: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, choice2), HandCardCount(&state, player1, choice2));} 


  /* -- TEST 13 Special Case: Hand has 2+ card of choice1 -- */
  printf("----- TEST 13 Special Case: Hand has 2+ cards of choice1 -----\n");
  num_players = 2;
  handPos = 0;
  flagFail = 0;
  choice1 = 1;  // hand position for choice 1.
  choice2 = silver;
  ResetGame(&state, num_players);

  // Set up Specific Test Parameters:
  state.hand[player1][handPos] = mine;
  state.hand[player1][choice1] = copper;

  // Copy state into testState to Run Tests
  memcpy(&testState, &state, sizeof(struct gameState));

  // Call the tributeCard Function & verify it handles the Illegal/Invalid Move Correctly
  mine_return = mineCard(handPos, player1, choice1, choice2, &testState);

  // Check for return value
  assert_state = AssertTest((mine_return == 0), "Mine Card Successfully Returned");
  if(assert_state){flagFail = 1; printf("\tERROR - Mine Card has Failed.\n"); }

  // -1 handCount --> choice2 - mine - choice1
  assert_state = AssertTest((testState.handCount[player1] == state.handCount[player1]-1), "Hand Count -1");
  if(assert_state){flagFail = 1; printf("\tHand Count: Current = %d, Expected = %d\n", testState.handCount[player1], state.handCount[player1]-1);}

  // +1 discardCount --> mine
  assert_state = AssertTest((testState.discardCount[player1] == state.discardCount[player1]+1), "Discard Count +1");
  if(assert_state){flagFail = 1; printf("\tDiscard Count: Current = %d, Expected = %d\n", testState.discardCount[player1], state.discardCount[player1]+1);}

  // +0 deckCount
  assert_state = AssertTest((testState.deckCount[player1] == state.deckCount[player1]), "Deck Count +0");
  if(assert_state){flagFail = 1; printf("\tDeck Count: Current = %d, Expected = %d\n", testState.deckCount[player1], state.deckCount[player1]);}

  // +1 trashedCardCount --> choice1
  assert_state = AssertTest((testState.trashedCardCount == state.trashedCardCount+1), "Trashed Card Count +1");
  if(assert_state){flagFail = 1; printf("\tTrashed Count: Current = %d, Expected = %d\n", testState.trashedCardCount, state.trashedCardCount+1);}

  // choice2 SupplyCount -1
  assert_state = AssertTest((supplyCount(choice2, &testState) == supplyCount(choice2, &state)-1), "choice2 Supply Count -1");
  if(assert_state){flagFail = 1; printf("\t choice2 Supply Count: Current= %d, Expected = %d\n", supplyCount(choice2, &testState), supplyCount(choice2, &state)-1);}    

  // choice1 Hand Count -1 
  assert_state = AssertTest((HandCardCount(&testState, player1, state.hand[player1][choice1]) == HandCardCount(&state, player1, state.hand[player1][choice1])-1), "choice1 Hand Count -1");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice1: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, state.hand[player1][choice1]), HandCardCount(&state, player1, state.hand[player1][choice1])-1);}

  // choice2 Hand Count +1. 
  assert_state = AssertTest((HandCardCount(&testState, player1, choice2) == HandCardCount(&state, player1, choice2)+1), "choice2 Hand Count +1");
  if(assert_state){flagFail = 1; printf("\tHand Count of choice2: Current = %d, Expected = %d\n",HandCardCount(&testState, player1, choice2), HandCardCount(&state, player1, choice2)+1);}


  // Display Beginning and Ending Hands
  if(flagFail)
  {
    DisplayHand(&state, player1, "Player1 (orig)");
    DisplayHand(&testState, player1, "Player1 (new)");
  }


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
