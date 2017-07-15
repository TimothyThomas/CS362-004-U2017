/* -----------------------------------------------------------------------
 * Test Case for isGameOver() in dominion.c 
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"


int main() {


    // set up a mock game state
    int i;
    struct gameState G;

    // all supply piles > 0
    for (i = 0; i < 25; i++) {
        G.supplyCount[i] = 1;
    }
    int expected = 0;
    int got = isGameOver(&G);
    if (got == expected) printf("isGameOver() all supply piles >0 PASS\n");
    else printf("isGameOver() test all supply piles >0 **FAIL**. Exepected %d, got %d\n", expected, got);

    // test provinces = 0, remaining supply piles > 0
    for (i = 0; i < 25; i++) {
        G.supplyCount[i] = 1;
    }
    G.supplyCount[province] = 0;
    expected = 1;
    got = isGameOver(&G);
    if (got == expected) printf("isGameOver() test provinces = 0; all supply piles >0 PASS\n");
    else printf("isGameOver() test provinces=0; all supply piles >0 **FAIL**. Exepected %d, got %d\n", expected, got);
    
    // test provinces > 0, two other supply piles = 0
    for (i = 0; i < 23; i++) {
        G.supplyCount[i] = 1;
    }
    G.supplyCount[province] = 1;
    G.supplyCount[22] = 0;
    G.supplyCount[23] = 0;
    expected = 0;
    got = isGameOver(&G);
    if (got == expected) printf("isGameOver() test provinces > 0; two supply piles =0 PASS\n");
    else printf("isGameOver() test provinces>0; 2 supply piles =0 **FAIL**. Exepected %d, got %d\n", expected, got);

    // test provinces > 0, three supply piles = 0
    for (i = 0; i < 22; i++) {
        G.supplyCount[i] = 1;
    }
    G.supplyCount[province] = 1;
    G.supplyCount[21] = 0;
    G.supplyCount[22] = 0;
    G.supplyCount[23] = 0;
    expected = 1;
    got = isGameOver(&G);
    if (got == expected) printf("isGameOver() test provinces > 0; 3 supply piles =0 PASS\n");
    else printf("isGameOver() test provinces>0; 3 supply piles =0 **FAIL**. Exepected %d, got %d\n", expected, got);

    // test provinces = 0, two supply piles = 0
    for (i = 0; i < 23; i++) {
        G.supplyCount[i] = 1;
    }
    G.supplyCount[province] = 0;
    G.supplyCount[22] = 0;
    G.supplyCount[23] = 0;
    expected = 1;
    got = isGameOver(&G);
    if (got == expected) printf("isGameOver() test provinces = 0; 3 supply piles =0 PASS\n");
    else printf("isGameOver() test provinces=0; 3 supply piles =0 **FAIL**. Exepected %d, got %d\n", expected, got);


    return 0;
}
