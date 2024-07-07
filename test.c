#include "chess.h"

/* ----------------------------- */
/* ---------- TESTING ---------- */
/* ----------------------------- */


void output_results() {

}

void test_wrapper(void (*test_func)(void), char* func_name) {
    clock_t start, end;
    double elapsed_time;

    start = clock();
    test_func();
    end = clock();

    printf("%f seconds: %s\n", ((double)(end - start) / CLOCKS_PER_SEC), func_name);
}


void test_evaluation_branching() {
    test_wrapper(test_moves, "test_moves");
    test_wrapper(test_forcing_moves, "test_forcing_moves");
    test_wrapper(test_forcing_moves_1, "test_forcing_moves_1");
    test_wrapper(test_forcing_moves_2, "test_forcing_moves_2");
    // test_wrapper(test_forcing_moves_3, "test_forcing_moves_3");
    test_wrapper(test_puzzle_fork, "test_puzzle_fork");
    test_wrapper(test_puzzle_win_queen, "test_puzzle_win_queen");
    test_wrapper(test_17_june_2024, "test_17_june_2024");
    test_wrapper(test_16_june_2024_partial, "test_16_june_2024_partial");
    // test_wrapper(test_16_june_2024, "test_16_june_2024");
    // test_wrapper(test_15_june_2024, "test_15_june_2024");
    test_wrapper(test_15_june_2024_partial, "test_15_june_2024_partial");
    test_wrapper(test_15_june_2024_end, "test_15_june_2024_end");
    
    test_wrapper(test_detect_mate_1, "test_detect_mate_1");
    test_wrapper(test_detect_mate_2, "test_detect_mate_2");
    test_wrapper(test_mate_in_four, "test_mate_in_four");
    test_wrapper(test_mate_in_four_2, "test_mate_in_four_2");
    test_wrapper(test_draw_by_repetition_best_line, "test_draw_by_repetition_best_line");
}


void test_forcing_moves_1() {
    Board* board = set_board_notation("bkg6 bcc6 wqe3 wka1 ");
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(2, 3, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);
    
    assert(scores->moves->moves[0]->from == e3);
    assert((scores->moves->moves[0]->destination == e8) || ((scores->moves->moves[0]->destination == e4)));
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == queen);

    assert((scores->moves->moves[2]->from == e8) || ((scores->moves->moves[2]->from == e4)));
    assert(scores->moves->moves[2]->destination == c6);
    assert(scores->moves->moves[2]->piece->c == white);
    assert(scores->moves->moves[2]->piece->type == queen);
}

void test_dont_be_stupid() {
    Board* board = set_board_notation("bkg6 bcd6 wqe6 wka1 ");
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(3, 3, black);
    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));
    assert(test_board == board->bitboard);

    assert(scores->moves->moves[0]->from == d6);
    assert(scores->moves->moves[0]->destination == e6);
    assert(scores->moves->moves[0]->piece->c == black);
    assert(scores->moves->moves[0]->piece->type == castle);
}

void test_forcing_moves_2() {
    Board* board = set_board_notation("bkg6 bcd6 wqe3 wka1 ");
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(4, 4, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    assert(scores->moves->moves[0]->from == e3);
    assert(scores->moves->moves[0]->destination == g3);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == queen);

    assert((scores->moves->moves[2]->from == g3));
    assert(scores->moves->moves[2]->destination == d6);
    assert(scores->moves->moves[2]->piece->c == white);
    assert(scores->moves->moves[2]->piece->type == queen);
}

void test_forcing_moves_3() {
    Board* board = set_board_notation("bkg7 bcc6 bpf6 bhg6 wkh1 wqe3 wcg1 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(6, 6, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    print_scores(scores);

    assert(scores->eval > 800);

    assert(scores->moves->moves[0]->from == g1);
    assert(scores->moves->moves[0]->destination == g6);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == castle);
}

void test_forcing_moves_3_partial() {

}

void test_puzzle_fork() {
    // bpf6
    Board* board = set_board_notation("wkb1 wqe3 wch2 whc3 wpb2 wpb4 wpd5 wpf4 wpg3 bkg6 bqd7 bcc4 bbh5 bpa7 bpb6 bpd6 bpf5 bph7 ");
    // print_board_pro(board);
    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(4, 3, white);
    
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->moves->moves[0]->from == h2);
    assert(scores->moves->moves[0]->destination == h5);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == castle);
}

void test_puzzle_win_queen() {
    Board* board = set_board_notation("wkg1 wqg3 wba4 wbc1 whb1 whd5 wph2 wpg2 wpd2 wpc3 wpb2 wpa2 wca1 bkd8 bqh4 bcf8 bca8 bhc6 bhg4 bph7 bpg7 bpe4 bpd6 bpc7 bpb7 bpa6 ");
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(6, 4, black);

    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));
    assert(test_board == board->bitboard);
    
    // print_scores(scores);

    assert(scores->moves->moves[0]->from == f8);
    assert(scores->moves->moves[0]->destination == f1);
    assert(scores->moves->moves[0]->piece->c == black);
    assert(scores->moves->moves[0]->piece->type == castle);

    assert(scores->moves->moves[1]->from == g1);
    assert(scores->moves->moves[1]->destination == f1);
    assert(scores->moves->moves[1]->piece->c == white);
    assert(scores->moves->moves[1]->piece->type == king);

    assert(scores->moves->moves[2]->from == g4);
    assert(scores->moves->moves[2]->destination == h2);
    assert(scores->moves->moves[2]->piece->c == black);
    assert(scores->moves->moves[2]->piece->type == knight);

    assert(scores->moves->moves[3]->from == g3);
    assert(scores->moves->moves[3]->destination == h2);
    assert(scores->moves->moves[3]->piece->c == white);
    assert(scores->moves->moves[3]->piece->type == queen);

    assert(scores->moves->moves[4]->from == h4);
    assert(scores->moves->moves[4]->destination == h2);
    assert(scores->moves->moves[4]->piece->c == black);
    assert(scores->moves->moves[4]->piece->type == queen);
}

void test_forcing_moves() {
    Board* board = forcing_move_setup();
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    // evaluation function not good enough for a lower breadth...
    // https://www.chess.com/blog/EnPassantFork/2024-03-01-dpa-order-matters
    Grapher* grapher = init_grapher(8, 4, black);
    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));

    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->moves->moves[0]->from == f6);
    assert(scores->moves->moves[0]->destination == h4);
    assert(scores->moves->moves[0]->piece->c == black);
    assert(scores->moves->moves[0]->piece->type == bishop);
}

void test_18_june_2024() {
    Board* board = set_board_notation("wkb1 wcd1 wcg7 wbd4 whd5 wpa3 wpb2 wpc2 wpe4 wpf3 wpf5 wph5 bkh8 bqd8 bcb8 bcf8 bbc8 bbe7 bha4 bpa6 bpd2 bpd6 bpf7 bph7 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(8, 4, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->eval == MAX_SCORE - 3);

    assert(scores->moves->moves[0]->from == g7);
    assert(scores->moves->moves[0]->destination == g8);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == castle);

    assert(scores->moves->moves[1]->from == h8);
    assert(scores->moves->moves[1]->destination == g8);
    assert(scores->moves->moves[1]->piece->c == black);
    assert(scores->moves->moves[1]->piece->type == king);

    assert(scores->moves->moves[2]->from == d5);
    assert(scores->moves->moves[2]->destination == e7);
    assert(scores->moves->moves[2]->piece->c == white);
    assert(scores->moves->moves[2]->piece->type == knight);

    assert(scores->moves->moves[3]->from == d8);
    assert(scores->moves->moves[3]->destination == e7);
    assert(scores->moves->moves[3]->piece->c == black);
    assert(scores->moves->moves[3]->piece->type == queen);

    assert(scores->moves->moves[4]->from == d1);
    assert(scores->moves->moves[4]->destination == g1);
    assert(scores->moves->moves[4]->piece->c == white);
    assert(scores->moves->moves[4]->piece->type == castle);

    assert(scores->moves->moves[5]->from == e7);
    assert(scores->moves->moves[5]->destination == g5);
    assert(scores->moves->moves[5]->piece->c == black);
    assert(scores->moves->moves[5]->piece->type == queen);

    assert(scores->moves->moves[6]->from == g1);
    assert(scores->moves->moves[6]->destination == g5);
    assert(scores->moves->moves[6]->piece->c == white);
    assert(scores->moves->moves[6]->piece->type == castle);
}

void test_17_june_2024() {
    Board* board = set_board_notation("wpa3 wpb4 wpc3 wpg4 whd4 wbe2 wch8 wkg2 bkc8 bce8 bhe6 bhc7 bpa7 bpb6 bpc6 bpf6 bpg5 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(8, 4, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    assert(scores->moves->moves[0]->from == h8);
    assert(scores->moves->moves[0]->destination == e8);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == castle);

    assert(scores->moves->moves[1]->from == c7);
    assert(scores->moves->moves[1]->destination == e8);
    assert(scores->moves->moves[1]->piece->c == black);
    assert(scores->moves->moves[1]->piece->type == knight);

    assert(scores->moves->moves[2]->from == d4);
    assert(scores->moves->moves[2]->destination == e6);
    assert(scores->moves->moves[2]->piece->c == white);
    assert(scores->moves->moves[2]->piece->type == knight);
}

void test_16_june_2024() {
    Board* board = set_board_notation("wpb5 wpb2 wpc2 wpf2 wph2 wkc1 wqc3 whd3 wbg3 wcg1 bpa4 bpe6 bpf7 bpg7 bph7 bkg8 bqd5 bbe4 bca8 bcf8 ");

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(9, 6, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);
    assert(scores->eval == MAX_SCORE - 5);

    assert(scores->moves->moves[0]->from == c3);
    assert(scores->moves->moves[0]->destination == g7);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == queen);

    assert(scores->moves->moves[1]->from == g8);
    assert(scores->moves->moves[1]->destination == g7);
    assert(scores->moves->moves[1]->piece->c == black);
    assert(scores->moves->moves[1]->piece->type == king);

    assert(scores->moves->moves[2]->from == g3);
    assert(scores->moves->moves[2]->destination == e5);
    assert(scores->moves->moves[2]->piece->c == white);
    assert(scores->moves->moves[2]->piece->type == bishop);

    assert(scores->moves->moves[3]->from == g7);
    assert(scores->moves->moves[3]->destination == h6);
    assert(scores->moves->moves[3]->piece->c == black);
    assert(scores->moves->moves[3]->piece->type == king);

    assert(scores->moves->moves[4]->from == e5);
    assert(scores->moves->moves[4]->destination == g7);
    assert(scores->moves->moves[4]->piece->c == white);
    assert(scores->moves->moves[4]->piece->type == bishop);

    assert(scores->moves->moves[5]->from == h6);
    assert(scores->moves->moves[5]->destination == h5);
    assert(scores->moves->moves[5]->piece->c == black);
    assert(scores->moves->moves[5]->piece->type == king);

    assert(scores->moves->moves[6]->from == d3);
    assert(scores->moves->moves[6]->destination == f4);
    assert(scores->moves->moves[6]->piece->c == white);
    assert(scores->moves->moves[6]->piece->type == knight);

    assert(scores->moves->moves[7]->from == h5);
    assert(scores->moves->moves[7]->destination == h4);
    assert(scores->moves->moves[7]->piece->c == black);
    assert(scores->moves->moves[7]->piece->type == king);

    assert(scores->moves->moves[8]->from == g7);
    assert(scores->moves->moves[8]->destination == f6);
    assert(scores->moves->moves[8]->piece->c == white);
    assert(scores->moves->moves[8]->piece->type == bishop);

    assert(scores->moves->moves[9]->from == d5);
    assert(scores->moves->moves[9]->destination == g5);
    assert(scores->moves->moves[9]->piece->c == black);
    assert(scores->moves->moves[9]->piece->type == queen);

    assert(scores->moves->moves[10]->from == f6);
    assert(scores->moves->moves[10]->destination == g5);
    assert(scores->moves->moves[10]->piece->c == white);
    assert(scores->moves->moves[10]->piece->type == bishop);
}

void test_16_june_2024_partial() {
    Board* board = set_board_notation("wpb5 wpb2 wpc2 wpf2 wph2 wkc1 whf4 wbg7 wcg1 bpa4 bpe6 bpf7 bph7 bkh4 bqd5 bbe4 bca8 bcf8 ");

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(9, 3, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);
    assert(scores->eval == MAX_SCORE - 1);

    assert(scores->moves->moves[0]->from == g7);
    assert(scores->moves->moves[0]->destination == f6);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == bishop);

    assert(scores->moves->moves[1]->from == d5);
    assert(scores->moves->moves[1]->destination == g5);
    assert(scores->moves->moves[1]->piece->c == black);
    assert(scores->moves->moves[1]->piece->type == queen);

    assert(scores->moves->moves[2]->from == f6);
    assert(scores->moves->moves[2]->destination == g5);
    assert(scores->moves->moves[2]->piece->c == white);
    assert(scores->moves->moves[2]->piece->type == bishop);
}

void test_15_june_2024() {
    // free parking
    // Difficult for heuristic function to see this first move... (knight h7 to f8)
    Board* board = set_board_notation("wkc1 wpb2 wpc2 wpd4 wpf2 wpg5 wbd2 wbd3 whh7 wqh3 wch1 bpa3 bpa5 bpc6 bpe6 bpf7 bpg6 bhd5 bhd7 bcb8 bce8 bqd8 bkg8 bbe7 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(15, 6, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    print_scores(scores);
}

void test_15_june_2024_partial() {
    // free parking
    // Difficult for heuristic function to see this first move... (knight h7 to f8)
    Board* board = set_board_notation("wkb1 wpc2 wpd4 wpf2 wpg5 wbc3 wbd3 whf8 wqh3 wch1 bpb2 bpa5 bpc6 bpe6 bpf7 bpg6 bhd7 bcb8 bce8 bqd8 bkg8 bbf6 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(12, 3, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->eval == MAX_SCORE - 2);
    assert(scores->moves->length == 5);

    assert(scores->moves->moves[0]->from == f8);
    assert(scores->moves->moves[0]->destination == d7);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == knight);

    assert(scores->moves->moves[1]->from == d8);
    assert(scores->moves->moves[1]->destination == d7);
    assert(scores->moves->moves[1]->piece->c == black);
    assert(scores->moves->moves[1]->piece->type == queen);

    assert(scores->moves->moves[2]->from == g5);
    assert(scores->moves->moves[2]->destination == f6);
    assert(scores->moves->moves[2]->piece->c == white);
    assert(scores->moves->moves[2]->piece->type == pawn);

    // no good moves for black, don't assert

    assert(scores->moves->moves[4]->from == h3);
    assert(scores->moves->moves[4]->destination == h8);
    assert(scores->moves->moves[4]->piece->c == white);
    assert(scores->moves->moves[4]->piece->type == queen);
}

void test_15_june_2024_end() {
    // free parking
    // Difficult for heuristic function to see this first move... (knight h7 to f8)
    Board* board = set_board_notation("wkb1 wpc2 wpd4 wpf2 wpf6 wbc3 wbd3 whf8 wqh3 wch1 bpb2 bpa5 bpc6 bpe5 bpf7 bpg6 bcb8 bce8 bqd7 bkg8 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(12, 3, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->eval == MAX_SCORE);
    assert(scores->moves->length == 1);

    assert(scores->moves->moves[0]->from == h3);
    assert(scores->moves->moves[0]->destination == h8);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == queen);
}


void test_14_june_2024() {
    Board* board = set_board_notation("wce1 wpa2 wpb3 wpf2 wpg2 wph3 wbb2 whd4 wkg1 bkc8 bqa5 bbg8 bpc7 bpf6 bpg7 bph6 ");
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(5, 4, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->moves->moves[0]->from == e1);
    assert(scores->moves->moves[0]->destination == e8);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == castle);

    assert(scores->moves->moves[1]->from == c8);
    assert((scores->moves->moves[1]->destination == b7) || (scores->moves->moves[1]->destination == d7));
    assert(scores->moves->moves[1]->piece->c == black);
    assert(scores->moves->moves[1]->piece->type == king);

    assert(scores->moves->moves[2]->from == e8);
    if (scores->moves->moves[1]->destination == b7) {
        assert(scores->moves->moves[2]->destination == b8);
    }else if (scores->moves->moves[1]->destination == d7) {
        assert(scores->moves->moves[2]->destination == d8);
    }else {assert(false);}
    assert(scores->moves->moves[2]->piece->c == white);
    assert(scores->moves->moves[2]->piece->type == castle);

    if (scores->moves->moves[1]->destination == b7) {
        assert(scores->moves->moves[3]->from == b7);
        assert(scores->moves->moves[3]->destination == b8);
    }else if (scores->moves->moves[1]->destination == d7) {
        assert(scores->moves->moves[3]->from == d7);
        assert(scores->moves->moves[3]->destination == d8);
    }else {assert(false);}
    assert(scores->moves->moves[3]->piece->c == black);
    assert(scores->moves->moves[3]->piece->type == king);

    assert(scores->moves->moves[4]->from == d4);
    assert(scores->moves->moves[4]->destination == c6);
    assert(scores->moves->moves[4]->piece->c == white);
    assert(scores->moves->moves[4]->piece->type == knight);

    assert(scores->moves->moves[6]->from == c6);
    assert(scores->moves->moves[6]->destination == a5);
    assert(scores->moves->moves[6]->piece->c == white);
    assert(scores->moves->moves[6]->piece->type == knight);
}

void test_13_june_2024() {
    // very difficult for evaluation function to see this...
    Board* board = set_board_notation("bkg7 bbd2 bcd4 bpf7 bpg6 bpg3 wkg1 wce2 wcc2 wph3 wpg2 wpa2 wpe5 ");
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(5, 4, black);
    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->moves->moves[0]->from == d4);
    assert(scores->moves->moves[0]->destination == b4);
    assert(scores->moves->moves[0]->piece->c == black);
    assert(scores->moves->moves[0]->piece->type == castle);
}

void test_12_june_2024() {
    Board* board = set_board_notation("bkg8 bqf2 bca8 bhe4 bhf3 bbg3 bph6 bpg7 bpf6 bpe5 bpc6 wkh1 wqc2 wcf1 wce1 wbd2 wbg2 wha5 wph3 wpc3 wpb4 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(9, 2, black);
    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->eval == MAX_SCORE - 1);

    assert(scores->moves->moves[0]->from == f2);
    assert(scores->moves->moves[0]->destination == g1);
    assert(scores->moves->moves[0]->piece->c == black);
    assert(scores->moves->moves[0]->piece->type == queen);

    assert(scores->moves->moves[1]->from == f1);
    assert(scores->moves->moves[1]->destination == g1);
    assert(scores->moves->moves[1]->piece->c == white);
    assert(scores->moves->moves[1]->piece->type == castle);

    assert(scores->moves->moves[2]->from == e4);
    assert(scores->moves->moves[2]->destination == f2);
    assert(scores->moves->moves[2]->piece->c == black);
    assert(scores->moves->moves[2]->piece->type == knight);
}

