/* -----------------------------------------------------------------------
 * Test Case for scoreFor() in dominion.c 
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
    
    // use a value of 7 for each of these so that we can test each of the 6 cards that gives point
    // and 1 card that does not
    //                      -1      1       3      6            1    fullDeckCount/10   0
    int cards_to_test[] = {curse, estate, duchy, province, great_hall,      gardens,  feast};

    // First we test each card individually to ensure the values are correct
    // give player 1 of each cards type in each pile
    G.handCount[player] = 1;     
    G.discardCount[player] = 1;
    G.deckCount[player] = 1;

    G.deck[player][0] = curse;
    G.hand[player][0] = curse;
    G.discard[player][0] = curse;
    int expected = -3;
    int got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test curse PASS\n");
    else printf("scoreFor() test curses **FAIL**. Exepected %d, got %d\n", expected, got);

    G.deck[player][0] = estate;
    G.hand[player][0] = estate;
    G.discard[player][0] = estate;
    expected = 3;
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test estate PASS\n");
    else printf("scoreFor() test estates **FAIL**. Exepected %d, got %d\n", expected, got);

    G.deck[player][0] = duchy;
    G.hand[player][0] = duchy;
    G.discard[player][0] = duchy;
    expected = 9;
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test duchy PASS\n");
    else printf("scoreFor() test duchy **FAIL**. Exepected %d, got %d\n", expected, got);

    G.deck[player][0] = province;
    G.hand[player][0] = province;
    G.discard[player][0] = province;
    expected = 18;
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test province PASS\n");
    else printf("scoreFor() test province **FAIL**. Exepected %d, got %d\n", expected, got);

    G.deck[player][0] = great_hall;
    G.hand[player][0] = great_hall;
    G.discard[player][0] = great_hall;
    expected = 3;
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test_great hall PASS\n");
    else printf("scoreFor() test great_hall **FAIL**. Exepected %d, got %d\n", expected, got);

    // Test a card that should give no points
    G.deck[player][0] = feast;
    G.hand[player][0] = feast;
    G.discard[player][0] = feast;
    expected = 0;
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() feast hall PASS\n");
    else printf("scoreFor() test feast **FAIL**. Exepected %d, got %d\n", expected, got);


    // need to test a couple different cases for gardens (fullDeckCount of 3, 9, 10)
    //
    // Test full deck count of 3
    G.handCount[player] = 1;     
    G.discardCount[player] = 1;
    G.deckCount[player] = 1;
    G.deck[player][0] = gardens;
    G.hand[player][0] = gardens;
    G.discard[player][0] = gardens;
    expected = 0;
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test gardens (count=3) PASS\n");
    else printf("scoreFor() test gardens (count=3) **FAIL**. Exepected %d, got %d\n", expected, got);

    // Test full deck count of 9 (make sure rounds down)
    G.handCount[player] = 3;     
    G.discardCount[player] = 3;
    G.deckCount[player] = 3;
    for (i = 0; i < 3; i++) {
        G.deck[player][i] = gardens;
        G.hand[player][i] = gardens;
        G.discard[player][i] = gardens;
    }
    expected = 0;
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test gardens (count=9) PASS\n");
    else printf("scoreFor() test gardens (count=9) **FAIL**. Exepected %d, got %d\n", expected, got);

    // Test full deck count of 10
    // NOTE THIS TEST **FAIL**S since there is a bug in fullDeckCount() which is
    // called by scoreFor().  the total number of cards in all piles should be
    // counted to determine the point value of gardens, 
    // except fullDeckCount() only counts the number of occurrences of a specific card
    G.handCount[player] = 3;     
    G.discardCount[player] = 3;
    G.deckCount[player] = 4;
    for (i = 0; i < 3; i++) {
        G.deck[player][i] = gardens;
        G.hand[player][i] = gardens;
        G.discard[player][i] = gardens;
    }
    G.deck[player][3] = gardens; // didn't catch this one in loop
    expected = 10;
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test gardens (count=10) PASS\n");
    else printf("scoreFor() test gardens (count=10) **FAIL**. Exepected %d, got %d\n", expected, got);

    // Test sum (put 2 of each in each pile)
    int count = 14;
    G.handCount[player] = count;     
    G.discardCount[player] = count;
    G.deckCount[player] = count;

    for (i = 0; i < count/2; i++) {
        G.deck[player][i] = cards_to_test[i]; 
        G.deck[player][i+7] = cards_to_test[i]; 
        G.hand[player][i] = cards_to_test[i]; 
        G.hand[player][i+7] = cards_to_test[i]; 
        G.discard[player][i] = cards_to_test[i]; 
        G.discard[player][i+7] = cards_to_test[i]; 
    }

    expected = 72;    // 3*(-2 + 2 + 6 + 12 + 2 + 14*3/10) =72 
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test sum PASS\n");
    else printf("scoreFor() test sum **FAIL**. Exepected %d, got %d\n", expected, got);

    // Test score for 0 cards in all piles
    G.handCount[player] = 0;     
    G.discardCount[player] = 0;
    G.deckCount[player] = 0;
    expected = 0;
    got = scoreFor(player, &G);
    if (got == expected) printf("scoreFor() test zero cards PASS\n");
    else printf("scoreFor() test zero cards **FAIL**. Exepected %d, got %d\n", expected, got);

    return 0;
}
