/* -----------------------------------------------------------------------
 * Random Test for play_sea_hag() in dominion.c 
 *
 * The sea hag causes all other players to discard the top card from the deck,
 * and then gain a curse card that is put on top of the deck.
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
    int player;
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
        G.numPlayers = 0;
        while (G.numPlayers < 1) {
            G.numPlayers = rand() % MAX_PLAYERS_TEST;
        }

        int invalid_state = 1;
        while (invalid_state) {

            for (player = 0; player < G.numPlayers; player++) {

                // Randomize players' decks
                G.deckCount[player] = rand() % MAX_DECK_TEST; 
                if (G.deckCount[player] > 0) {
                    for (i = 0; i < G.deckCount[player]; i++) {
                        G.deck[player][i] = rand() % 27;  // 27 possible cards
                    }
                }
                
                // Randomize players' hands (players must have at least 1 card in
                // hand to hold the sea_hag
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
                    G.discard[player][i] = G.deck[player][G.deckCount[player]-1]; 
                    G.deckCount[player]--;
                }
            }

            // Randomize  numActions for current player
            G.numActions = rand() % 500; 
                
            // Randomize  coins for current player
            G.coins = rand() % 500; 

            // Choose a player at random to have the sea_hag and put in hand at a random position
            currentPlayer = rand() % G.numPlayers;
            position = rand() % G.handCount[currentPlayer];
            G.hand[currentPlayer][position] = sea_hag;

            invalid_state = 0;

        }

        // Set played cards pile to 0 so we can check for discard
        G.playedCardCount = 0;

        // Copy the initial game state
        memcpy(&initG, &G, sizeof(struct gameState));

        /*
         *  CALL FUNCTION UNDER TEST 
         *
         *  G will be the state after function call and initG the state before the call.
        */
        play_sea_hag(currentPlayer, &G, position);


        /*
         *  COMPARE RESULTS TO ORACLES 
        */

        // Test that hand has 1 fewer card (-1 for discarded sea_hag)
        int expected = initG.handCount[currentPlayer] - 1;
        int got = G.handCount[currentPlayer];
        if (got != expected) { 
            if (PRINT_FAILURES) 
                printf("TEST %d: current player 1 less card in hand **FAIL**. "
                    "Exepected handCount=%d, got handCount=%d\n", test_count, expected, got);
            failed_flag = 1;
            /*
            printf("Hand before:\n");
            printHand(currentPlayer, &initG);
            printf("Hand after:\n");
            printHand(currentPlayer, &G);
            */
        }
        
        // Test no change to actions
        expected = initG.numActions;
        got = G.numActions;
        if (got != expected) {
            if (PRINT_FAILURES) 
                printf("TEST %d: +2 actions **FAIL**.  "
                        "Exepected %d, got %d\n", test_count, expected, got);
            failed_flag = 1;
        }
        
        // Test no change to coins
        expected = initG.coins;
        got = G.coins;
        if (got != expected) {
            if (PRINT_FAILURES) 
                printf("TEST %d: no change to coins **FAIL**.  "
                        "Exepected %d, got %d\n", test_count, expected, got);
            failed_flag = 1;
        }
        
        // test that sea_hag was discarded
        if (G.playedCards[G.playedCardCount-1] != sea_hag) {
            if (PRINT_FAILURES) 
                printf("TEST %d: sea_hag removed from hand **FAIL**\n", test_count);
            failed_flag = 1;
        }
        
        // test total number of cards in hand, discard and deck are -1 for currenPlayer
        got = G.handCount[currentPlayer] + G.deckCount[currentPlayer] + G.discardCount[currentPlayer];
        expected = initG.handCount[currentPlayer] + initG.deckCount[currentPlayer] + initG.discardCount[currentPlayer]-1;
        if (got != expected) {
            if (PRINT_FAILURES) 
                printf("TEST %d: current player -1 total number of cards **FAIL**. "
                        "Exepected %d, got %d\n", test_count, expected, got);
            failed_flag = 1;
        }
        
        // test total number of cards in hand, discard and deck increase by 1 for all other players 
        // (since all other players gain a curse)
        for (i = 0; i < G.numPlayers; i++) {
            if (i == currentPlayer) continue;

            expected = initG.handCount[i] + initG.deckCount[i] + initG.discardCount[i]+1;
            got = G.handCount[i] + G.deckCount[i] + G.discardCount[i];
            if (got != expected) {
                if (PRINT_FAILURES) 
                    printf("TEST %d: other players card total unchanged (curse) **FAIL**. "
                            "Exepected %d, got %d\n", test_count, expected, got);
                failed_flag = 1;
            }
        }

        // test that other players discarded top deck card and gained a curse
        for (player = 0; player < G.numPlayers; player++) {
            if (player == currentPlayer) continue;

            if (G.discard[player][G.discardCount[player]-1] != initG.deck[player][initG.deckCount[player]-1]) {
                if (PRINT_FAILURES) 
                    printf("TEST %d: player %d top card discarded **FAIL**\n", test_count, player);
                failed_flag = 1;
            }

            if (G.deck[player][G.deckCount[player]-1] != curse) {
                if (PRINT_FAILURES) 
                    printf("TEST %d: player %d top card is curse **FAIL**\n", test_count, player);
                failed_flag = 1;
                /*
                printf("state before:\n");
                printDeck(player, &initG);
                printf("state after:\n");
                printDeck(player, &G);
                */
            }

            if (G.deckCount[player] != initG.deckCount[player]) {
                if (PRINT_FAILURES) 
                    printf("TEST %d: player %d deck count changed! **FAIL**\n", test_count, player);
                failed_flag = 1;
            }
        }

        if (failed_flag != 0) 
            tests_failed++;
        else
            tests_passed++;
    }

    printf("##############################################################################\n");
    printf("  RANDOM TEST SUMMARY for play_sea_hag \n");
    printf("  Number of tests: %d\n", num_tests);
    printf("  passed: %d\n", tests_passed);
    printf("  failed: %d\n", tests_failed);
    printf("##############################################################################\n");

    return 0;
}
