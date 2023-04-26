//
// Created by dalae on 2023-03-12.
//

#include "C4_game_engine.h"
#include <pthread.h>
#include <stdlib.h>



short** create_board(){
    //allocate memory for the board and initialize values to 0
    short** board = malloc(BOARD_HEIGHT*sizeof(short*));
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        board[i] = malloc(BOARD_WIDTH * sizeof(short));
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            board[i][j] = EMPTY;
        }
    }
    return board;
}

void print_board(short** board){
    printf("\n");
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        printf("%d  ", i+1);
    }
    printf("\n");
    for (int i = 0; i < 3*BOARD_WIDTH - 2; ++i) {
        printf("_");
    }
    printf("\n");
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if(board[i][j] == HUMAN)
                printf("H  ");
                //colourChange(RED);
            else if(board[i][j] == AI)
                printf("X  ");
                //colourChange(YELLOW);
            else printf("O  ");//colourChange(BLUE);

            //printf("0  ")
        }
        printf("\n");
    }
    //colourChange(CYAN);
}

short is_legal_move(short** board, short column){
    if(board[0][column] == 0 && (7 > column && column >= 0))
        return 1;
    else return 0;
}

void flush_stdin(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

short play_move(short** board){
    short move;
    printf("Enter the index of the column in which you would like to play, in a range of 1-7\n");
    scanf("%hd", &move);
    move--;
    if(is_legal_move(board, move) == 1) {
        flush_stdin();
        return move;
    }
    printf("That was not a legal move, please try again\n");
    flush_stdin();
    return play_move(board);
}

short** place_chip(short** board, short column, short player){
    if(!is_legal_move(board, column)){ //check if column is full
        return 0;
    }
    for (int i = 0; i < BOARD_HEIGHT; ++i) { //search for current height of chips in given column
        if(board[i][column] != 0){ //add chip above last added chip
            board[i-1][column] = player;
            return board;
        }
    }
    board[BOARD_HEIGHT-1][column] = player; //add chip at bottom if no chips in column
    return board;
}

short isdraw(short** board){
    short draw = 1;
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        if(board[0][i] == 0)
            return 0;
    }
    return draw;
}

short iswin(short** board){
    if(isdraw(board)){
        return 3;
    }
    for (int i = 0; i < BOARD_HEIGHT; ++i) { //check rows
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            if (board[i][j] == board[i][j + 1] && board[i][j + 1] == board[i][j + 2] && board[i][j + 2] == board[i][j + 3] && board[i][j] != 0) {
                return board[i][j];
            }
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check columns
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board[i][j] == board[i + 1][j] && board[i+1][j] == board[i + 2][j] && board[i + 2][j] == board[i + 3][j] && board[i][j] != 0) {
                return board[i][j];
            }
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check upward diagonals
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            if(board[i][j] == board[i+1][j+1] && board[i+1][j+1] == board[i+2][j+2] && board[i+2][j+2] == board[i+3][j+3] && board[i][j] != 0){
                return board[i][j];
            }
        }
    }
    for (int i = 3; i < BOARD_HEIGHT; ++i) { //check downward diagonals
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if(board[i][j] == board[i-1][j+1] && board[i-1][j+1] == board[i-2][j+2] && board[i-2][j+2] == board[i-3][j+3] && board[i][j] != 0){
                return board[i][j];
            }
        }
    }
    return 0;
}

short** reset_board(short** board){
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            board[i][j] = 0;
        }
    }
    return board;

}

short handle_win(short** board) {
    short state = iswin(board);
    if (state == 2) {
        printf("\nThe computer has won. Resetting board...\n");
    } else if (state == 1) {
        printf("\nYou won! Resetting board...\n");
    } else if(state == 3){
        printf("\nIt's a draw. Resetting board...\n");
    }
    if(state>0){
        reset_board(board);
        sleep(3);
        print_board(board);
        return 1;
    }
    return 0;
}

long long evaluate_window(short window[4], short player) {
    short opponent = 3 - player;
    short player_count = 0;
    short opponent_count = 0;
    for (int i = 0; i < 4; ++i) { //count chips in window
        if (window[i] == player) {
            player_count++;
        } else if (window[i] == opponent) {
            opponent_count++;
        }
    }
    if (player_count == 4)
        return  10000000000;
    if (opponent_count == 4)
        return  -(10000000000);
    if (opponent_count == 0)
        return player_count * player_count * 100;
    if (player_count == 0)
        return -(opponent_count * opponent_count * 100);
    return player_count - opponent_count;
}

