//
// Created by Edgar Barrios on 06/07/2024.
//

#include "functions.h"
#include <stdio.h> // for printf, scanf
#include <unistd.h> // for sleep, usleep
#include <time.h>
#include <stdlib.h>
#include <ctype.h> // For toupper




int debug_mode(){

    int debug;
    char debug_char;

    printf("enable debug mode? (1 for yes, 0 for no)\n");
    scanf(" %c", &debug_char);

    while (debug_char != '0' && debug_char != '1'){
        printf("Invalid choice\n");
        printf("enable debug mode? (1 for yes, 0 for no)\n");
        scanf(" %c", &debug_char);
    }

    printf("\n");

    debug = debug_char - '0'; // convert the char to an int

    if (debug == 1){
        printf("Debug mode enabled\n");
        printf("\n");
    }

    return debug;
}



// this function is in charge of printing the welcome message
void start() {
    printf("Welcome to my game of minesweeper\n");
    sleep(1);
    printf("Created by Edgar Barrios\n");
    sleep(1);
    printf("\n");
}



//this function is in charge of printing the rules
void rules() {

    char choice_rules;

    printf("Do you know the rules? (1 for yes/0 for no)\n");
    scanf(" %c", &choice_rules);
    while (choice_rules != '1' && choice_rules != '0'){
        printf("Invalid choice\n");
        printf("Do you know the rules? (1 for yes/0 for no)\n");
        scanf(" %c", &choice_rules);
    }

    if (choice_rules == '0'){
        printf("The rules are simple: you have to find all the bombs in the board\n");
        printf(" - You can flag a cell if you think there is a bomb\n");
        printf(" - You can reveal a cell to see what is inside\n");
        printf(" - If you reveal a cell with a bomb, you lose\n");
        printf(" - If you reveal all the cells without bombs, you win\n");
        printf(" - The number in a cell indicates the number of bombs around it\n");
        printf("      (even the corners, so up to 8 bombs around a single cell)\n");
        printf("Good luck!\n");
        sleep(3);
    }

    printf("\n");
}



//in: debug (integer), takes 0 as not activated or 1 if activated
//does: prints out a menu
//out: difficulty (integer), 1 for easy, 2 for medium, 3 for hard and 0 to go back to the main menu
int menu() {

    int option;
    char option_char;

    printf("Menu:\n");
    printf(" Press 1 to Play\n");
    printf(" Press 2 to Load saved game (Option not available yet)\n");
    printf(" Press 3 to Exit\n");
    scanf(" %c", &option_char);
    while (option_char < '1' || option_char > '3'){
        printf("Invalid choice\n");
        sleep(1);
        printf(" Press 1 to Play\n");
        printf(" Press 2 to Load saved game (Option not available yet)\n");
        printf(" Press 3 to Exit\n");
        scanf(" %c", &option_char);
    }

    // convert to int
    option = option_char - '0';

    return option;
}



int choose_difficulty (int debug) {

    int difficulty;
    char difficulty_char;

    printf("\n");
    printf("Here you can choose between 3 difficulties:\n");
    sleep(1);
    printf("  Press 1 for easy\n");
    printf("  Press 2 for average\n");
    printf("  Press 3 for hard\n");
    printf("  Press 0 to go back to the main menu\n");
    printf("difficulty: ");
    scanf(" %c", &difficulty_char);

    while (difficulty_char < '0' || difficulty_char > '3'){
        printf("Invalid difficulty\n");
        sleep(1);
        printf("  Press 1 for easy\n");
        printf("  Press 2 for average\n");
        printf("  Press 3 for hard\n");
        printf("  Press 0 to go back to the main menu\n");
        printf("difficulty: ");
        scanf(" %c", &difficulty_char);
    }

    // convert to int
    difficulty = difficulty_char - '0';

    printf("\n");

    return difficulty;
}



