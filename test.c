#include "chess.h"

/* ----------------------------- */
/* ---------- TESTING ---------- */
/* ----------------------------- */



void eval_function_test() {

}








void test_take_train() {
    Board* board = set_board_notation("wpe4 wbf3 wqg2 bpd5 bbc6 bqb7 ");
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(6, 1, black);

    int score = create_graph(grapher, grapher->start, board, black);
    assert(test_board == board->bitboard);
    printf("score: %i\n", score);

    Tracer* tracer = init_tracer(black);
    dfs(grapher->start, tracer);
    print_tracer(tracer);
}

void test_forcing_moves_1() {
    Board* board = set_board_notation("bkg6 bcc6 wqe3 ");

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(3, 3, white);
    int score = create_graph(grapher, grapher->start, board, white);
    assert(test_board == board->bitboard);
    Moves* tracer = calloc(1, sizeof(Moves));
    bfs(grapher->start, tracer);

    assert(score == 0);
    assert(tracer->moves[0]->from == e3);
    assert((tracer->moves[0]->destination == e8) || ((tracer->moves[0]->destination == e4)));
    assert(tracer->moves[0]->piece->c == white);
    assert(tracer->moves[0]->piece->type == queen);

    assert((tracer->moves[2]->from == e8) || ((tracer->moves[2]->from == e4)));
    assert(tracer->moves[2]->destination == c6);
    assert(tracer->moves[2]->piece->c == white);
    assert(tracer->moves[2]->piece->type == queen);
}

void test_dont_be_stupid() {
    Board* board = set_board_notation("bkg6 bcd6 wqe6 wka1 ");

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(3, 3, black);
    int score = create_graph(grapher, grapher->start, board, black);
    assert(test_board == board->bitboard);
    Moves* tracer = calloc(1, sizeof(Moves));
    bfs(grapher->start, tracer);

    assert(score == 0);
    assert(tracer->moves[0]->from == d6);
    assert(tracer->moves[0]->destination == e6);
    assert(tracer->moves[0]->piece->c == black);
    assert(tracer->moves[0]->piece->type == castle);
}

void test_forcing_moves_2() {
    Board* board = set_board_notation("bkg6 bcd6 wqe3 ");
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(5, 5, white);
    int score = create_graph(grapher, grapher->start, board, white);
    assert(test_board == board->bitboard);
    Moves* tracer = calloc(1, sizeof(Moves));
    bfs(grapher->start, tracer);
    print_moves(tracer);

    assert(score == 0);
    assert(tracer->moves[0]->from == e3);
    assert(tracer->moves[0]->destination == g3);
    assert(tracer->moves[0]->piece->c == white);
    assert(tracer->moves[0]->piece->type == queen);
}

void test_forcing_moves_3() {
    Board* board = set_board_notation("bkg7 bcc6 bpf6 bhg6 wqe3 wcg1 ");
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(2, 7, white);
    int score = create_graph(grapher, grapher->start, board, white);
    assert(test_board == board->bitboard);
    Tracer* tracer = init_tracer(white);
    dfs(grapher->start, tracer);
    print_tracer(tracer);

    assert(score == 0);
    // assert(tracer->best_eval == 5);
    // assert(tracer->best->length == 3);
    // assert(tracer->best->moves[0]->from == e3);
    // assert(tracer->best->moves[0]->destination == g3);
    // assert(tracer->best->moves[0]->piece->c == white);
    // assert(tracer->best->moves[0]->piece->type == queen);

}

void test_puzzle_fork() {
    // bpf6
    Board* board = set_board_notation("wkb1 wqe3 wch2 whc3 wpb2 wpb4 wpd5 wpf4 wpg3 bkg6 bqd7 bcc4 bbh5 bpa7 bpf6 bpb6 bpd6 bpf5 bph7 ");
    print_board_pro(board);
    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(2, 3, white);
    

    int score = create_graph(grapher, grapher->start, board, white);
    assert(test_board == board->bitboard);
    printf("score: %i\n", score);
    printf("leaves: %i\n", board->leaves);

    Tracer* tracer = init_tracer(white);
    dfs(grapher->start, tracer);
    print_tracer(tracer);

    assert(score == 0);
    assert(tracer->best_eval == 3);
    assert(tracer->best->moves[0]->from == h2);
    assert(tracer->best->moves[0]->destination == h5);
    assert(tracer->best->moves[0]->evaluation == 3);
    assert(tracer->best->moves[0]->piece->type == castle);

}