void test_11_june_2024() {
    Board* board = set_board_notation("wpa4 wpb2 wpe4 wpf2 wpg5 wkc2 whc3 wch1 wch6 wqh3 bkg7 bqc8 bca8 bch8 bbe7 bhf3 bpa5 bpc6 bpd6 bpe5 bpf7 bpg6 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(200, 3, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    print_scores(scores);
}

void test_11_june_2024_partial() {
    // holy crap it really should be detecting a mate in 1...
    Board* board = set_board_notation("wpa4 wpb2 wpe4 wpf2 wpg5 wkc2 whc3 wch1 wch8 wqh3 bkg7 bqc8 bca8 bbe7 bhf3 bpa5 bpc6 bpd6 bpe5 bpf7 bpg6 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(200, 2, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    print_scores(scores);

    assert(scores->eval == MAX_SCORE);
}

void test_puzzle_trap_bishop() {
    Board* board = set_board_notation("wkd2 wca1 wch1 whf3 whg3 wbd3 wpa2 wpb2 wpe4 wpf2 wpg2 wph4 bkg8 bcf8 bca8 bhb4 bhe7 bbg6 bpa6 bpb5 bpd4 bpf7 bpg7 bph7 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(200, 2, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->moves->moves[0]->from == h4);
    assert(scores->moves->moves[0]->destination == h5);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == pawn);
}

void test_puzzle_win_knight_because_pawn_fork() {
    Board* board = set_board_notation("wkg1 wqd1 wce1 wca1 wbc4 wbc1 whe4 whg6 wph2 wpg2 wpf2 wpc2 wpb2 wpa2 bkg8 bqd8 bcf7 bca8 bbc8 bbf6 bhb8 bhc5 bph6 bpg7 bpe6 bpd7 bpc7 bpb7 bpa6 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(200, 2, black);
    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));
    assert(test_board == board->bitboard);

    print_scores(scores);

    assert(scores->moves->moves[0]->from == c5);
    assert(scores->moves->moves[0]->destination == e4);
    assert(scores->moves->moves[0]->piece->c == black);
    assert(scores->moves->moves[0]->piece->type == knight);
}



void test_draw_by_repetition_1() {
    Board* board = init_board();
    assert(!draw_by_repetition(board));

    U64 hash_original = board->hash_value;

    // 1: white knight b1 -> a3
    Moves* moves = get_all_moves_for_colour(board, white);
    assert(moves->moves[16]->from == b1);
    assert(moves->moves[16]->destination == a3);
    Piece* killed = pretend_move(board, moves->moves[16]);
    hash_and_save(board, moves->moves[16], killed, 0);
    assert(!draw_by_repetition(board));
    assert(board->lm_length == 1);
    assert(hash_original != board->hash_value);

    // 1: black knight b8 -> a6
    moves = get_all_moves_for_colour(board, black);
    assert(moves->moves[0]->from == b8);
    assert(moves->moves[0]->destination == a6);
    killed = pretend_move(board, moves->moves[0]);
    hash_and_save(board, moves->moves[0], killed, 0);
    assert(!draw_by_repetition(board));
    assert(hash_original != board->hash_value);

    // 1: white knight a3 -> b1
    moves = get_all_moves_for_colour(board, white);
    assert(moves->moves[17]->from == a3);
    assert(moves->moves[17]->destination == b1);
    killed = pretend_move(board, moves->moves[17]);
    hash_and_save(board, moves->moves[17], killed, 0);
    assert(!draw_by_repetition(board));
    assert(hash_original != board->hash_value);

    // 1: black knight a6 -> b8
    moves = get_all_moves_for_colour(board, black);
    assert(moves->moves[1]->from == a6);
    assert(moves->moves[1]->destination == b8);
    killed = pretend_move(board, moves->moves[1]);
    hash_and_save(board, moves->moves[1], killed, 0);
    assert(!draw_by_repetition(board));
    assert(hash_original == board->hash_value);


    // 2: white knight b1 -> a3
    moves = get_all_moves_for_colour(board, white);
    assert(moves->moves[16]->from == b1);
    assert(moves->moves[16]->destination == a3);
    killed = pretend_move(board, moves->moves[16]);
    hash_and_save(board, moves->moves[16], killed, 0);
    assert(!draw_by_repetition(board));
    assert(hash_original != board->hash_value);

    // 2: black knight b8 -> a6
    moves = get_all_moves_for_colour(board, black);
    assert(moves->moves[0]->from == b8);
    assert(moves->moves[0]->destination == a6);
    killed = pretend_move(board, moves->moves[0]);
    hash_and_save(board, moves->moves[0], killed, 0);
    assert(!draw_by_repetition(board));
    assert(hash_original != board->hash_value);

    // 2: white knight a3 -> b1
    moves = get_all_moves_for_colour(board, white);
    assert(moves->moves[17]->from == a3);
    assert(moves->moves[17]->destination == b1);
    killed = pretend_move(board, moves->moves[17]);
    hash_and_save(board, moves->moves[17], killed, 0);
    assert(!draw_by_repetition(board));
    assert(hash_original != board->hash_value);

    // 2: black knight a6 -> b8
    moves = get_all_moves_for_colour(board, black);
    assert(moves->moves[1]->from == a6);
    assert(moves->moves[1]->destination == b8);
    killed = pretend_move(board, moves->moves[1]);
    hash_and_save(board, moves->moves[1], killed, 0);
    assert(board->lm_length == 8);
    assert(!draw_by_repetition(board));
    assert(hash_original == board->hash_value);



    // 3: white knight b1 -> a3
    // should be on this move (if reptition starts from start)
    moves = get_all_moves_for_colour(board, white);
    assert(moves->moves[16]->from == b1);
    assert(moves->moves[16]->destination == a3);
    killed = pretend_move(board, moves->moves[16]);
    hash_and_save(board, moves->moves[16], killed, 0);
    assert(draw_by_repetition(board));
    assert(hash_original != board->hash_value);

    // 3: black knight b8 -> a6
    moves = get_all_moves_for_colour(board, black);
    assert(moves->moves[0]->from == b8);
    assert(moves->moves[0]->destination == a6);
    killed = pretend_move(board, moves->moves[0]);
    hash_and_save(board, moves->moves[0], killed, 0);
    assert(draw_by_repetition(board));
    assert(hash_original != board->hash_value);

    // 3: white knight a3 -> b1
    moves = get_all_moves_for_colour(board, white);
    assert(moves->moves[17]->from == a3);
    assert(moves->moves[17]->destination == b1);
    killed = pretend_move(board, moves->moves[17]);
    hash_and_save(board, moves->moves[17], killed, 0);
    assert(draw_by_repetition(board));
    assert(hash_original != board->hash_value);

    // 3: black knight a6 -> b8
    moves = get_all_moves_for_colour(board, black);
    assert(moves->moves[1]->from == a6);
    assert(moves->moves[1]->destination == b8);
    killed = pretend_move(board, moves->moves[1]);
    hash_and_save(board, moves->moves[1], killed, 0);
    assert(hash_original == board->hash_value);
    assert(draw_by_repetition(board));

}

void test_draw_by_repetition_best_line() {
    Board* board = set_board_notation("wka1 wqf2 bcb8 bcc7 bkh1 bpg2 bpd6 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->pieces[black][KING_INDEX(black)]->no_moves += 1;

    // how the fuck can black not see mate with b=5?
    Grapher* grapher = init_grapher(6, 5, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));

    assert(scores->eval == 0);
    assert(scores->moves->length >= 6);
    assert((scores->moves->moves[0]->destination == h4 || scores->moves->moves[0]->destination == e1) && scores->moves->moves[0]->piece->type == queen);
    assert((scores->moves->moves[2]->destination == h4 || scores->moves->moves[2]->destination == e1) && scores->moves->moves[2]->piece->type == queen);
    assert((scores->moves->moves[4]->destination == h4 || scores->moves->moves[4]->destination == e1) && scores->moves->moves[4]->piece->type == queen);
}


void test_hashing_1() {
    Board* board = set_board_notation("wpe4 bcf5 ");
    Moves* moves = get_all_moves_for_colour(board, white);

    U64 hash = board->hash_value;
    hash_move_piece(board, moves->moves[0], NULL);
    hash_move_piece(board, moves->moves[0], NULL);
    assert(board->hash_value == hash);

    Piece* killed = pretend_move(board, moves->moves[1]);
    hash = board->hash_value;
    hash_move_piece(board, moves->moves[1], killed);
    hash_move_piece(board, moves->moves[1], killed);
    assert(board->hash_value == hash);
}

void test_hashing_2() {
    Board* board = set_board_notation("wkd4 bcb7 bcc8 ");
    Moves* moves = get_all_moves_for_colour(board, black);
    U64 hash = board->hash_value;

    Piece* killed = pretend_move(board, moves->moves[2]);
    hash_move_piece(board, moves->moves[2], killed);

    hash_move_piece(board, moves->moves[2], killed);
    undo_pretend_move(board, moves->moves[2], killed);
    assert(hash == board->hash_value);
}

void test_hashing_graph_simulation() {
    Board* board = set_board_notation("wkd4 bcb7 bcc8 ");
    Moves* moves = get_all_moves_for_colour(board, black);
    U64 hash1 = board->hash_value;

    Piece* killed = pretend_move(board, moves->moves[2]);
    hash_move_piece(board, moves->moves[2], killed);
    U64 hash2 = board->hash_value;


    Moves* response = get_all_moves_for_colour(board, black);

    Piece* killed_response = pretend_move(board, response->moves[0]);
    hash_move_piece(board, response->moves[0], killed_response);

    hash_move_piece(board, response->moves[0], killed_response);
    undo_pretend_move(board, response->moves[0], killed_response);
    assert(hash2 == board->hash_value);

    hash_move_piece(board, moves->moves[2], killed);
    undo_pretend_move(board, moves->moves[2], killed);
    assert(hash1 == board->hash_value);
}

void test_hashing_en_passant() {
    Board* board = init_board();

    Moves* moves = get_all_moves_for_colour(board, white);
    assert(moves->moves[0]->destination == a4);
    assert(moves->moves[0]->from == a2);
    assert(moves->moves[0]->piece->type == pawn);
    U64 hash_start = board->hash_value;

    Piece* killed = pretend_move(board, moves->moves[0]);
    hash_and_save(board, moves->moves[0], killed, 0);
    assert(moves->moves[0]->piece->cell == a4);
    assert(board->map[a4] == moves->moves[0]->piece);

    U64 hash_advance_two = board->hash_value;
    U64 bitboard_advance_two = board->bitboard;

    undo_hash(board, moves->moves[0], killed);
    undo_pretend_move(board, moves->moves[0], killed);
    
    assert(board->hash_value == hash_start);


    assert(moves->moves[1]->destination == a3);
    assert(moves->moves[1]->from == a2);
    assert(moves->moves[1]->piece->type == pawn);

    killed = pretend_move(board, moves->moves[1]);
    hash_and_save(board, moves->moves[1], killed, 0);

    // switch colour back instead of moving a black piece
    board->hash_value ^= board->key_mover;

    Moves* moves_2 = get_all_moves_for_colour(board, white);
    assert(moves_2->moves[0]->destination == a4);
    assert(moves_2->moves[0]->from == a3);
    assert(moves_2->moves[0]->piece->type == pawn);

    killed = pretend_move(board, moves_2->moves[0]);
    hash_and_save(board, moves_2->moves[0], killed, 0);
    assert(moves_2->moves[0]->piece->cell == a4);
    assert(board->map[a4] == moves_2->moves[0]->piece);

    // should be different -- first one had en passant rights, now they're foresaken.
    assert(board->hash_value != hash_advance_two);
    assert(bitboard_advance_two == board->bitboard);
    

}

void test_hashing_different_move_order() {
    Board* board = init_board();
    Moves* moves = get_all_moves_for_colour(board, white);
    assert(moves->moves[1]->destination == a3);
    assert(moves->moves[1]->from == a2);
    assert(moves->moves[1]->piece->type == pawn);
    U64 hash_start = board->hash_value;
    U64 bitboard_start = board->bitboard;

    Piece* killed = pretend_move(board, moves->moves[1]);
    hash_and_save(board, moves->moves[1], killed, 0);
    assert(board->map[a3] == moves->moves[1]->piece);

    Moves* moves_black = get_all_moves_for_colour(board, black);
    Piece* killed_black = pretend_move(board, moves_black->moves[0]);
    hash_and_save(board, moves_black->moves[0], killed_black, 0);
    
    Moves* moves_white_2 = get_all_moves_for_colour(board, white);
    Piece* killed_white_2 = pretend_move(board, moves_white_2->moves[2]);
    hash_and_save(board, moves_white_2->moves[2], killed_white_2, 0);
    assert(board->map[b3] == moves_white_2->moves[2]->piece);

    U64 final_hash = board->hash_value;
    U64 final_bitboard = board->bitboard;

    undo_hash(board, moves_white_2->moves[2], killed_white_2);
    undo_pretend_move(board, moves_white_2->moves[2], killed_white_2);
    
    undo_hash(board, moves_black->moves[0], killed_black);
    undo_pretend_move(board, moves_black->moves[0], killed_black);

    undo_hash(board, moves->moves[1], killed);
    undo_pretend_move(board, moves->moves[1], killed);

    assert(board->hash_value == hash_start);
    assert(board->bitboard == bitboard_start);

    killed = pretend_move(board, moves->moves[3]);
    hash_and_save(board, moves->moves[3], killed, 0);
    assert(board->map[b3] == moves->moves[3]->piece);

    killed_black = pretend_move(board, moves_black->moves[0]);
    hash_and_save(board, moves_black->moves[0], killed_black, 0);

    moves_white_2 = get_all_moves_for_colour(board, white);
    killed_white_2 = pretend_move(board, moves_white_2->moves[1]);
    hash_and_save(board, moves_white_2->moves[1], killed_white_2, 0);
    assert(board->map[a3] == moves_white_2->moves[1]->piece);

    assert(board->bitboard == final_bitboard);
    assert(board->hash_value == final_hash);
}


void test_hashing() {
    test_hashing_1();
    test_hashing_2();
    test_hashing_graph_simulation();
    test_hashing_en_passant();
    test_hashing_different_move_order();
}



void test_queen_moves() {
    Board* board = init_board();
    clear_board(board);
    Piece* piece = calloc(1, sizeof(Piece));
    piece->cell = d4;
    U64 mask = get_queen_mask(board, piece);
    U64 test_board = 0ULL;
    set_bit(test_board, d8);
    set_bit(test_board, d7);
    set_bit(test_board, d6);
    set_bit(test_board, d5);
    set_bit(test_board, d3);
    set_bit(test_board, d2);
    set_bit(test_board, d1);
    set_bit(test_board, a4);
    set_bit(test_board, b4);
    set_bit(test_board, c4);
    set_bit(test_board, e4);
    set_bit(test_board, f4);
    set_bit(test_board, g4);
    set_bit(test_board, h4);
    set_bit(test_board, a7);
    set_bit(test_board, b6);
    set_bit(test_board, c5);
    set_bit(test_board, e3);
    set_bit(test_board, f2);
    set_bit(test_board, g1);
    set_bit(test_board, c3);
    set_bit(test_board, b2);
    set_bit(test_board, a1);
    set_bit(test_board, e5);
    set_bit(test_board, f6);
    set_bit(test_board, g7);
    set_bit(test_board, h8);
    assert(mask == test_board);

    clear_board(board);
    piece->cell = a8;
    mask = get_queen_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, a4);
    set_bit(test_board, a7);
    set_bit(test_board, a6);
    set_bit(test_board, a5);
    set_bit(test_board, a3);
    set_bit(test_board, a2);
    set_bit(test_board, a1);
    set_bit(test_board, d8);
    set_bit(test_board, b8);
    set_bit(test_board, c8);
    set_bit(test_board, e8);
    set_bit(test_board, f8);
    set_bit(test_board, g8);
    set_bit(test_board, h8);
    set_bit(test_board, b7);
    set_bit(test_board, c6);
    set_bit(test_board, d5);
    set_bit(test_board, e4);
    set_bit(test_board, f3);
    set_bit(test_board, g2);
    set_bit(test_board, h1);
    assert(mask == test_board);

}

void test_bishop_moves() {
    Board* board = init_board();
    clear_board(board);
    // U64 bitboard = 0ULL;
    Piece* piece = calloc(1, sizeof(Piece));
    piece->cell = c4;
    U64 mask = get_bishop_mask(board, piece);
    U64 test_board = 0ULL;
    set_bit(test_board, a6);
    set_bit(test_board, b5);
    set_bit(test_board, b3);
    set_bit(test_board, a2);
    set_bit(test_board, d5);
    set_bit(test_board, e6);
    set_bit(test_board, f7);
    set_bit(test_board, g8);
    set_bit(test_board, d3);
    set_bit(test_board, e2);
    set_bit(test_board, f1);
    assert(mask == test_board);

    clear_board(board);
    piece->cell = f5;
    mask = get_bishop_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, g6);
    set_bit(test_board, h7);
    set_bit(test_board, g4);
    set_bit(test_board, h3);
    set_bit(test_board, e6);
    set_bit(test_board, d7);
    set_bit(test_board, c8);
    set_bit(test_board, e4);
    set_bit(test_board, d3);
    set_bit(test_board, c2);
    set_bit(test_board, b1);
    assert(mask == test_board);

    clear_board(board);
    set_bit(board->bitboard, d4);
    set_bit(board->bitboard, f4);
    piece->cell = e3;
    mask = get_bishop_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, d4);
    set_bit(test_board, f4);
    set_bit(test_board, f2);
    set_bit(test_board, d2);
    set_bit(test_board, c1);
    set_bit(test_board, g1);
    assert(mask == test_board);

    clear_board(board);
    set_bit(board->bitboard, b4);
    set_bit(board->bitboard, f4);
    piece->cell = d6;
    mask = get_bishop_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, b8);
    set_bit(test_board, c7);
    set_bit(test_board, e7);
    set_bit(test_board, f8);
    set_bit(test_board, c5);
    set_bit(test_board, e5);
    set_bit(test_board, b4);
    set_bit(test_board, f4);
    assert(mask == test_board);
    
}

void test_castle_moves() {
    Board* board = init_board();
    clear_board(board);
    // U64 bitboard = 0ULL;
    set_bit(board->bitboard, d6);
    set_bit(board->bitboard, d2);
    set_bit(board->bitboard, c4);
    set_bit(board->bitboard, g4);
    Piece* piece = calloc(1, sizeof(Piece));
    piece->cell = d4;
    U64 mask = get_castle_mask(board, piece);
    U64 test_board = 0ULL;
    set_bit(test_board, d6);
    set_bit(test_board, d5);
    set_bit(test_board, d3);
    set_bit(test_board, d2);
    set_bit(test_board, c4);
    set_bit(test_board, e4);
    set_bit(test_board, f4);
    set_bit(test_board, g4);
    assert(mask == test_board);
}

void test_knight_moves() {
    Board* board = init_board();
    clear_board(board);
    // U64 bitboard = 0ULL;
    Piece* piece = calloc(1, sizeof(Piece));
    piece->cell = d4;
    U64 mask = get_knight_mask(board, piece);
    U64 test_board = 0ULL;
    set_bit(test_board, c6);
    set_bit(test_board, e6);
    set_bit(test_board, b5);
    set_bit(test_board, f5);
    set_bit(test_board, b3);
    set_bit(test_board, f3);
    set_bit(test_board, c2);
    set_bit(test_board, e2);
    assert(mask == test_board);

    clear_board(board);
    piece->cell = c6;
    mask = get_knight_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, b8);
    set_bit(test_board, a7);
    set_bit(test_board, d8);
    set_bit(test_board, e7);
    set_bit(test_board, a5);
    set_bit(test_board, e5);
    set_bit(test_board, b4);
    set_bit(test_board, d4);
    assert(mask == test_board);

    clear_board(board);
    piece->cell = c7;
    mask = get_knight_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, a8);
    set_bit(test_board, e8);
    set_bit(test_board, a6);
    set_bit(test_board, e6);
    set_bit(test_board, b5);
    set_bit(test_board, d5);
    assert(mask == test_board);

    clear_board(board);
    piece->cell = b7;
    mask = get_knight_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, d8);
    set_bit(test_board, d6);
    set_bit(test_board, a5);
    set_bit(test_board, c5);
    assert(mask == test_board);

    clear_board(board);
    piece->cell = h1;
    mask = get_knight_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, g3);
    set_bit(test_board, f2);
    assert(mask == test_board);
}

void test_king_moves() {
    Board* board = init_board();
    clear_board(board);
    // U64 bitboard = 0ULL;
    Piece* piece = calloc(1, sizeof(Piece));
    piece->cell = c4;
    U64 king_mask = get_king_mask(board, piece);
    U64 test_board = 0ULL;
    set_bit(test_board, b5);
    set_bit(test_board, c5);
    set_bit(test_board, d5);
    set_bit(test_board, b4);
    set_bit(test_board, d4);
    set_bit(test_board, b3);
    set_bit(test_board, c3);
    set_bit(test_board, d3);
    assert(king_mask == test_board);

    clear_board(board);
    piece->cell = a1;
    king_mask = get_king_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, a2);
    set_bit(test_board, b2);
    set_bit(test_board, b1);
    assert(king_mask == test_board);

    clear_board(board);
    piece->cell = h1;
    king_mask = get_king_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, g2);
    set_bit(test_board, h2);
    set_bit(test_board, g1);
    assert(king_mask == test_board);

    clear_board(board);
    piece->cell = a8;
    king_mask = get_king_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, b8);
    set_bit(test_board, b7);
    set_bit(test_board, a7);
    assert(king_mask == test_board);

    clear_board(board);
    piece->cell = h8;
    king_mask = get_king_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, g8);
    set_bit(test_board, g7);
    set_bit(test_board, h7);
    assert(king_mask == test_board);

    clear_board(board);
    piece->cell = c1;
    king_mask = get_king_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, d1);
    set_bit(test_board, b1);
    set_bit(test_board, b2);
    set_bit(test_board, c2);
    set_bit(test_board, d2);
    assert(king_mask == test_board);

    clear_board(board);
    piece->cell = b2;
    king_mask = get_king_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, a1);
    set_bit(test_board, b1);
    set_bit(test_board, c1);
    set_bit(test_board, a2);
    set_bit(test_board, c2);
    set_bit(test_board, a3);
    set_bit(test_board, b3);
    set_bit(test_board, c3);
    assert(king_mask == test_board);
}

