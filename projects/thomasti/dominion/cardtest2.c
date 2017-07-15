/* -----------------------------------------------------------------------
 * Test Case for play_smithy() in dominion.c 
 *
 * The smithy card effect should be to draw 3 cards and then discard the 
 * smithy.
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

    // Put a few cards in hand, deck and discard (how many and type aren't
    // really important for this test since we just want to test the smithy effect.
    // Put smithy at position 0.
    for (i = 1; i < 6; i++ ) {
        G.deck[player][i] = gold; 
        G.hand[player][i] = gold; 
        G.discard[player][i] = gold; 
        G.handCount[player]++;
        G.deckCount[player]++;
        G.discardCount[player]++;
    }

    // Now put smithy in hand
    G.hand[player][0] = smithy;
    G.handCount[player] = i + 1;
    
    // Test that 3 cards are drawn (so handcount should be +2 from initial since smithy gets discarded)
    int expected = G.handCount[player] + 2;;
    play_smithy(player, &G, 0);
    int got = G.handCount[player];
    if (got == expected) printf("play_smithy() test 3 drawn, 1 discarded PASS\n");
    else printf("play_smithy() test 3 drawn, 1 discarded **FAIL**. Exepected handCount=%d, got handCount=%d\n", expected, got);

    // test that smithy was discarded
    int smithy_in_hand = 0;    // 0 if smithy not in hand, 1 if in hand
    for (i = 0; i < G.handCount[player]; i++) {
        if (G.hand[player][i] == smithy) smithy_in_hand = 1;
    }
    if (smithy_in_hand == 0) printf("play_smithy() smithy removed from hand PASS\n");
    else printf("play_smithy() smithy removed from hand **FAIL**\n");
    
    // test that smithy in played cards pile
    int smithy_in_playedCards = 0;    // 0 if smithy not in hand, 1 if in hand
    for (i = 0; i < G.playedCardCount; i++) {
        if (G.playedCards[i] == smithy) smithy_in_playedCards = 1;
    }
    if (smithy_in_playedCards == 1) printf("play_smithy() smithy put in playedCards PASS\n");
    else printf("play_smithy() smithy put in playedCards **FAIL**\n");


    return 0;
}