void test_puzzle_win_queen() {
    Board* board = set_board_notation("wkg1 wqg3 wba4 wbc1 whb1 whd5 wph2 wpg2 wpd2 wpc3 wpb2 wpa2 wca1 bkd8 bqh4 bcf8 bca8 bhc6 bhg4 bph7 bpg7 bpe4 bpd6 bpc7 bpb7 bpa6 ");
    print_board_pro(board);
    // print_board(board->bitboard);
    // Moves* moves = calloc(1, sizeof(Moves));
    // get_all_moves_for_piece(board, board->pieces[black][CASTLE_1(black)], moves);
    // print_moves(moves);
    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(6, 4, black);

    int score = create_graph(grapher, grapher->start, board, black);
    assert(test_board == board->bitboard);
    printf("score: %i\n", score);

    Tracer* tracer = init_tracer(black);
    dfs(grapher->start, tracer);
    print_tracer(tracer);
}

void test_board_notation() {
    Board* board = set_board_notation("wkh2 wqc1 wcf1 wce1 wbb1 wba5 whf4 wph3 wpg2 wpb3 wpa4 bkh7 bqh4 bcf8 bce8 bbg7 bbh5 bhh6 bhe5 bpg6 bpc6 bpb7 ");
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(3, 8, black);

    int score = create_graph(grapher, grapher->start, board, black);
    assert(test_board == board->bitboard);
    // assert(score == 97);
    printf("score: %i\n", score);

    Tracer* tracer = init_tracer(black);
    dfs(grapher->start, tracer);
    print_tracer(tracer);

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
    board->last_moved = board->pieces[white][PAWN_INDEX(white, 2)];
    
    U64 pawn_attack_mask = get_pawn_attack_mask(board, piece);
    U64 test = 0ULL;
    set_bit(test, c4);
    assert(pawn_attack_mask == test);
}

void test_pawn_en_passant_2() {
    
    Board* board = init_board();

    execute_move(board, board->pieces[white][PAWN_INDEX(white, 1)], b4);
    execute_move(board, board->pieces[black][PAWN_INDEX(black, 0)], a5);
    execute_move(board, board->pieces[white][PAWN_INDEX(white, 1)], b5);
    execute_move(board, board->pieces[black][PAWN_INDEX(black, 2)], c5);
    
    U64 pawn_attack_mask = get_pawn_attack_mask(board, board->pieces[white][PAWN_INDEX(white, 1)]);
    U64 test = 0ULL;
    set_bit(test, c5);
    assert(pawn_attack_mask == test);

    execute_move(board, board->pieces[white][PAWN_INDEX(white, 7)], h4);
    execute_move(board, board->pieces[black][PAWN_INDEX(black, 6)], g5);
    pawn_attack_mask = get_pawn_attack_mask(board, board->pieces[white][PAWN_INDEX(white, 1)]);
    assert(pawn_attack_mask == 0ULL);
}

