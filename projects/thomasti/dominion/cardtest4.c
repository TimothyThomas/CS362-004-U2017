/* -----------------------------------------------------------------------
 * Test Case for play_sea_hag() in dominion.c 
 *
 * The sea hag causes all other players to discard the top card from the deck,
 * and then gain a curse card that is put on top of the deck.
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"


int main() {

    // set up a mock game state for players 0 and 1
    int i;
    int p;
    struct gameState G;

    G.numPlayers = 2;
    for (p = 0; p < G.numPlayers; p++) {
        G.handCount[p] = 0;
        G.deckCount[p] = 0;
        G.discardCount[p] = 0;
    }

    // Put a few cards in each player's hand, deck and discard. How many and type aren't
    // really important for this test since we just want to test the
    // sea_hag effect in isolation.
    for (p = 0; p < G.numPlayers; p++) {
        for (i = 0; i < 3; i++ ) {
            G.deck[p][i] = gold; 
            G.hand[p][i] = silver; 
            G.discard[p][i] = copper; 
            G.handCount[p]++;
            G.deckCount[p]++;
            G.discardCount[p]++;
        }
    }
    // Now put sea_hag at position 0 of player 0, and have player 0 play the card 
    G.hand[0][0] = sea_hag;
    play_sea_hag(0, &G);


    // Test that player 1 discarded top deck card
    if (G.discard[1][G.discardCount[1]-1] == gold) printf("play_sea_hag() test P1 top of deck discarded PASS\n");
    else printf("play_sea_hag() test P1 top of deck discarded **FAIL**\n");

    // Test that player 0 top deck card is not curse
    if (G.deck[0][G.deckCount[0]-1] != curse) printf("play_sea_hag() test P0 top of deck is not curse PASS\n");
    else printf("play_sea_hag() test P0 top of deck is not curse **FAIL**\n");
    
    // Test that player 1 top deck card is now curse
    if (G.deck[1][G.deckCount[1]-1] == curse) printf("play_sea_hag() test P1 top of deck is curse PASS\n");
    else printf("play_sea_hag() test P1 top of deck is curse **FAIL**\n");
    
    // Test that player 1 deck count remains the same
    if (G.deckCount[1] == 3) printf("play_sea_hag() test P1 deck count unchanged PASS\n");
    else printf("play_sea_hag() test P1 deck count unchanged **FAIL**\n");

    // Test that player 1 discard count is incremented by 1 
    int expected = 4;
    int got = G.discardCount[1];
    if (got == expected) printf("play_sea_hag() discard count incremented by 1 PASS\n");
    else printf("play_sea_hag() discard count incremented by 1 **FAIL**\n");

    // Test that player 0 discards sea hag 
    int sea_hag_in_hand = 0;    // 0 if sea_hag not in hand, 1 if in hand
    for (i = 0; i < G.handCount[0]; i++) {
        if (G.hand[0][i] == sea_hag) sea_hag_in_hand = 1;
    }
    if (sea_hag_in_hand == 0) printf("play_sea_hag() test sea_hag removed from P0 hand PASS\n");
    else printf("play_sea_hag() test sea_hag removed from P0 hand **FAIL**\n");
    
    
    return 0;
}
