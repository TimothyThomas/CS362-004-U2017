/* -----------------------------------------------------------------------
 * Test Case for fullDeckCount() in dominion.c 
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
    int player = 0;
    struct gameState G;
    
    // 0 cards in each pile, count curses
    G.handCount[player] = 0;     
    G.discardCount[player] = 0;
    G.deckCount[player] = 0;
    int expected = 0;
    int got = fullDeckCount(player, 0, &G);
    if (got == expected) printf("fullDeckCount() test 0 cards in each pile PASS\n");
    else printf("fullDeckCount() test 0 cards in each pile **FAIL**. Exepected %d, got %d\n", expected, got);

    // 1 curse card in hand, non-curse in deck and discard, count curses
    G.handCount[player] = 1;     
    G.discardCount[player] = 1;
    G.deckCount[player] = 1;
    G.deck[player][0] = smithy;
    G.hand[player][0] = curse;
    G.discard[player][0] = province;
    expected = 1;
    got = fullDeckCount(player, curse, &G);
    if (got == expected) printf("fullDeckCount() test 1 curse in hand, non-curse in deck/discard: PASS\n");
    else printf("fullDeckCount() test 1 card in each pile: **FAIL**. Exepected %d, got %d\n", expected, got);
    
    // 1 curse card in deck, non-curse in hand and discard, count curses
    G.handCount[player] = 1;     
    G.discardCount[player] = 1;
    G.deckCount[player] = 1;
    G.deck[player][0] = curse;
    G.hand[player][0] = duchy;
    G.discard[player][0] = estate;
    expected = 1;
    got = fullDeckCount(player, curse, &G);
    if (got == expected) printf("fullDeckCount() test 1 card in each pile PASS\n");
    else printf("fullDeckCount() test 1 card in each pile **FAIL**. Exepected %d, got %d\n", expected, got);
    
    // 1 curse card in discard, non-curse in hand and deck, count curses
    G.handCount[player] = 1;     
    G.discardCount[player] = 1;
    G.deckCount[player] = 1;
    G.deck[player][0] = gold;
    G.hand[player][0] = village;
    G.discard[player][0] = curse;
    expected = 1;
    got = fullDeckCount(player, curse, &G);
    if (got == expected) printf("fullDeckCount() test 1 card in each pile PASS\n");
    else printf("fullDeckCount() test 1 card in each pile **FAIL**. Exepected %d, got %d\n", expected, got);
    
    // 1 curse card in each pile, count curses
    G.handCount[player] = 1;     
    G.discardCount[player] = 1;
    G.deckCount[player] = 1;
    G.deck[player][0] = curse;
    G.hand[player][0] = curse;
    G.discard[player][0] = curse;
    expected = 3;
    got = fullDeckCount(player, curse, &G);
    if (got == expected) printf("fullDeckCount() test 1 card in each pile PASS\n");
    else printf("fullDeckCount() test 1 card in each pile **FAIL**. Exepected %d, got %d\n", expected, got);
    
    // 1 non-curse card in each pile, count curses
    G.handCount[player] = 1;     
    G.discardCount[player] = 1;
    G.deckCount[player] = 1;
    G.deck[player][0] = duchy;
    G.hand[player][0] = duchy;
    G.discard[player][0] = duchy;
    expected = 0;
    got = fullDeckCount(player, curse, &G);
    if (got == expected) printf("fullDeckCount() test 1 card in each pile PASS\n");
    else printf("fullDeckCount() test 1 card in each pile **FAIL**. Exepected %d, got %d\n", expected, got);
    
    return 0;
}
