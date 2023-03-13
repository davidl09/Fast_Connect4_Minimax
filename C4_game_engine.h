//
// Created by dalae on 2023-03-12.
//

#ifndef CONNECT4_C4_GAME_ENGINE_H
#define CONNECT4_C4_GAME_ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <math.h>
#include "C4_game_engine.h"

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

#define AI 2
#define HUMAN 1
#define EMPTY 0

#define YELLOW 6
#define BLUE 1
#define RED 4



struct minimax_return{
    short column;
    long long score;
};

//function prototypes

short** create_board();

short play_move(short** board);

void flush_stdin();

long long evaluate_window(short window[4], short player);

void print_board(short** board);

short is_legal_move(short** board, short column);

short** place_chip(short** board, short column, short player);

short isdraw(short** board);

short iswin(short** board);

short** reset_board(short** board);

short handle_win(short** board);

long long evaluate_window(short window[4], short player);

long long evaluate_board(short** board, short player);

short** copy_board(short** board);

void free_board(short** board);

struct minimax_return minimax(short** board, short depth, long long alpha, long long beta, short player);

#endif //CONNECT4_C4_GAME_ENGINE_H
