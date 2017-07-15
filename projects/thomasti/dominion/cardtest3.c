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
    struct gameState G;

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
    
    // Test that 1 card is drawn (so handcount should be same as initial since village gets discarded)
    int expected = G.handCount[player];
    play_village(player, &G, 0);
    int got = G.handCount[player];
    if (got == expected) printf("play_village() test 1 drawn, 1 discarded PASS\n");
    else printf("play_village() test 1 drawn, 1 discarded **FAIL**. Exepected handCount=%d, got handCount=%d\n", expected, got);
    
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
