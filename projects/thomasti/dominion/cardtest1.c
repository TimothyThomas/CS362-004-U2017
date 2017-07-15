/* -----------------------------------------------------------------------
 * Test Case for play_adventurer() in dominion.c 
 *
 * The adventurer card allows the player to reveal cards in the deck
 * until 2 treasure cards are found.  The 2 treasure cards go to the hand
 * and the other revealed cards are discarded.
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
    int temphand[MAX_HAND];
    int z = 0;   //temp hand counter

    // Put a few cards in hand. How many and type aren't
    // really important for this test since we just want to test the
    // adventurer effect in isolation.
    for (i = 0; i < 5; i++ ) {
        G.hand[player][i] = estate; 
        G.handCount[player]++;
    }
    
    // Now put adventurer in hand at pos 0
    G.hand[player][0] = adventurer;
    
    // Put a few cards in deck (including 2 treasure)
    for (i = 0; i < 5; i++ ) {
        G.deckCount[player]++;
        // gold in positions 1 and 3, estate in positions 0, 2 and 4
        if (i % 2 != 0) G.deck[player][i] = estate; 
        else G.deck[player][i] = gold; 
    }

    // Test that hand now has 1 additional card (+2 treasure; -1 for discarded adventurer)
    int expected = G.handCount[player] + 1;
    play_adventurer(player, &G, temphand, &z);
    int got = G.handCount[player];
    if (got == expected) printf("play_adventurer() test 1 additional card in hand PASS\n");
    else printf("play_adventurer() test 1 additional card in hand **FAIL**. Exepected handCount=%d, got handCount=%d\n", expected, got);
    
    // Test that 2 treasure cards (gold) are now in hand
    got = 0;
    expected = 2;
    for (i = 0; i < G.handCount[player]; i++) {
        if (G.hand[player][i] == gold) got++;
    }
    if (got == expected) printf("play_adventurer() test 2 gold cards in hand PASS\n");
    else printf("play_adventurer() test 2 gold cards in hand **FAIL**. Exepected %d, got %d\n", expected, got);

    // test that adventurer was discarded
    int adventurer_in_hand = 0;    // 0 if adventurer not in hand, 1 if in hand
    for (i = 0; i < G.handCount[player]; i++) {
        if (G.hand[player][i] == adventurer) adventurer_in_hand = 1;
    }
    if (adventurer_in_hand == 0) printf("play_adventurer() adventurer removed from hand PASS\n");
    else printf("play_adventurer() adventurer removed from hand **FAIL**\n");

    return 0;
}