void test_castling_1() {
    Board* board = init_board();
    board->pieces[white][KING_INDEX(white)]->no_moves = 0;

    execute_move(board, board->pieces[white][PAWN_INDEX(white, 4)], e4);
    execute_move(board, board->pieces[black][PAWN_INDEX(black, 4)], e5);
    execute_move(board, board->pieces[white][BISHOP_2(white)], c4);
    execute_move(board, board->pieces[black][BISHOP_2(black)], c5);
    execute_move(board, board->pieces[white][KNIGHT_2(white)], f3);
    execute_move(board, board->pieces[black][KNIGHT_2(black)], f6);

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
    Piece* killed = pretend_move(board, board->pieces[black][0], a8);
    pop_bit(test_board, e8);
    set_bit(test_board, a8);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][0]->cell == a8);
    assert(board->map[a8] == board->pieces[black][0]);
    assert(board->map[e8] == NULL);

    // SIMULATE SECOND MOVE
    square from_2 = board->pieces[white][KING_INDEX(white)]->cell;
    Piece* killed_2 = pretend_move(board, board->pieces[white][KING_INDEX(white)], b2);
    pop_bit(test_board, b1);
    set_bit(test_board, b2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[b1] == NULL);

    // UNDO SECOND MOVE
    undo_pretend_move(board, board->pieces[white][KING_INDEX(white)], killed_2, from_2);
    pop_bit(test_board, b2);
    set_bit(test_board, b1);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == b1);
    assert(board->map[b1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[b2] == NULL);

    // REVERSE FIRST MOVE
    undo_pretend_move(board, board->pieces[black][0], killed, original_from);
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
    Piece* killed = pretend_move(board, board->pieces[black][CASTLE_1(black)], d1);
    pop_bit(test_board, d7);
    set_bit(test_board, d1);
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d1);
    assert(board->map[d1] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d7] == NULL);

    undo_pretend_move(board, board->pieces[black][CASTLE_1(black)], killed, original_from);
    assert(board->bitboard == saved_board);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d7);
    assert(board->map[d7] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d1] == NULL);

    killed = pretend_move(board, board->pieces[black][CASTLE_1(black)], d1);
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d1);
    assert(board->map[d1] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d7] == NULL);
    saved_board = board->bitboard;

    // SIMULATE FIRST MOVE (WHITE)
    square opponent_from = board->pieces[white][QUEEN_INDEX(white)]->cell;
    Piece* opponent_killed = pretend_move(board, board->pieces[white][QUEEN_INDEX(white)], d1);
    pop_bit(test_board, e2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][QUEEN_INDEX(white)]->cell == d1);
    assert(board->map[d1] == board->pieces[white][QUEEN_INDEX(white)]);
    assert(opponent_killed == board->pieces[black][CASTLE_1(black)]);
    assert(!board->pieces[black][CASTLE_1(black)]->alive);
    assert(board->map[e2] == NULL);

    undo_pretend_move(board, board->pieces[white][QUEEN_INDEX(white)], opponent_killed, opponent_from);
    assert(saved_board == board->bitboard);
    assert(board->pieces[black][CASTLE_1(black)]->cell == d1);
    assert(board->pieces[black][CASTLE_1(black)]->alive);
    assert(board->map[d1] == board->pieces[black][CASTLE_1(black)]);
    assert(board->map[d7] == NULL);
    assert(board->pieces[white][QUEEN_INDEX(white)]->cell == e2);
    assert(board->map[e2] == board->pieces[white][QUEEN_INDEX(white)]);
    assert(board->pieces[white][QUEEN_INDEX(white)]->alive);

    opponent_killed = pretend_move(board, board->pieces[white][QUEEN_INDEX(white)], d1);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][QUEEN_INDEX(white)]->cell == d1);
    assert(board->map[d1] == board->pieces[white][QUEEN_INDEX(white)]);
    assert(opponent_killed == board->pieces[black][CASTLE_1(black)]);
    assert(!board->pieces[black][CASTLE_1(black)]->alive);
    assert(board->map[e2] == NULL);


    // SIMULATE SECOND MOVE (BLACK)
    original_from = board->pieces[black][QUEEN_INDEX(black)]->cell;
    killed = pretend_move(board, board->pieces[black][QUEEN_INDEX(black)], a2);
    pop_bit(test_board, a3);
    set_bit(test_board, a2);
    assert(killed==NULL);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][QUEEN_INDEX(black)]->cell == a2);
    assert(board->map[a2] == board->pieces[black][QUEEN_INDEX(black)]);
    assert(board->map[a3] == NULL);

    // SIMULATE SECOND MOVE (WHITE)
    opponent_from = board->pieces[white][KING_INDEX(white)]->cell;
    opponent_killed = pretend_move(board, board->pieces[white][KING_INDEX(white)], c1);
    pop_bit(test_board, b1);
    set_bit(test_board, c1);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][KING_INDEX(white)]->cell == c1);
    assert(board->map[c1] == board->pieces[white][KING_INDEX(white)]);
    assert(board->map[b1] == NULL);
    assert(opponent_killed == NULL);

    // SIMULATE THIRD MOVE (BLACK)
    original_from = board->pieces[black][BISHOP_1(black)]->cell;
    killed = pretend_move(board, board->pieces[black][BISHOP_1(black)], a3);
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
    opponent_killed = pretend_move(board, board->pieces[white][BISHOP_1(white)], b2);
    pop_bit(test_board, e5);
    set_bit(test_board, b2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][BISHOP_1(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][BISHOP_1(white)]);
    assert(board->map[e5] == NULL);
    assert(opponent_killed == NULL);

    undo_pretend_move(board, board->pieces[white][BISHOP_1(white)], opponent_killed, opponent_from);
    assert(saved_board == board->bitboard);
    assert(board->pieces[black][BISHOP_1(black)]->cell == a3);
    assert(board->map[a3] == board->pieces[black][BISHOP_1(black)]);
    assert(board->map[c5] == NULL);

    opponent_killed = pretend_move(board, board->pieces[white][BISHOP_1(white)], b2);
    assert(test_board == board->bitboard);
    assert(board->pieces[white][BISHOP_1(white)]->cell == b2);
    assert(board->map[b2] == board->pieces[white][BISHOP_1(white)]);
    assert(board->map[e5] == NULL);
    assert(opponent_killed == NULL);

    // SIMULATE FOURTH MOVE (BLACK)
    original_from = board->pieces[black][BISHOP_1(black)]->cell;
    killed = pretend_move(board, board->pieces[black][BISHOP_1(black)], b2);
    pop_bit(test_board, a3);
    set_bit(test_board, b2);
    assert(killed==board->pieces[white][BISHOP_1(white)]);
    assert(test_board == board->bitboard);
    assert(board->pieces[black][BISHOP_1(black)]->cell == b2);
    assert(board->map[b2] == board->pieces[black][BISHOP_1(black)]);
    assert(board->map[a3] == NULL);


    // SIMULATE FOURTH MOVE (WHITE)
    opponent_from = board->pieces[white][KING_INDEX(white)]->cell;
    opponent_killed = pretend_move(board, board->pieces[white][KING_INDEX(white)], d2);
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
    Grapher* grapher = init_grapher(1, 2, black);
    int score = create_graph(grapher, grapher->start, board, black);
    assert(test_board == board->bitboard);
    // printf("score: %i\n", score);
    Tracer* tracer = init_tracer(black);
    dfs(grapher->start, tracer);
    // print_tracer(tracer);
    assert(score == 99);
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

    int score;

    Grapher* grapher = init_grapher(1, 2, black);
    score = create_graph(grapher, grapher->start, board, black);
    assert(score == 0);
    assert(test_board == board->bitboard);

    board->pieces[white][KING_INDEX(white)]->cell = a1;
    board->map[a1] = board->pieces[white][KING_INDEX(white)];
    pop_bit(board->bitboard, c1);
    pop_bit(test_board, c1);
    set_bit(board->bitboard, a1);
    set_bit(test_board, a1);

    Grapher* grapher2 = init_grapher(1, 2, black);
    score = create_graph(grapher2, grapher2->start, board, black);
    assert(score == 100);
    assert(test_board == board->bitboard);

    
}