long long evaluate_board(short** board, short player){
    long long score = 0;
    short window[4];
    long long window_score;
    for (int i = 0; i < BOARD_HEIGHT; ++i) { //check rows
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i][j + k];
            }
            window_score = evaluate_window(window, player);
            if(llabs(window_score) >= 10000000000)
                return window_score;
            score += window_score;
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check columns
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i+k][j];
            }
            window_score = evaluate_window(window, player);
            if(llabs(window_score) >= 10000000000)
                return window_score;
            score += window_score;
        }
    }
    for (int i = 0; i < BOARD_HEIGHT - 3; ++i) { //check upward diagonals
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i+k][j+k];
            }
            window_score = evaluate_window(window, player);
            if(llabs(window_score) >= 10000000000)
                return window_score;
            score += window_score;
        }
    }
    for (int i = 3; i < BOARD_HEIGHT; ++i) { //check downward diagonals
        for (int j = 0; j < BOARD_WIDTH - 3; ++j) {
            for (int k = 0; k < 4; ++k) {
                window[k] = board[i-k][j+k];
            }
            window_score = evaluate_window(window, player);
            if(llabs(window_score) >= 10000000000)
                return window_score;
            score += window_score;
        }
    }
    return score;
}

short** copy_board(short** board){
    //returns pointer to a copy of values in board array
    short** new_board = malloc(sizeof(short*) * BOARD_HEIGHT); //allocate memory for board copy
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        new_board[i] = malloc(sizeof(short) * BOARD_WIDTH);
        memcpy(new_board[i], board[i], BOARD_WIDTH*sizeof(board[i][0]));
    }
    return new_board;
}

void free_board(short** board){
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        free(board[i]);
    }
    free(board);
}

struct minimax_return minimax(short** board, short depth, long long alpha, long long beta, short player){
    struct minimax_return best_move;
    best_move.column = -1;
    best_move.score = player == AI ? -10000000000 : 10000000000;
    short win_state = iswin(board);

    //stopping conditions
    if(win_state == 2){
        best_move.score =  (long long)round(pow(10, depth + 4));
        free_board(board);
        return best_move;
    }
    if(win_state == 1){
        best_move.score =  -10000000000;
        free_board(board);
        return best_move;
    }
    if(win_state == 3){
        best_move.score = 0;
        free_board(board);
        return best_move;
    }
    if(depth == 0){
        best_move.column = -1;
        best_move.score = evaluate_board(board, AI);
        free_board(board);
        return best_move;
    }

    long long score;
    short** new_board;

    //check for wins before calling minimax
    for (short i = 0; i < BOARD_WIDTH; ++i) {
        new_board = copy_board(board);
        if(place_chip(new_board, i, player) != 0) {
            if(iswin(new_board) == AI){
                best_move.score = (long long)round(pow(10, depth + 4));
                best_move.column = i;
                return best_move;
            }
            if(iswin(new_board) == HUMAN){
                best_move.score = -10000000000;
                best_move.column = i;
                return best_move;
            }
        }
        free_board(new_board);
    }
    //if no wins found at this depth continue searching

    for (short i = 0; i < BOARD_WIDTH; ++i) {
        new_board = copy_board(board);
        if(place_chip(new_board, i, player) != 0) {
            score = minimax(new_board, depth - 1, alpha, beta, 3 - player).score;
            if (player == AI) { //maximising
                if (score > best_move.score) {
                    best_move.score = score;
                    best_move.column = i;
                }
                alpha = max(alpha, best_move.score);
                if (alpha >= beta)
                    break;
            } else {
                if (score < best_move.score) {
                    best_move.score = score;
                    best_move.column = i;
                }
                beta = min(beta, best_move.score);
                if (alpha >= beta)
                    break;
            }
        }
    }
    free_board(board);
    return best_move;
}

void* minimax_mt(void* args){ //receive general args in separate execution thread
    MIN_ARGS* ret_args = (MIN_ARGS*)args;
    ret_args->score = minimax(ret_args->board, ret_args->depth, ret_args->alpha, ret_args->beta, ret_args->player).score;
    pthread_exit(NULL);
}
