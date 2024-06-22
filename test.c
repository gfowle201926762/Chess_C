#include "chess.h"

/* ----------------------------- */
/* ---------- TESTING ---------- */
/* ----------------------------- */


void output_results() {

}


// void test_evaluation_branching(int breadth) {
//     test_forcing_moves(breadth);
//     test_forcing_moves_1(breadth);
//     test_forcing_moves_2(breadth);
//     test_forcing_moves_3(breadth);
//     test_puzzle_fork(breadth);
//     test_puzzle_win_queen(breadth);
//     test_17_june_2024(breadth);
//     test_16_june_2024_partial(breadth);
//     test_16_june_2024(breadth);
//     test_15_june_2024(breadth);
//     test_detect_mate_1(breadth);
//     test_detect_mate_2(breadth);
//     test_mate_in_four(breadth);
//     test_mate_in_four_2(breadth);
// }


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
    Board* board = set_board_notation("bkg7 bcc6 bpf6 bhg6 wqe3 wcg1 ");
    // print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(3, 7, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    assert(scores->moves->moves[0]->from == g1);
    assert(scores->moves->moves[0]->destination == g6);
    assert(scores->moves->moves[0]->piece->c == white);
    assert(scores->moves->moves[0]->piece->type == castle);
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
    // print_board_pro(board);
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
    // print_board_pro(board);
    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(8, 4, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    // print_scores(scores);

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
    // print_board_pro(board);

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
    // print_board_pro(board);

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
    // Difficult for heuristic function to see this first move... (knight h7 to f8)
    Board* board = set_board_notation("wkc1 wpb2 wpc2 wpd4 wpf2 wpg5 wbd2 wbd3 whh7 wqh3 wch1 bpa3 bpa5 bpc6 bpe6 bpf7 bpg6 bhd5 bhd7 bcb8 bce8 bqd8 bkg8 bbe7 ");
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(5, 3, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    print_scores(scores);
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
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(200, 2, white);
    Scores* scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
    assert(test_board == board->bitboard);

    print_scores(scores);

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




void test_hashing_1() {
    Board* board = set_board_notation("wpe4 bcf5 ");
    Moves* moves = get_all_moves_for_colour(board, white);

    U64 hash = board->hash_value;
    hash_move_piece(board, moves->moves[0], NULL);
    hash_move_piece(board, moves->moves[0], NULL);
    assert(board->hash_value == hash);

    Piece* killed = pretend_move(board, moves->moves[1]->piece, moves->moves[1]->destination, moves->moves[1]->promotion);
    hash = board->hash_value;
    hash_move_piece(board, moves->moves[1], killed);
    hash_move_piece(board, moves->moves[1], killed);
    assert(board->hash_value == hash);
}

void test_hashing_2() {
    Board* board = set_board_notation("wkd4 bcb7 bcc8 ");
    Moves* moves = get_all_moves_for_colour(board, black);
    U64 hash = board->hash_value;

    Piece* killed = pretend_move(board, moves->moves[2]->piece, moves->moves[2]->destination, moves->moves[2]->promotion);
    hash_move_piece(board, moves->moves[2], killed);

    hash_move_piece(board, moves->moves[2], killed);
    undo_pretend_move(board, moves->moves[2]->piece, killed, moves->moves[2]->from, moves->moves[2]->promotion);
    assert(hash == board->hash_value);
}

void test_hashing_graph_simulation() {
    Board* board = set_board_notation("wkd4 bcb7 bcc8 ");
    Moves* moves = get_all_moves_for_colour(board, black);
    U64 hash1 = board->hash_value;

    Piece* killed = pretend_move(board, moves->moves[2]->piece, moves->moves[2]->destination, moves->moves[2]->promotion);
    hash_move_piece(board, moves->moves[2], killed);
    U64 hash2 = board->hash_value;


    Moves* response = get_all_moves_for_colour(board, black);

    Piece* killed_response = pretend_move(board, response->moves[0]->piece, response->moves[0]->destination, response->moves[0]->promotion);
    hash_move_piece(board, response->moves[0], killed_response);

    hash_move_piece(board, response->moves[0], killed_response);
    undo_pretend_move(board, response->moves[0]->piece, killed_response, response->moves[0]->from, response->moves[0]->promotion);
    assert(hash2 == board->hash_value);

    hash_move_piece(board, moves->moves[2], killed);
    undo_pretend_move(board, moves->moves[2]->piece, killed, moves->moves[2]->from, moves->moves[2]->promotion);
    assert(hash1 == board->hash_value);
}

void test_hashing_graph_execution() {
    Board* board = set_board_notation("wpe7 bhd8 ");
    print_board_pro(board);

    Grapher* grapher = init_grapher(200, 2, black);
    Scores* scores = create_graph(grapher, grapher->start, board, black, init_limit(true));


}

void test_hashing() {
    test_hashing_1();
    test_hashing_2();
    test_hashing_graph_simulation();
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
    board->last_moved[0] = board->pieces[white][PAWN_INDEX(white, 2)];
    board->lm_length += 1;
    
    U64 pawn_attack_mask = get_pawn_attack_mask(board, piece);
    U64 test = 0ULL;
    set_bit(test, c4);
    assert(pawn_attack_mask == test);
}

void test_pawn_en_passant_2() {
    
    Board* board = init_board();

    pretend_move(board, board->pieces[white][PAWN_INDEX(white, 1)], b4, none);
    pretend_move(board, board->pieces[black][PAWN_INDEX(black, 0)], a5, none);
    pretend_move(board, board->pieces[white][PAWN_INDEX(white, 1)], b5, none);
    pretend_move(board, board->pieces[black][PAWN_INDEX(black, 2)], c5, none);
    
    U64 pawn_attack_mask = get_pawn_attack_mask(board, board->pieces[white][PAWN_INDEX(white, 1)]);
    U64 test = 0ULL;
    set_bit(test, c5);
    assert(pawn_attack_mask == test);

    pretend_move(board, board->pieces[white][PAWN_INDEX(white, 7)], h4, none);
    pretend_move(board, board->pieces[black][PAWN_INDEX(black, 6)], g5, none);
    pawn_attack_mask = get_pawn_attack_mask(board, board->pieces[white][PAWN_INDEX(white, 1)]);
    assert(pawn_attack_mask == 0ULL);
}

void test_castling_1() {
    Board* board = init_board();
    board->pieces[white][KING_INDEX(white)]->no_moves = 0;

    execute_move(board, board->pieces[white][PAWN_INDEX(white, 4)], e4, none);
    execute_move(board, board->pieces[black][PAWN_INDEX(black, 4)], e5, none);
    execute_move(board, board->pieces[white][BISHOP_2(white)], c4, none);
    execute_move(board, board->pieces[black][BISHOP_2(black)], c5, none);
    execute_move(board, board->pieces[white][KNIGHT_2(white)], f3, none);
    execute_move(board, board->pieces[black][KNIGHT_2(black)], f6, none);

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

    board->pieces[white][CASTLE_1(white)]->alive = true;
    board->pieces[white][CASTLE_1(white)]->cell = a1;
    board->map[a1] = board->pieces[white][CASTLE_1(white)];
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



    board->pieces[black][CASTLE_1(black)]->alive = true;
    board->pieces[black][CASTLE_1(black)]->cell = c8;
    board->map[c8] = board->pieces[black][CASTLE_1(black)];
    set_bit(board->bitboard, c8);

    test_board = board->bitboard;
    moves = get_all_moves_for_colour(board, white);
    assert(test_board == board->bitboard);
    for (int i = 0; i < moves->length; i++) {
        if (moves->moves[i]->piece == board->pieces[white][KING_INDEX(white)]) {
            assert(moves->moves[i]->destination != c1);
        }
    }

    board->pieces[black][CASTLE_1(black)]->cell = d8;
    board->map[d8] = board->pieces[black][CASTLE_1(black)];
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


    board->pieces[black][CASTLE_1(black)]->cell = h1;
    board->map[h1] = board->pieces[black][CASTLE_1(black)];
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

    board->pieces[black][CASTLE_1(black)]->alive = false;
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

    Piece* killed = pretend_move(board, moves->moves[0]->piece, moves->moves[0]->destination, moves->moves[0]->promotion);
    hash_move_piece(board, moves->moves[0], killed);

    assert(moves->moves[0]->piece->type == queen);
    assert(moves->moves[0]->piece->value == QUEEN_VALUE);
    assert(moves->moves[0]->piece->move_func == get_queen_mask);
    assert(moves->moves[0]->piece->cell == b8);
    assert(board->map[b8] == moves->moves[0]->piece);
    assert(board->map[b7] == NULL);

    hash_move_piece(board, moves->moves[0], killed);
    undo_pretend_move(board, moves->moves[0]->piece, killed, moves->moves[0]->from, moves->moves[0]->promotion);
    
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

    killed = pretend_move(board, moves->moves[1]->piece, moves->moves[1]->destination, moves->moves[1]->promotion);
    hash_move_piece(board, moves->moves[1], killed);

    assert(moves->moves[1]->piece->type == castle);
    assert(moves->moves[1]->piece->value == CASTLE_VALUE);
    assert(moves->moves[1]->piece->move_func == get_castle_mask);
    assert(moves->moves[1]->piece->cell == b8);
    assert(board->map[b8] == moves->moves[1]->piece);
    assert(board->map[b7] == NULL);

    hash_move_piece(board, moves->moves[1], killed);
    undo_pretend_move(board, moves->moves[1]->piece, killed, moves->moves[1]->from, moves->moves[1]->promotion);
    
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

    killed = pretend_move(board, moves->moves[2]->piece, moves->moves[2]->destination, moves->moves[2]->promotion);
    hash_move_piece(board, moves->moves[2], killed);

    assert(moves->moves[2]->piece->type == bishop);
    assert(moves->moves[2]->piece->value == BISHOP_VALUE);
    assert(moves->moves[2]->piece->move_func == get_bishop_mask);
    assert(moves->moves[2]->piece->cell == b8);
    assert(board->map[b8] == moves->moves[2]->piece);
    assert(board->map[b7] == NULL);

    hash_move_piece(board, moves->moves[2], killed);
    undo_pretend_move(board, moves->moves[2]->piece, killed, moves->moves[2]->from, moves->moves[2]->promotion);
    
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

    killed = pretend_move(board, moves->moves[3]->piece, moves->moves[3]->destination, moves->moves[3]->promotion);
    hash_move_piece(board, moves->moves[3], killed);

    assert(moves->moves[3]->piece->type == knight);
    assert(moves->moves[3]->piece->value == KNIGHT_VALUE);
    assert(moves->moves[3]->piece->move_func == get_knight_mask);
    assert(moves->moves[3]->piece->cell == b8);
    assert(board->map[b8] == moves->moves[3]->piece);
    assert(board->map[b7] == NULL);

    hash_move_piece(board, moves->moves[3], killed);
    undo_pretend_move(board, moves->moves[3]->piece, killed, moves->moves[3]->from, moves->moves[3]->promotion);
    
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

    killed = pretend_move(board, moves->moves[4]->piece, moves->moves[4]->destination, moves->moves[4]->promotion);
    hash_move_piece(board, moves->moves[4], killed);

    assert(moves->moves[4]->piece->type == queen);
    assert(moves->moves[4]->piece->value == QUEEN_VALUE);
    assert(moves->moves[4]->piece->move_func == get_queen_mask);
    assert(moves->moves[4]->piece->cell == c8);
    assert(board->map[c8] == moves->moves[4]->piece);
    assert(board->map[b7] == NULL);
    assert(board->map[b8] == NULL);

    hash_move_piece(board, moves->moves[4], killed);
    undo_pretend_move(board, moves->moves[4]->piece, killed, moves->moves[4]->from, moves->moves[4]->promotion);

    assert(hash == board->hash_value);
    assert(moves->moves[4]->piece->type == pawn);
    assert(moves->moves[4]->piece->value == PAWN_VALUE);
    assert(moves->moves[4]->piece->move_func == get_pawn_mask);
    assert(moves->moves[4]->piece->cell == b7);
    assert(board->map[b7] == moves->moves[4]->piece);
    assert(board->map[b8] == NULL);
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

    killed = pretend_move(board, moves->moves[5]->piece, moves->moves[5]->destination, moves->moves[5]->promotion);
    hash_move_piece(board, moves->moves[5], killed);

    assert(moves->moves[1]->piece->type == castle);
    assert(moves->moves[1]->piece->value == CASTLE_VALUE);
    assert(moves->moves[1]->piece->move_func == get_castle_mask);
    assert(moves->moves[1]->piece->cell == c8);
    assert(board->map[c8] == moves->moves[5]->piece);
    assert(board->map[b7] == NULL);
    assert(board->map[b8] == NULL);

    hash_move_piece(board, moves->moves[5], killed);
    undo_pretend_move(board, moves->moves[5]->piece, killed, moves->moves[5]->from, moves->moves[5]->promotion);
    
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

    killed = pretend_move(board, moves->moves[6]->piece, moves->moves[6]->destination, moves->moves[6]->promotion);
    hash_move_piece(board, moves->moves[6], killed);

    assert(moves->moves[6]->piece->type == bishop);
    assert(moves->moves[6]->piece->value == BISHOP_VALUE);
    assert(moves->moves[6]->piece->move_func == get_bishop_mask);
    assert(moves->moves[6]->piece->cell == c8);
    assert(board->map[c8] == moves->moves[6]->piece);
    assert(board->map[b7] == NULL);
    assert(board->map[b8] == NULL);

    hash_move_piece(board, moves->moves[6], killed);
    undo_pretend_move(board, moves->moves[6]->piece, killed, moves->moves[6]->from, moves->moves[6]->promotion);
    
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

    killed = pretend_move(board, moves->moves[7]->piece, moves->moves[7]->destination, moves->moves[7]->promotion);
    hash_move_piece(board, moves->moves[7], killed);

    assert(moves->moves[7]->piece->type == knight);
    assert(moves->moves[7]->piece->value == KNIGHT_VALUE);
    assert(moves->moves[7]->piece->move_func == get_knight_mask);
    assert(moves->moves[7]->piece->cell == c8);
    assert(board->map[c8] == moves->moves[7]->piece);
    assert(board->map[b7] == NULL);
    assert(board->map[b8] == NULL);

    hash_move_piece(board, moves->moves[7], killed);
    undo_pretend_move(board, moves->moves[7]->piece, killed, moves->moves[7]->from, moves->moves[7]->promotion);
    
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

void test_moves() {
    test_pawn_attacks();
    test_pawn_moves();
    test_knight_moves();
    test_king_moves();
    test_castle_moves();
    test_bishop_moves();
    test_queen_moves();
    test_pawn_en_passant_1();
    test_pawn_en_passant_2();
    test_castling_1();
    test_castling_2();
    test_promotion_1();
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
    square original_from = board->pieces[black][0]->cell;
    Piece* killed = pretend_move(board, board->pieces[black][0], a8, none);
    pop_bit(test_board, e8);
    set_bit(test_board, a8);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][0]->cell == a8);
    assert(board->map[a8] == board->pieces[black][0]);
    assert(board->map[e8] == NULL);

    // SIMULATE SECOND MOVE
    square from_2 = board->pieces[white][KING_INDEX(white)]->cell;
    Piece* killed_2 = pretend_move(board, board->pieces[white][KING_INDEX(white)], b2, none);
    pop_bit(test_board, b1);
    set_bit(test_board, b2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[b1] == NULL);

    // UNDO SECOND MOVE
    undo_pretend_move(board, board->pieces[white][KING_INDEX(white)], killed_2, from_2, none);
    pop_bit(test_board, b2);
    set_bit(test_board, b1);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == b1);
    assert(board->map[b1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[b2] == NULL);

    // REVERSE FIRST MOVE
    undo_pretend_move(board, board->pieces[black][0], killed, original_from, none);
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
    square original_from = board->pieces[black][CASTLE_1(black)]->cell;
    Piece* killed = pretend_move(board, board->pieces[black][CASTLE_1(black)], d1, none);
    pop_bit(test_board, d7);
    set_bit(test_board, d1);
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d1);
    assert(board->map[d1] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d7] == NULL);

    undo_pretend_move(board, board->pieces[black][CASTLE_1(black)], killed, original_from, none);
    assert(board->bitboard == saved_board);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d7);
    assert(board->map[d7] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d1] == NULL);

    killed = pretend_move(board, board->pieces[black][CASTLE_1(black)], d1, none);
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d1);
    assert(board->map[d1] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d7] == NULL);
    saved_board = board->bitboard;

    // SIMULATE FIRST MOVE (WHITE)
    square opponent_from = board->pieces[white][QUEEN_INDEX(white)]->cell;
    Piece* opponent_killed = pretend_move(board, board->pieces[white][QUEEN_INDEX(white)], d1, none);
    pop_bit(test_board, e2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][QUEEN_INDEX(white)]->cell == d1);
    assert(board->map[d1] == board->pieces[white][QUEEN_INDEX(white)]);
    assert(opponent_killed == board->pieces[black][CASTLE_1(black)]);
    assert(!board->pieces[black][CASTLE_1(black)]->alive);
    assert(board->map[e2] == NULL);

    undo_pretend_move(board, board->pieces[white][QUEEN_INDEX(white)], opponent_killed, opponent_from, none);
    assert(saved_board == board->bitboard);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d1);
    assert(board->pieces[black][CASTLE_1(black)]->alive);
    assert(board->map[d1] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d7] == NULL);
    assert(board->pieces[white][QUEEN_INDEX(white)]->cell == e2);
    assert(board->map[e2] == board->pieces[white][QUEEN_INDEX(white)]);
    assert(board->pieces[white][QUEEN_INDEX(white)]->alive);

    opponent_killed = pretend_move(board, board->pieces[white][QUEEN_INDEX(white)], d1, none);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][QUEEN_INDEX(white)]->cell == d1);
    assert(board->map[d1] == board->pieces[white][QUEEN_INDEX(white)]);
    assert(opponent_killed == board->pieces[black][CASTLE_1(black)]);
    assert(!board->pieces[black][CASTLE_1(black)]->alive);
    assert(board->map[e2] == NULL);


    // SIMULATE SECOND MOVE (BLACK)
    original_from = board->pieces[black][QUEEN_INDEX(black)]->cell;
    killed = pretend_move(board, board->pieces[black][QUEEN_INDEX(black)], a2, none);
    pop_bit(test_board, a3);
    set_bit(test_board, a2);
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][QUEEN_INDEX(black)]->cell == a2);
    assert(board->map[a2] == board->pieces[black][QUEEN_INDEX(black)]);
    assert(board->map[a3] == NULL);

    // SIMULATE SECOND MOVE (WHITE)
    opponent_from = board->pieces[white][KING_INDEX(white)]->cell;
    opponent_killed = pretend_move(board, board->pieces[white][KING_INDEX(white)], c1, none);
    pop_bit(test_board, b1);
    set_bit(test_board, c1);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == c1);
    assert(board->map[c1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[b1] == NULL);
    assert(opponent_killed == NULL);

    // SIMULATE THIRD MOVE (BLACK)
    original_from = board->pieces[black][BISHOP_1(black)]->cell;
    killed = pretend_move(board, board->pieces[black][BISHOP_1(black)], a3, none);
    pop_bit(test_board, c5);
    set_bit(test_board, a3);
    saved_board = test_board;
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][BISHOP_1(black)]->cell == a3);
    assert(board->map[a3] == board->pieces[black][BISHOP_1(black)]);
    assert(board->map[c5] == NULL);

    // SIMULATE THIRD MOVE (WHITE)
    opponent_from = board->pieces[white][BISHOP_1(white)]->cell;
    opponent_killed = pretend_move(board, board->pieces[white][BISHOP_1(white)], b2, none);
    pop_bit(test_board, e5);
    set_bit(test_board, b2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][BISHOP_1(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][BISHOP_1(white)]);
    assert(board->map[e5] == NULL);
    assert(opponent_killed == NULL);

    undo_pretend_move(board, board->pieces[white][BISHOP_1(white)], opponent_killed, opponent_from, none);
    assert(saved_board == board->bitboard);
    assert(board->pieces[black][BISHOP_1(black)]->cell == a3);
    assert(board->map[a3] == board->pieces[black][BISHOP_1(black)]);
    assert(board->map[c5] == NULL);

    opponent_killed = pretend_move(board, board->pieces[white][BISHOP_1(white)], b2, none);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][BISHOP_1(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][BISHOP_1(white)]);
    assert(board->map[e5] == NULL);
    assert(opponent_killed == NULL);

    // SIMULATE FOURTH MOVE (BLACK)
    original_from = board->pieces[black][BISHOP_1(black)]->cell;
    killed = pretend_move(board, board->pieces[black][BISHOP_1(black)], b2, none);
    pop_bit(test_board, a3);
    set_bit(test_board, b2);
    assert(killed==board->pieces[white][BISHOP_1(white)]);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][BISHOP_1(black)]->cell == b2);
    assert(board->map[b2] == board->pieces[black][BISHOP_1(black)]);
    assert(board->map[a3] == NULL);


    // SIMULATE FOURTH MOVE (WHITE)
    opponent_from = board->pieces[white][KING_INDEX(white)]->cell;
    opponent_killed = pretend_move(board, board->pieces[white][KING_INDEX(white)], d2, none);
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

    assert(scores->moves->moves[4]->from == c4);
    assert(scores->moves->moves[4]->destination == b5);
    assert(scores->moves->moves[4]->piece->c == white);
    assert(scores->moves->moves[4]->piece->type == bishop);

    assert(scores->moves->moves[5]->from == c7);
    assert(scores->moves->moves[5]->destination == c6);
    assert(scores->moves->moves[5]->piece->c == black);
    assert(scores->moves->moves[5]->piece->type == pawn);

    assert(scores->moves->moves[6]->from == e1);
    assert(scores->moves->moves[6]->destination == e7);
    assert(scores->moves->moves[6]->piece->c == white);
    assert(scores->moves->moves[6]->piece->type == castle);
}

void test_mate_detection() {
    test_detect_mate_1();
    test_detect_mate_2();
    test_mate_in_four();
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
    
    
    execute_move(board, board->pieces[black][CASTLE_1(black)], d1, none);
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
    execute_move(board, board->pieces[black][CASTLE_1(black)], d1, none);
    execute_move(board, board->pieces[white][QUEEN_INDEX(white)], d1, none);
    execute_move(board, board->pieces[black][QUEEN_INDEX(black)], a2, none);
    U64 test_board = board->bitboard;

    Moves* moves = get_all_moves_for_colour(board, white);
    assert(test_board == board->bitboard);
    assert(moves->length == 1);
    assert(moves->moves[0]->destination == c1);
    assert(moves->moves[0]->piece->cell == b1);
    assert(moves->moves[0]->piece->type == king);

    execute_move(board, board->pieces[white][KING_INDEX(white)], a2, none);
    assert(is_check(board, white));
}



void test() {
    test_moves();
    test_legality();
    test_move_selection();
    test_move_logic();
    test_hashing();
    test_move_logic_mate_in_four();
    test_mate_detection();
}