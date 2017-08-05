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
    struct gameState G, initG;
    int temphand[MAX_HAND];

    G.handCount[player] = 0;
    G.deckCount[player] = 0;
    G.discardCount[player] = 0;
    G.playedCardCount = 0;

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

    // Copy the initial game state for player 0 to player 1
    G.handCount[1] = G.handCount[player];
    G.deckCount[1] = G.deckCount[player];
    G.discardCount[1] = G.discardCount[player];

    // Copy the overall initial game state
    memcpy(&initG, &G, sizeof(struct gameState));

    // make the call.  G is now the state after function call and initG before the call.
    //play_adventurer(player, &G, 0);
    adventurerCard(player, temphand, &G);

    // Test that hand now has 1 additional card (+2 treasure; -1 for discarded adventurer)
    int expected = initG.handCount[player] + 1;
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
    
    // test that treasure cards came from player 0 deck/discard
    if (G.deckCount[0] + G.discardCount[0] == initG.deckCount[0] + initG.discardCount[0] - 1)
        printf("play_adventurer() test cards came from player 0 piles PASS\n");
    else
        printf("play_adventurer() test cards came from player 0 piles **FAIL**\n");

    // test that adventurer was discarded
    int adventurer_in_hand = 0;    // 0 if adventurer not in hand, 1 if in hand
    for (i = 0; i < G.handCount[player]; i++) {
        if (G.hand[player][i] == adventurer) adventurer_in_hand = 1;
    }
    if (adventurer_in_hand == 0) printf("play_adventurer() adventurer removed from hand PASS\n");
    else printf("play_adventurer() adventurer removed from hand **FAIL**\n");

    // test that game state has not changed for player 1
    if (G.handCount[1] == initG.handCount[1]) printf("play_adventurer player 1 hand count PASS\n");
    else  printf("play_adventurer player 1 hand count **FAIL**\n");

    if (G.deckCount[1] == initG.deckCount[1]) printf("play_adventurer player 1 deck count PASS\n");
    else  printf("play_adventurer player 1 deck count **FAIL**\n");

    if (G.discardCount[1] == initG.discardCount[1]) printf("play_adventurer player 1 discard count PASS\n");
    else  printf("play_adventurer player 1 discard count **FAIL**\n");

    return 0;
}