void test_pawn_moves() {

    Piece* piece = calloc(1, sizeof(Piece));
    piece->cell = d2;
    piece->c = white;

    // can move two spaces
    Board* board = init_board();
    clear_board(board);
    // U64 bitboard = 0ULL;
    U64 pawn_move_mask = get_pawn_move_mask(board, piece);
    U64 test_board = 0ULL;
    set_bit(test_board, d3);
    set_bit(test_board, d4);
    assert(pawn_move_mask == test_board);
    
    // can't move any spaces
    clear_board(board);
    set_bit(board->bitboard, d3);
    pawn_move_mask = get_pawn_move_mask(board, piece);
    assert(pawn_move_mask == 0ULL);

    // can only move one space
    clear_board(board);
    set_bit(board->bitboard, d4);
    pawn_move_mask = get_pawn_move_mask(board, piece);
    test_board = 0ULL;
    set_bit(test_board, d3);
    assert(pawn_move_mask == test_board);
}

void test_pawn_attacks() {

    Piece* piece = calloc(1, sizeof(Piece));
    piece->cell = d1;
    piece->c = black;
    
    Board* board = init_board();
    clear_board(board);
    // prevent black going down over
    U64 pawn_attack_mask = get_pawn_attack_mask(board, piece);
    assert(pawn_attack_mask == 0ULL);
    piece->cell = a1;
    pawn_attack_mask = get_pawn_attack_mask(board, piece);
    assert(pawn_attack_mask == 0ULL);
    piece->cell = h1;
    pawn_attack_mask = get_pawn_attack_mask(board, piece);
    assert(pawn_attack_mask == 0ULL);

    // prevent white going up over
    piece->c = white;
    piece->cell = d8;
    pawn_attack_mask = get_pawn_attack_mask(board, piece);
    assert(pawn_attack_mask == 0ULL);
    piece->cell = a8;
    pawn_attack_mask = get_pawn_attack_mask(board, piece);
    assert(pawn_attack_mask == 0ULL);
    piece->cell = h8;
    pawn_attack_mask = get_pawn_attack_mask(board, piece);
    assert(pawn_attack_mask == 0ULL);

    // prevent left / right spill overs
    set_bit(board->bitboard, b3);
    set_bit(board->bitboard, h3);
    piece->cell = a2;
    pawn_attack_mask = get_pawn_attack_mask(board, piece);
    U64 test = 0ULL;
    set_bit(test, b3);
    assert(pawn_attack_mask == test);

    clear_board(board);
    set_bit(board->bitboard, b8);
    set_bit(board->bitboard, h8);
    piece->cell = a7;
    pawn_attack_mask = get_pawn_attack_mask(board, piece);
    test = 0ULL;
    set_bit(test, b8);
    assert(pawn_attack_mask == test);

    clear_board(board);
    set_bit(board->bitboard, g3);
    set_bit(board->bitboard, a3);
    piece->cell = h2;
    pawn_attack_mask = get_pawn_attack_mask(board, piece);
    test = 0ULL;
    set_bit(test, g3);
    assert(pawn_attack_mask == test);

    // hmmm
    clear_board(board);
    set_bit(board->bitboard, a2);
    set_bit(board->bitboard, b3);
    piece->cell = b3;
    piece->c = black;
    pawn_attack_mask = get_pawn_attack_mask(board, piece);
    test = 0ULL;
    set_bit(test, a2);
    assert(pawn_attack_mask == test);

    U64 mask = get_pawn_move_mask(board, piece);
    test = 0ULL;
    set_bit(test, b2);
    assert(mask == test);
}

void test_pawn_en_passant_1() {
    Piece* piece = calloc(1, sizeof(Piece));
    piece->cell = d4;
    piece->c = black;
    
    Board* board = init_board();
    clear_board(board);

    board->pieces[white][PAWN_INDEX(white, 2)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 2)]->cell = c4;
    board->map[c4] = board->pieces[white][PAWN_INDEX(white, 2)];
    set_bit(board->bitboard, c4);
    
    Move* move = calloc(1, sizeof(Move));
    move->piece = board->map[c4];
    move->from = c2;
    move->destination = c4;
    board->last_moved[board->lm_length] = move;
    // board->last_moved[0] = board->pieces[white][PAWN_INDEX(white, 2)];
    board->lm_length += 1;

    U64 pawn_attack_mask = get_pawn_attack_mask(board, piece);
    U64 test = 0ULL;
    set_bit(test, c3);
    assert(pawn_attack_mask == test);
}

void test_pawn_en_passant_2() {
    
    Board* board = init_board();

    Move* move = calloc(1, sizeof(Move));
    move->piece = board->pieces[white][PAWN_INDEX(white, 1)];
    move->destination = b4;
    move->from = move->piece->cell;
    pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 0)];
    move->destination = a5;
    move->from = move->piece->cell;
    pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 1)];
    move->destination = b5;
    move->from = move->piece->cell;
    pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 2)];
    move->destination = c5;
    move->from = move->piece->cell;
    pretend_move(board, move);
    
    U64 pawn_attack_mask = get_pawn_attack_mask(board, board->pieces[white][PAWN_INDEX(white, 1)]);
    U64 test = 0ULL;
    set_bit(test, c6);
    assert(pawn_attack_mask == test);

    move->piece = board->pieces[white][PAWN_INDEX(white, 7)];
    move->destination = h4;
    move->from = move->piece->cell;
    pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 6)];
    move->destination = g5;
    move->from = move->piece->cell;
    pretend_move(board, move);

    pawn_attack_mask = get_pawn_attack_mask(board, board->pieces[white][PAWN_INDEX(white, 1)]);
    assert(pawn_attack_mask == 0ULL);
}

void test_pawn_en_passant_3() {
    Board* board = init_board();

    Move* move = calloc(1, sizeof(Move));
    move->piece = board->pieces[white][PAWN_INDEX(white, 1)];
    move->destination = b4;
    move->from = move->piece->cell;
    Piece* killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 0)];
    move->destination = a5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 1)];
    move->destination = b5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 2)];
    move->destination = c5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    Moves* moves = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[white][PAWN_INDEX(white, 1)], moves);

    assert(moves->length == 2);
    assert(moves->moves[0]->destination == b6);
    assert(moves->moves[0]->from == b5);
    assert(moves->moves[1]->destination == c6);
    assert(moves->moves[1]->from == b5);

    assert(board->map[b5] == moves->moves[0]->piece && board->map[b5] == moves->moves[1]->piece);
    assert(get_bit(board->bitboard, c5));
    assert(get_bit(board->bitboard, b5));
    assert(!get_bit(board->bitboard, c6));
    assert(board->map[c5] != NULL);
    U64 test_board = board->bitboard;

    killed = pretend_move(board, moves->moves[1]);

    assert(killed);
    assert(killed->type == pawn);
    assert(killed->alive == false);
    assert(killed->cell == c5);
    assert(board->map[c5] == NULL);
    assert(!get_bit(board->bitboard, c5));
    assert(get_bit(board->bitboard, c6));
    assert(moves->moves[1]->piece->cell == c6);
    assert(board->map[c6] == moves->moves[1]->piece);

    undo_pretend_move(board, moves->moves[1], killed);

    assert(board->map[b5] == moves->moves[1]->piece);
    assert(get_bit(board->bitboard, c5));
    assert(get_bit(board->bitboard, b5));
    assert(!get_bit(board->bitboard, c6));
    assert(board->map[c5] == killed);
    assert(killed->alive);
    assert(killed->cell == c5);
    assert(killed->type == pawn);
    assert(test_board == board->bitboard);

    Moves* moves_2 = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[white][PAWN_INDEX(white, 1)], moves_2);

    assert(moves_2->length == 2);
    assert(moves_2->moves[0]->destination == b6);
    assert(moves_2->moves[0]->from == b5);
    assert(moves_2->moves[1]->destination == c6);
    assert(moves_2->moves[1]->from == b5);
}

void test_pawn_en_passant_4() {
    Board* board = init_board();

    Move* move = calloc(1, sizeof(Move));
    move->piece = board->pieces[white][PAWN_INDEX(white, 1)];
    move->destination = b4;
    move->from = move->piece->cell;
    Piece* killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 2)];
    move->destination = c5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 1)];
    move->destination = b5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 0)];
    move->destination = a5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    Moves* moves = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[white][PAWN_INDEX(white, 1)], moves);

    assert(moves->length == 2);
    assert(moves->moves[0]->destination == a6);
    assert(moves->moves[0]->from == b5);
    assert(moves->moves[1]->destination == b6);
    assert(moves->moves[1]->from == b5);
    assert(moves->moves[1]->piece->cell == b5);

    assert(board->map[b5] == moves->moves[0]->piece && board->map[b5] == moves->moves[1]->piece);
    assert(get_bit(board->bitboard, a5));
    assert(get_bit(board->bitboard, b5));
    assert(!get_bit(board->bitboard, c6));
    assert(!get_bit(board->bitboard, a6));
    assert(board->map[a5] != NULL);
    U64 test_board = board->bitboard;

    killed = pretend_move(board, moves->moves[0]);

    assert(killed);
    assert(killed->type == pawn);
    assert(killed->alive == false);
    assert(killed->cell == a5);
    assert(board->map[a5] == NULL);
    assert(!get_bit(board->bitboard, a5));
    assert(get_bit(board->bitboard, a6));
    assert(moves->moves[0]->piece->cell == a6);
    assert(board->map[a6] == moves->moves[0]->piece);

    undo_pretend_move(board, moves->moves[0], killed);

    assert(board->map[b5] == moves->moves[0]->piece);
    assert(moves->moves[0]->piece->cell == b5);
    assert(get_bit(board->bitboard, a5));
    assert(get_bit(board->bitboard, b5));
    assert(!get_bit(board->bitboard, c6));
    assert(!get_bit(board->bitboard, a6));
    assert(board->map[a5] == killed);
    assert(killed->alive);
    assert(killed->cell == a5);
    assert(killed->type == pawn);
    assert(test_board == board->bitboard);

    Moves* moves_2 = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[white][PAWN_INDEX(white, 1)], moves_2);

    assert(moves_2->length == 2);
    assert(moves_2->moves[0]->destination == a6);
    assert(moves_2->moves[0]->from == b5);
    assert(moves_2->moves[1]->destination == b6);
    assert(moves_2->moves[1]->from == b5);
    assert(moves_2->moves[1]->piece->cell == b5);
}

void test_pawn_en_passant_5() {
    Board* board = init_board();

    Move* move = calloc(1, sizeof(Move));
    move->piece = board->pieces[white][PAWN_INDEX(white, 0)];
    assert(move->piece->type == pawn);
    move->destination = a4;
    move->from = move->piece->cell;
    Piece* killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 1)];
    assert(move->piece->type == pawn);
    move->destination = b6;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 0)];
    assert(move->piece->type == pawn);
    move->destination = a5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 7)];
    assert(move->piece->type == pawn);
    move->destination = h5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    Moves* moves = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[white][PAWN_INDEX(white, 0)], moves);

    assert(moves->length == 2);
    assert(moves->moves[0]->destination == a6);
    assert(moves->moves[0]->from == a5);
    assert(moves->moves[1]->destination == b6);
    assert(moves->moves[1]->from == a5);
    assert(moves->moves[1]->piece->cell == a5);

    assert(board->map[a5] == moves->moves[0]->piece && board->map[a5] == moves->moves[1]->piece);
    assert(get_bit(board->bitboard, a5));
    assert(get_bit(board->bitboard, b6));
    assert(get_bit(board->bitboard, h5));
    assert(!get_bit(board->bitboard, a6));
    assert(board->map[h5] != NULL);
    U64 test_board = board->bitboard;

    killed = pretend_move(board, moves->moves[0]);

    assert(!killed);
    assert(board->map[a5] == NULL);
    assert(board->map[h5] != NULL);
    assert(!get_bit(board->bitboard, a5));
    assert(get_bit(board->bitboard, a6));
    assert(get_bit(board->bitboard, h5));
    assert(moves->moves[0]->piece->cell == a6);
    assert(board->map[a6] == moves->moves[0]->piece);

    undo_pretend_move(board, moves->moves[0], killed);

    assert(board->map[a5] == moves->moves[0]->piece);
    assert(moves->moves[0]->piece->cell == a5);
    assert(get_bit(board->bitboard, a5));
    assert(get_bit(board->bitboard, b6));
    assert(get_bit(board->bitboard, h5));
    assert(!get_bit(board->bitboard, a6));
    assert(board->map[h5] != NULL);
    assert(test_board == board->bitboard);

    killed = pretend_move(board, moves->moves[1]);

    assert(killed);
    assert(killed->cell == b6);
    assert(killed->alive == false);
    assert(board->map[a5] == NULL);
    assert(board->map[h5] != NULL);
    assert(!get_bit(board->bitboard, a5));
    assert(!get_bit(board->bitboard, a6));
    assert(get_bit(board->bitboard, h5));
    assert(get_bit(board->bitboard, b6));
    assert(moves->moves[1]->piece->cell == b6);
    assert(board->map[b6] == moves->moves[1]->piece);

    undo_pretend_move(board, moves->moves[1], killed);

    assert(board->map[a5] == moves->moves[1]->piece);
    assert(moves->moves[1]->piece->cell == a5);
    assert(get_bit(board->bitboard, a5));
    assert(get_bit(board->bitboard, b6));
    assert(get_bit(board->bitboard, h5));
    assert(!get_bit(board->bitboard, a6));
    assert(board->map[h5] != NULL);
    assert(test_board == board->bitboard);
    assert(board->map[b6] == killed);
    assert(killed->alive);
    assert(killed->cell == b6);
    assert(killed->type == pawn);

    Moves* moves_2 = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[white][PAWN_INDEX(white, 0)], moves_2);

    assert(moves_2->length == 2);
    assert(moves_2->moves[0]->destination == a6);
    assert(moves_2->moves[0]->from == a5);
    assert(moves_2->moves[1]->destination == b6);
    assert(moves_2->moves[1]->from == a5);
    assert(moves_2->moves[1]->piece->cell == a5);
}

void test_pawn_en_passant_6() {
    Board* board = init_board();

    Move* move = calloc(1, sizeof(Move));
    move->piece = board->pieces[white][PAWN_INDEX(white, 7)];
    assert(move->piece->type == pawn);
    move->destination = h4;
    move->from = move->piece->cell;
    Piece* killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 6)];
    assert(move->piece->type == pawn);
    move->destination = g6;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 7)];
    assert(move->piece->type == pawn);
    move->destination = h5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 0)];
    assert(move->piece->type == pawn);
    move->destination = a5;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    Moves* moves = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[white][PAWN_INDEX(white, 7)], moves);

    assert(moves->length == 2);
    assert(moves->moves[0]->destination == g6);
    assert(moves->moves[0]->from == h5);
    assert(moves->moves[1]->destination == h6);
    assert(moves->moves[1]->from == h5);
    assert(moves->moves[1]->piece->cell == h5);

    assert(board->map[h5] == moves->moves[0]->piece && board->map[h5] == moves->moves[1]->piece);
    assert(get_bit(board->bitboard, h5));
    assert(get_bit(board->bitboard, g6));
    assert(get_bit(board->bitboard, a5));
    assert(!get_bit(board->bitboard, h6));
    assert(board->map[a5] != NULL);
    U64 test_board = board->bitboard;

    killed = pretend_move(board, moves->moves[0]);

    assert(killed);
    assert(killed->cell == g6);
    assert(killed->alive == false);
    assert(killed->type == pawn);
    assert(board->map[a5] != NULL);
    assert(board->map[h5] == NULL);
    assert(!get_bit(board->bitboard, h5));
    assert(!get_bit(board->bitboard, h6));
    assert(get_bit(board->bitboard, g6));
    assert(get_bit(board->bitboard, a5));
    assert(moves->moves[0]->piece->cell == g6);
    assert(board->map[g6] == moves->moves[0]->piece);

    undo_pretend_move(board, moves->moves[0], killed);

    assert(board->map[h5] == moves->moves[0]->piece);
    assert(moves->moves[0]->piece->cell == h5);
    assert(get_bit(board->bitboard, h5));
    assert(get_bit(board->bitboard, g6));
    assert(get_bit(board->bitboard, a5));
    assert(!get_bit(board->bitboard, h6));
    assert(board->map[a5] != NULL);
    assert(test_board == board->bitboard);
    assert(killed->alive);
    assert(killed->cell == g6);
    assert(killed->type == pawn);

    killed = pretend_move(board, moves->moves[1]);

    assert(!killed);
    assert(board->map[h5] == NULL);
    assert(board->map[a5] != NULL);
    assert(!get_bit(board->bitboard, h5));
    assert(get_bit(board->bitboard, a5));
    assert(get_bit(board->bitboard, g6));
    assert(get_bit(board->bitboard, h6));
    assert(moves->moves[1]->piece->cell == h6);
    assert(board->map[h6] == moves->moves[1]->piece);

    undo_pretend_move(board, moves->moves[1], killed);

    assert(board->map[h5] == moves->moves[1]->piece);
    assert(moves->moves[1]->piece->cell == h5);
    assert(get_bit(board->bitboard, h5));
    assert(get_bit(board->bitboard, g6));
    assert(get_bit(board->bitboard, a5));
    assert(!get_bit(board->bitboard, h6));
    assert(board->map[a5] != NULL);
    assert(test_board == board->bitboard);

    Moves* moves_2 = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[white][PAWN_INDEX(white, 7)], moves_2);

    assert(moves_2->length == 2);
    assert(moves_2->moves[0]->destination == g6);
    assert(moves_2->moves[0]->from == h5);
    assert(moves_2->moves[1]->destination == h6);
    assert(moves_2->moves[1]->from == h5);
    assert(moves_2->moves[1]->piece->cell == h5);
}

void test_pawn_en_passant_7() {
    Board* board = init_board();

    assert(board->pieces[white][PAWN_INDEX(white, 0)]->type == pawn);
    assert(board->pieces[white][PAWN_INDEX(white, 1)]->type == pawn);
    assert(board->pieces[white][PAWN_INDEX(white, 2)]->type == pawn);
    assert(board->pieces[white][PAWN_INDEX(white, 3)]->type == pawn);
    assert(board->pieces[white][PAWN_INDEX(white, 4)]->type == pawn);
    assert(board->pieces[white][PAWN_INDEX(white, 5)]->type == pawn);
    assert(board->pieces[white][PAWN_INDEX(white, 6)]->type == pawn);
    assert(board->pieces[white][PAWN_INDEX(white, 7)]->type == pawn);

    assert(board->pieces[black][PAWN_INDEX(black, 0)]->type == pawn);
    assert(board->pieces[black][PAWN_INDEX(black, 1)]->type == pawn);
    assert(board->pieces[black][PAWN_INDEX(black, 2)]->type == pawn);
    assert(board->pieces[black][PAWN_INDEX(black, 3)]->type == pawn);
    assert(board->pieces[black][PAWN_INDEX(black, 4)]->type == pawn);
    assert(board->pieces[black][PAWN_INDEX(black, 5)]->type == pawn);
    assert(board->pieces[black][PAWN_INDEX(black, 6)]->type == pawn);
    assert(board->pieces[black][PAWN_INDEX(black, 7)]->type == pawn);

    Move* move = calloc(1, sizeof(Move));
    move->piece = board->pieces[black][PAWN_INDEX(black, 4)];
    assert(move->piece->type == pawn);
    move->destination = e5;
    move->from = move->piece->cell;
    Piece* killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 3)];
    move->destination = d4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 4)];
    move->destination = e4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 5)];
    move->destination = f4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    Moves* moves = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[black][PAWN_INDEX(black, 4)], moves);

    assert(moves->length == 2);
    assert(moves->moves[0]->destination == e3);
    assert(moves->moves[0]->from == e4);
    assert(moves->moves[1]->destination == f3);
    assert(moves->moves[1]->from == e4);
    assert(moves->moves[1]->piece->cell == e4);

    assert(board->map[e4] == moves->moves[0]->piece && board->map[e4] == moves->moves[1]->piece);
    assert(get_bit(board->bitboard, e4));
    assert(get_bit(board->bitboard, f4));
    assert(!get_bit(board->bitboard, f3));
    assert(!get_bit(board->bitboard, e3));
    assert(board->map[f4] != NULL);
    U64 test_board = board->bitboard;

    killed = pretend_move(board, moves->moves[0]);

    assert(!killed);
    assert(board->map[e4] == NULL);
    assert(!get_bit(board->bitboard, e4));
    assert(get_bit(board->bitboard, e3));
    assert(get_bit(board->bitboard, f4));
    assert(!get_bit(board->bitboard, f3));
    assert(moves->moves[0]->piece->cell == e3);
    assert(board->map[e3] == moves->moves[0]->piece);

    undo_pretend_move(board, moves->moves[0], killed);

    assert(board->map[e4] == moves->moves[0]->piece);
    assert(moves->moves[0]->piece->cell == e4);
    assert(get_bit(board->bitboard, e4));
    assert(get_bit(board->bitboard, f4));
    assert(!get_bit(board->bitboard, f3));
    assert(!get_bit(board->bitboard, e3));
    assert(test_board == board->bitboard);

    killed = pretend_move(board, moves->moves[1]);

    assert(killed);
    assert(killed->alive == false);
    assert(killed->cell == f4);
    assert(killed->type == pawn);
    assert(board->map[f4] == NULL);
    assert(board->map[e4] == NULL);
    assert(!get_bit(board->bitboard, f4));
    assert(!get_bit(board->bitboard, e3));
    assert(get_bit(board->bitboard, f3));
    assert(!get_bit(board->bitboard, e4));
    assert(moves->moves[1]->piece->cell == f3);
    assert(board->map[f3] == moves->moves[1]->piece);

    undo_pretend_move(board, moves->moves[1], killed);

    assert(board->map[e4] == moves->moves[0]->piece);
    assert(board->map[f4] == killed);
    assert(killed->alive);
    assert(killed->cell == f4);
    assert(moves->moves[0]->piece->cell == e4);
    assert(get_bit(board->bitboard, e4));
    assert(get_bit(board->bitboard, f4));
    assert(!get_bit(board->bitboard, f3));
    assert(!get_bit(board->bitboard, e3));
    assert(test_board == board->bitboard);

    Moves* moves_2 = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[black][PAWN_INDEX(black, 4)], moves_2);

    assert(moves_2->length == 2);
    assert(moves_2->moves[0]->destination == e3);
    assert(moves_2->moves[0]->from == e4);
    assert(moves_2->moves[1]->destination == f3);
    assert(moves_2->moves[1]->from == e4);
    assert(moves_2->moves[1]->piece->cell == e4);
}