// this function is in charge of putting all the important information about the difficulty in a struct info_difficulty
// the info_difficulty variable will be called game
// In case you might want to change the number of columns, you will need to modify the play_turn function
info_difficulty save_difficulty_info (int difficulty, int debug) {

    info_difficulty game; //the info about the difficulty is stored in a variable called game

    game.debug = debug;

    if (difficulty == 1) {
        printf("You chose easy\n");
        printf("The board will be 8x8 and will have 8 bombs\n");
        sleep(1);
        game.nb_columns = 8;
        game.nb_rows = 8;
        game.nb_bombs = 8;
    } else if (difficulty == 2) {
        printf("You chose average\n");
        printf("The board will be 10x10 and will have 15 bombs\n");
        sleep(1);
        game.nb_columns = 10;
        game.nb_rows = 10;
        game.nb_bombs = 15;
    } else if (difficulty == 3) {
        printf("You chose hard\n");
        printf("The board will be 12x12 and will have 24 bombs\n");
        sleep(1);
        game.nb_columns = 12;
        game.nb_rows = 12;
        game.nb_bombs = 24;
    } else {
        if (game.debug == 1){
            printf("Error in save_difficulty_info function\n");
        } else {
            printf("Error\n");
        }
        sleep(1);
    }

    return game;
}



// this function is in charge of creating a board, allocating the memory space for it
char** create_board(info_difficulty game) {

    char **board;
    int row;

    board = malloc(sizeof(char*) * game.nb_rows);
    for (row = 0; row < game.nb_rows; row = row + 1) {
        board[row] = malloc(sizeof(char) * game.nb_columns);
    }

    return board;
}



void free_board(char** board, info_difficulty game){
    int row;
    for (row = 0; row < game.nb_rows; row = row + 1){
        free(board[row]);
    }
    free(board);
}



// This function is in charge of initializing every square of the board to 0
void initialization_of_board(char** board, info_difficulty game, char character){

    for (int i = 0; i<game.nb_rows; i++) {
        for (int j = 0; j<game.nb_columns; j++) {
            board[i][j] = character;
        }
    }
}



// This function is in charge of filling the board with bombs and the surrounding numbers according to the number of bombs around
char** fill_board(char** board, info_difficulty game) {

    printf("Bombs are being placed... please wait\n");
    sleep(1);

    int bombs_to_place = game.nb_bombs;
    if (game.debug == 1){
        printf("Number of bombs to place: %d\n", bombs_to_place);
    }

    int random_nb;
    srand(time(0)); //initialize the random number generator
                    // (reminder: must not be in the loop, otherwise the random number will be the same for every iteration of the loop)

    while (bombs_to_place > 0) {
        for (int i = 0; i<game.nb_rows; i++) {
            for (int j = 0; j<game.nb_columns; j++) {

                random_nb = rand() % (game.nb_rows * game.nb_columns); //generate a random number between 0 and the number of cells in the board
                if (board[i][j] != 'B' && random_nb == 1) {
                    board[i][j] = 'B';
                    bombs_to_place = bombs_to_place - 1;

                    if(game.debug==1){
                        printf("Bomb placed at %dth row, %dth column\n", i+1, j+1);
                        printf("Bombs left to place: %d\n", bombs_to_place);
                    }

                    //check if the cells around the bomb are not bombs
                    //if they are not bombs, increment the value of the cell by 1
                    //if they are bombs, do not increment the value of the cell
                    //check if the cell is in the board

                    //cell above left
                    if (i-1 >= 0 && j-1 >= 0 && board[i-1][j-1] != 'B') {
                        board[i-1][j-1] = board[i-1][j-1] + 1;
                    }
                    //cell above
                    if (i-1 >= 0 && board[i-1][j] != 'B') {
                        board[i-1][j] = board[i-1][j] + 1;
                    }
                    //cell above right
                    if (i-1 >= 0 && j+1 < game.nb_columns && board[i-1][j+1] != 'B') {
                        board[i-1][j+1] = board[i-1][j+1] + 1;
                    }
                    //cell left
                    if (j-1 >= 0 && board[i][j-1] != 'B') {
                        board[i][j-1] = board[i][j-1] + 1;
                    }
                    //cell right
                    if (j+1 < game.nb_columns && board[i][j+1] != 'B') {
                        board[i][j+1] = board[i][j+1] + 1;
                    }
                    //cell below left
                    if (i+1 < game.nb_rows && j-1 >= 0 && board[i+1][j-1] != 'B') {
                        board[i+1][j-1] = board[i+1][j-1] + 1;
                    }
                    //cell below
                    if (i+1 < game.nb_rows && board[i+1][j] != 'B') {
                        board[i+1][j] = board[i+1][j] + 1;
                    }
                    //cell below right
                    if (i+1 < game.nb_rows && j+1 < game.nb_columns && board[i+1][j+1] != 'B') {
                        board[i + 1][j + 1] = board[i + 1][j + 1] + 1;
                    }

                }
                if (bombs_to_place <= 0) {
                    break; // break out of 1st for loop
                }
            }
            if (bombs_to_place <= 0) {
                break; // break out of 2nd for loop
            }
        }
    }
    printf(" -> bombs have been placed\n");
    sleep(1);

    return board;
}



