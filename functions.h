//
// Created by Edgar Barrios on 06/07/2024.
//

#ifndef MINESWEEPER_FUNCTIONS_H
#define MINESWEEPER_FUNCTIONS_H



//STRUCTS
//creation of a struct named info_difficulty to hold information according to the difficulty
struct thing {
    int nb_columns;
    int nb_rows;
    int nb_bombs;
    int debug;
};
typedef struct thing info_difficulty;



//FUNCTIONS
int debug_mode();

void start();

void rules();

int menu();

int choose_difficulty(int debug);

info_difficulty save_difficulty_info (int difficulty, int debug);

char** create_board(info_difficulty game);

void free_board(char** board, info_difficulty game);

void initialization_of_board(char** board, info_difficulty game, char character);

char** fill_board(char** board, info_difficulty game);

void print_board(char** board, info_difficulty game);

int count_flags(char** player_board, info_difficulty game);

int players_turn();

int play_turn(char** player_board, char** bomb_board, info_difficulty game, int choice_turn);

int check_cell(char** player_board, char** bomb_board, info_difficulty game, int row, int column);

void reveal_cells(char** player_board, char** bomb_board, info_difficulty game, int row, int column, int sub_call);

int check_win(char** player_board, char** bomb_board, info_difficulty game);

int wanna_play_again();

int save_game();

int load_game();



#endif //MINESWEEPER_FUNCTIONS_H