void test_pawn_en_passant_8() {
    Board* board = init_board();

    Move* move = calloc(1, sizeof(Move));
    move->piece = board->pieces[black][PAWN_INDEX(black, 4)];
    assert(move->piece->type == pawn);
    move->destination = e5;
    move->from = move->piece->cell;
    Piece* killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 5)];
    move->destination = f4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 4)];
    move->destination = e4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 3)];
    move->destination = d4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    Moves* moves = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[black][PAWN_INDEX(black, 4)], moves);

    assert(moves->length == 2);
    assert(moves->moves[0]->destination == d3);
    assert(moves->moves[0]->from == e4);
    assert(moves->moves[1]->destination == e3);
    assert(moves->moves[1]->from == e4);
    assert(moves->moves[1]->piece->cell == e4);

    assert(board->map[e4] == moves->moves[0]->piece && board->map[e4] == moves->moves[1]->piece);
    assert(get_bit(board->bitboard, e4));
    assert(get_bit(board->bitboard, d4));
    assert(!get_bit(board->bitboard, d3));
    assert(!get_bit(board->bitboard, e3));
    assert(board->map[d4] != NULL);
    U64 test_board = board->bitboard;

    killed = pretend_move(board, moves->moves[0]);

    assert(killed);
    assert(killed->alive == false);
    assert(killed->cell == d4);
    assert(killed->type == pawn);
    assert(board->map[d4] == NULL);
    assert(board->map[e4] == NULL);
    assert(!get_bit(board->bitboard, e4));
    assert(get_bit(board->bitboard, d3));
    assert(!get_bit(board->bitboard, d4));
    assert(!get_bit(board->bitboard, e3));
    assert(moves->moves[0]->piece->cell == d3);
    assert(board->map[d3] == moves->moves[0]->piece);

    undo_pretend_move(board, moves->moves[0], killed);

    assert(board->map[e4] == moves->moves[0]->piece);
    assert(board->map[d4] == killed);
    assert(killed->alive);
    assert(killed->cell == d4);
    assert(moves->moves[0]->piece->cell == e4);
    assert(get_bit(board->bitboard, e4));
    assert(get_bit(board->bitboard, d4));
    assert(!get_bit(board->bitboard, d3));
    assert(!get_bit(board->bitboard, e3));
    assert(test_board == board->bitboard);

    killed = pretend_move(board, moves->moves[1]);

    assert(!killed);
    assert(board->map[e4] == NULL);
    assert(!get_bit(board->bitboard, d3));
    assert(get_bit(board->bitboard, e3));
    assert(get_bit(board->bitboard, d4));
    assert(!get_bit(board->bitboard, e4));
    assert(moves->moves[1]->piece->cell == e3);
    assert(board->map[e3] == moves->moves[1]->piece);

    undo_pretend_move(board, moves->moves[1], killed);

    assert(board->map[e4] == moves->moves[1]->piece);
    assert(board->map[d4] != NULL);
    assert(moves->moves[1]->piece->cell == e4);
    assert(get_bit(board->bitboard, e4));
    assert(get_bit(board->bitboard, d4));
    assert(!get_bit(board->bitboard, d3));
    assert(!get_bit(board->bitboard, e3));
    assert(test_board == board->bitboard);

    Moves* moves_2 = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[black][PAWN_INDEX(black, 4)], moves_2);

    assert(moves_2->length == 2);
    assert(moves_2->moves[0]->destination == d3);
    assert(moves_2->moves[0]->from == e4);
    assert(moves_2->moves[1]->destination == e3);
    assert(moves_2->moves[1]->from == e4);
    assert(moves_2->moves[1]->piece->cell == e4);
}

void test_pawn_en_passant_9() {
    Board* board = init_board();

    Move* move = calloc(1, sizeof(Move));
    move->piece = board->pieces[black][PAWN_INDEX(black, 7)];
    assert(move->piece->type == pawn);
    move->destination = h5;
    move->from = move->piece->cell;
    Piece* killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 6)];
    move->destination = g3;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 7)];
    move->destination = h4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 0)];
    move->destination = a4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    Moves* moves = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[black][PAWN_INDEX(black, 7)], moves);

    assert(moves->length == 2);
    assert(moves->moves[0]->destination == g3);
    assert(moves->moves[0]->from == h4);
    assert(moves->moves[1]->destination == h3);
    assert(moves->moves[1]->from == h4);
    assert(moves->moves[1]->piece->cell == h4);

    assert(board->map[h4] == moves->moves[0]->piece && board->map[h4] == moves->moves[1]->piece);
    assert(get_bit(board->bitboard, h4));
    assert(get_bit(board->bitboard, g3));
    assert(get_bit(board->bitboard, a4));
    assert(!get_bit(board->bitboard, h3));
    assert(board->map[a4] != NULL);
    U64 test_board = board->bitboard;

    killed = pretend_move(board, moves->moves[0]);

    assert(killed);
    assert(killed->alive == false);
    assert(killed->cell == g3);
    assert(killed->type == pawn);
    assert(board->map[h4] == NULL);
    assert(board->map[a4] != NULL);
    assert(!get_bit(board->bitboard, h4));
    assert(get_bit(board->bitboard, g3));
    assert(get_bit(board->bitboard, a4));
    assert(!get_bit(board->bitboard, h3));
    assert(moves->moves[0]->piece->cell == g3);
    assert(board->map[g3] == moves->moves[0]->piece);

    undo_pretend_move(board, moves->moves[0], killed);

    assert(board->map[h4] == moves->moves[0]->piece);
    assert(board->map[g3] == killed);
    assert(board->map[a4] != NULL);
    assert(killed->alive);
    assert(killed->cell == g3);
    assert(moves->moves[0]->piece->cell == h4);
    assert(get_bit(board->bitboard, h4));
    assert(get_bit(board->bitboard, g3));
    assert(!get_bit(board->bitboard, h3));
    assert(get_bit(board->bitboard, a4));
    assert(test_board == board->bitboard);

    killed = pretend_move(board, moves->moves[1]);

    assert(!killed);
    assert(board->map[h4] == NULL);
    assert(!get_bit(board->bitboard, h4));
    assert(get_bit(board->bitboard, h3));
    assert(get_bit(board->bitboard, g3));
    assert(get_bit(board->bitboard, a4));
    assert(moves->moves[1]->piece->cell == h3);
    assert(board->map[h3] == moves->moves[1]->piece);

    undo_pretend_move(board, moves->moves[1], killed);

    assert(board->map[h4] == moves->moves[1]->piece);
    assert(board->map[a4] != NULL);
    assert(moves->moves[1]->piece->cell == h4);
    assert(get_bit(board->bitboard, h4));
    assert(get_bit(board->bitboard, g3));
    assert(!get_bit(board->bitboard, h3));
    assert(get_bit(board->bitboard, a4));
    assert(test_board == board->bitboard);

    Moves* moves_2 = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[black][PAWN_INDEX(black, 7)], moves_2);

    assert(moves_2->length == 2);
    assert(moves_2->moves[0]->destination == g3);
    assert(moves_2->moves[0]->from == h4);
    assert(moves_2->moves[1]->destination == h3);
    assert(moves_2->moves[1]->from == h4);
    assert(moves_2->moves[1]->piece->cell == h4);
}

void test_pawn_en_passant_10() {
    Board* board = init_board();

    Move* move = calloc(1, sizeof(Move));
    move->piece = board->pieces[black][PAWN_INDEX(black, 0)];
    assert(move->piece->type == pawn);
    move->destination = a5;
    move->from = move->piece->cell;
    Piece* killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 1)];
    move->destination = b3;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[black][PAWN_INDEX(black, 0)];
    move->destination = a4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    move->piece = board->pieces[white][PAWN_INDEX(white, 7)];
    move->destination = h4;
    move->from = move->piece->cell;
    killed = pretend_move(board, move);

    Moves* moves = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[black][PAWN_INDEX(black, 0)], moves);

    assert(moves->length == 2);
    assert(moves->moves[0]->destination == a3);
    assert(moves->moves[0]->from == a4);
    assert(moves->moves[1]->destination == b3);
    assert(moves->moves[1]->from == a4);
    assert(moves->moves[1]->piece->cell == a4);

    assert(board->map[a4] == moves->moves[0]->piece && board->map[a4] == moves->moves[1]->piece);
    assert(get_bit(board->bitboard, a4));
    assert(get_bit(board->bitboard, b3));
    assert(get_bit(board->bitboard, h4));
    assert(!get_bit(board->bitboard, a3));
    assert(board->map[h4] != NULL);
    U64 test_board = board->bitboard;

    killed = pretend_move(board, moves->moves[0]);

    assert(!killed);
    assert(board->map[a4] == NULL);
    assert(board->map[h4] != NULL);
    assert(!get_bit(board->bitboard, a4));
    assert(get_bit(board->bitboard, b3));
    assert(get_bit(board->bitboard, a3));
    assert(get_bit(board->bitboard, h4));
    assert(moves->moves[0]->piece->cell == a3);
    assert(board->map[a3] == moves->moves[0]->piece);

    undo_pretend_move(board, moves->moves[0], killed);

    assert(board->map[a4] == moves->moves[0]->piece);
    assert(board->map[h4] != NULL);
    assert(moves->moves[0]->piece->cell == a4);
    assert(get_bit(board->bitboard, a4));
    assert(get_bit(board->bitboard, b3));
    assert(!get_bit(board->bitboard, a3));
    assert(get_bit(board->bitboard, h4));
    assert(test_board == board->bitboard);

    killed = pretend_move(board, moves->moves[1]);

    assert(killed);
    assert(killed->alive == false);
    assert(killed->cell == b3);
    assert(killed->type == pawn);
    assert(board->map[a4] == NULL);
    assert(board->map[h4] != NULL);
    assert(!get_bit(board->bitboard, a4));
    assert(!get_bit(board->bitboard, a3));
    assert(get_bit(board->bitboard, h4));
    assert(get_bit(board->bitboard, b3));
    assert(moves->moves[1]->piece->cell == b3);
    assert(board->map[b3] == moves->moves[1]->piece);

    undo_pretend_move(board, moves->moves[1], killed);

    assert(board->map[a4] == moves->moves[1]->piece);
    assert(board->map[h4] != NULL);
    assert(moves->moves[1]->piece->cell == a4);
    assert(get_bit(board->bitboard, a4));
    assert(get_bit(board->bitboard, b3));
    assert(!get_bit(board->bitboard, a3));
    assert(get_bit(board->bitboard, h4));
    assert(test_board == board->bitboard);

    Moves* moves_2 = calloc(1, sizeof(Moves));
    get_all_moves_for_piece(board, board->pieces[black][PAWN_INDEX(black, 0)], moves_2);

    assert(moves_2->length == 2);
    assert(moves_2->moves[0]->destination == a3);
    assert(moves_2->moves[0]->from == a4);
    assert(moves_2->moves[1]->destination == b3);
    assert(moves_2->moves[1]->from == a4);
    assert(moves_2->moves[1]->piece->cell == a4);
}


void test_pawn_en_passant_legality_1() {
    // This is actually a hypothetical move order,
    // and I don't think it could ever happen.
    Board* board = set_board_notation("wkh1, wpc5 bbb7 bpd5 ");
    board->map[h1]->no_moves += 1;
    Move* move = calloc(1, sizeof(Move));
    move->piece = board->map[d5];
    move->from = d7;
    move->destination = d5;
    board->last_moved[board->lm_length] = move;
    board->lm_length += 1;

    Moves* moves = get_all_moves_for_colour(board, white);

    assert(moves->length == 4);
    assert(moves->moves[0]->destination == c6);
    assert(moves->moves[0]->from == c5);
    assert(moves->moves[0]->piece->type == pawn);
    assert(moves->moves[1]->destination == g2);
    assert(moves->moves[1]->from == h1);
    assert(moves->moves[1]->piece->type == king);
    assert(moves->moves[2]->destination == h2);
    assert(moves->moves[2]->from == h1);
    assert(moves->moves[2]->piece->type == king);
    assert(moves->moves[3]->destination == g1);
    assert(moves->moves[3]->from == h1);
    assert(moves->moves[3]->piece->type == king);
}

void test_pawn_en_passant_legality_2() {
    Board* board = set_board_notation("wkc1, wpc5 bcc8 bpd5 ");
    board->map[c1]->no_moves += 1;
    Move* move = calloc(1, sizeof(Move));
    move->piece = board->map[d5];
    move->from = d7;
    move->destination = d5;
    board->last_moved[board->lm_length] = move;
    board->lm_length += 1;

    Moves* moves = get_all_moves_for_colour(board, white);

    assert(moves->length == 6);
    assert(moves->moves[0]->destination == c6);
    assert(moves->moves[0]->from == c5);
    assert(moves->moves[0]->piece->type == pawn);
    assert(moves->moves[1]->destination == b2);
    assert(moves->moves[1]->from == c1);
    assert(moves->moves[1]->piece->type == king);
    assert(moves->moves[2]->destination == c2);
    assert(moves->moves[2]->from == c1);
    assert(moves->moves[2]->piece->type == king);
    assert(moves->moves[3]->destination == d2);
    assert(moves->moves[3]->from == c1);
    assert(moves->moves[3]->piece->type == king);
    assert(moves->moves[4]->destination == b1);
    assert(moves->moves[4]->from == c1);
    assert(moves->moves[4]->piece->type == king);
    assert(moves->moves[5]->destination == d1);
    assert(moves->moves[5]->from == c1);
    assert(moves->moves[5]->piece->type == king);
}

void test_pawn_en_passant_legality_3() {
    Board* board = set_board_notation("wkb5 wpc5 bcf5 bpd5 ");
    board->map[b5]->no_moves += 1;
    Move* move = calloc(1, sizeof(Move));
    move->piece = board->map[d5];
    move->from = d7;
    move->destination = d5;
    board->last_moved[board->lm_length] = move;
    board->lm_length += 1;

    Moves* moves = get_all_moves_for_colour(board, white);

    assert(moves->length == 7);
    assert(moves->moves[0]->destination == c6);
    assert(moves->moves[0]->from == c5);
    assert(moves->moves[0]->piece->type == pawn);
    assert(moves->moves[1]->destination == a6);
    assert(moves->moves[1]->from == b5);
    assert(moves->moves[1]->piece->type == king);
    assert(moves->moves[2]->destination == b6);
    assert(moves->moves[2]->from == b5);
    assert(moves->moves[2]->piece->type == king);
    assert(moves->moves[3]->destination == c6);
    assert(moves->moves[3]->from == b5);
    assert(moves->moves[3]->piece->type == king);
    assert(moves->moves[4]->destination == a5);
    assert(moves->moves[4]->from == b5);
    assert(moves->moves[4]->piece->type == king);
    assert(moves->moves[5]->destination == a4);
    assert(moves->moves[5]->from == b5);
    assert(moves->moves[5]->piece->type == king);
    assert(moves->moves[6]->destination == b4);
    assert(moves->moves[6]->from == b5);
    assert(moves->moves[6]->piece->type == king);

    moves = get_all_moves_for_colour(board, black);
    
    // move castle out the way
    pretend_move(board, moves->moves[0]);
    assert(board->map[f8]);
    assert(board->map[f8]->type == castle);
    assert(board->map[f5] == NULL);
    assert(get_bit(board->bitboard, f8));
    assert(!get_bit(board->bitboard, f5));

    moves = get_all_moves_for_colour(board, white);

    // still can't do en passant because last moved has changed
    assert(moves->length == 7);
    assert(moves->moves[0]->destination == c6);
    assert(moves->moves[0]->from == c5);
    assert(moves->moves[0]->piece->type == pawn);
    assert(moves->moves[1]->destination == a6);
    assert(moves->moves[1]->from == b5);
    assert(moves->moves[1]->piece->type == king);
    assert(moves->moves[2]->destination == b6);
    assert(moves->moves[2]->from == b5);
    assert(moves->moves[2]->piece->type == king);
    assert(moves->moves[3]->destination == c6);
    assert(moves->moves[3]->from == b5);
    assert(moves->moves[3]->piece->type == king);
    assert(moves->moves[4]->destination == a5);
    assert(moves->moves[4]->from == b5);
    assert(moves->moves[4]->piece->type == king);
    assert(moves->moves[5]->destination == a4);
    assert(moves->moves[5]->from == b5);
    assert(moves->moves[5]->piece->type == king);
    assert(moves->moves[6]->destination == b4);
    assert(moves->moves[6]->from == b5);
    assert(moves->moves[6]->piece->type == king);

    Move* move_2 = calloc(1, sizeof(Move));
    move_2->piece = board->map[d5];
    move_2->from = d7;
    move_2->destination = d5;
    board->last_moved[board->lm_length] = move_2;
    board->lm_length += 1;
    moves = get_all_moves_for_colour(board, white);

    assert(moves->length == 8);
    assert(moves->moves[0]->destination == c6);
    assert(moves->moves[0]->from == c5);
    assert(moves->moves[0]->piece->type == pawn);
    assert(moves->moves[1]->destination == d6);
    assert(moves->moves[1]->from == c5);
    assert(moves->moves[1]->piece->type == pawn);
    assert(moves->moves[2]->destination == a6);
    assert(moves->moves[2]->from == b5);
    assert(moves->moves[2]->piece->type == king);
    assert(moves->moves[3]->destination == b6);
    assert(moves->moves[3]->from == b5);
    assert(moves->moves[3]->piece->type == king);
    assert(moves->moves[4]->destination == c6);
    assert(moves->moves[4]->from == b5);
    assert(moves->moves[4]->piece->type == king);
    assert(moves->moves[5]->destination == a5);
    assert(moves->moves[5]->from == b5);
    assert(moves->moves[5]->piece->type == king);
    assert(moves->moves[6]->destination == a4);
    assert(moves->moves[6]->from == b5);
    assert(moves->moves[6]->piece->type == king);
    assert(moves->moves[7]->destination == b4);
    assert(moves->moves[7]->from == b5);
    assert(moves->moves[7]->piece->type == king);
}

void test_pawn_en_passant_legality_4() {
    Board* board = set_board_notation("wpd5 bcc5 ");
    Move* move = calloc(1, sizeof(Move));
    move->piece = board->map[c5];
    move->from = c7;
    move->destination = c5;
    board->last_moved[board->lm_length] = move;
    board->lm_length += 1;

    Moves* moves = get_all_moves_for_colour(board, white);

    assert(moves->length == 1);
    assert(moves->moves[0]->destination == d6);
    assert(moves->moves[0]->from == d5);
}