void test_mate_in_four() {
    Board* board = board_setup_1();

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(2, 4, black); // should work with 2, 4

    // print_board_pro(board);
    int score = create_graph(grapher, grapher->start, board, black);
    printf("positions evaluated: %i\n", board->counter);
    assert(test_board == board->bitboard);
    assert(score == 97);
    // printf("score: %i\n", score);

    Tracer* tracer = init_tracer(black);
    dfs(grapher->start, tracer);
    // print_tracer(tracer);
    assert(tracer->best_eval == 95);
    assert(tracer->best->length == 7);
    assert(tracer->best->moves[0]->from == d7);
    assert(tracer->best->moves[0]->destination == d1);
    assert(tracer->best->moves[0]->piece->c == black);
    assert(tracer->best->moves[0]->piece->type == castle);
    assert(tracer->best->moves[1]->from == e2);
    assert(tracer->best->moves[1]->destination == d1);
    assert(tracer->best->moves[1]->piece->c == white);
    assert(tracer->best->moves[1]->piece->type == queen);
    assert(tracer->best->moves[2]->from == a3);
    assert(tracer->best->moves[2]->destination == a2);
    assert(tracer->best->moves[2]->piece->c == black);
    assert(tracer->best->moves[2]->piece->type == queen);
    assert(tracer->best->moves[3]->from == b1);
    assert(tracer->best->moves[3]->destination == c1);
    assert(tracer->best->moves[3]->piece->c == white);
    assert(tracer->best->moves[3]->piece->type == king);
    assert(tracer->best->moves[4]->from == c5);
    assert(tracer->best->moves[4]->destination == a3);
    assert(tracer->best->moves[4]->piece->c == black);
    assert(tracer->best->moves[4]->piece->type == bishop);
    assert(tracer->best->moves[5]->from == e5);
    assert(tracer->best->moves[5]->destination == b2);
    assert(tracer->best->moves[5]->piece->c == white);
    assert(tracer->best->moves[5]->piece->type == bishop);
    assert(tracer->best->moves[6]->from == a2);
    assert(tracer->best->moves[6]->destination == b2);
    assert(tracer->best->moves[6]->piece->c == black);
    assert(tracer->best->moves[6]->piece->type == queen);

}