// this function is in charge of printing the board
void print_board(char** board, info_difficulty game) {
    printf("\n");

    //print the column numbers
    printf("      ");
    for (int i = 0; i < game.nb_columns; ++i) {
        printf("%d ", i+1);
        if (i+1 < 10){
            printf(" ");
        }
    }

    // print the number of flags placed
    printf("   ");
    printf("number of flags placed: %d", count_flags(board, game));

    printf("\n");

    // print a separation line between the column numbers and the board
    printf("      ");
    for (int i = 0; i < game.nb_columns; ++i) {
        printf("|  ");
    }

    //print the number of bombs in the Board
    printf("   ");
    printf("number of bombs in the board: %d", game.nb_bombs);

    printf("\n");

    //print the board
    for (int i = 0; i<game.nb_rows; i++) {
        //print the row numbers
        printf(" %c - ", 65+i);
        for (int j = 0; j<game.nb_columns; j++) {

            //color for the cell
            if (board[i][j] == 'F' || board[i][j] == 'B'){
                printf("\033[0;31m"); //Red
            }
            if (board[i][j] == '0'){
                printf("\033[0;37m"); //Grey
            }
            if (board[i][j] == '1'){
                printf("\033[0;34m"); //Blue
            }
            if (board[i][j] == '2'){
                printf("\033[0;32m"); //Green
            }
            if (board[i][j] == '3'){
                printf("\033[0;35m"); //Purple
            }
            if (board[i][j] >= '4' && board[i][j]<= '8'){
                printf("\033[0;36m"); //Cyan
            }
            //print the cell
            printf(" %c ", board[i][j]);
            // reset the color
            printf("\033[0m"); //reset the color
        }
        printf("\n");
    }
    printf("\n");
}



int count_flags (char** player_board, info_difficulty game){
    int nb_flags = 0;
    for (int i = 0; i < game.nb_columns; ++i) {
        for (int j = 0; j < game.nb_rows; ++j) {
            if (player_board[i][j] == 'F'){
                nb_flags++;
            }
        }
    }
    return nb_flags;
}



//this function is in charge of asking the player for the option to play or save the game
//output: 1 if the player wants to reveal a cell, 2 if the player wants to flag a cell, 3 if the player wants to save the game
int players_turn() {

    int choice;
    char choice_char;

    printf("It's your turn, choose an option\n");
    printf("  Press 1 to reveal a cell\n");
    printf("  Press 2 to flag a cell\n");
    printf("  Press 3 to save the game\n");
    scanf(" %c", &choice_char);

    while (choice_char != '1' && choice_char != '2' && choice_char != '3'){
        printf("Invalid option\n");
        sleep(1);
        printf("  Press 1 to reveal a cell\n");
        printf("  Press 2 to flag a cell\n");
        printf("  Press 3 to save the game\n");
        scanf(" %c", &choice_char);
    }

    // convert to int
    choice = choice_char - '0';

    return choice;
}



// this function is in charge of checking of all characters of a string are digits
// output: 0 if it is not the case, 1 if it is the case
// used in the play_turn function
int is_valid_number(const char *str) {
    int i = 0;
    // Check if each character is a digit
    while (str[i] != '\0') {
        if (!isdigit(str[i])) {
            return 0; // Return false if any character is not a digit
        }
        i++;
    }
    return 1; // Return true if the string contains only digits
}



