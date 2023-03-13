#include "C4_game_engine.h"

int main() {

    short** board = create_board();
    short move;
    print_board(board);

    while(1) {
        place_chip(board, play_move(board), HUMAN);

        printf("\033[2J");
        print_board(board);

        handle_win(board);

        move = minimax(copy_board(board), 7, -10000000000, 10000000000, AI).column;
        place_chip(board, move, AI);

        printf("\033[2J");
        print_board(board);
        printf("AI move: %hd\n", move + 1);

        handle_win(board);
    }
}