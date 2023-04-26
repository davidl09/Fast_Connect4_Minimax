#include "C4_game_engine.h"

int main() {

    pthread_t threads[BOARD_WIDTH];

    short** board = create_board();
    short move;
    MIN_ARGS min_args[BOARD_WIDTH];

    print_board(board);

    while(1) {
        place_chip(board, play_move(board), HUMAN);

        printf("\033[2J");
        print_board(board);

        handle_win(board);

        for (int i = 0; i < BOARD_WIDTH; ++i) {
            min_args[i] = (MIN_ARGS){copy_board(board), 10, -10000000000, 10000000000, HUMAN, -10000000000};
            if(place_chip(min_args[i].board, i, AI)){
                pthread_create(&threads[i], NULL, minimax_mt, (void*)&min_args[i]);
            }else threads[i] = 0;
        }

        for (int i = 0; i < BOARD_WIDTH; ++i) {
            if(threads[i] != 0)
                pthread_join(threads[i], NULL);
        }

        long long min_score = -10000000000;
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            if(min_args[i].score > min_score){
                move = i;
                min_score = min_args[i].score;
            }
        }
        //move = minimax(copy_board(board), 7, -10000000000, 10000000000, AI).column;
        place_chip(board, move, AI);

        printf("\033[2J");
        print_board(board);
        printf("AI move: %d\n", move + 1);

        handle_win(board);
    }
}