//this function is in charge of asking the player for the row and column
//In the case of revealing a cell, it calls the function check_cell (choice_turn == 1)
    //therefore returning 0 (=dead) if the cell is a bomb, 1 (=alive) if the cell is not a bomb
//In the case of flagging a cell (choice_turn == 2), the function returns 1 (=alive)
int play_turn(char** player_board, char** bomb_board, info_difficulty game, int choice_turn) {
    printf("choose a cell\n");

    //ask the player for the row and column
    char row, row_upper;
    char column_string[4];

    //Row
    printf("Row (letters): ");
    scanf(" %c", &row); //the space before %c is important to avoid the buffer
    while( (row < 'a'   ||   row > 'a' + game.nb_rows - 1)   &&   (row < 'A'   ||   row > 'A' + game.nb_rows - 1) ){
        printf("Invalid row, please choose a letter between A and %c, or between a and %c\n", 'A' + game.nb_rows - 1, 'a' + game.nb_rows - 1);
        printf("Row (letters): ");
        scanf(" %c", &row);
    }
    if (row >= 'a' && row <= 'z'){
        row_upper = toupper(row); //convert the letter to uppercase
    } else{
        row_upper = row;
    }

    //Column
    int column_number;
    while(1){
        printf("Column (numbers): ");
        scanf(" %3s", &column_string);

        // Check if the input is a valid number
        if (is_valid_number(column_string)) {
            // Convert the valid string to an integer
            column_number = atoi(column_string);

            // Check if the number is within the expected range
            if (game.nb_columns == 8){
                if (column_number >= 1 && column_number <= 8) {
                    break;  // Valid input, exit the loop
                } else {
                    printf("Invalid column, please choose a number between 1 and %d\n", game.nb_columns);
                }
            } else if (game.nb_columns == 10) {
                if (column_number >= 1 && column_number <= 10) {
                    break;  // Valid input, exit the loop
                } else {
                    printf("Invalid column, please choose a number between 1 and %d\n", game.nb_columns);
                }
            } else if (game.nb_columns == 12){
                if (column_number >= 1 && column_number <= 12) {
                    break;  // Valid input, exit the loop
                } else {
                    printf("Invalid column, please choose a number between 1 and %d\n", game.nb_columns);
                }
            }
        } else {
            printf("Invalid input. Please enter a valid number.\n");
        }
    }

    // In the case of flagging a cell, flags the cell
    if (choice_turn == 2) {
        if (player_board[row_upper-'A'][column_number-1] == 'F'){
            printf("Unflagging cell\n");
            sleep(1);
            player_board[row_upper-'A'][column_number-1] = 'X';
        } else if (player_board[row_upper-'A'][column_number-1] == 'X') {
            player_board[row_upper-'A'][column_number-1] = 'F';
        } else {
            printf("you cannot flag a cell that has already been revealed\n");
            sleep(1);
        }
        return 1;
    }

    //In the case of revealing a cell, checks if the cell is a bomb
    if (choice_turn == 1){
        if (player_board[row_upper-'A'][column_number-1] == 'X'){
            return check_cell(player_board, bomb_board, game, row_upper, column_number);
        } else if (player_board[row_upper-'A'][column_number-1] == 'F') {
            printf("You cannot reveal a flagged cell\n");
            printf("Un-flag the cell first\n");
        } else {
            printf("You have already revealed this cell\n");
        }
        sleep(1);
        return 1;
    }
    else {
        if (game.debug == 1){
            printf("Error in play_turn function\n");
            sleep(1);
            return 0;
        } else {
            printf("Error\n");
            sleep(1);
            return 0;
        }
    }
}



// this function is in charge of checking if the cell is a bomb
// output: 0 if the cell is a bomb, 1 if the cell is not a bomb
int check_cell(char** player_board, char** bomb_board,info_difficulty game, int row, int column) {

    if (bomb_board[row-65][column-1] == 'B') {
        printf("You hit a bomb\n");
        sleep(1);
        return 0;
    } else {
        printf("You did not hit a bomb\n");
        sleep(1);
        reveal_cells(player_board, bomb_board, game, row, column, 0);
        return 1;
    }
}



