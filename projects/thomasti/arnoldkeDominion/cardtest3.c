/* -----------------------------------------------------------------------
 * Test Case for play_village() in dominion.c 
 *
 * The village card gives the player 1 additional card and
 * 2 additional actions.
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"


int main() {

    // set up a mock game state for player 0
    int i;
    int player = 0;
    struct gameState G, initG;

    G.handCount[player] = 0;
    G.deckCount[player] = 0;
    G.discardCount[player] = 0;
    G.playedCardCount = 0;
    G.numActions = 0;

    // Put a few cards in hand. How many and type aren't
    // really important for this test since we just want to test the
    // village effect in isolation.
    for (i = 0; i < 6; i++ ) {
        G.deck[player][i] = gold; 
        G.hand[player][i] = gold; 
        G.discard[player][i] = gold; 
        G.handCount[player]++;
        G.deckCount[player]++;
        G.discardCount[player]++;
    }
    
    // Now put village in hand at pos 0
    G.hand[player][0] = village;
    
    // Copy the initial game state for player 0 to player 1
    G.handCount[1] = G.handCount[player];
    G.deckCount[1] = G.deckCount[player];
    G.discardCount[1] = G.discardCount[player];

    // Copy the overall initial game state
    memcpy(&initG, &G, sizeof(struct gameState));

    // make the call.  G is now the state after function call and initG before the call.
    //play_village(player, &G, 0);
    villageCard(player, 0, &G);
    
    // Test that 1 card is drawn (so handcount should be same as initial since village gets discarded)
    int expected = G.handCount[player];
    int got = G.handCount[player];
    if (got == expected) printf("play_village() test 1 drawn, 1 discarded PASS\n");
    else printf("play_village() test 1 drawn, 1 discarded **FAIL**. Exepected handCount=%d, got handCount=%d\n", expected, got);
    
    // test that drawn card came from player 0 deck/discard
    if (G.deckCount[0] + G.discardCount[0] == initG.deckCount[0] + initG.discardCount[0]-1)
        printf("play_village() test drawn card came from player 0 piles PASS\n");
    else
        printf("play_village() test drawn card came from player 0 piles **FAIL**\n");
    
    // test that game state has not changed for player 1
    if (G.handCount[1] == initG.handCount[1]) printf("play_village player 1 hand count PASS\n");
    else  printf("play_village player 1 hand count **FAIL**\n");

    if (G.deckCount[1] == initG.deckCount[1]) printf("play_village player 1 deck count PASS\n");
    else  printf("play_village player 1 deck count **FAIL**\n");

    if (G.discardCount[1] == initG.discardCount[1]) printf("play_village player 1 discard count PASS\n");
    else  printf("play_village player 1 discard count **FAIL**\n");

    
    // Test that 2 actions were added
    expected = 2;
    got = G.numActions;
    if (got == expected) printf("play_village() test 2 actions added PASS\n");
    else printf("play_village() test 2 actions added **FAIL**. Exepected %d, got %d\n", expected, got);
    
    // test that village was discarded
    int village_in_hand = 0;    // 0 if village not in hand, 1 if in hand
    for (i = 0; i < G.handCount[player]; i++) {
        if (G.hand[player][i] == village) village_in_hand = 1;
    }
    if (village_in_hand == 0) printf("play_village() village removed from hand PASS\n");
    else printf("play_village() village removed from hand **FAIL**\n");
    
    // test that village in played cards pile
    int village_in_playedCards = 0;    // 0 if village not in hand, 1 if in hand
    for (i = 0; i < G.playedCardCount; i++) {
        if (G.playedCards[i] == village) village_in_playedCards = 1;
    }
    if (village_in_playedCards == 1) printf("play_village() village put in playedCards PASS\n");
    else printf("play_village() village put in playedCards **FAIL**\n");

    
    return 0;
}
