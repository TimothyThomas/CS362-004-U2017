/* -----------------------------------------------------------------------
 * Random Test for play_adventurer() in dominion.c 
 *
 * The adventurer card allows the player to reveal cards in the deck
 * until 2 treasure cards are found.  The 2 treasure cards go to the hand
 * and the other revealed cards are discarded.
 * -----------------------------------------------------------------------
 */

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "interface.h"
#include "dominion_helpers.h"
#include "rngs.h"

#define MAX_HAND_TEST 500
#define MAX_DECK_TEST 500
#define MAX_INITIAL_DISCARD 20
#define MAX_PLAYERS_TEST 4
#define NUM_TESTS 10000
#define PRINT_FAILURES 0

int main() {

    srand(time(NULL));
    int numplayers;
    int player;
    int card;
    int position;
    int i;  // loop counter
    int test_count;
    int num_tests = NUM_TESTS;
    int currentPlayer;
    struct gameState G, initG;
    int failed_flag;
    int tests_passed = 0;
    int tests_failed = 0;

    // start loop
    for (test_count = 1; test_count <= num_tests; test_count++) {
        failed_flag = 0;

        /*
         *  INITIALIZE A RANDOM GAME STATE 
        */
        numplayers = 0;
        while (numplayers < 1) {
            numplayers = rand() % MAX_PLAYERS_TEST;
        }

        int invalid_state = 1;
        while (invalid_state) {

            for (player = 0; player < numplayers; player++) {

                // Randomize players' decks
                G.deckCount[player] = rand() % MAX_DECK_TEST; 
                if (G.deckCount[player] > 0) {
                    for (i = 0; i < G.deckCount[player]; i++) {
                        G.deck[player][i] = rand() % 27;  // 27 possible cards
                    }
                }
                
                // Randomize players' hands (players must have at least 1 card in
                // hand to hold the adventurer
                while (1) {
                    G.handCount[player] = rand() % MAX_HAND_TEST; 
                    if (G.handCount[player] > 0)
                        break;
                }
                for (i = 0; i < G.handCount[player]; i++) {
                    G.hand[player][i] = rand() % 27;  // 27 possible cards
                }

                // Randomize players' discard by moving top deck cards to discard pile.
                // A max discard size of 20 is reasonable. 
                G.discardCount[player] = rand() % MAX_INITIAL_DISCARD; 
                for (i = 0; i < G.deckCount[player]; i++) {
                    G.discard[player][i] = G.deck[player][--G.deckCount[player]]; 
                }
            }

            // Choose a player at random to have the adventurer and put in hand at a random position
            currentPlayer = rand() % numplayers;
            position = rand() % G.handCount[currentPlayer];
            G.hand[currentPlayer][position] = adventurer;

            // Make sure player's deck+discard has at least 2 treasure cards
            // to draw, otherwise we would go into an infinite
            // loop inside play_adventurer (this should be considered a BUG!!)
            int treasure_count = 0; 
            for (i = 0; i < G.deckCount[currentPlayer]; i++) {
                card = G.deck[currentPlayer][i];
                if (card == copper || card == silver || card == gold) { 
                    treasure_count++; 
                }
            }
            for (i = 0; i < G.discardCount[currentPlayer]; i++) {
                card = G.discard[currentPlayer][i];
                if (card == copper || card == silver || card == gold) { 
                    treasure_count++; 
                }
            }
            if (treasure_count >=2) {
                invalid_state = 0;
            }
        }

        // Copy the initial game state
        memcpy(&initG, &G, sizeof(struct gameState));

        /*
         *  CALL FUNCTION UNDER TEST 
         *
         *  G will be the state after function call and initG the state before the call.
        */
        //play_adventurer(currentPlayer, &G, position);
        int temphand[MAX_HAND];  // not used
        adventurerCard(currentPlayer, temphand, &G);


        /*
         *  COMPARE RESULTS TO ORACLES 
        */

        // Test that hand now has 1 additional card (+2 treasure; -1 for discarded adventurer)
        int expected = initG.handCount[currentPlayer] + 1;
        int got = G.handCount[currentPlayer];
        if (got != expected) { 
            if (PRINT_FAILURES) 
                printf("TEST %d: current player 1 NET additional card in hand **FAIL**. "
                    "Exepected handCount=%d, got handCount=%d\n", test_count, expected, got);
            failed_flag = 1;
            /*
            printf("Hand before:\n");
            printHand(currentPlayer, &initG);
            printf("Hand after:\n");
            printHand(currentPlayer, &G);
            */
        }
        
        // Test that 2 additional treasure cards are now in hand
        int count_before = 0;
        for (i = 0; i < initG.handCount[currentPlayer]; i++) {
            card = initG.hand[currentPlayer][i];
            if (card == gold || card == silver || card == copper) count_before++;
        }
        expected = 2 + count_before;
        got = 0;
        for (i = 0; i < G.handCount[currentPlayer]; i++) {
            card = G.hand[currentPlayer][i];
            if (card == gold || card == silver || card == copper) got++;
        }
        if (got != expected) {
            if (PRINT_FAILURES) 
                printf("TEST %d: current player gained +2 treasure cards in hand **FAIL**.\n"
                        "Exepected %d, got %d\n", test_count, expected, got);
            failed_flag = 1;
            /*
            printf("Hand before:\n");
            printHand(currentPlayer, &initG);
            printf("Hand after:\n");
            printHand(currentPlayer, &G);
            */
        }
        
        // test that treasure cards came from current player deck/discard
        if (G.deckCount[currentPlayer] + G.discardCount[currentPlayer] != 
            initG.deckCount[currentPlayer] + initG.discardCount[currentPlayer] - 1) {
            if (PRINT_FAILURES) 
                printf("TEST %d: new cards came from currentPlayer piles **FAIL**\n", test_count);
            failed_flag = 1;
            /*
            printf("State before:\n");
            printState(&initG);
            printf("State after:\n");
            printState(&G);
            */
        }

        // test that adventurer was discarded
        if (G.hand[currentPlayer][position] == adventurer) {
            if (PRINT_FAILURES) 
                printf("TEST %d: adventurer removed from hand **FAIL**\n", test_count);
            /*
            printf("Hand before:\n");
            printHand(currentPlayer, &initG);
            printf("Hand after:\n");
            printHand(currentPlayer, &G);
            */
            failed_flag = 1;
        }
        
        // test number of cards in hand, discard and deck are unchanged 
        got = G.handCount[currentPlayer] + G.deckCount[currentPlayer] + G.discardCount[currentPlayer];
        expected = initG.handCount[currentPlayer] + initG.deckCount[currentPlayer] + initG.discardCount[currentPlayer];
        if (got != expected) {
            if (PRINT_FAILURES) 
                printf("TEST %d: current player total number of cards changed **FAIL**. "
                        "Exepected %d, got %d\n", test_count, expected, got);
            failed_flag = 1;
        }


        // test that game state has not changed for the other players
        for (player = 0; player < numplayers; player++) {
            if (player == currentPlayer) continue;

            if (G.handCount[player] != initG.handCount[player]) {
                if (PRINT_FAILURES) 
                    printf("TEST %d: player %d hand count changed! **FAIL**\n", test_count, player);
                failed_flag = 1;
            }

            if (G.deckCount[player] != initG.deckCount[player]) {
                if (PRINT_FAILURES) 
                    printf("TEST %d: player %d deck count changed! **FAIL**\n", test_count, player);
                failed_flag = 1;
            }

            if (G.discardCount[player] != initG.discardCount[player]) {
                if (PRINT_FAILURES) 
                    printf("TEST %d: player %d discard count changed! **FAIL**\n", test_count, player);
                failed_flag = 1;
            }
        }

        if (failed_flag != 0) 
            tests_failed++;
        else
            tests_passed++;
    }

    printf("##############################################################################\n");
    printf("  RANDOM TEST SUMMARY for play_adventurer \n");
    printf("  Number of tests: %d\n", num_tests);
    printf("  passed: %d\n", tests_passed);
    printf("  failed: %d\n", tests_failed);
    printf("##############################################################################\n");

    return 0;
}