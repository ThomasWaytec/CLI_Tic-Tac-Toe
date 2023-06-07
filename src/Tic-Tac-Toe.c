#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include "controls.h"
#include "interface.h"

#define BLANK " "

bool is_space_occupied(char*** game_board, Cursor cursor) {
    if (game_board[cursor.x_coord][cursor.y_coord] != BLANK)
    {
        printf("!!SPACE OCCUPIED!!");
        Sleep(1500);
        return true;
    }
    return false;
}

void hide_terminal_cursor(void) {

    /* hide Windows terminal cursor */
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(STD_HANDLE, &cursorInfo);
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(STD_HANDLE, &cursorInfo);

}

char* get_user_chosen_board_size(void) {
    
    /* define the grid dimensions */
    const size_t GRID_HEIGHT = 7;
    const size_t GRID_LENGTH = 1;

    /* allocating space for the grid */
    char*** grid = calloc(GRID_HEIGHT, __SIZEOF_POINTER__);    
    for (size_t row = 0; row < GRID_HEIGHT; row++) {grid[row] = calloc(GRID_LENGTH, __SIZEOF_POINTER__);}
    

    /* initializing the grid */
    const size_t LONGEST_STRING_LENGTH_IN_GRID = 3;
    grid[0][0] = "3x3";
    grid[1][0] = "4x4";
    grid[2][0] = "5x5";
    grid[3][0] = "6x6";
    grid[4][0] = "7x7";
    grid[5][0] = "8x8";
    grid[6][0] = "9x9";


    /* get the chosen value from the user */
    Cursor cursor = live_grid(GRID_HEIGHT, GRID_LENGTH, grid);

    /* return chosen value */
    char* chosen_grid_size = grid[cursor.x_coord][cursor.y_coord];
    return chosen_grid_size;

}

bool check_if_player_won(char* player_symbol, Cursor* cursor, size_t BOARD_HEIGHT, size_t BOARD_LENGTH, char*** game_board) {

    const size_t BOARD_SIZE = BOARD_HEIGHT;



    /* row */
    for (int i = 0; i < BOARD_LENGTH; i++) {

        if (game_board[cursor->x_coord][i] != player_symbol) {break;}

        if (i == BOARD_LENGTH - 1) {return true;}
    }
    



    /* column */
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        
        if (game_board[i][cursor->y_coord] != player_symbol) {break;}

        if(i == BOARD_HEIGHT - 1) {return true;}
    }

 


    /* diagonal */
    if (cursor->x_coord == cursor->y_coord) {
        
        for (int i = 0; i < BOARD_SIZE; i++) {

            if (game_board[i][i] != player_symbol) {break;}

            if(i == BOARD_SIZE - 1) {return true;}
        }
    }
        



    /* anti-diagonal */
    if (cursor->x_coord + cursor->y_coord == BOARD_SIZE - 1) {

        for (int i = 0; i < BOARD_SIZE; i++) {

            if (game_board[i][(BOARD_SIZE - 1) - i] != player_symbol) {break;}

            if(i == BOARD_SIZE - 1) {return true;}
        }
    }


    return false;
}




int main(void) {
    
    hide_terminal_cursor();

    /* get the chosen board size from the user */
    char* chosen_board_size = get_user_chosen_board_size();
    int board_size = chosen_board_size[0] - '0'; /* convert board size to integer*/



    /* create game board of chosen size */
    const size_t BOARD_HEIGHT = board_size;
    const size_t BOARD_LENGTH = board_size;

    char*** game_board = calloc(BOARD_HEIGHT, __SIZEOF_POINTER__); /* allocating space for the board */

    for (size_t row = 0; row < BOARD_HEIGHT; row++) {
        game_board[row] = calloc(BOARD_LENGTH, __SIZEOF_POINTER__);

        for (size_t column = 0; column < BOARD_LENGTH; column++)
        {
            game_board[row][column] = BLANK;     /* initializing the board */
        }
    }



    /* main gameplay loop */
    Cursor cursor;

    char* player_symbols[2] = {"X", "O"};
    size_t turn_count = 0;
    char* current_player_symbol = player_symbols[turn_count % 2]; 
    
    bool game_end = false;
    while (!game_end)
    {
        
        //printf("turn_count=%d\n%d", turn_count, BOARD_HEIGHT * BOARD_LENGTH);
        //Sleep(1 * 1000);

        /* check draw */
        if (turn_count == BOARD_HEIGHT * BOARD_LENGTH) {printf("Tie!"); break;}

        /* check if win */
        if (check_if_player_won(current_player_symbol, &cursor, BOARD_HEIGHT, BOARD_LENGTH, game_board)) {printf("%s HAS WON!!!!\n", current_player_symbol); break;}

        current_player_symbol = player_symbols[turn_count % 2];

        /* make player move */
        cursor = live_grid(BOARD_HEIGHT, BOARD_LENGTH, game_board);
        if (is_space_occupied(game_board, cursor)) {continue;}
        game_board[cursor.x_coord][cursor.y_coord] = current_player_symbol;
        turn_count++;

    }
    
    return 0;
}