void test_pawn_en_passant_legality_5() {
    Board* board = set_board_notation("wpd5 bpc5 ");
    Move* move = calloc(1, sizeof(Move));
    move->piece = board->map[c5];
    move->from = c6;
    move->destination = c5;
    board->last_moved[board->lm_length] = move;
    board->lm_length += 1;

    Moves* moves = get_all_moves_for_colour(board, white);

    assert(moves->length == 1);
    assert(moves->moves[0]->destination == d6);
    assert(moves->moves[0]->from == d5);

    move->from = c7;
    Moves* moves_2 = get_all_moves_for_colour(board, white);
    assert(moves_2->length == 2);
    assert(moves_2->moves[0]->destination == c6);
    assert(moves_2->moves[0]->from == d5);
    assert(moves_2->moves[1]->destination == d6);
    assert(moves_2->moves[1]->from == d5);
}

void test_castling_execution_white_short() {
    Board* board = init_board();
    clear_board(board);
    board->castle_pieces[white][king_side]->alive = true;
    board->pieces[white][KING_INDEX(white)]->alive = true;
    move_single_piece(board, board->castle_pieces[white][king_side], board->castle_pieces[white][king_side]->cell, none);
    move_single_piece(board, board->pieces[white][KING_INDEX(white)], board->pieces[white][KING_INDEX(white)]->cell, none);

    Moves* moves = get_all_moves_for_colour(board, white);

    bool found = false;
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->castle && moves->moves[i]->castle_side == queen_side));
        if (moves->moves[i]->castle && moves->moves[i]->castle_side == king_side) {
            found = true;
        }
    }
    assert(found);

    assert(moves->moves[5]->destination == g1);
    assert(moves->moves[5]->from == e1);
    assert(moves->moves[5]->piece->type == king);
    assert(moves->moves[5]->castle == true);
    assert(moves->moves[5]->castle_side == king_side);

    assert(!get_bit(board->bitboard, g1));
    assert(!get_bit(board->bitboard, f1));
    assert(get_bit(board->bitboard, e1));
    assert(get_bit(board->bitboard, h1));
    assert(board->map[g1] == NULL);
    assert(board->map[f1] == NULL);
    assert(board->map[e1] == moves->moves[5]->piece);
    assert(moves->moves[5]->piece->cell == e1);
    assert(board->map[h1] == board->castle_pieces[white][king_side]);
    assert(board->castle_pieces[white][king_side]->cell == h1);
    assert(moves->moves[5]->piece->no_moves == 0);
    U64 test_board = board->bitboard;

    Piece* killed = pretend_move(board, moves->moves[5]);

    assert(!killed);
    assert(get_bit(board->bitboard, g1));
    assert(get_bit(board->bitboard, f1));
    assert(!get_bit(board->bitboard, e1));
    assert(!get_bit(board->bitboard, h1));
    assert(board->map[e1] == NULL);
    assert(board->map[h1] == NULL);
    assert(board->map[g1] == moves->moves[5]->piece);
    assert(moves->moves[5]->piece->cell == g1);
    assert(board->map[f1] == board->castle_pieces[white][king_side]);
    assert(board->castle_pieces[white][king_side]->cell == f1);
    assert(moves->moves[5]->piece->no_moves == 1);

    undo_pretend_move(board, moves->moves[5], killed);

    assert(!get_bit(board->bitboard, g1));
    assert(!get_bit(board->bitboard, f1));
    assert(get_bit(board->bitboard, e1));
    assert(get_bit(board->bitboard, h1));
    assert(test_board == board->bitboard);
    assert(board->map[g1] == NULL);
    assert(board->map[f1] == NULL);
    assert(board->map[e1] == moves->moves[5]->piece);
    assert(moves->moves[5]->piece->cell == e1);
    assert(board->map[h1] == board->castle_pieces[white][king_side]);
    assert(board->castle_pieces[white][king_side]->cell == h1);
    assert(moves->moves[5]->piece->no_moves == 0);
}

void test_castling_execution_white_long() {
    Board* board = init_board();
    clear_board(board);
    board->castle_pieces[white][queen_side]->alive = true;
    board->pieces[white][KING_INDEX(white)]->alive = true;
    move_single_piece(board, board->castle_pieces[white][queen_side], board->castle_pieces[white][queen_side]->cell, none);
    move_single_piece(board, board->pieces[white][KING_INDEX(white)], board->pieces[white][KING_INDEX(white)]->cell, none);

    Moves* moves = get_all_moves_for_colour(board, white);

    bool found = false;
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->castle && moves->moves[i]->castle_side == king_side));
        if (moves->moves[i]->castle && moves->moves[i]->castle_side == queen_side) {
            found = true;
        }
    }
    assert(found);

    assert(moves->moves[13]->destination == c1);
    assert(moves->moves[13]->from == e1);
    assert(moves->moves[13]->piece->type == king);
    assert(moves->moves[13]->castle == true);
    assert(moves->moves[13]->castle_side == queen_side);

    assert(!get_bit(board->bitboard, d1));
    assert(!get_bit(board->bitboard, c1));
    assert(!get_bit(board->bitboard, b1));
    assert(get_bit(board->bitboard, e1));
    assert(get_bit(board->bitboard, a1));
    assert(board->map[d1] == NULL);
    assert(board->map[c1] == NULL);
    assert(board->map[b1] == NULL);
    assert(board->map[e1] == moves->moves[13]->piece);
    assert(moves->moves[13]->piece->cell == e1);
    assert(board->map[a1] == board->castle_pieces[white][queen_side]);
    assert(board->castle_pieces[white][queen_side]->cell == a1);
    assert(moves->moves[13]->piece->no_moves == 0);
    U64 test_board = board->bitboard;

    Piece* killed = pretend_move(board, moves->moves[13]);

    assert(!killed);
    assert(get_bit(board->bitboard, d1));
    assert(get_bit(board->bitboard, c1));
    assert(!get_bit(board->bitboard, e1));
    assert(!get_bit(board->bitboard, a1));
    assert(!get_bit(board->bitboard, b1));
    assert(board->map[e1] == NULL);
    assert(board->map[a1] == NULL);
    assert(board->map[b1] == NULL);
    assert(board->map[c1] == moves->moves[13]->piece);
    assert(moves->moves[13]->piece->cell == c1);
    assert(board->map[d1] == board->castle_pieces[white][queen_side]);
    assert(board->castle_pieces[white][queen_side]->cell == d1);
    assert(moves->moves[13]->piece->no_moves == 1);

    undo_pretend_move(board, moves->moves[13], killed);

    assert(!get_bit(board->bitboard, d1));
    assert(!get_bit(board->bitboard, c1));
    assert(!get_bit(board->bitboard, b1));
    assert(get_bit(board->bitboard, e1));
    assert(get_bit(board->bitboard, a1));
    assert(board->map[d1] == NULL);
    assert(board->map[c1] == NULL);
    assert(board->map[b1] == NULL);
    assert(board->map[e1] == moves->moves[13]->piece);
    assert(moves->moves[13]->piece->cell == e1);
    assert(board->map[a1] == board->castle_pieces[white][queen_side]);
    assert(board->castle_pieces[white][queen_side]->cell == a1);
    assert(moves->moves[13]->piece->no_moves == 0);
    assert(test_board == board->bitboard);
}

void test_castling_execution_black_short() {
    Board* board = init_board();
    clear_board(board);
    board->castle_pieces[black][king_side]->alive = true;
    board->pieces[black][KING_INDEX(black)]->alive = true;
    move_single_piece(board, board->castle_pieces[black][king_side], board->castle_pieces[black][king_side]->cell, none);
    move_single_piece(board, board->pieces[black][KING_INDEX(black)], board->pieces[black][KING_INDEX(black)]->cell, none);

    Moves* moves = get_all_moves_for_colour(board, black);

    bool found = false;
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->castle && moves->moves[i]->castle_side == queen_side));
        if (moves->moves[i]->castle && moves->moves[i]->castle_side == king_side) {
            found = true;
        }
    }
    assert(found);

    assert(moves->moves[2]->destination == g8);
    assert(moves->moves[2]->from == e8);
    assert(moves->moves[2]->piece->type == king);
    assert(moves->moves[2]->castle == true);
    assert(moves->moves[2]->castle_side == king_side);

    assert(!get_bit(board->bitboard, g8));
    assert(!get_bit(board->bitboard, f8));
    assert(get_bit(board->bitboard, e8));
    assert(get_bit(board->bitboard, h8));
    assert(board->map[g8] == NULL);
    assert(board->map[f8] == NULL);
    assert(board->map[e8] == moves->moves[2]->piece);
    assert(moves->moves[2]->piece->cell == e8);
    assert(board->map[h8] == board->castle_pieces[black][king_side]);
    assert(board->castle_pieces[black][king_side]->cell == h8);
    assert(moves->moves[2]->piece->no_moves == 0);
    U64 test_board = board->bitboard;

    Piece* killed = pretend_move(board, moves->moves[2]);

    assert(!killed);
    assert(get_bit(board->bitboard, g8));
    assert(get_bit(board->bitboard, f8));
    assert(!get_bit(board->bitboard, e8));
    assert(!get_bit(board->bitboard, h8));
    assert(board->map[e8] == NULL);
    assert(board->map[h8] == NULL);
    assert(board->map[g8] == moves->moves[2]->piece);
    assert(moves->moves[2]->piece->cell == g8);
    assert(board->map[f8] == board->castle_pieces[black][king_side]);
    assert(board->castle_pieces[black][king_side]->cell == f8);
    assert(moves->moves[2]->piece->no_moves == 1);

    undo_pretend_move(board, moves->moves[2], killed);

    assert(!get_bit(board->bitboard, g8));
    assert(!get_bit(board->bitboard, f8));
    assert(get_bit(board->bitboard, e8));
    assert(get_bit(board->bitboard, h8));
    assert(test_board == board->bitboard);
    assert(board->map[g8] == NULL);
    assert(board->map[f8] == NULL);
    assert(board->map[e8] == moves->moves[2]->piece);
    assert(moves->moves[2]->piece->cell == e8);
    assert(board->map[h8] == board->castle_pieces[black][king_side]);
    assert(board->castle_pieces[black][king_side]->cell == h8);
    assert(moves->moves[2]->piece->no_moves == 0);
}

void test_castling_execution_black_long() {
    Board* board = init_board();
    clear_board(board);
    board->castle_pieces[black][queen_side]->alive = true;
    board->pieces[black][KING_INDEX(black)]->alive = true;
    move_single_piece(board, board->castle_pieces[black][queen_side], board->castle_pieces[black][queen_side]->cell, none);
    move_single_piece(board, board->pieces[black][KING_INDEX(black)], board->pieces[black][KING_INDEX(black)]->cell, none);

    Moves* moves = get_all_moves_for_colour(board, black);

    bool found = false;
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->castle && moves->moves[i]->castle_side == king_side));
        if (moves->moves[i]->castle && moves->moves[i]->castle_side == queen_side) {
            found = true;
        }
    }
    assert(found);

    assert(moves->moves[10]->destination == c8);
    assert(moves->moves[10]->from == e8);
    assert(moves->moves[10]->piece->type == king);
    assert(moves->moves[10]->castle == true);
    assert(moves->moves[10]->castle_side == queen_side);

    assert(!get_bit(board->bitboard, d8));
    assert(!get_bit(board->bitboard, c8));
    assert(!get_bit(board->bitboard, b8));
    assert(get_bit(board->bitboard, e8));
    assert(get_bit(board->bitboard, a8));
    assert(board->map[d8] == NULL);
    assert(board->map[c8] == NULL);
    assert(board->map[b8] == NULL);
    assert(board->map[e8] == moves->moves[10]->piece);
    assert(moves->moves[10]->piece->cell == e8);
    assert(board->map[a8] == board->castle_pieces[black][queen_side]);
    assert(board->castle_pieces[black][queen_side]->cell == a8);
    assert(moves->moves[10]->piece->no_moves == 0);
    U64 test_board = board->bitboard;

    Piece* killed = pretend_move(board, moves->moves[10]);

    assert(!killed);
    assert(get_bit(board->bitboard, d8));
    assert(get_bit(board->bitboard, c8));
    assert(!get_bit(board->bitboard, e8));
    assert(!get_bit(board->bitboard, a8));
    assert(!get_bit(board->bitboard, b8));
    assert(board->map[e8] == NULL);
    assert(board->map[a8] == NULL);
    assert(board->map[b8] == NULL);
    assert(board->map[c8] == moves->moves[10]->piece);
    assert(moves->moves[10]->piece->cell == c8);
    assert(board->map[d8] == board->castle_pieces[black][queen_side]);
    assert(board->castle_pieces[black][queen_side]->cell == d8);
    assert(moves->moves[10]->piece->no_moves == 1);

    undo_pretend_move(board, moves->moves[10], killed);

    assert(!get_bit(board->bitboard, d8));
    assert(!get_bit(board->bitboard, c8));
    assert(!get_bit(board->bitboard, b8));
    assert(get_bit(board->bitboard, e8));
    assert(get_bit(board->bitboard, a8));
    assert(board->map[d8] == NULL);
    assert(board->map[c8] == NULL);
    assert(board->map[b8] == NULL);
    assert(board->map[e8] == moves->moves[10]->piece);
    assert(moves->moves[10]->piece->cell == e8);
    assert(board->map[a8] == board->castle_pieces[black][queen_side]);
    assert(board->castle_pieces[black][queen_side]->cell == a8);
    assert(moves->moves[10]->piece->no_moves == 0);
    assert(test_board == board->bitboard);
}

void test_castling_1() {
    Board* board = init_board();
    board->pieces[white][KING_INDEX(white)]->no_moves = 0;

    move_single_piece(board, board->pieces[white][PAWN_INDEX(white, 4)], e4, none);
    move_single_piece(board, board->pieces[black][PAWN_INDEX(black, 4)], e5, none);
    move_single_piece(board, board->pieces[white][BISHOP_2(white)], c4, none);
    move_single_piece(board, board->pieces[black][BISHOP_2(black)], c5, none);
    move_single_piece(board, board->pieces[white][KNIGHT_2(white)], f3, none);
    move_single_piece(board, board->pieces[black][KNIGHT_2(black)], f6, none);

    U64 test_board = board->bitboard;
    Moves* moves = get_all_moves_for_colour(board, white);
    assert(board->bitboard == test_board);
    assert(moves->moves[16]->piece == board->pieces[white][KING_INDEX(white)]);
    assert(moves->moves[16]->destination == g1);
    assert(moves->moves[16]->from = e1);
}

void test_castling_2() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->map[e1] = board->pieces[white][KING_INDEX(white)];
    set_bit(board->bitboard, e1);

    board->castle_pieces[white][queen_side]->alive = true;
    board->castle_pieces[white][queen_side]->cell = a1;
    board->map[a1] = board->castle_pieces[white][queen_side];
    set_bit(board->bitboard, a1);

    board->pieces[black][PAWN_INDEX(black, 1)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 1)]->cell = a2;
    board->map[a2] = board->pieces[black][PAWN_INDEX(black, 1)];
    set_bit(board->bitboard, a2);

    U64 test_board = board->bitboard;
    Moves* moves = get_all_moves_for_colour(board, white);
    assert(test_board == board->bitboard);
    assert(moves->length == 10);
    assert(moves->moves[7]->destination == c1);
    assert(moves->moves[7]->from == e1);
    assert(moves->moves[7]->piece == board->pieces[white][KING_INDEX(white)]);



    board->castle_pieces[black][queen_side]->alive = true;
    board->castle_pieces[black][queen_side]->cell = c8;
    board->map[c8] = board->castle_pieces[black][queen_side];
    set_bit(board->bitboard, c8);

    test_board = board->bitboard;
    moves = get_all_moves_for_colour(board, white);
    assert(test_board == board->bitboard);
    for (int i = 0; i < moves->length; i++) {
        if (moves->moves[i]->piece == board->pieces[white][KING_INDEX(white)]) {
            assert(moves->moves[i]->destination != c1);
        }
    }

    board->castle_pieces[black][queen_side]->cell = d8;
    board->map[d8] = board->castle_pieces[black][queen_side];
    set_bit(board->bitboard, d8);
    pop_bit(board->bitboard, c8);

    test_board = board->bitboard;
    moves = get_all_moves_for_colour(board, white);
    assert(test_board == board->bitboard);
    for (int i = 0; i < moves->length; i++) {
        if (moves->moves[i]->piece == board->pieces[white][KING_INDEX(white)]) {
            assert(moves->moves[i]->destination != c1);
            assert(moves->moves[i]->destination != d1);
            assert(moves->moves[i]->destination != d2);
        }
    }


    board->castle_pieces[black][queen_side]->cell = h1;
    board->map[h1] = board->castle_pieces[black][queen_side];
    set_bit(board->bitboard, h1);
    pop_bit(board->bitboard, d8);

    test_board = board->bitboard;
    moves = get_all_moves_for_colour(board, white);
    assert(test_board == board->bitboard);
    for (int i = 0; i < moves->length; i++) {
        if (moves->moves[i]->piece == board->pieces[white][KING_INDEX(white)]) {
            assert(moves->moves[i]->destination != c1);
            assert(moves->moves[i]->destination != d1);
            assert(moves->moves[i]->destination != f1);
        }
    }

    board->castle_pieces[black][queen_side]->alive = false;
    board->pieces[black][BISHOP_1(black)]->alive = true;
    board->pieces[black][BISHOP_1(black)]->cell = f4;
    board->map[f4] = board->pieces[black][BISHOP_1(black)];
    set_bit(board->bitboard, f4);
    pop_bit(board->bitboard, h1);

    test_board = board->bitboard;
    moves = get_all_moves_for_colour(board, white);
    assert(test_board == board->bitboard);
    for (int i = 0; i < moves->length; i++) {
        if (moves->moves[i]->piece == board->pieces[white][KING_INDEX(white)]) {
            assert(moves->moves[i]->destination != c1);
            assert(moves->moves[i]->destination != d2);
        }
    }

    board->pieces[black][BISHOP_1(black)]->cell = b4;
    board->map[b4] = board->pieces[black][BISHOP_1(black)];
    set_bit(board->bitboard, b4);
    pop_bit(board->bitboard, f4);

    test_board = board->bitboard;
    moves = get_all_moves_for_colour(board, white);
    assert(test_board == board->bitboard);
    assert(moves->length == 4);
    assert(moves->moves[0]->piece = board->pieces[white][KING_INDEX(white)]);
    assert(moves->moves[0]->from = e1);
    assert(moves->moves[0]->destination = e2);
    assert(moves->moves[0]->piece = board->pieces[white][KING_INDEX(white)]);
    assert(moves->moves[0]->from = e1);
    assert(moves->moves[0]->destination = f2);
    assert(moves->moves[0]->piece = board->pieces[white][KING_INDEX(white)]);
    assert(moves->moves[0]->from = e1);
    assert(moves->moves[0]->destination = d1);
    assert(moves->moves[0]->piece = board->pieces[white][KING_INDEX(white)]);
    assert(moves->moves[0]->from = e1);
    assert(moves->moves[0]->destination = f1);
    for (int i = 0; i < moves->length; i++) {
        if (moves->moves[i]->piece == board->pieces[white][KING_INDEX(white)]) {
            assert(moves->moves[i]->destination != c1);
            assert(moves->moves[i]->destination != d2);
        }
    }
}

void test_castling_3() {
    Board* board = set_board_notation("wke1 wch1 bke8 ");

    Moves* moves = get_all_moves_for_colour(board, white);
    assert(moves->moves[5]->destination == g1);
    assert(moves->moves[5]->from == e1);
    assert(moves->moves[5]->piece->type == king);

    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    moves = get_all_moves_for_colour(board, white);
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->destination == g1 && moves->moves[i]->from == e1));
    }
}

