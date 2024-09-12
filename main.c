#include "functions.h"
#include <stdio.h>
#include <unistd.h>


int main() {
    int play_again = 1;
    int debug; // 1 if the debug mode is activated, 0 if it is not
    debug = debug_mode(); // be careful, naming the variable and the function the same way will cause an error, and is not recommended

    start();

    // Rules
    if (debug == 0) {
        rules();
    }

    // Menu: Play, Load, Exit

    while (play_again == 1) {

        int choice_menu = menu();

        // first choice: Play
        if (choice_menu == 1) {

            // Choose difficulty
            int difficulty = choose_difficulty(debug);

            // If the user wants to go back to the main menu
            while (difficulty == 0) {
                choice_menu = menu();

                if (choice_menu == 1) {
                    difficulty = choose_difficulty(debug);
                } else if (choice_menu == 2) {
                    load_game();
                } else {
                    printf("Exiting game\n");
                    sleep(1);
                    return 0;
                }
            }

            // Put the info of the difficulty in a variable called game of type struct called info_difficulty (cf functions.h)
            info_difficulty game = save_difficulty_info(difficulty, debug);

            // Allocate memory space for 2 boards: bomb_board and player_board
            char **bomb_board;
            char **player_board;
            if (debug == 1) {
                printf("Allocating memory space for the boards... please wait\n");
                sleep(1);
                //Bomb_board
                bomb_board = create_board(game);
                printf(" -> Memory space allocated for bomb_board\n");

                //Player_board
                player_board = create_board(game);
                printf(" -> Memory space allocated for player board\n");

            }
            if (debug == 0) {
                printf("Creating board... please wait\n");
                sleep(1);
                bomb_board = create_board(game);
                player_board = create_board(game);
                printf(" -> Board created\n");
            }

            // verification of the allocation of memory space
            if (bomb_board != NULL && player_board != NULL) {
                // initialize every square of the board to 0
                char character;

                if (debug == 1) {
                    printf("Initializing the boards... please wait\n");
                    sleep(1);
                    //Bomb_board
                    character = '0';
                    initialization_of_board(bomb_board, game, character);
                    printf(" -> bomb_board initialized\n");
                    print_board(bomb_board, game);
                    //Player_board
                    character = 'X';
                    initialization_of_board(player_board, game, character);
                    printf(" -> player_board initialized\n");
                    print_board(player_board, game);
                }
                if (debug == 0) {
                    printf("Initializing the board... please wait\n");
                    sleep(1);
                    character = '0';
                    initialization_of_board(bomb_board, game, character);
                    character = 'X';
                    initialization_of_board(player_board, game, character);
                    printf(" -> Board initialized\n");
                }

                // fill the board with bombs and the surrounding numbers according to the number of bombs around
                bomb_board = fill_board(bomb_board, game);

                if (debug == 1) {
                    print_board(bomb_board, game);
                }
                printf("\n");
                printf("The following board is filled with X for each hidden cell\n");
                sleep(1);
                print_board(player_board, game);
                sleep(1);



                // Players turn
                int alive = 1; // 1 if the player is still alive, 0 if the player has lost
                int win; // 1 if the player wins, 0 if the player has not won yet
                int choice_turn; // 1 to reveal a cell, 2 to flag a cell, 3 for save


                while (alive == 1) {

                    // The player chooses to reveal a cell (1) or flag a cell (2) or save the game (3)
                    choice_turn = players_turn();

                    // If the player wants to save the game
                    if (choice_turn == 3) {
                        save_game();
                        // temporary message
                        printf("back to the game\n");
                    }

                    // If the player chooses to flag a cell
                    else if (choice_turn == 2) {
                        printf("Flagging a cell\n");
                        play_turn(player_board, bomb_board, game, choice_turn);
                    }

                    // If the player chooses to reveal a cell
                    else if (choice_turn == 1) {
                        printf("Revealing a cell\n");
                        alive = play_turn(player_board, bomb_board, game, choice_turn);

                        // If the player hits a bomb
                        if (alive == 0) {
                            break;
                        }
                    }

                    // If the player has not hit a bomb
                    win = check_win(player_board, bomb_board, game);

                    // give a warning message in the case where the player has put more flags than bombs on the board


                    // If the player has not won yet, it prints the board
                    if (win == 0) {
                        if (debug == 1) {
                            print_board(bomb_board, game);
                        }
                        print_board(player_board, game);
                        sleep(1);
                    }

                    if (count_flags(player_board, game) > game.nb_bombs){
                        printf("Be aware, you have placed more flags than bombs, you might have a flag on a cell without a bomb\n");
                        sleep(2);
                    }

                    // If the player wins
                    if (win == 1) {
                        printf("Congratulations! You won!\n");
                        sleep(1);

                        // print the boards
                        if (debug == 1) {
                            print_board(bomb_board, game);
                        }
                        print_board(player_board, game);
                        sleep(1);

                        // Ask the player if he wants to play again
                        play_again = wanna_play_again();

                        //we have to free the two boards
                        free_board(bomb_board, game);
                        free_board(player_board, game);
                        if (game.debug == 1) {
                            printf("\n");
                            printf("the memory for the two boards have been freed\n");
                            printf("\n");
                        }
                    }
                }

                // If the player hits a bomb
                if (alive == 0) {
                    printf("Game Over\n");
                    sleep(1);
                    print_board(bomb_board, game);
                    sleep(1);

                    // Ask the player if he wants to play again
                    play_again = wanna_play_again();

                    //we have to free the two boards
                    free_board(bomb_board, game);
                    free_board(player_board, game);
                    if (game.debug == 1){
                        printf("the memory for the two boards have been freed\n");
                    }
                }
            } else {
                printf("Error creating the board\n");
            }
        }


        // Second choice: Load Game
        else if (choice_menu == 2) {
            load_game();
        }

        // Third choice: Exit
        else {
            printf("Exiting game\n");
            sleep(1);
        }
    }

    // If the player does not want to play again (in the case where he has lost or won)
    if (play_again == 0) {
        printf("Good-bye\n");
        sleep(1);
    }

    return 0;
}