void test_mate_in_four_2() {
    Board* board = board_setup_1();

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(3, 5, black);
    int score = create_graph(grapher, grapher->start, board, black);
    assert(test_board == board->bitboard);
    Tracer* tracer = init_tracer(black);
    dfs(grapher->start, tracer);
    assert(score == 97);

}

void test_mate_in_four_3() {
    Board* board = board_setup_m4_2();

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(1, 4, white);
    int score = create_graph(grapher, grapher->start, board, white);
    assert(test_board == board->bitboard);
    assert(score == 97);

    board = board_setup_m4_2();
    Grapher* grapher2 = init_grapher(1, 3, white);
    test_board = board->bitboard;
    score = create_graph(grapher2, grapher2->start, board, white);
    assert(test_board == board->bitboard);
    assert(score == 0);

    Tracer* tracer = init_tracer(white);
    dfs(grapher->start, tracer);
    // print_tracer(tracer);
    assert(tracer->best_eval == 104);
    assert(tracer->best->length == 7);
    assert(tracer->best->moves[0]->from == e5);
    assert(tracer->best->moves[0]->destination == f7);
    assert(tracer->best->moves[0]->piece->c == white);
    assert(tracer->best->moves[0]->piece->type == knight);
    assert(tracer->best->moves[1]->from == d7);
    assert(tracer->best->moves[1]->destination == f7);
    assert(tracer->best->moves[1]->piece->c == black);
    assert(tracer->best->moves[1]->piece->type == queen);
    assert(tracer->best->moves[2]->from == g5);
    assert(tracer->best->moves[2]->destination == f7);
    assert(tracer->best->moves[2]->piece->c == white);
    assert(tracer->best->moves[2]->piece->type == knight);
    assert(tracer->best->moves[3]->from == d8);
    assert(tracer->best->moves[3]->destination == d7);
    assert(tracer->best->moves[3]->piece->c == black);
    assert(tracer->best->moves[3]->piece->type == king);
    assert(tracer->best->moves[4]->from == c4);
    assert(tracer->best->moves[4]->destination == b5);
    assert(tracer->best->moves[4]->piece->c == white);
    assert(tracer->best->moves[4]->piece->type == bishop);
    assert(tracer->best->moves[5]->from == c7);
    assert(tracer->best->moves[5]->destination == c6);
    assert(tracer->best->moves[5]->piece->c == black);
    assert(tracer->best->moves[5]->piece->type == pawn);
    assert(tracer->best->moves[6]->from == e1);
    assert(tracer->best->moves[6]->destination == e7);
    assert(tracer->best->moves[6]->piece->c == white);
    assert(tracer->best->moves[6]->piece->type == castle);

}

void test_mate_detection() {
    test_detect_mate_1();
    test_detect_mate_2();
    test_mate_in_four();
    test_mate_in_four_2();
    test_mate_in_four_3();
}

void test_forcing_moves() {
    Board* board = forcing_move_setup();
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(2, 7, black);
    int score = create_graph(grapher, grapher->start, board, black);
    printf("score: %i\n", score);

    assert(test_board == board->bitboard);

    Tracer* tracer = init_tracer(black);
    dfs(grapher->start, tracer);
    print_tracer(tracer);
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
    
    
    execute_move(board, board->pieces[black][CASTLE_1(black)], d1);
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
    execute_move(board, board->pieces[black][CASTLE_1(black)], d1);
    execute_move(board, board->pieces[white][QUEEN_INDEX(white)], d1);
    execute_move(board, board->pieces[black][QUEEN_INDEX(black)], a2);
    U64 test_board = board->bitboard;

    Moves* moves = get_all_moves_for_colour(board, white);
    assert(test_board == board->bitboard);
    assert(moves->length == 1);
    assert(moves->moves[0]->destination == c1);
    assert(moves->moves[0]->piece->cell == b1);
    assert(moves->moves[0]->piece->type == king);

    execute_move(board, board->pieces[white][KING_INDEX(white)], a2);
    assert(is_check(board, white));
}



void test() {
    test_moves();
    test_legality();
    test_move_selection();
    test_move_logic();
    test_move_logic_mate_in_four();
    test_mate_detection();
}