void test_castling_4() {
    Board* board = set_board_notation("wke1 ");
    board->castle_pieces[white][queen_side]->alive = true;
    board->castle_pieces[white][queen_side]->cell = a1;
    board->map[a1] = board->castle_pieces[white][queen_side];
    set_bit(board->bitboard, a1);
    board->castle_pieces[white][king_side]->alive = true;
    board->castle_pieces[white][king_side]->cell = h1;
    board->map[h1] = board->castle_pieces[white][king_side];
    set_bit(board->bitboard, h1);

    Moves* moves = get_all_moves_for_colour(board, white);
    
    U64 saved_bitboard = board->bitboard;
    assert(moves->moves[16]->from == e1);
    assert(moves->moves[16]->destination == g1);
    assert(moves->moves[16]->castle == true);
    assert(moves->moves[16]->castle_side == king_side);
    assert(moves->moves[16]->piece->no_moves == 0);
    assert(board->castle_pieces[white][king_side]->no_moves == 0);
    assert(board->map[e1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[board->from_castle_coords[white][king_side]] == board->castle_pieces[white][king_side]);
    assert(board->map[board->castling_coordinates[white][king_side]] == NULL);
    assert(board->map[board->to_castle_coords[white][king_side]] == NULL);

    Piece* killed = pretend_move(board, moves->moves[16]);

    assert(board->pieces[white][KING_INDEX(white)]->cell == board->castling_coordinates[white][king_side]);
    assert(board->castle_pieces[white][king_side]->cell == board->to_castle_coords[white][king_side]);
    assert(moves->moves[16]->piece->no_moves == 1);
    assert(board->castle_pieces[white][king_side]->no_moves == 0);
    assert(board->map[e1] == NULL);
    assert(board->map[board->from_castle_coords[white][king_side]] == NULL);
    assert(board->map[board->castling_coordinates[white][king_side]] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[board->to_castle_coords[white][king_side]] == board->castle_pieces[white][king_side]);

    undo_pretend_move(board, moves->moves[16], killed);

    assert(board->pieces[white][KING_INDEX(white)]->cell == e1);
    assert(board->castle_pieces[white][king_side]->cell == board->from_castle_coords[white][king_side]);
    assert(board->map[e1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[board->from_castle_coords[white][king_side]] == board->castle_pieces[white][king_side]);
    assert(moves->moves[16]->piece->no_moves == 0);
    assert(board->castle_pieces[white][king_side]->no_moves == 0);
    assert(board->map[board->castling_coordinates[white][king_side]] == NULL);
    assert(board->map[board->to_castle_coords[white][king_side]] == NULL);
    assert(saved_bitboard == board->bitboard);

    assert(moves->moves[13]->from == e1);
    assert(moves->moves[13]->destination == c1);
    assert(moves->moves[13]->castle == true);
    assert(moves->moves[13]->castle_side == queen_side);
    assert(moves->moves[13]->piece->no_moves == 0);
    assert(board->castle_pieces[white][queen_side]->no_moves == 0);
    assert(board->map[e1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[board->from_castle_coords[white][queen_side]] == board->castle_pieces[white][queen_side]);
    assert(board->map[board->castling_coordinates[white][queen_side]] == NULL);
    assert(board->map[board->to_castle_coords[white][queen_side]] == NULL);

    killed = pretend_move(board, moves->moves[13]);

    assert(board->pieces[white][KING_INDEX(white)]->cell == board->castling_coordinates[white][queen_side]);
    assert(board->castle_pieces[white][queen_side]->cell == board->to_castle_coords[white][queen_side]);
    assert(moves->moves[13]->piece->no_moves == 1);
    assert(board->castle_pieces[white][queen_side]->no_moves == 0);
    assert(board->map[e1] == NULL);
    assert(board->map[board->from_castle_coords[white][queen_side]] == NULL);
    assert(board->map[board->castling_coordinates[white][queen_side]] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[board->to_castle_coords[white][queen_side]] == board->castle_pieces[white][queen_side]);

    undo_pretend_move(board, moves->moves[13], killed);

    assert(board->pieces[white][KING_INDEX(white)]->cell == e1);
    assert(board->castle_pieces[white][queen_side]->cell == board->from_castle_coords[white][queen_side]);
    assert(board->map[e1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[board->from_castle_coords[white][queen_side]] == board->castle_pieces[white][queen_side]);
    assert(moves->moves[13]->piece->no_moves == 0);
    assert(board->castle_pieces[white][queen_side]->no_moves == 0);
    assert(board->map[board->castling_coordinates[white][queen_side]] == NULL);
    assert(board->map[board->to_castle_coords[white][queen_side]] == NULL);
    assert(saved_bitboard == board->bitboard);
}

void test_castling_5() {
    Board* board = set_board_notation("wke1 wch1 wca1 ");
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    Moves* moves = get_all_moves_for_colour(board, white);
    
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->destination == g1 && moves->moves[i]->piece->type == king));
        assert(!(moves->moves[i]->destination == c1 && moves->moves[i]->piece->type == king));
        assert(!moves->moves[i]->castle);
    }

    board->pieces[white][KING_INDEX(white)]->no_moves = 0;
    board->castle_pieces[white][king_side]->no_moves = 1;
    U64 saved_bitboard = board->bitboard;
    moves = get_all_moves_for_colour(board, white);
    
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->destination == g1 && moves->moves[i]->piece->type == king));
        assert(!(moves->moves[i]->castle && moves->moves[i]->castle_side == king_side));
    }

    assert(moves->moves[13]->from == e1);
    assert(moves->moves[13]->destination == c1);
    assert(moves->moves[13]->castle == true);
    assert(moves->moves[13]->castle_side == queen_side);
    assert(moves->moves[13]->piece->no_moves == 0);
    assert(board->castle_pieces[white][queen_side]->no_moves == 0);
    assert(board->map[e1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[board->from_castle_coords[white][queen_side]] == board->castle_pieces[white][queen_side]);
    assert(board->map[board->castling_coordinates[white][queen_side]] == NULL);
    assert(board->map[board->to_castle_coords[white][queen_side]] == NULL);

    Piece* killed = pretend_move(board, moves->moves[13]);

    assert(board->pieces[white][KING_INDEX(white)]->cell == board->castling_coordinates[white][queen_side]);
    assert(board->castle_pieces[white][queen_side]->cell == board->to_castle_coords[white][queen_side]);
    assert(moves->moves[13]->piece->no_moves == 1);
    assert(board->castle_pieces[white][queen_side]->no_moves == 0);
    assert(board->map[e1] == NULL);
    assert(board->map[board->from_castle_coords[white][queen_side]] == NULL);
    assert(board->map[board->castling_coordinates[white][queen_side]] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[board->to_castle_coords[white][queen_side]] == board->castle_pieces[white][queen_side]);

    undo_pretend_move(board, moves->moves[13], killed);

    assert(board->pieces[white][KING_INDEX(white)]->cell == e1);
    assert(board->castle_pieces[white][queen_side]->cell == board->from_castle_coords[white][queen_side]);
    assert(board->map[e1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[board->from_castle_coords[white][queen_side]] == board->castle_pieces[white][queen_side]);
    assert(moves->moves[13]->piece->no_moves == 0);
    assert(board->castle_pieces[white][queen_side]->no_moves == 0);
    assert(board->map[board->castling_coordinates[white][queen_side]] == NULL);
    assert(board->map[board->to_castle_coords[white][queen_side]] == NULL);
    assert(saved_bitboard == board->bitboard);
}

void test_castling_6() {
    Board* board = set_board_notation("wke1 wch1 wbf1 ");
    Moves* moves = get_all_moves_for_colour(board, white);

    for (int i = 0; i < moves->length; i++) {
        assert(!moves->moves[i]->castle);
    }
}

void test_castling_7() {
    Board* board = set_board_notation("wke1 wch1 wbg1 ");
    Moves* moves = get_all_moves_for_colour(board, white);

    for (int i = 0; i < moves->length; i++) {
        assert(!moves->moves[i]->castle);
    }
}

void test_castling_8() {
    Board* board = set_board_notation("wke1 wch1 bbf1 ");
    Moves* moves = get_all_moves_for_colour(board, white);
    
    for (int i = 0; i < moves->length; i++) {
        assert(!moves->moves[i]->castle);
    }
}

void test_castling_9() {
    Board* board = set_board_notation("wke1 wch1 bbg1 ");
    Moves* moves = get_all_moves_for_colour(board, white);
    
    for (int i = 0; i < moves->length; i++) {
        assert(!moves->moves[i]->castle);
    }
}

void test_castling_10() {
    Board* board = set_board_notation("wke1 wch1 bbg1 ");
    board->castle_pieces[white][king_side]->no_moves = 1;
    Moves* moves = get_all_moves_for_colour(board, white);
    
    for (int i = 0; i < moves->length; i++) {
        assert(!moves->moves[i]->castle);
    }
}

void test_castling_11() {
    // can't just use set board notation because the index it uses for the castle conflicts.
    Board* board = set_board_notation("bke8 ");
    board->castle_pieces[black][queen_side]->alive = true;
    board->castle_pieces[black][queen_side]->cell = a8;
    board->map[a8] = board->castle_pieces[black][queen_side];
    set_bit(board->bitboard, a8);
    Moves* moves = get_all_moves_for_colour(board, black);
    
    bool found = false;
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->castle && moves->moves[i]->castle_side == king_side));
        if (moves->moves[i]->castle && moves->moves[i]->castle_side == queen_side) {
            found = true;
        }
    }
    assert(found);
}

void test_castling_12() {
    // can't just use set board notation because the index it uses for the castle conflicts.
    Board* board = set_board_notation("bke8 bhc8 ");
    board->castle_pieces[black][queen_side]->alive = true;
    board->castle_pieces[black][queen_side]->cell = a8;
    board->map[a8] = board->castle_pieces[black][queen_side];
    set_bit(board->bitboard, a8);
    Moves* moves = get_all_moves_for_colour(board, black);
    
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->castle));
    }
}

void test_castling_13() {
    // can't just use set board notation because the index it uses for the castle conflicts.
    Board* board = set_board_notation("bke8 bhb8 ");
    board->castle_pieces[black][queen_side]->alive = true;
    board->castle_pieces[black][queen_side]->cell = a8;
    board->map[a8] = board->castle_pieces[black][queen_side];
    set_bit(board->bitboard, a8);
    Moves* moves = get_all_moves_for_colour(board, black);
    
    for (int i = 0; i < moves->length; i++) {
        assert(!(moves->moves[i]->castle));
    }
}

void test_promotion_1() {
    Board* board = set_board_notation("wpb7 bcc8 ");
    Moves* moves = get_all_moves_for_colour(board, white);
    U64 hash = board->hash_value;
    assert(moves->length == 8);

    assert(moves->moves[0]->piece->type == pawn);
    assert(moves->moves[0]->piece->value == PAWN_VALUE);
    assert(moves->moves[0]->piece->move_func == get_pawn_mask);
    assert(moves->moves[0]->piece->cell == b7);
    assert(moves->moves[0]->from == b7);
    assert(moves->moves[0]->destination == b8);
    assert(board->map[b7] == moves->moves[0]->piece);
    assert(board->map[b8] == NULL);

    Piece* killed = pretend_move(board, moves->moves[0]);
    hash_move_piece(board, moves->moves[0], killed);

    assert(moves->moves[0]->piece->type == queen);
    assert(moves->moves[0]->piece->value == QUEEN_VALUE);
    assert(moves->moves[0]->piece->move_func == get_queen_mask);
    assert(moves->moves[0]->piece->cell == b8);
    assert(board->map[b8] == moves->moves[0]->piece);
    assert(board->map[b7] == NULL);

    hash_move_piece(board, moves->moves[0], killed);
    undo_pretend_move(board, moves->moves[0], killed);
    
    assert(hash == board->hash_value);
    assert(moves->moves[0]->piece->type == pawn);
    assert(moves->moves[0]->piece->value == PAWN_VALUE);
    assert(moves->moves[0]->piece->move_func == get_pawn_mask);
    assert(moves->moves[0]->piece->cell == b7);
    assert(board->map[b7] == moves->moves[0]->piece);
    assert(board->map[b8] == NULL);



    assert(moves->moves[1]->piece->type == pawn);
    assert(moves->moves[1]->piece->value == PAWN_VALUE);
    assert(moves->moves[1]->piece->move_func == get_pawn_mask);
    assert(moves->moves[1]->piece->cell == b7);
    assert(moves->moves[1]->from == b7);
    assert(moves->moves[1]->destination == b8);
    assert(board->map[b7] == moves->moves[1]->piece);
    assert(board->map[b8] == NULL);

    killed = pretend_move(board, moves->moves[1]);
    hash_move_piece(board, moves->moves[1], killed);

    assert(moves->moves[1]->piece->type == castle);
    assert(moves->moves[1]->piece->value == CASTLE_VALUE);
    assert(moves->moves[1]->piece->move_func == get_castle_mask);
    assert(moves->moves[1]->piece->cell == b8);
    assert(board->map[b8] == moves->moves[1]->piece);
    assert(board->map[b7] == NULL);

    hash_move_piece(board, moves->moves[1], killed);
    undo_pretend_move(board, moves->moves[1], killed);
    
    assert(board->hash_value == hash);
    assert(moves->moves[1]->piece->type == pawn);
    assert(moves->moves[1]->piece->value == PAWN_VALUE);
    assert(moves->moves[1]->piece->move_func == get_pawn_mask);
    assert(moves->moves[1]->piece->cell == b7);
    assert(board->map[b7] == moves->moves[1]->piece);
    assert(board->map[b8] == NULL);





    assert(moves->moves[2]->piece->type == pawn);
    assert(moves->moves[2]->piece->value == PAWN_VALUE);
    assert(moves->moves[2]->piece->move_func == get_pawn_mask);
    assert(moves->moves[2]->piece->cell == b7);
    assert(moves->moves[2]->from == b7);
    assert(moves->moves[2]->destination == b8);
    assert(board->map[b7] == moves->moves[2]->piece);
    assert(board->map[b8] == NULL);

    killed = pretend_move(board, moves->moves[2]);
    hash_move_piece(board, moves->moves[2], killed);

    assert(moves->moves[2]->piece->type == bishop);
    assert(moves->moves[2]->piece->value == BISHOP_VALUE);
    assert(moves->moves[2]->piece->move_func == get_bishop_mask);
    assert(moves->moves[2]->piece->cell == b8);
    assert(board->map[b8] == moves->moves[2]->piece);
    assert(board->map[b7] == NULL);

    hash_move_piece(board, moves->moves[2], killed);
    undo_pretend_move(board, moves->moves[2], killed);
    
    assert(hash == board->hash_value);
    assert(moves->moves[2]->piece->type == pawn);
    assert(moves->moves[2]->piece->value == PAWN_VALUE);
    assert(moves->moves[2]->piece->move_func == get_pawn_mask);
    assert(moves->moves[2]->piece->cell == b7);
    assert(board->map[b7] == moves->moves[2]->piece);
    assert(board->map[b8] == NULL);




    assert(moves->moves[3]->piece->type == pawn);
    assert(moves->moves[3]->piece->value == PAWN_VALUE);
    assert(moves->moves[3]->piece->move_func == get_pawn_mask);
    assert(moves->moves[3]->piece->cell == b7);
    assert(moves->moves[3]->from == b7);
    assert(moves->moves[3]->destination == b8);
    assert(board->map[b7] == moves->moves[3]->piece);
    assert(board->map[b8] == NULL);

    killed = pretend_move(board, moves->moves[3]);
    hash_move_piece(board, moves->moves[3], killed);

    assert(moves->moves[3]->piece->type == knight);
    assert(moves->moves[3]->piece->value == KNIGHT_VALUE);
    assert(moves->moves[3]->piece->move_func == get_knight_mask);
    assert(moves->moves[3]->piece->cell == b8);
    assert(board->map[b8] == moves->moves[3]->piece);
    assert(board->map[b7] == NULL);

    hash_move_piece(board, moves->moves[3], killed);
    undo_pretend_move(board, moves->moves[3], killed);
    
    assert(hash == board->hash_value);
    assert(moves->moves[3]->piece->type == pawn);
    assert(moves->moves[3]->piece->value == PAWN_VALUE);
    assert(moves->moves[3]->piece->move_func == get_pawn_mask);
    assert(moves->moves[3]->piece->cell == b7);
    assert(board->map[b7] == moves->moves[3]->piece);
    assert(board->map[b8] == NULL);



    assert(moves->moves[4]->piece->type == pawn);
    assert(moves->moves[4]->piece->value == PAWN_VALUE);
    assert(moves->moves[4]->piece->move_func == get_pawn_mask);
    assert(moves->moves[4]->piece->cell == b7);
    assert(moves->moves[4]->from == b7);
    assert(moves->moves[4]->destination == c8);
    assert(board->map[b7] == moves->moves[4]->piece);
    assert(board->map[b8] == NULL);
    assert(board->map[c8]->type == castle);
    assert(board->map[c8]->c == black);
    assert(board->map[c8]->value == CASTLE_VALUE);

    killed = pretend_move(board, moves->moves[4]);
    hash_move_piece(board, moves->moves[4], killed);

    assert(moves->moves[4]->piece->type == queen);
    assert(moves->moves[4]->piece->value == QUEEN_VALUE);
    assert(moves->moves[4]->piece->move_func == get_queen_mask);
    assert(moves->moves[4]->piece->cell == c8);
    assert(board->map[c8] == moves->moves[4]->piece);
    assert(board->map[b7] == NULL);
    assert(board->map[b8] == NULL);

    hash_move_piece(board, moves->moves[4], killed);
    undo_pretend_move(board, moves->moves[4], killed);

    assert(hash == board->hash_value);
    assert(moves->moves[4]->piece->type == pawn);
    assert(moves->moves[4]->piece->value == PAWN_VALUE);
    assert(moves->moves[4]->piece->move_func == get_pawn_mask);
    assert(moves->moves[4]->piece->cell == b7);
    assert(board->map[b7] == moves->moves[4]->piece);
    assert(board->map[b8] == NULL);
    assert(board->map[c8]);
    assert(board->map[c8]->type == castle);
    assert(board->map[c8]->c == black);
    assert(board->map[c8]->value == CASTLE_VALUE);



    assert(moves->moves[5]->piece->type == pawn);
    assert(moves->moves[5]->piece->value == PAWN_VALUE);
    assert(moves->moves[5]->piece->move_func == get_pawn_mask);
    assert(moves->moves[5]->piece->cell == b7);
    assert(moves->moves[5]->from == b7);
    assert(moves->moves[5]->destination == c8);
    assert(board->map[b7] == moves->moves[5]->piece);
    assert(board->map[b8] == NULL);
    assert(board->map[c8]->type == castle);
    assert(board->map[c8]->c == black);
    assert(board->map[c8]->value == CASTLE_VALUE);

    killed = pretend_move(board, moves->moves[5]);
    hash_move_piece(board, moves->moves[5], killed);

    assert(moves->moves[1]->piece->type == castle);
    assert(moves->moves[1]->piece->value == CASTLE_VALUE);
    assert(moves->moves[1]->piece->move_func == get_castle_mask);
    assert(moves->moves[1]->piece->cell == c8);
    assert(board->map[c8] == moves->moves[5]->piece);
    assert(board->map[b7] == NULL);
    assert(board->map[b8] == NULL);

    hash_move_piece(board, moves->moves[5], killed);
    undo_pretend_move(board, moves->moves[5], killed);
    
    assert(hash == board->hash_value);
    assert(moves->moves[5]->piece->type == pawn);
    assert(moves->moves[5]->piece->value == PAWN_VALUE);
    assert(moves->moves[5]->piece->move_func == get_pawn_mask);
    assert(moves->moves[5]->piece->cell == b7);
    assert(board->map[b7] == moves->moves[5]->piece);
    assert(board->map[b8] == NULL);
    assert(board->map[c8]->type == castle);
    assert(board->map[c8]->c == black);
    assert(board->map[c8]->value == CASTLE_VALUE);





    assert(moves->moves[6]->piece->type == pawn);
    assert(moves->moves[6]->piece->value == PAWN_VALUE);
    assert(moves->moves[6]->piece->move_func == get_pawn_mask);
    assert(moves->moves[6]->piece->cell == b7);
    assert(moves->moves[6]->from == b7);
    assert(moves->moves[6]->destination == c8);
    assert(board->map[b7] == moves->moves[6]->piece);
    assert(board->map[b8] == NULL);
    assert(board->map[c8]->type == castle);
    assert(board->map[c8]->c == black);
    assert(board->map[c8]->value == CASTLE_VALUE);

    killed = pretend_move(board, moves->moves[6]);
    hash_move_piece(board, moves->moves[6], killed);

    assert(moves->moves[6]->piece->type == bishop);
    assert(moves->moves[6]->piece->value == BISHOP_VALUE);
    assert(moves->moves[6]->piece->move_func == get_bishop_mask);
    assert(moves->moves[6]->piece->cell == c8);
    assert(board->map[c8] == moves->moves[6]->piece);
    assert(board->map[b7] == NULL);
    assert(board->map[b8] == NULL);

    hash_move_piece(board, moves->moves[6], killed);
    undo_pretend_move(board, moves->moves[6], killed);
    
    assert(hash == board->hash_value);
    assert(moves->moves[6]->piece->type == pawn);
    assert(moves->moves[6]->piece->value == PAWN_VALUE);
    assert(moves->moves[6]->piece->move_func == get_pawn_mask);
    assert(moves->moves[6]->piece->cell == b7);
    assert(board->map[b7] == moves->moves[6]->piece);
    assert(board->map[b8] == NULL);
    assert(board->map[c8]->type == castle);
    assert(board->map[c8]->c == black);
    assert(board->map[c8]->value == CASTLE_VALUE);




    assert(moves->moves[7]->piece->type == pawn);
    assert(moves->moves[7]->piece->value == PAWN_VALUE);
    assert(moves->moves[7]->piece->move_func == get_pawn_mask);
    assert(moves->moves[7]->piece->cell == b7);
    assert(moves->moves[7]->from == b7);
    assert(moves->moves[7]->destination == c8);
    assert(board->map[b7] == moves->moves[7]->piece);
    assert(board->map[b8] == NULL);
    assert(board->map[c8]->type == castle);
    assert(board->map[c8]->c == black);
    assert(board->map[c8]->value == CASTLE_VALUE);

    killed = pretend_move(board, moves->moves[7]);
    hash_move_piece(board, moves->moves[7], killed);

    assert(moves->moves[7]->piece->type == knight);
    assert(moves->moves[7]->piece->value == KNIGHT_VALUE);
    assert(moves->moves[7]->piece->move_func == get_knight_mask);
    assert(moves->moves[7]->piece->cell == c8);
    assert(board->map[c8] == moves->moves[7]->piece);
    assert(board->map[b7] == NULL);
    assert(board->map[b8] == NULL);

    hash_move_piece(board, moves->moves[7], killed);
    undo_pretend_move(board, moves->moves[7], killed);
    
    assert(hash == board->hash_value);
    assert(moves->moves[7]->piece->type == pawn);
    assert(moves->moves[7]->piece->value == PAWN_VALUE);
    assert(moves->moves[7]->piece->move_func == get_pawn_mask);
    assert(moves->moves[7]->piece->cell == b7);
    assert(board->map[b7] == moves->moves[7]->piece);
    assert(board->map[b8] == NULL);
    assert(board->map[c8]->type == castle);
    assert(board->map[c8]->c == black);
    assert(board->map[c8]->value == CASTLE_VALUE);
}