// this function is in charge of revealing the cells around the cell chosen
// input: player_board, bomb_board, game, row, column
    // row and column are the coordinates of the cell chosen by the player
        // row: A to nb_rows (depends on the difficulty)
        // column: 1 to nb_columns (depends on the difficulty)
    // sub_call is a variable that is used to check if the function is called by itself
        // if it is called by itself, it is set to 1
        // if it is not called by itself, it is set to 0
// output: void
void reveal_cells(char** player_board, char** bomb_board, info_difficulty game, int row, int column, int sub_call) {

    // if the cell is not revealed or flagged
    if (player_board[row-'A'][column-1] == 'X' || player_board[row-'A'][column-1] == 'F'){

        // if the cell is a 0
        if (bomb_board[row-'A'][column-1] == '0') {
            if (game.debug == 1){
                printf("revealing a 0 cell at row %c column %d \n", row, column);
            }
            player_board[row-'A'][column-1] = '0';

            if (row != 'A'){ //if the row is not the first row
                if (column != 1){ //if the column is not the first column
                    // cell above left
                    reveal_cells(player_board, bomb_board, game, row-1, column-1, 1);
                }
                // cell above
                reveal_cells(player_board, bomb_board, game, row-1, column, 1);

                if (column != game.nb_columns){ //if the column is not the last column
                    // cell above right
                    reveal_cells(player_board, bomb_board, game, row-1, column+1, 1);
                }
            }
            if (column != 1){ //if the column is not the first column
                // cell left
                reveal_cells(player_board, bomb_board, game, row, column-1, 1);
            }
            if (column != game.nb_columns){ //if the column is not the last column
                // cell right
                reveal_cells(player_board, bomb_board, game, row, column+1, 1);
            }

            if (row != game.nb_rows+'A'-1){ //if the row is not the last row
                if (column != 1){ //if the column is not the first column
                    // cell below left
                    reveal_cells(player_board, bomb_board, game, row+1, column-1, 1);
                }
                // cell below
                reveal_cells(player_board, bomb_board, game, row+1, column, 1);
                if (column != game.nb_columns){ //if the column is not the last column
                    // cell below right
                    reveal_cells(player_board, bomb_board, game, row+1, column+1, 1);
                }
            }

        // if the function has called itself and if the cell is flagged
        } else if (sub_call == 1 && player_board[row-65][column-1] == 'F') {
            if (game.debug == 1){
                printf("sub_call of reveal_cells, revealing flagged cell row %c column %d\n", row, column);
            }
            player_board[row-65][column-1] = bomb_board[row-65][column-1];

        // if the cell is not a 0 or (not a flag and not a sub call)
        } else { // if the cell is not a 0 or not a flag in the case of a sub call
            if (game.debug == 1){
                printf("revealing only cell row %c column %d\n", row, column);
            }
            player_board[row-65][column-1] = bomb_board[row-65][column-1];
        }
    }
}



// this function is in charge of checking if the player has won
// output: 1 if the player has won, 0 if the player has not won yet
int check_win(char** player_board, char** bomb_board, info_difficulty game){

    for (int i = 0; i < game.nb_columns; ++i) {
        for (int j = 0; j < game.nb_rows; ++j) {
            if (player_board[i][j] == 'X'){
                return 0;
            }
        }
    }
    if (game.debug == 1){
        printf("check_win: All cells have been revealed\n");
    }
    if (count_flags(player_board, game) == game.nb_bombs){
        if (game.debug == 1){
            printf("check_win: The number of flags equal the number of bombs\n");
        }
        return 1;
    } else {
        return 0;
    }
}



int wanna_play_again(){
    int play_again;
    char play_again_char;

    printf("Do you want to play again? (1 for Yes, 0 for no)\n");
    scanf(" %c", &play_again_char);

    while (play_again_char != '0' && play_again_char != '1'){
        printf("Do you want to play again? (1 for Yes, 0 for no)\n");
        scanf(" %c", &play_again_char);
    }

    // convert char to int
    play_again = play_again_char - '0';

    return play_again;
}



int save_game() {
    printf("\n");
    printf("The option to save a game is not available yet\n");
    sleep(1);
    printf("\n");
    return 0;
}



int load_game() {
    printf("\n");
    printf("The option to load a game is not available yet\n");
    sleep(1);
    printf("\n");
    return 0;
}