/* -----------------------------------------------------------------------
 * Test Case for getCost() in dominion.c 
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {

    int card;  // ENUM from dominion.h  can have values >=0
    int got;       // observed result of test
    int expected;  // expected result of test

    // test a few cards (realistically we might do one test for each card time
    // in our game)
    card = smithy;
    got = getCost(card);
    expected = 4;
    if (got == expected) printf("getCost(smithy) PASS\n");
    else printf("getCost(smithy) **FAIL**. Exepected %d, got %d\n", expected, got);
        
    card = gold;
    got = getCost(card);
    expected = 6;
    if (got == expected) printf("getCost(gold) PASS\n");
    else printf("getCost(gold) **FAIL**. Exepected %d, got %d\n", expected, got);

    card = feast;
    got = getCost(card);
    expected = 4;
    if (got == expected) printf("getCost(feast) PASS\n");
    else printf("getCost(feast) **FAIL**. Exepected %d, got %d\n", expected, got);

    // BOUNDARY TESTS
    //
    // Test a card value of 0 (curse)
    card = 0;
    got = getCost(card);
    expected = 0;
    if (got == expected) printf("getCost(0) PASS\n");
    else printf("getCost(0) **FAIL**. Exepected %d, got %d\n", expected, got);
    
    // The following card input values doe not exist in our game and so should return -1
    card = -1;
    got = getCost(card);
    expected = -1;
    if (got == expected) printf("getCost(-1) PASS\n");
    else printf("getCost(-1) **FAIL**. Exepected %d, got %d\n", expected, got);

    card = INT_MAX;
    got = getCost(card);
    expected = -1;
    if (got == expected) printf("getCost(INT_MAX) PASS\n");
    else printf("getCost(INT_MAX) **FAIL**. Exepected %d, got %d\n", expected, got);

    card = -INT_MAX;
    got = getCost(card);
    expected = -1;
    if (got == expected) printf("getCost(-INT_MAX) PASS\n");
    else printf("getCost(-INT_MAX) **FAIL**. Exepected %d, got %d\n", expected, got);

    return 0;
}