void test_moves() {
    printf("test_pawn_attacks\n");
    test_pawn_attacks();
    printf("test_pawn_moves\n");
    test_pawn_moves();
    printf("test_knight_moves\n");
    test_knight_moves();
    printf("test_king_moves\n");
    test_king_moves();
    printf("test_castle_moves\n");
    test_castle_moves();
    printf("test_bishop_moves\n");
    test_bishop_moves();
    printf("test_queen_moves\n");
    test_queen_moves();
    printf("test_pawn_en_passant_1\n");
    test_pawn_en_passant_1();
    printf("test_pawn_en_passant_2\n");
    test_pawn_en_passant_2();
    printf("test_pawn_en_passant_3\n");
    test_pawn_en_passant_3();
    printf("test_pawn_en_passant_4\n");
    test_pawn_en_passant_4();
    printf("test_pawn_en_passant_5\n");
    test_pawn_en_passant_5();
    printf("test_pawn_en_passant_6\n");
    test_pawn_en_passant_6();
    printf("test_pawn_en_passant_7\n");
    test_pawn_en_passant_7();
    printf("test_pawn_en_passant_8\n");
    test_pawn_en_passant_8();
    printf("test_pawn_en_passant_9\n");
    test_pawn_en_passant_9();
    printf("test_pawn_en_passant_10\n");
    test_pawn_en_passant_10();
    printf("test_pawn_en_passant_legality_1\n");
    test_pawn_en_passant_legality_1();
    printf("test_pawn_en_passant_legality_2\n");
    test_pawn_en_passant_legality_2();
    printf("test_pawn_en_passant_legality_3\n");
    test_pawn_en_passant_legality_3();
    printf("test_pawn_en_passant_legality_4\n");
    test_pawn_en_passant_legality_4();
    printf("test_pawn_en_passant_legality_5\n");
    test_pawn_en_passant_legality_5();
    printf("test_castling_execution_white_short\n");
    test_castling_execution_white_short();
    printf("test_castling_execution_white_long\n");
    test_castling_execution_white_long();
    printf("test_castling_execution_black_short\n");
    test_castling_execution_black_short();
    printf("test_castling_execution_black_long\n");
    test_castling_execution_black_long();
    printf("test_castling_1\n");
    test_castling_1();
    printf("test_castling_2\n");
    test_castling_2();
    printf("test_castling_3\n");
    test_castling_3();
    printf("test_castling_4\n");
    test_castling_4();
    printf("test_castling_5\n");
    test_castling_5();
    printf("test_castling_6\n");
    test_castling_6();
    printf("test_castling_7\n");
    test_castling_7();
    printf("test_castling_8\n");
    test_castling_8();
    printf("test_castling_9\n");
    test_castling_9();
    printf("test_castling_10\n");
    test_castling_10();
    printf("test_castling_11\n");
    test_castling_11();
    printf("test_castling_12\n");
    test_castling_12();
    printf("test_castling_13\n");
    test_castling_13();
    printf("test_promotion_1\n");
    test_promotion_1();
}






void test_check_3() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = e1;
    board->map[e1] = board->pieces[white][KING_INDEX(white)];

    board->pieces[white][10]->alive = true;
    board->pieces[white][10]->cell = g5;
    board->map[30] = board->pieces[white][10];

    board->pieces[black][0]->alive = true;
    board->pieces[black][0]->cell = e8;
    board->map[e8] = board->pieces[black][0];

    set_bit(board->bitboard, e8);
    set_bit(board->bitboard, e1);
    set_bit(board->bitboard, g5);

    U64 test_board = 0ULL;
    set_bit(test_board, e1);
    set_bit(test_board, e8);
    set_bit(test_board, g5);
    assert(test_board == board->bitboard);

    bool legal = is_move_legal(board, board->pieces[white][10], h6);
    assert(!legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][10], h4);
    assert(!legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][10], e3);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][10], e7);
    assert(legal);
    assert(test_board == board->bitboard);


    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], e2);
    assert(!legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], f1);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], d1);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], d2);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], f2);
    assert(legal);
    assert(test_board == board->bitboard);
}

void test_check_2() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = e1;
    board->map[e1] = board->pieces[white][KING_INDEX(white)];

    board->pieces[white][10]->alive = true;
    board->pieces[white][10]->cell = e5;
    board->map[28] = board->pieces[white][10];

    board->pieces[black][0]->alive = true;
    board->pieces[black][0]->cell = e8;
    board->map[e8] = board->pieces[black][0];
    set_bit(board->bitboard, e8);
    set_bit(board->bitboard, e1);
    set_bit(board->bitboard, e5);

    U64 test_board = 0ULL;
    set_bit(test_board, e1);
    set_bit(test_board, e8);
    set_bit(test_board, e5);
    assert(test_board == board->bitboard);

    bool legal = is_move_legal(board, board->pieces[white][10], f6);
    assert(!legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][10], g7);
    assert(!legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][10], d4);
    assert(!legal);
    assert(test_board == board->bitboard);


    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], e2);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], f1);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], d1);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], d2);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], f2);
    assert(legal);
    assert(test_board == board->bitboard);
}

void test_check_1() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = e1;
    board->map[e1] = board->pieces[white][KING_INDEX(white)];
    board->pieces[black][0]->alive = true;
    board->pieces[black][0]->cell = e8;
    board->map[e8] = board->pieces[black][0];
    set_bit(board->bitboard, e8);
    set_bit(board->bitboard, e1);

    U64 test_board = 0ULL;
    set_bit(test_board, e1);
    set_bit(test_board, e8);
    assert(test_board == board->bitboard);


    bool legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], e2);
    assert(!legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], f1);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], d1);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], d2);
    assert(legal);
    assert(test_board == board->bitboard);

    legal = is_move_legal(board, board->pieces[white][KING_INDEX(white)], f2);
    assert(legal);
    assert(test_board == board->bitboard);
}


void test_legality() {
    test_check_1();
    test_check_2();
    test_check_3();
}

void test_get_all_moves_1() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = e1;
    board->map[e1] = board->pieces[white][KING_INDEX(white)];

    board->pieces[white][10]->alive = true;
    board->pieces[white][10]->cell = g5;
    board->map[g5] = board->pieces[white][10];

    board->pieces[black][0]->alive = true;
    board->pieces[black][0]->cell = e8;
    board->map[e8] = board->pieces[black][0];

    set_bit(board->bitboard, e8);
    set_bit(board->bitboard, e1);
    set_bit(board->bitboard, g5);

    U64 test_board = 0ULL;
    set_bit(test_board, e1);
    set_bit(test_board, e8);
    set_bit(test_board, g5);
    assert(test_board == board->bitboard);

    Moves* moves = get_all_moves_for_colour(board, white);

    assert(moves->length == 6);
    assert(moves->moves[0]->piece->type == bishop);
    assert(moves->moves[0]->destination == e7);
    assert(moves->moves[1]->piece->type == bishop);
    assert(moves->moves[1]->destination == e3);
    assert(moves->moves[2]->piece->type == king);
    assert(moves->moves[2]->destination == d2);
    assert(moves->moves[3]->piece->type == king);
    assert(moves->moves[3]->destination == f2);
    assert(moves->moves[4]->piece->type == king);
    assert(moves->moves[4]->destination == d1);
    assert(moves->moves[5]->piece->type == king);
    assert(moves->moves[5]->destination == f1);
    assert(test_board == board->bitboard);
}

void test_get_all_moves_2() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = b2;
    board->map[b2] = board->pieces[white][KING_INDEX(white)];
    board->pieces[white][KING_INDEX(white)]->no_moves = 1;

    board->pieces[black][7]->alive = true;
    board->pieces[black][7]->cell = c7;
    board->map[c7] = board->pieces[black][7];

    board->pieces[black][0]->alive = true;
    board->pieces[black][0]->cell = b8;
    board->map[b8] = board->pieces[black][0];

    set_bit(board->bitboard, b2);
    set_bit(board->bitboard, c7);
    set_bit(board->bitboard, b8);

    U64 test_board = 0ULL;
    set_bit(test_board, b2);
    set_bit(test_board, c7);
    set_bit(test_board, b8);
    assert(test_board == board->bitboard);
    Moves* moves = get_all_moves_for_colour(board, white);
    assert(moves->length == 3);
    assert(moves->moves[0]->destination == a3);
    assert(moves->moves[1]->destination == a2);
    assert(moves->moves[2]->destination == a1);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][KING_INDEX(white)]);
    assert(board->pieces[black][7]->cell == c7);
    assert(board->pieces[black][7]->type == castle);
    assert(board->map[c7] == board->pieces[black][7]);
    assert(board->pieces[black][0]->cell == b8);
    assert(board->pieces[black][7]->type == castle);
    assert(board->map[b8] == board->pieces[black][0]);
}






void test_move_logic() {

    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = b1;
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    board->map[b1] = board->pieces[white][KING_INDEX(white)];

    board->pieces[black][7]->alive = true;
    board->pieces[black][7]->cell = c7;
    board->map[c7] = board->pieces[white][7];

    board->pieces[black][0]->alive = true;
    board->pieces[black][0]->cell = e8;
    board->map[e8] = board->pieces[black][0];

    set_bit(board->bitboard, b1);
    set_bit(board->bitboard, c7);
    set_bit(board->bitboard, e8);

    U64 test_board = 0ULL;
    set_bit(test_board, b1);
    set_bit(test_board, c7);
    set_bit(test_board, e8);
    assert(test_board == board->bitboard);


    // SIMULATE FIRST MOVE
    Move* move_1 = calloc(1, sizeof(Move));
    move_1->piece = board->pieces[black][0];
    move_1->from = e8;
    move_1->destination = a8;

    Piece* killed = pretend_move(board, move_1);
    pop_bit(test_board, e8);
    set_bit(test_board, a8);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][0]->cell == a8);
    assert(board->map[a8] == board->pieces[black][0]);
    assert(board->map[e8] == NULL);

    // SIMULATE SECOND MOVE
    Move* move_2 = calloc(1, sizeof(Move));
    move_2->piece = board->pieces[white][KING_INDEX(white)];
    move_2->from = b1;
    move_2->destination = b2;

    Piece* killed_2 = pretend_move(board, move_2);
    pop_bit(test_board, b1);
    set_bit(test_board, b2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[b1] == NULL);

    // UNDO SECOND MOVE
    undo_pretend_move(board, move_2, killed_2);
    pop_bit(test_board, b2);
    set_bit(test_board, b1);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == b1);
    assert(board->map[b1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[b2] == NULL);

    // REVERSE FIRST MOVE
    undo_pretend_move(board, move_1, killed);
    pop_bit(test_board, a8);
    set_bit(test_board, e8);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][0]->cell == e8);
    assert(board->map[e8] == board->pieces[black][0]);
    assert(board->map[a8] == NULL);

}

void test_move_logic_mate_in_four() {
    Board* board = board_setup_1();
    U64 test_board = board->bitboard;
    U64 saved_board = test_board;

    // SIMULATE FIRST MOVE (BLACK)
    Move* move_1 = calloc(1, sizeof(Move));
    move_1->piece = board->pieces[black][CASTLE_1(black)];
    move_1->from = board->pieces[black][CASTLE_1(black)]->cell;
    move_1->destination = d1;

    Piece* killed = pretend_move(board, move_1);
    pop_bit(test_board, d7);
    set_bit(test_board, d1);
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d1);
    assert(board->map[d1] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d7] == NULL);

    undo_pretend_move(board, move_1, killed);
    assert(board->bitboard == saved_board);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d7);
    assert(board->map[d7] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d1] == NULL);

    killed = pretend_move(board, move_1);
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d1);
    assert(board->map[d1] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d7] == NULL);
    saved_board = board->bitboard;

    // SIMULATE FIRST MOVE (WHITE)
    Move* move_2 = calloc(1, sizeof(Move));
    move_2->piece = board->pieces[white][QUEEN_INDEX(white)];
    move_2->from = board->pieces[white][QUEEN_INDEX(white)]->cell;
    move_2->destination = d1;

    Piece* opponent_killed = pretend_move(board, move_2);
    pop_bit(test_board, e2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][QUEEN_INDEX(white)]->cell == d1);
    assert(board->map[d1] == board->pieces[white][QUEEN_INDEX(white)]);
    assert(opponent_killed == board->pieces[black][CASTLE_1(black)]);
    assert(!board->pieces[black][CASTLE_1(black)]->alive);
    assert(board->map[e2] == NULL);

    undo_pretend_move(board, move_2, opponent_killed);
    assert(saved_board == board->bitboard);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d1);
    assert(board->pieces[black][CASTLE_1(black)]->alive);
    assert(board->map[d1] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d7] == NULL);
    assert(board->pieces[white][QUEEN_INDEX(white)]->cell == e2);
    assert(board->map[e2] == board->pieces[white][QUEEN_INDEX(white)]);
    assert(board->pieces[white][QUEEN_INDEX(white)]->alive);

    opponent_killed = pretend_move(board, move_2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][QUEEN_INDEX(white)]->cell == d1);
    assert(board->map[d1] == board->pieces[white][QUEEN_INDEX(white)]);
    assert(opponent_killed == board->pieces[black][CASTLE_1(black)]);
    assert(!board->pieces[black][CASTLE_1(black)]->alive);
    assert(board->map[e2] == NULL);


    // SIMULATE SECOND MOVE (BLACK)
    Move* move_3 = calloc(1, sizeof(Move));
    move_3->piece = board->pieces[black][QUEEN_INDEX(black)];
    move_3->from = board->pieces[black][QUEEN_INDEX(black)]->cell;
    move_3->destination = a2;

    killed = pretend_move(board, move_3);
    pop_bit(test_board, a3);
    set_bit(test_board, a2);
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][QUEEN_INDEX(black)]->cell == a2);
    assert(board->map[a2] == board->pieces[black][QUEEN_INDEX(black)]);
    assert(board->map[a3] == NULL);

    // SIMULATE SECOND MOVE (WHITE)
    Move* move_4 = calloc(1, sizeof(Move));
    move_4->piece = board->pieces[white][KING_INDEX(white)];
    move_4->from = board->pieces[white][KING_INDEX(white)]->cell;
    move_4->destination = c1;

    opponent_killed = pretend_move(board, move_4);
    pop_bit(test_board, b1);
    set_bit(test_board, c1);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == c1);
    assert(board->map[c1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[b1] == NULL);
    assert(opponent_killed == NULL);

    // SIMULATE THIRD MOVE (BLACK)
    Move* move_5 = calloc(1, sizeof(Move));
    move_5->piece = board->pieces[black][BISHOP_1(black)];
    move_5->from = board->pieces[black][BISHOP_1(black)]->cell;
    move_5->destination = a3;

    killed = pretend_move(board, move_5);
    pop_bit(test_board, c5);
    set_bit(test_board, a3);
    saved_board = test_board;
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][BISHOP_1(black)]->cell == a3);
    assert(board->map[a3] == board->pieces[black][BISHOP_1(black)]);
    assert(board->map[c5] == NULL);

    // SIMULATE THIRD MOVE (WHITE)
    Move* move_6 = calloc(1, sizeof(Move));
    move_6->piece = board->pieces[white][BISHOP_1(white)];
    move_6->from = board->pieces[white][BISHOP_1(white)]->cell;
    move_6->destination = b2;

    opponent_killed = pretend_move(board, move_6);
    pop_bit(test_board, e5);
    set_bit(test_board, b2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][BISHOP_1(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][BISHOP_1(white)]);
    assert(board->map[e5] == NULL);
    assert(opponent_killed == NULL);

    undo_pretend_move(board, move_6, opponent_killed);
    assert(saved_board == board->bitboard);
    assert(board->pieces[black][BISHOP_1(black)]->cell == a3);
    assert(board->map[a3] == board->pieces[black][BISHOP_1(black)]);
    assert(board->map[c5] == NULL);

    opponent_killed = pretend_move(board, move_6);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][BISHOP_1(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][BISHOP_1(white)]);
    assert(board->map[e5] == NULL);
    assert(opponent_killed == NULL);

    // SIMULATE FOURTH MOVE (BLACK)
    Move* move_7 = calloc(1, sizeof(Move));
    move_7->piece = board->pieces[black][BISHOP_1(black)];
    move_7->from = board->pieces[black][BISHOP_1(black)]->cell;
    move_7->destination = b2;

    killed = pretend_move(board, move_7);
    pop_bit(test_board, a3);
    set_bit(test_board, b2);
    assert(killed==board->pieces[white][BISHOP_1(white)]);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][BISHOP_1(black)]->cell == b2);
    assert(board->map[b2] == board->pieces[black][BISHOP_1(black)]);
    assert(board->map[a3] == NULL);


    // SIMULATE FOURTH MOVE (WHITE)
    Move* move_8 = calloc(1, sizeof(Move));
    move_8->piece = board->pieces[white][KING_INDEX(white)];
    move_8->from = board->pieces[white][KING_INDEX(white)]->cell;
    move_8->destination = d2;

    opponent_killed = pretend_move(board, move_8);
    pop_bit(test_board, c1);
    set_bit(test_board, d2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == d2);
    assert(board->map[d2] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[c1] == NULL);
    assert(opponent_killed == NULL);

    
}







void test_move_selection() {
    test_get_all_moves_1();
    test_get_all_moves_2();
    test_moves_board_setup_1();
    test_moves_board_setup_2();
}

void test_detect_mate_2() {

    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = b1;
    board->map[b1] = board->pieces[white][KING_INDEX(white)];

    board->pieces[black][7]->alive = true;
    board->pieces[black][7]->cell = c7;
    board->map[c7] = board->pieces[black][7];

    board->pieces[black][0]->alive = true;
    board->pieces[black][0]->cell = e8;
    board->map[e8] = board->pieces[black][0];

    set_bit(board->bitboard, b1);
    set_bit(board->bitboard, c7);
    set_bit(board->bitboard, e8);

    U64 test_board = 0ULL;
    set_bit(test_board, b1);
    set_bit(test_board, c7);
    set_bit(test_board, e8);
    assert(test_board == board->bitboard);

    // print_board_pro(board);
    Grapher* grapher = init_grapher(3, 2, black);
    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));
    assert(test_board == board->bitboard);

    assert(scores->eval == MAX_SCORE - 1);
    assert(scores->moves->moves[0]->from == e8);
    assert(scores->moves->moves[0]->destination == b8);
    assert(scores->moves->moves[0]->piece->c == black);
    assert(scores->moves->moves[0]->piece->type == castle);

    assert(scores->moves->moves[2]->from == c7);
    assert(scores->moves->moves[2]->destination == a7);
    assert(scores->moves->moves[2]->piece->c == black);
    assert(scores->moves->moves[2]->piece->type == castle);
}

void test_detect_mate_1() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = c1;
    board->map[c1] = board->pieces[white][KING_INDEX(white)];
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;

    board->pieces[black][7]->alive = true;
    board->pieces[black][7]->cell = b7;
    board->map[b7] = board->pieces[black][7];

    board->pieces[black][0]->alive = true;
    board->pieces[black][0]->cell = e8;
    board->map[e8] = board->pieces[black][0];

    set_bit(board->bitboard, c1);
    set_bit(board->bitboard, b7);
    set_bit(board->bitboard, e8);

    U64 test_board = 0ULL;
    set_bit(test_board, c1);
    set_bit(test_board, b7);
    set_bit(test_board, e8);
    assert(test_board == board->bitboard);

    // DIFFICULT MATE IN 4 (no threats made immediately)
    // print_board_pro(board);
    Grapher* grapher = init_grapher(10, 4, black);
    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));
    // assert(score == 0);
    assert(test_board == board->bitboard);
    // print_scores(scores);


    // EASY MATE IN 1 BELOW:
    board->pieces[white][KING_INDEX(white)]->cell = a1;
    board->map[a1] = board->pieces[white][KING_INDEX(white)];
    pop_bit(board->bitboard, c1);
    pop_bit(test_board, c1);
    set_bit(board->bitboard, a1);
    set_bit(test_board, a1);

    // print_board_pro(board);
    Grapher* grapher2 = init_grapher(1, 4, black);
    scores = create_graph(grapher2, grapher2->start, board, black, init_limit(true));
    assert(test_board == board->bitboard);
    // print_scores(scores);

    assert(scores->moves->length == 1);
    assert(scores->moves->moves[0]->piece->type == castle);
    assert(scores->moves->moves[0]->piece->c == black);
    assert(scores->moves->moves[0]->from == e8);
    assert(scores->moves->moves[0]->destination == a8);
    assert(scores->eval == MAX_SCORE);
}

void test_mate_in_four() {
    Board* board = board_setup_1();

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(10, 4, black); 
    // should work with 2, 4 in old version
    // evaluation function needs to be improved

    // print_board_pro(board);
    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

    assert(scores->eval == MAX_SCORE - 3);

    assert(scores->moves->moves[0]->from == d7);
    assert(scores->moves->moves[0]->destination == d1);
    assert(scores->moves->moves[0]->piece->c == black);
    assert(scores->moves->moves[0]->piece->type == castle);

    assert(scores->moves->moves[1]->from == e2);
    assert(scores->moves->moves[1]->destination == d1);
    assert(scores->moves->moves[1]->piece->c == white);
    assert(scores->moves->moves[1]->piece->type == queen);

    assert(scores->moves->moves[2]->from == a3);
    assert(scores->moves->moves[2]->destination == a2);
    assert(scores->moves->moves[2]->piece->c == black);
    assert(scores->moves->moves[2]->piece->type == queen);

    assert(scores->moves->moves[3]->from == b1);
    assert(scores->moves->moves[3]->destination == c1);
    assert(scores->moves->moves[3]->piece->c == white);
    assert(scores->moves->moves[3]->piece->type == king);

    assert(scores->moves->moves[4]->from == c5);
    assert(scores->moves->moves[4]->destination == a3);
    assert(scores->moves->moves[4]->piece->c == black);
    assert(scores->moves->moves[4]->piece->type == bishop);

    assert(scores->moves->moves[5]->from == e5);
    assert(scores->moves->moves[5]->destination == b2);
    assert(scores->moves->moves[5]->piece->c == white);
    assert(scores->moves->moves[5]->piece->type == bishop);

    assert(scores->moves->moves[6]->from == a2);
    assert(scores->moves->moves[6]->destination == b2);
    assert(scores->moves->moves[6]->piece->c == black);
    assert(scores->moves->moves[6]->piece->type == queen);
}


void test_mate_in_four_2() {
    Board* board = board_setup_m4_2();
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(10, 4, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);
    // print_scores(scores);

    assert(scores->eval == MAX_SCORE - 3);

    assert(scores->moves->moves[0]->from == e5);
    assert(scores->moves->moves[0]->destination == f7);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == knight);

    assert(scores->moves->moves[1]->from == d7);
    assert(scores->moves->moves[1]->destination == f7);
    assert(scores->moves->moves[1]->piece->c == black);
    assert(scores->moves->moves[1]->piece->type == queen);

    assert(scores->moves->moves[2]->from == g5);
    assert(scores->moves->moves[2]->destination == f7);
    assert(scores->moves->moves[2]->piece->c == white);
    assert(scores->moves->moves[2]->piece->type == knight);

    assert(scores->moves->moves[3]->from == d8);
    assert(scores->moves->moves[3]->destination == d7);
    assert(scores->moves->moves[3]->piece->c == black);
    assert(scores->moves->moves[3]->piece->type == king);

    // assert(scores->moves->moves[4]->from == c4);
    // assert(scores->moves->moves[4]->destination == b5);
    // assert(scores->moves->moves[4]->piece->c == white);
    // assert(scores->moves->moves[4]->piece->type == bishop);

    // assert(scores->moves->moves[5]->from == c7);
    // assert(scores->moves->moves[5]->destination == c6);
    // assert(scores->moves->moves[5]->piece->c == black);
    // assert(scores->moves->moves[5]->piece->type == pawn);

    // assert(scores->moves->moves[6]->from == e1);
    // assert(scores->moves->moves[6]->destination == e7);
    // assert(scores->moves->moves[6]->piece->c == white);
    // assert(scores->moves->moves[6]->piece->type == castle);
}

void test_mate_detection() {
    printf("test_detect_mate_1\n");
    test_detect_mate_1();
    printf("test_detect_mate_2\n");
    test_detect_mate_2();
    printf("test_mate_in_four\n");
    test_mate_in_four();
    printf("test_mate_in_four_2\n");
    test_mate_in_four_2();
}

Board* forcing_move_setup() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[black][KING_INDEX(black)]->alive = true;
    board->pieces[black][KING_INDEX(black)]->cell = g8;
    board->pieces[black][KING_INDEX(black)]->no_moves = 1;
    board->map[g8] = board->pieces[black][KING_INDEX(black)];

    board->pieces[black][CASTLE_1(black)]->alive = true;
    board->pieces[black][CASTLE_1(black)]->cell = f8;
    board->map[f8] = board->pieces[black][CASTLE_1(black)];

    board->pieces[black][CASTLE_2(black)]->alive = true;
    board->pieces[black][CASTLE_2(black)]->cell = d8;
    board->map[d8] = board->pieces[black][CASTLE_2(black)];

    board->pieces[black][BISHOP_1(black)]->alive = true;
    board->pieces[black][BISHOP_1(black)]->cell = f7;
    board->map[f7] = board->pieces[black][BISHOP_1(black)];

    board->pieces[black][BISHOP_2(black)]->alive = true;
    board->pieces[black][BISHOP_2(black)]->cell = f6;
    board->map[f6] = board->pieces[black][BISHOP_2(black)];

    board->pieces[black][QUEEN_INDEX(black)]->alive = true;
    board->pieces[black][QUEEN_INDEX(black)]->cell = a6;
    board->map[a6] = board->pieces[black][QUEEN_INDEX(black)];

    board->pieces[black][PAWN_INDEX(black, 1)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 1)]->cell = h7;
    board->map[h7] = board->pieces[black][PAWN_INDEX(black, 1)];

    board->pieces[black][PAWN_INDEX(black, 2)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 2)]->cell = g7;
    board->map[g7] = board->pieces[black][PAWN_INDEX(black, 2)];

    board->pieces[black][PAWN_INDEX(black, 3)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 3)]->cell = e5;
    board->map[e5] = board->pieces[black][PAWN_INDEX(black, 3)];

    board->pieces[black][PAWN_INDEX(black, 4)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 4)]->cell = b7;
    board->map[b7] = board->pieces[black][PAWN_INDEX(black, 4)];

    board->pieces[black][PAWN_INDEX(black, 5)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 5)]->cell = a7;
    board->map[a7] = board->pieces[black][PAWN_INDEX(black, 5)];



    board->pieces[white][PAWN_INDEX(white, 1)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 1)]->cell = h2;
    board->map[h2] = board->pieces[white][PAWN_INDEX(white, 1)];

    board->pieces[white][PAWN_INDEX(white, 2)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 2)]->cell = g2;
    board->map[g2] = board->pieces[white][PAWN_INDEX(white, 2)];

    board->pieces[white][PAWN_INDEX(white, 3)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 3)]->cell = e4;
    board->map[e4] = board->pieces[white][PAWN_INDEX(white, 3)];

    board->pieces[white][PAWN_INDEX(white, 4)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 4)]->cell = c3;
    board->map[c3] = board->pieces[white][PAWN_INDEX(white, 4)];

    board->pieces[white][PAWN_INDEX(white, 5)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 5)]->cell = b3;
    board->map[b3] = board->pieces[white][PAWN_INDEX(white, 5)];

    board->pieces[white][PAWN_INDEX(white, 6)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 6)]->cell = a2;
    board->map[a2] = board->pieces[white][PAWN_INDEX(white, 6)];

    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = e1;
    board->pieces[white][KING_INDEX(white)]->no_moves = 1;
    board->map[e1] = board->pieces[white][KING_INDEX(white)];

    board->pieces[white][QUEEN_INDEX(white)]->alive = true;
    board->pieces[white][QUEEN_INDEX(white)]->cell = g3;
    board->map[g3] = board->pieces[white][QUEEN_INDEX(white)];

    board->pieces[white][BISHOP_1(white)]->alive = true;
    board->pieces[white][BISHOP_1(white)]->cell = d2;
    board->map[d2] = board->pieces[white][BISHOP_1(white)];

    board->pieces[white][KNIGHT_1(white)]->alive = true;
    board->pieces[white][KNIGHT_1(white)]->cell = g1;
    board->map[g1] = board->pieces[white][KNIGHT_1(white)];

    board->pieces[white][KNIGHT_2(white)]->alive = true;
    board->pieces[white][KNIGHT_2(white)]->cell = c2;
    board->map[c2] = board->pieces[white][KNIGHT_2(white)];

    board->pieces[white][CASTLE_1(white)]->alive = true;
    board->pieces[white][CASTLE_1(white)]->cell = h1;
    board->map[h1] = board->pieces[white][CASTLE_1(white)];

    board->pieces[white][CASTLE_2(white)]->alive = true;
    board->pieces[white][CASTLE_2(white)]->cell = a1;
    board->map[a1] = board->pieces[white][CASTLE_2(white)];

    set_board(board);
    return board;
    

}

Board* board_setup_m4_2() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = g1;
    board->map[g1] = board->pieces[white][KING_INDEX(white)];
    board->pieces[white][KING_INDEX(white)]->no_moves += 1;

    board->pieces[white][CASTLE_1(white)]->alive = true;
    board->pieces[white][CASTLE_1(white)]->cell = e1;
    board->map[e1] = board->pieces[white][CASTLE_1(white)];

    board->pieces[white][BISHOP_1(white)]->alive = true;
    board->pieces[white][BISHOP_1(white)]->cell = c4;
    board->map[c4] = board->pieces[white][BISHOP_1(white)];

    board->pieces[white][BISHOP_2(white)]->alive = true;
    board->pieces[white][BISHOP_2(white)]->cell = a3;
    board->map[a3] = board->pieces[white][BISHOP_2(white)];

    board->pieces[white][KNIGHT_1(white)]->alive = true;
    board->pieces[white][KNIGHT_1(white)]->cell = e5;
    board->map[e5] = board->pieces[white][KNIGHT_1(white)];

    board->pieces[white][KNIGHT_2(white)]->alive = true;
    board->pieces[white][KNIGHT_2(white)]->cell = g5;
    board->map[g5] = board->pieces[white][KNIGHT_2(white)];

    board->pieces[white][PAWN_INDEX(white, 1)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 1)]->cell = a2;
    board->map[a2] = board->pieces[white][PAWN_INDEX(white, 1)];

    board->pieces[white][PAWN_INDEX(white, 2)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 2)]->cell = f2;
    board->map[f2] = board->pieces[white][PAWN_INDEX(white, 2)];

    board->pieces[white][PAWN_INDEX(white, 3)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 3)]->cell = g2;
    board->map[g2] = board->pieces[white][PAWN_INDEX(white, 3)];

    board->pieces[white][PAWN_INDEX(white, 4)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 4)]->cell = h2;
    board->map[h2] = board->pieces[white][PAWN_INDEX(white, 4)];


    board->pieces[black][KING_INDEX(black)]->alive = true;
    board->pieces[black][KING_INDEX(black)]->cell = d8;
    board->map[d8] = board->pieces[black][KING_INDEX(black)];
    board->pieces[black][KING_INDEX(black)]->no_moves = 1;

    board->pieces[black][QUEEN_INDEX(black)]->alive = true;
    board->pieces[black][QUEEN_INDEX(black)]->cell = d7;
    board->map[d7] = board->pieces[black][QUEEN_INDEX(black)];

    board->pieces[black][BISHOP_1(black)]->alive = true;
    board->pieces[black][BISHOP_1(black)]->cell = c8;
    board->map[c8] = board->pieces[black][BISHOP_1(black)];

    board->pieces[black][CASTLE_1(black)]->alive = true;
    board->pieces[black][CASTLE_1(black)]->cell = a8;
    board->map[a8] = board->pieces[black][CASTLE_1(black)];

    board->pieces[black][CASTLE_2(black)]->alive = true;
    board->pieces[black][CASTLE_2(black)]->cell = h8;
    board->map[h8] = board->pieces[black][CASTLE_2(black)];

    board->pieces[black][KNIGHT_1(black)]->alive = true;
    board->pieces[black][KNIGHT_1(black)]->cell = b3;
    board->map[b3] = board->pieces[black][KNIGHT_1(black)];

    board->pieces[black][KNIGHT_2(black)]->alive = true;
    board->pieces[black][KNIGHT_2(black)]->cell = f6;
    board->map[f6] = board->pieces[black][KNIGHT_2(black)];


    board->pieces[black][PAWN_INDEX(black, 1)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 1)]->cell = a7;
    board->map[a7] = board->pieces[black][PAWN_INDEX(black, 1)];

    board->pieces[black][PAWN_INDEX(black, 2)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 2)]->cell = b7;
    board->map[b7] = board->pieces[black][PAWN_INDEX(black, 2)];

    board->pieces[black][PAWN_INDEX(black, 3)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 3)]->cell = c7;
    board->map[c7] = board->pieces[black][PAWN_INDEX(black, 3)];

    board->pieces[black][PAWN_INDEX(black, 4)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 4)]->cell = d4;
    board->map[d4] = board->pieces[black][PAWN_INDEX(black, 4)];

    board->pieces[black][PAWN_INDEX(black, 6)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 6)]->cell = f7;
    board->map[f7] = board->pieces[black][PAWN_INDEX(black, 6)];

    board->pieces[black][PAWN_INDEX(black, 7)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 7)]->cell = g7;
    board->map[g7] = board->pieces[black][PAWN_INDEX(black, 7)];

    board->pieces[black][PAWN_INDEX(black, 5)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 5)]->cell = h7;
    board->map[h7] = board->pieces[black][PAWN_INDEX(black, 5)];

    set_board(board);
    return board;
    
}

Board* board_setup_1() {
    Board* board = init_board();
    clear_board(board);
    board->pieces[white][KING_INDEX(white)]->alive = true;
    board->pieces[white][KING_INDEX(white)]->cell = b1;
    board->map[b1] = board->pieces[white][KING_INDEX(white)];
    board->pieces[white][KING_INDEX(white)]->no_moves = 1;

    board->pieces[white][QUEEN_INDEX(white)]->alive = true;
    board->pieces[white][QUEEN_INDEX(white)]->cell = e2;
    board->map[e2] = board->pieces[white][QUEEN_INDEX(white)];

    board->pieces[white][CASTLE_1(white)]->alive = true;
    board->pieces[white][CASTLE_1(white)]->cell = g3;
    board->map[g3] = board->pieces[white][CASTLE_1(white)];

    board->pieces[white][BISHOP_1(white)]->alive = true;
    board->pieces[white][BISHOP_1(white)]->cell = e5;
    board->map[e5] = board->pieces[white][BISHOP_1(white)];

    board->pieces[white][PAWN_INDEX(white, 0)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 0)]->cell = f2;
    board->map[f2] = board->pieces[white][PAWN_INDEX(white, 0)];

    board->pieces[white][PAWN_INDEX(white, 1)]->alive = true;
    board->pieces[white][PAWN_INDEX(white, 1)]->cell = a4;
    board->map[a4] = board->pieces[white][PAWN_INDEX(white, 1)];


    board->pieces[black][KING_INDEX(black)]->alive = true;
    board->pieces[black][KING_INDEX(black)]->cell = a7;
    board->map[a7] = board->pieces[black][KING_INDEX(black)];
    board->pieces[black][KING_INDEX(black)]->no_moves = 1;

    board->pieces[black][QUEEN_INDEX(black)]->alive = true;
    board->pieces[black][QUEEN_INDEX(black)]->cell = a3;
    board->map[a3] = board->pieces[black][QUEEN_INDEX(black)];

    board->pieces[black][CASTLE_1(black)]->alive = true;
    board->pieces[black][CASTLE_1(black)]->cell = d7;
    board->map[d7] = board->pieces[black][CASTLE_1(black)];

    board->pieces[black][BISHOP_1(black)]->alive = true;
    board->pieces[black][BISHOP_1(black)]->cell = c5;
    board->map[c5] = board->pieces[black][BISHOP_1(black)];

    board->pieces[black][PAWN_INDEX(black, 1)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 1)]->cell = a6;
    board->map[a6] = board->pieces[black][PAWN_INDEX(black, 1)];

    board->pieces[black][PAWN_INDEX(black, 2)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 2)]->cell = b3;
    board->map[b3] = board->pieces[black][PAWN_INDEX(black, 2)];

    board->pieces[black][PAWN_INDEX(black, 3)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 3)]->cell = e4;
    board->map[e4] = board->pieces[black][PAWN_INDEX(black, 3)];

    board->pieces[black][PAWN_INDEX(black, 4)]->alive = true;
    board->pieces[black][PAWN_INDEX(black, 4)]->cell = f5;
    board->map[f5] = board->pieces[black][PAWN_INDEX(black, 4)];

    
    set_board(board);
    return board;
}


void test_moves_board_setup_1() {
    Board* board = board_setup_1();
    
    
    move_single_piece(board, board->pieces[black][CASTLE_1(black)], d1, none);
    U64 test_board = board->bitboard;

    Moves* moves = get_all_moves_for_colour(board, white);

    assert(moves->length == 1);
    assert(moves->moves[0]->destination == d1);
    assert(moves->moves[0]->piece->cell == e2);
    assert(moves->moves[0]->piece->type == queen);
    assert(board->bitboard == test_board);
}

void test_moves_board_setup_2() {
    Board* board = board_setup_1();
    move_single_piece(board, board->pieces[black][CASTLE_1(black)], d1, none);
    move_single_piece(board, board->pieces[white][QUEEN_INDEX(white)], d1, none);
    move_single_piece(board, board->pieces[black][QUEEN_INDEX(black)], a2, none);
    U64 test_board = board->bitboard;

    Moves* moves = get_all_moves_for_colour(board, white);

    assert(test_board == board->bitboard);
    assert(moves->length == 1);
    assert(moves->moves[0]->destination == c1);
    assert(moves->moves[0]->piece->cell == b1);
    assert(moves->moves[0]->piece->type == king);

    move_single_piece(board, board->pieces[white][KING_INDEX(white)], a2, none);
    assert(is_check(board, white));
}



void test_copy() {
    Board* board = set_board_notation("wke1, wpf4, bph8, bbb2 ");
    Board* copy = copy_board(board);
    assert(compare_boards(board, copy, "should_NOT_fail_test_copy"));

    copy->map[e1]->no_moves += 1;
    assert(!compare_boards(board, copy, "SHOULD_FAIL_test_copy"));
}


void test() {
    test_copy();
    test_moves();
    test_legality();
    test_move_selection();
    test_move_logic();
    test_hashing();
    test_move_logic_mate_in_four();
    test_mate_detection();
}