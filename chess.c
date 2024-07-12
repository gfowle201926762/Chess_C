#include "test.c"

int main(void) {
    initialise();

    // test();
    test_evaluation_branching();

    // test_wrapper(test_forcing_moves_3, "test_forcing_moves_3");
    // test_wrapper(test_mate_in_four_2, "test_mate_in_four_2");
    // test();

    return 0;
}

void initialise(void) {
    setlocale(LC_CTYPE, "");
    srand(time(NULL));
}




/* ----------------------------- */
/* --------- PLAY GAME --------- */
/* ----------------------------- */


square string_to_square(char c1, char c2) {
    c1 -= 'a';
    c2 -= ('0');
    c2 = 8 - c2;
    c2 *= 8;

    square s = c1 + c2;
    return s;
}

void play_game() {
    Board* board = init_board();
    print_board_pro(board);

    Grapher* grapher;
    Scores* scores;
    Moves* moves;
    int length = 1;


    // char from1;
    // char from2;
    // char to1 = 'c';
    // char to2;

    while (length) {
        // printf("From: ");
        // scanf("%c%c%*c", &from1, &from2);
        // printf("To: ");
        // scanf("%c%c%*c", &to1, &to2);
        // square from = string_to_square(from1, from2);
        // square to = string_to_square(to1, to2);
        grapher = init_grapher(8, 4, white);
        scores = create_graph(grapher, grapher->start, board, white, init_limit(true));
        moves = scores->moves;
        if (moves->length == 0) {
            printf("BLACK WINS\n");
            return;
        }
        pretend_move(board, moves->moves[0]);

        print_board_pro(board);

        grapher = init_grapher(8, 4, black);
        scores = create_graph(grapher, grapher->start, board, black, init_limit(true));
        moves = scores->moves;
        pretend_move(board, moves->moves[0]);
        if (moves->length == 0) {
            printf("WHITE WINS\n");
            return;
        }

        print_board_pro(board);
        length = moves->length;
    }
    

}




/* ------------------------------ */
/* ------- EVALUATE MOVES ------- */
/* ------------------------------ */

Scores* evaluate_no_moves(Grapher* grapher, Move* move, Board* board, colour mover) {
    if (!is_check(board, mover)) {
        // STALEMATE
        move->evaluation = 0;
        return init_scores(move, reverse_depth(grapher));
    }
    move->evaluation = MAX_SCORE - ((grapher->max_depth - grapher->depth) / 2);
    return init_scores(move, reverse_depth(grapher));
}

int count_bitboard(U64 mask) {
    int count = 0;
    for (int i = 0; i < CELLS; i++) {
        if (mask % 2 == 1) {
            count += 1;
        }
        mask >>= 1;
    }
    return count;
}

int count_threats(Board* board, colour mover, U64 mask) {
    int count = 0;
    for (int i=0; i<16; i++) {
        if (board->pieces[invert_colour(mover)][i]->alive) {
            if (get_bit(mask, board->pieces[invert_colour(mover)][i]->cell)) {
                count += board->pieces[invert_colour(mover)][i]->value / 10;
            }
        }
    }
    return count;
}

int initiative_heuristic(Board* board, colour mover) {
    int count = 0;
    for (int i=0; i<16; i++) {
        if (board->pieces[mover][i]->alive) {
            U64 mask = board->pieces[mover][i]->move_func(board, board->pieces[mover][i]);
            // count += count_bitboard(mask);
            count += count_threats(board, mover, mask);
        }
    }
    return count;
}

int king_safety_heuristic(Board* board, Move* move, colour mover, Grapher* grapher) {
    int cell = board->pieces[mover][KING_INDEX(mover)]->cell;
    if (mover != grapher->start_player) {
        int row = 7 - (cell / 8);
        cell = (row * 8) + (cell % 8);
        return -KING_EVAL[cell];
    }
    return KING_EVAL[cell];
}

int measure_points(Board* board, colour mover) {
    int score = 0;
    for (int i = 0; i < 16; i++) {
        if (board->pieces[mover][i]->alive) {
            score += board->pieces[mover][i]->value;
        }
        if (board->pieces[invert_colour(mover)][i]->alive) {
            score -= board->pieces[invert_colour(mover)][i]->value;
        }
    }
    return score;
}

int evaluate_position(Board* board, colour mover) {
    int score = 0;
    // score += king_safety_heuristic(board, node, mover, grapher);
    score += initiative_heuristic(board, mover);   
    score += measure_points(board, mover);
    return score;
}


/* ------------------------------ */
/* -------- SEARCH GRAPH -------- */
/* ------------------------------ */

void reorder_best_scores(int* best_scores, Moves* best_moves, int max_breadth, Move* new_move) {
    int i = 0;
    int new_score = new_move->evaluation;
    for (i = 0; i < best_moves->length; i++) {
        if (new_score > best_scores[i]) {
            int saved = best_scores[i];
            Move* saved_move = best_moves->moves[i];
            best_scores[i] = new_score;
            best_moves->moves[i] = new_move;
            new_score = saved;
            new_move = saved_move;
        }
    }
    if (i < max_breadth) {
        best_scores[i] = new_score;
        best_moves->moves[i] = new_move;
        best_moves->length += 1;
    }
}

Moves* get_best_scores(Moves* moves, int max_breadth) {
    Moves* best_moves = calloc(1, sizeof(Moves));
    int best_scores[max_breadth];
    for (int i = 0; i < moves->length; i++) {
        if (max_breadth > best_moves->length || best_scores[max_breadth-1] < moves->moves[i]->evaluation) {
            reorder_best_scores(best_scores, best_moves, max_breadth, moves->moves[i]);
        }
    }
    return best_moves;
}

Scores* init_scores(Move* move, int depth) {
    Scores* scores = calloc(1, sizeof(Scores));
    scores->moves = calloc(1, sizeof(Moves));
    scores->eval = move->evaluation;
    scores->moves->moves[depth - 1] = move;
    scores->moves->length = depth;
    return scores;
}

Moves* hacky_Moves(Move* move) {
    Moves* moves = calloc(1, sizeof(Moves));
    moves->single = true;
    moves->moves[0] = move;
    return moves;
}

Moves* get_best_moves(Board* board, Moves* moves, colour mover, int max_breadth, int prune, bool original_mover, int depth) {
    // also check for a cutoff here from the transposition table.
    for (int i = 0; i < moves->length; i++) {
        Move* move = moves->moves[i];
        Piece* killed = pretend_move(board, move);

        Transposition* t = get(board, hash(board, move, killed), depth);
        if (prune != MAX_SCORE && prune != -MAX_SCORE && t && t->flag != LOWER_BOUND && (!original_mover && t->eval <= prune)) {
            move->evaluation = t->eval;
            undo_pretend_move(board, move, killed);
            return hacky_Moves(move);
        }
        if (prune != MAX_SCORE && prune != -MAX_SCORE && t && t->flag != UPPER_BOUND && (original_mover && t->eval >= prune)) {
            move->evaluation = t->eval;
            undo_pretend_move(board, move, killed);
            return hacky_Moves(move);
        }
        if (t && (t->flag == EXACT || (t->flag != UPPER_BOUND && (original_mover && t->eval >= prune)) || LOWER_BOUND && (!original_mover && t->eval <= prune))) {
            move->evaluation = t->eval;
        }
        else {
            move->evaluation = evaluate_position(board, mover);
        }
        undo_pretend_move(board, move, killed);
    }

    return get_best_scores(moves, max_breadth);
}

bool init_original_mover(Move* move, Grapher* grapher) {
    if (move) {
        return move->piece->c != grapher->start_player;
    }
    return true;
}

int init_limit(bool original_mover) {
    if (original_mover) {
        return -MAX_SCORE;
    }
    return MAX_SCORE;
}

int reverse_depth(Grapher* grapher) {
    return grapher->max_depth - grapher->depth;
}

Scores* create_graph(Grapher* grapher, Move* parent_move, Board* board, colour mover, int prune) {
    if (grapher->depth == grapher->base) {
        board->leaves += 1;
        parent_move->evaluation = evaluate_position(board, mover);
        return init_scores(parent_move, reverse_depth(grapher));
    }
    
    Moves* moves = get_all_moves_for_colour(board, mover);
    if (moves->length == 0) {
        return evaluate_no_moves(grapher, parent_move, board, mover);
    }

    bool original_mover = init_original_mover(parent_move, grapher);
    bool pruned = false;
    int limit = init_limit(original_mover);
    Moves* best_moves = get_best_moves(board, moves, mover, grapher->max_breadth, prune, original_mover, grapher->depth);
    if (best_moves->single) {
        // prunable
        put(board, board->hash_value, best_moves->moves[0]->evaluation, grapher->depth, true, original_mover);
        return init_scores(best_moves->moves[0], reverse_depth(grapher));
    }

    Scores* best_scores;
    for (int i = 0; i < best_moves->length; i++) {
        Move* move = best_moves->moves[i];
        // Board* copy = copy_board(board);
        
        Piece* killed = pretend_move(board, move);
        Transposition* t = hash_and_save(board, move, killed, grapher->depth);

        Scores* scores; 
        
        if (t && (t->flag == EXACT || (t->flag != UPPER_BOUND && (original_mover && t->eval >= prune)) || LOWER_BOUND && (!original_mover && t->eval <= prune))) {
            // found in TT, move->evaluation updated in get_best_moves
            scores = init_scores(move, reverse_depth(grapher));
        }
        else if (!draw_by_repetition(board)) {
            grapher->depth -= 1;
            scores = create_graph(grapher, move, board, invert_colour(mover), limit);
            grapher->depth += 1;
        }
        else {
            move->evaluation = 0;
            scores = init_scores(move, reverse_depth(grapher));
        }

        // REVERSE
        undo_hash(board, move, killed);
        undo_pretend_move(board, move, killed);

        // assert(compare_boards(board, copy, "create_graph"));
        // free_copy_board(copy);

        if ((original_mover && scores->eval > limit) || (!original_mover && scores->eval < limit)) {
            limit = scores->eval;
            best_scores = scores;
        }
        else {
            free_scores(scores, reverse_depth(grapher));
        }
        if (pruned || (prune != MAX_SCORE && prune != -MAX_SCORE && ((original_mover && best_scores->eval >= prune) || (!original_mover && best_scores->eval <= prune)))) {
            pruned = true;
            break;
        }
    }
    if (parent_move != NULL) {
        best_scores->moves->moves[reverse_depth(grapher) - 1] = parent_move;
    }
    put(board, board->hash_value, best_scores->eval, grapher->depth, pruned, original_mover);
    return best_scores;
}



/* -------------------------------- */
/* ------- SEARCH ALL MOVES ------- */
/* -------------------------------- */

void generate_promotion_moves(Board* board, Moves* moves, Piece* piece, square cell) {
    for (int i = 0; i < MAX_PROMOTABLE_PIECES; i++) {
        Move* move = calloc(1, sizeof(Move));
        move->destination = cell;
        move->from = piece->cell;
        move->piece = piece;
        move->promotion = board->promotable_pieces[i];
        moves->moves[moves->length] = move;
        moves->length += 1;
    }
}

void get_all_moves_for_piece(Board* board, Piece* piece, Moves* moves) {

    // print_piece(piece);
    square saved_from = piece->cell;
    U64 mask = piece->move_func(board, piece);
    // print_board(mask);
    // printf("\n");
    for (int i = 0; i < CELLS; i++) {
        U64 saved = board->bitboard;
        if (mask % 2 == 1) {
            // bit found on the bit board
            if (is_move_legal(board, piece, i)) {
                if(saved != board->bitboard) {
                    print_piece(piece);
                    assert(false);
                }
                bool allowed = true;
                bool castle = false;
                castle_type type = king_side;
                if (piece->type == king && piece->no_moves == 0) {
                    if (i == board->castling_coordinates[piece->c][king_side]) {
                        allowed = is_castle_legal(board, piece, king_side);
                        type = king_side;
                        castle = true;
                    }
                    else if (i == board->castling_coordinates[piece->c][queen_side]) {
                        allowed = is_castle_legal(board, piece, queen_side);
                        type = queen_side;
                        castle = true;
                        
                    }
                } 
                if (piece->type == pawn) {
                    if ((piece->c == white && i / 8 == 0) || (piece->c == black && i / 8 == 7)) {
                        generate_promotion_moves(board, moves, piece, i);
                        allowed = false;
                    }
                }
                if(saved != board->bitboard) {
                    char s[10] = {'\0'};
                    char f[10] = {'\0'};
                    char fr[10] = {'\0'};
                    square_to_string(saved_from, fr);
                    square_to_string(i, s);
                    square_to_string(piece->cell, f);
                    printf("moving to %s\n", s);
                    printf("moving from %s\n", s);
                    printf("moving from fr: %s\n", fr);
                    printf("castle status: %i\n", castle);
                    print_piece(piece);
                    print_board_pro(board);
                    assert(false);
                }
                if (allowed) {
                    moves->moves[moves->length] = calloc(1, sizeof(Move));
                    moves->moves[moves->length]->piece = piece;
                    moves->moves[moves->length]->destination = i;
                    moves->moves[moves->length]->from = piece->cell;
                    moves->moves[moves->length]->castle = castle;
                    moves->moves[moves->length]->castle_side = type;
                    moves->length += 1;
                }
            }
            // if(piece->type == pawn) {
            //     char s[10] = {'\0'};
            //     char f[10] = {'\0'};
            //     square_to_string(i, s);
            //     square_to_string(piece->cell, f);
            //     printf("moving to %s\n", s);
            //     printf("moving from %s\n", s);
            //     print_piece(piece);
            // }
            if(saved != board->bitboard) {
                char s[10] = {'\0'};
                char f[10] = {'\0'};
                square_to_string(i, s);
                square_to_string(piece->cell, f);
                printf("moving to %s\n", s);
                printf("moving from %s\n", s);
                print_piece(piece);
                print_board(saved);
                print_board(board->bitboard);
                print_board_pro(board);
                assert(false);
            }
        }
        
        mask >>= 1;
    }
}

Moves* get_all_moves_for_colour(Board* board, colour c) {
    Moves* moves = calloc(1, sizeof(Moves));
    for (int i = 0; i < 16; i++) {
        if (board->pieces[c][i]->alive) {
            get_all_moves_for_piece(board, board->pieces[c][i], moves);
        }
    }
    return moves;
}



/* -------------------------------- */
/* ------- LEGAL MOVE LOGIC ------- */
/* -------------------------------- */

bool draw_by_repetition(Board* board) {
    int count = 0;
    for (int i = 0; i < board->lm_length; i++) {
        if (board->last_positions[i] == board->hash_repeats) {
            count += 1;
            if (count == DRAW_REPITIONS) {
                return true;
            }
        }
    }
    return false;
}

bool is_castle_legal(Board* board, Piece* piece, castle_type type) {
    if (piece->no_moves > 0 || board->castle_pieces[piece->c][type]->no_moves > 0) {
        return false;
    }
    if (!board->castle_pieces[piece->c][type]->alive) {
        return false;
    }
    if (is_check(board, piece->c)) {
        return false;
    }
    // test every square between castle and king is null
    int diff = board->from_castle_coords[piece->c][type] - piece->cell;
    for (int i = 1; i < abs(diff); i++) {
        if (board->map[type == king_side ? piece->cell + i : piece->cell - i] != NULL) {
            return false;
        }
    }

    // test legality of moves up to castling square
    diff = board->castling_coordinates[piece->c][type] - piece->cell;
    for (int i = 1; i < abs(diff); i++) {
        if (!is_move_legal(board, piece, type == king_side ? piece->cell + i : piece->cell - i)) {
            return false;
        }
    }
    // test legality of final castled move
    square king_original = piece->cell;
    square castle_original = board->castle_pieces[piece->c][type]->cell;
    move_single_piece(board, piece, board->castling_coordinates[piece->c][type], none);
    move_single_piece(board, board->castle_pieces[piece->c][type], type == king_side ? piece->cell - 1 : piece->cell + 1, none);
    bool allowed = true;
    if (is_check(board, piece->c)) {
        allowed = false;
    }
    move_single_piece(board, piece, king_original, none);
    move_single_piece(board, board->castle_pieces[piece->c][type], castle_original, none);
    return allowed;
}

Piece* move_single_piece(Board* board, Piece* piece, square to, name promotion) {
    // Literally just move a single piece from A to B.
    // Return killed (literally just for is_move_legal where it needs to be reversed).
    // Luckily, that function doesn't handle castling.
    // Do not care about castling (two pieces move when castling)
    // Do not care about en passant (this is handled in the wrapper function move_single_piece_with_en_passant)
    // Do promote.
    pop_bit(board->bitboard, piece->cell);
    set_bit(board->bitboard, to);
    board->map[piece->cell] = NULL;
    piece->cell = to;
    Piece* killed = board->map[to];
    if (board->map[to]) {
        board->map[to]->alive = false;
    }
    board->map[to] = piece;
    execute_promotion(piece, promotion);
    return killed;
}

Piece* handle_en_passant(Board* board, Piece* piece, square to, name promotion) {
    // must be a pawn which is attacking
    if (piece->type != pawn || piece->cell % 8 == to % 8) {
        return NULL;
    }
    // must be moving into an empty square -- guaranteed en passant.
    if (board->map[to] == NULL) {
        Piece* killed = board->map[piece->c == white ? to + 8 : to - 8];
        killed->alive = false;
        board->map[piece->c == white ? to + 8 : to - 8] = NULL;
        pop_bit(board->bitboard, (piece->c == white ? to + 8 : to - 8));
        return killed;
    }
    return NULL;
}

Piece* move_single_piece_with_en_passant(Board* board, Piece* piece, square to, name promotion) {
    Piece* killed = handle_en_passant(board, piece, to, promotion);
    if (killed) {
        move_single_piece(board, piece, to, promotion);
        return killed;
    }
    return move_single_piece(board, piece, to, promotion);
}

Piece* pretend_move(Board* board, Move* move) {
    // A move has already been generated. It is guaranteed to be legal. Execute the move.
    // Must handle castling and en passant.
    add_last_moved(board, move);
    move->piece->no_moves += 1;
    if (move->castle) {
        move_single_piece(board, move->piece, move->destination, none);
        move_single_piece(board, board->castle_pieces[move->piece->c][move->castle_side], board->to_castle_coords[move->piece->c][move->castle_side], none);
        return NULL;
    }
    return move_single_piece_with_en_passant(board, move->piece, move->destination, move->promotion);
}

void undo_pretend_move(Board* board, Move* move, Piece* killed) {
    pop_last_moved(board);
    move->piece->no_moves -= 1;
    if (move->castle) {
        move_single_piece(board, move->piece, move->from, none);
        move_single_piece(board, board->castle_pieces[move->piece->c][move->castle_side], board->from_castle_coords[move->piece->c][move->castle_side], none);
        return;
    }
    move_single_piece(board, move->piece, move->from, move->promotion == none ? none : pawn);
    if (killed) {
        killed->alive = true;
        set_bit(board->bitboard, killed->cell);
        board->map[killed->cell] = killed;
    }
}


void execute_promotion(Piece* piece, name promotion) {
    if (promotion == none) {
        return;
    }
    piece->type = promotion;
    switch (promotion)
    {
    case queen:
        piece->value = QUEEN_VALUE;
        piece->move_func = get_queen_mask;
        break;
    case castle:
        piece->value = CASTLE_VALUE;
        piece->move_func = get_castle_mask;
        break;
    case bishop:
        piece->value = BISHOP_VALUE;
        piece->move_func = get_bishop_mask;
        break;
    case knight:
        piece->value = KNIGHT_VALUE;
        piece->move_func = get_knight_mask;
        break;
    case pawn:
        piece->value = PAWN_VALUE;
        piece->move_func = get_pawn_mask;
        break;
    default:
        return;
    }
}

void add_last_moved(Board* board, Move* move) {
    board->last_moved[board->lm_length] = move;
    board->lm_length += 1;
}

void pop_last_moved(Board* board) {
    board->last_moved[board->lm_length - 1] = NULL;
    board->lm_length -= 1;
}

bool is_check(Board* board, colour c) {
    for (int i = 0; i < 16; i++){
        if (board->pieces[invert_colour(c)][i]->alive) {
            U64 mask = board->pieces[invert_colour(c)][i]->move_func(board, board->pieces[invert_colour(c)][i]);
            if (get_bit(mask, board->pieces[c][KING_INDEX(c)]->cell)) {
                return true;
            }
        }
    }
    return false;
}

bool is_move_legal(Board* board, Piece* piece, square to) {

    // Board* copy = copy_board(board);

    square from = piece->cell;
    Piece* killed = move_single_piece_with_en_passant(board, piece, to, none);
    bool legal = true;
    if (killed && killed->c == piece->c) {
        legal = false;
    }
    else if (is_check(board, piece->c)) {
        legal = false;
    }
    move_single_piece(board, piece, from, none);
    if (killed) {
        killed->alive = true;
        set_bit(board->bitboard, killed->cell);
        board->map[killed->cell] = killed;
    }
    // assert(compare_boards(board, copy, "is_move_legal"));
    // free_copy_board(copy);
    return legal;
}


/* --------------------------------- */
/* -------- INDEX FUNCTIONS -------- */
/* --------------------------------- */

int king_index(colour c, int i) {
    return KING_INDEX(c);
}

int queen_index(colour c, int i) {
    return QUEEN_INDEX(c);
}

int bishop_index(colour c, int i) {
    if (i == 1) {
        return BISHOP_1(c);
    }
    return BISHOP_2(c);
}

int knight_index(colour c, int i) {
    if (i == 1) {
        return KNIGHT_1(c);
    }
    return KNIGHT_2(c);
}

int castle_index(colour c, int i) {
    if (i == 1) {
        return CASTLE_2(c);
    }
    return CASTLE_1(c);
}

int pawn_index(colour c, int i) {
    return PAWN_INDEX(c, i);
}

int (*get_index_func(name n))(colour c, int i) {
    if (n == king) return king_index;
    if (n == queen) return queen_index;
    if (n == bishop) return bishop_index;
    if (n == knight) return knight_index;
    if (n == castle) return castle_index;
    return pawn_index;
}

/* ------------------------------- */
/* ----------- HASHING ----------- */
/* ------------------------------- */



Transposition* get(Board* board, U64 hash_value, int depth) {
    if (board->transpositions[hash_value % HASH_TABLE_SIZE] && board->transpositions[hash_value % HASH_TABLE_SIZE]->hash_value == hash_value && board->transpositions[hash_value % HASH_TABLE_SIZE]->depth >= depth) {
        return board->transpositions[hash_value % HASH_TABLE_SIZE];
    }
    return NULL;
}

void put(Board* board, U64 hash_value, int eval, int depth, bool pruned, bool original_mover) {
    int index = hash_value % HASH_TABLE_SIZE;
    if (board->transpositions[index] && board->transpositions[index]->depth >= depth) {
        // keep (assuming a deeper depth is more valuable)
        return;
    }
    if (board->transpositions[index] == NULL) {
        Transposition* transposition = calloc(1, sizeof(Transposition));
        board->transpositions[index] = transposition;
    }
    board->transpositions[index]->hash_value = hash_value;
    board->transpositions[index]->eval = eval;
    board->transpositions[index]->depth = depth;
    if (pruned && original_mover) {
        board->transpositions[index]->flag = LOWER_BOUND;
    }
    if (pruned && !original_mover) {
        board->transpositions[index]->flag = UPPER_BOUND;
    }
}

Transposition* hash_and_save(Board* board, Move* move, Piece* killed, int depth) {
    hash_move_piece(board, move, killed);
    // lm_length already been incremented in add last moved in pretend move.
    board->last_positions[board->lm_length - 1] = board->hash_repeats;

    return get(board, board->hash_value, depth);
    // put(board, board->hash_value, move->evaluation, depth);
    // return t;
}

void undo_hash(Board* board, Move* move, Piece* killed) {
    hash_move_piece(board, move, killed);
}

U64 hash(Board* board, Move* move, Piece* killed) {
    U64 value = board->hash_value;
    
    value ^= board->keys_position[move->piece->c][move->promotion == none ? move->piece->type : pawn][move->from];
    if (killed) {
        value ^= board->keys_position[killed->c][killed->type][killed->cell];
    }
    value ^= board->keys_position[move->piece->c][move->promotion == none ? move->piece->type : move->promotion][move->destination];

    if (move->piece->type == pawn && (int)move->from - move->destination == 16) {
        value ^= board->keys_last_moved[move->destination];
    }

    if ((move->piece == board->pieces[move->piece->c][KING_INDEX(move->piece->c)] && board->pieces[move->piece->c][KING_INDEX(move->piece->c)]->no_moves == 1 && board->castle_pieces[move->piece->c][king_side]->no_moves == 0) || (board->pieces[move->piece->c][KING_INDEX(move->piece->c)]->no_moves == 0 && move->piece == board->castle_pieces[move->piece->c][king_side] && board->castle_pieces[move->piece->c][king_side]->no_moves == 1)) {
        value ^= board->keys_castling[move->piece->c][king_side];
    }
    if ((move->piece == board->pieces[move->piece->c][KING_INDEX(move->piece->c)] && board->pieces[move->piece->c][KING_INDEX(move->piece->c)]->no_moves == 1 && board->castle_pieces[move->piece->c][queen_side]->no_moves == 0) || (board->pieces[move->piece->c][KING_INDEX(move->piece->c)]->no_moves == 0 && move->piece == board->castle_pieces[move->piece->c][queen_side] && board->castle_pieces[move->piece->c][queen_side]->no_moves == 1)) {
        value ^= board->keys_castling[move->piece->c][queen_side];
    }
    value ^= board->key_mover;
    return value;
}

// need two of these functions -> draw by repetition doesn't care about castling rights (ask chess.com)
void hash_move_piece(Board* board, Move* move, Piece* killed) {
    // hashing moving piece out
    board->hash_value ^= board->keys_position[move->piece->c][move->promotion == none ? move->piece->type : pawn][move->from];
    board->hash_repeats ^= board->keys_position[move->piece->c][move->promotion == none ? move->piece->type : pawn][move->from];
    if (killed) {
        // hashing killed piece out. Must be killed->cell, not move->destination, because of en passant.
        board->hash_value ^= board->keys_position[killed->c][killed->type][killed->cell];
        board->hash_repeats ^= board->keys_position[killed->c][killed->type][killed->cell];
    }
    // hashing moving piece in
    board->hash_value ^= board->keys_position[move->piece->c][move->promotion == none ? move->piece->type : move->promotion][move->destination];
    board->hash_repeats ^= board->keys_position[move->piece->c][move->promotion == none ? move->piece->type : move->promotion][move->destination];

    // hashing in position of last move destination if pawn and moved 2 squares (for en passant)
    if (move->piece->type == pawn && (int)move->from - move->destination == 16) {
        board->hash_value ^= board->keys_last_moved[move->destination];
    }
    
    // keep track of castling rights... must be at the point of losing the rights.
    // either king just moved or castle just moved, AND that move made the no_moves increment to 1.
    // hash function is made after pretend_move and before undo_pretend_move
    if ((move->piece == board->pieces[move->piece->c][KING_INDEX(move->piece->c)] && board->pieces[move->piece->c][KING_INDEX(move->piece->c)]->no_moves == 1 && board->castle_pieces[move->piece->c][king_side]->no_moves == 0) || (board->pieces[move->piece->c][KING_INDEX(move->piece->c)]->no_moves == 0 && move->piece == board->castle_pieces[move->piece->c][king_side] && board->castle_pieces[move->piece->c][king_side]->no_moves == 1)) {
        board->hash_value ^= board->keys_castling[move->piece->c][king_side];
    }
    if ((move->piece == board->pieces[move->piece->c][KING_INDEX(move->piece->c)] && board->pieces[move->piece->c][KING_INDEX(move->piece->c)]->no_moves == 1 && board->castle_pieces[move->piece->c][queen_side]->no_moves == 0) || (board->pieces[move->piece->c][KING_INDEX(move->piece->c)]->no_moves == 0 && move->piece == board->castle_pieces[move->piece->c][queen_side] && board->castle_pieces[move->piece->c][queen_side]->no_moves == 1)) {
        board->hash_value ^= board->keys_castling[move->piece->c][queen_side];
    }

    // hashing a change of colour
    board->hash_value ^= board->key_mover;
}


U64 rand64() {
    return rand() ^ ((U64)rand() << 15) ^ ((U64)rand() << 30) ^
        ((U64)rand() << 45) ^ ((U64)rand() << 60);
}

void init_hash_keys(Board* board) {
    for (int i = 0; i < MAX_COLOUR; i++) {
        for (int j = 0; j < MAX_PIECE_TYPE; j++) {
            for (int k = 0; k < CELLS; k++) {
                board->keys_position[i][j][k] = rand64();
            }
        }
        for (int j = 0; j < MAX_CASTLING_OPTIONS; j++) {
            board->keys_castling[i][j] = rand64();
        }
    }
    for (int i = 0; i < CELLS; i++) {
        board->keys_last_moved[i] = rand64();
    }
    for (int i = 0; i < MAX_REPETITIONS; i++) {
        board->keys_repetitions[i] = rand64();
    }
    board->key_mover = rand64();
}


/* -------------------------------- */
/* -------- INITIALISATION -------- */
/* -------------------------------- */

void init_hash_value(Board* board) {
    for (int i = 0; i < CELLS; i++) {
        if (board->map[i]) {
            board->hash_value ^= board->keys_position[board->map[i]->c][board->map[i]->type][i];
        }
    }
    board->hash_repeats = board->hash_value;
}

Board* init_board(void) {
    Board* board = calloc(1, sizeof(Board));
    board->bitboard = 0ULL;
    int cells[32] = {a8, b8, c8, d8, e8, f8, g8, h8, a7, b7, c7, d7, e7, f7, g7, h7, a2, b2, c2, d2, e2, f2, g2, h2, a1, b1, c1, d1, e1, f1, g1, h1};
    board->bitboard = set_multiple_bits(board->bitboard, cells, 32);

    board->promotable_pieces[0] = queen;
    board->promotable_pieces[1] = castle;
    board->promotable_pieces[2] = bishop;
    board->promotable_pieces[3] = knight;

    board->valid_pieces[0] = king;
    board->valid_pieces[1] = queen;
    board->valid_pieces[2] = castle;
    board->valid_pieces[3] = bishop;
    board->valid_pieces[4] = knight;
    board->valid_pieces[5] = pawn;


    for (int i = 0; i < 32; i++) {
        Piece* piece = calloc(1, sizeof(Piece));
        piece->id = i;
        piece->alive = true;
        if (i < 16) {
            board->pieces[black][i] = piece;
            piece->c = black;
            piece->cell = i;
            board->map[i] = piece;
        }
        else {
            board->pieces[white][i-16] = piece;
            piece->c = white;
            piece->cell = i + (8*4);
            board->map[i+(8*4)] = piece;
        }
        if (i < 8 || i > 23) {
            if (i % 8 == 0 || i % 8 == 7) {
                piece->type = castle;
                piece->value = CASTLE_VALUE;
                piece->move_func = get_castle_mask;
                piece->index_func = castle_index;
                if (i < 8) {
                    piece->character = L'♖';
                }
                else {
                    piece->character = L'♜';
                }
            }
            if (i % 8 == 1 || i % 8 == 6) {
                piece->type = knight;
                piece->value = KNIGHT_VALUE;
                piece->move_func = get_knight_mask;
                piece->index_func = knight_index;
                if (i < 8) {
                    piece->character = L'♘';
                }
                else {
                    piece->character = L'♞';
                }
            }
            if (i % 8 == 2 || i % 8 == 5) {
                piece->type = bishop;
                piece->value = BISHOP_VALUE;
                piece->move_func = get_bishop_mask;
                piece->index_func = bishop_index;
                if (i < 8) {
                    piece->character = L'♗';
                }
                else {
                    piece->character = L'♝';
                }
            }
            if (i % 8 == 3) {
                piece->type = queen;
                piece->value = QUEEN_VALUE;
                piece->move_func = get_queen_mask;
                piece->index_func = queen_index;
                if (i < 8) {
                    piece->character = L'♕';
                }
                else {
                    piece->character = L'♛';
                }
            }
            if (i % 8 == 4) {
                piece->type = king;
                piece->value = KING_VALUE;
                piece->move_func = get_king_mask;
                piece->index_func = king_index;
                if (i < 8) {
                    piece->character = L'♔';
                }
                else {
                    piece->character = L'♚';
                }
            }
        }
        else {
            piece->type = pawn;
            piece->value = PAWN_VALUE;
            piece->move_func = get_pawn_mask;
            piece->index_func = pawn_index;
            if (i < 16) {
                piece->character = L'♙';
            }
            else {
                piece->character = L'♟';
            }
        }
    }

    init_hash_keys(board);
    init_hash_value(board);

    board->castle_pieces[white][king_side] = board->pieces[white][CASTLE_1(white)];
    board->castling_coordinates[white][king_side] = g1;
    board->to_castle_coords[white][king_side] = f1;
    board->from_castle_coords[white][king_side] = h1;

    board->castle_pieces[white][queen_side] = board->pieces[white][CASTLE_2(white)];
    board->castling_coordinates[white][queen_side] = c1;
    board->to_castle_coords[white][queen_side] = d1;
    board->from_castle_coords[white][queen_side] = a1;

    board->castle_pieces[black][king_side] = board->pieces[black][CASTLE_1(black)];
    board->castling_coordinates[black][king_side] = g8;
    board->to_castle_coords[black][king_side] = f8;
    board->from_castle_coords[black][king_side] = h8;

    board->castle_pieces[black][queen_side] = board->pieces[black][CASTLE_2(black)];
    board->castling_coordinates[black][queen_side] = c8;
    board->to_castle_coords[black][queen_side] = d8;
    board->from_castle_coords[black][queen_side] = a8;
    return board;
}

Grapher* init_grapher(int breadth, int depth, colour start_player) {
    Grapher* grapher = calloc(1, sizeof(Grapher));
    // GraphNode* node = calloc(1, sizeof(GraphNode));
    // grapher->start = node;
    grapher->max_breadth = breadth;
    grapher->max_depth = depth * 2;
    grapher->depth = depth * 2;
    grapher->start_player = start_player;
    return grapher;
}


void set_board(Board* board) {
    for (int i = 0; i < 32; i++) {
        if (board->pieces[i/16][i%16]->alive == true) {
            set_bit(board->bitboard, board->pieces[i/16][i%16]->cell);
        }
    }
}

void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}

colour char_to_colour(char c) {
    if (c == 'w') {
        return white;
    }
    if (c == 'b') {
        return black;
    }
    on_error("Incorrect char for colour representation.");
    return white;
}

name char_to_name(char c) {
    if (c == 'k') return king;
    if (c == 'q') return queen;
    if (c == 'b') return bishop;
    if (c == 'h') return knight;
    if (c == 'c') return castle;
    if (c == 'p') return pawn;
    on_error("Incorrect char for piece representation.");
    return pawn;
}

int get_piece_number(name n, int bishops, int knights, int castles, int pawns) {
    if (n == bishop) return bishops % 2;
    if (n == knight) return knights % 2;
    if (n == castle) return castles % 2;
    return pawns % 8;
}

Board* set_board_notation(char* s) {
    // w = white, b = black
    // k = king, q = queen, b = bishop, k = knight, c = castle (1 or 2 to disambiguate)
    // square coordinate
    // comma and space

    Board* board = init_board();
    clear_board(board);
    int bishops = 0;
    int knights = 0;
    int castles = 0;
    int pawns = 0;
    int i = 0;
    int inner = 0;
    colour c;
    name p;
    square sq;
    while (s[i]) {
        if (inner == 0) {
            c = char_to_colour(s[i]);
        }
        if (inner == 1) {
            p = char_to_name(s[i]);
        }
        if (inner == 2) {
            sq = string_to_square(s[i], s[i+1]);
            i += 1;
            inner += 1;
        }
        if (s[i] == ' ') {
            int n = get_piece_number(p, bishops, knights, castles, pawns);
            // char square_string[5] = {'\0'};
            // char colour_string[10] = {'\0'};
            // char piece_string[10] = {'\0'};
            // colour_to_string(c, colour_string);
            // square_to_string(sq, square_string);
            // piece_to_string(p, piece_string);
            // printf("%s %s on %s\n", colour_string, piece_string, square_string);
            // print_piece(board->pieces[c][get_index_func(p)(c, n)]);
            board->pieces[c][get_index_func(p)(c, n)]->alive = true;
            board->pieces[c][get_index_func(p)(c, n)]->cell = sq;
            board->map[sq] = board->pieces[c][get_index_func(p)(c, n)];
            // print_piece(board->pieces[c][get_index_func(p)(c, n)]);
            if (p == bishop) bishops += 1;
            if (p == knight) knights += 1;
            if (p == pawn) pawns += 1;
            if (p == castle) castles += 1;
            inner = -1;
        }
        i += 1;
        inner += 1;
    }
    set_board(board);
    if (board->pieces[white][KING_INDEX(white)]->cell != e1) {
        board->pieces[white][KING_INDEX(white)]->no_moves += 1;
    }
    if (board->pieces[black][KING_INDEX(black)]->cell != e8) {
        board->pieces[black][KING_INDEX(black)]->no_moves += 1;
    }
    return board;
}

/* -------------------------------- */
/* -------- GET MOVE MASKS -------- */
/* -------------------------------- */

U64 get_queen_mask(Board* board, Piece* piece) {
    U64 mask = get_bishop_mask(board, piece);
    mask |= get_castle_mask(board, piece);
    return mask;
}

U64 get_bishop_mask(Board* board, Piece* piece) {
    U64 mask = 0ULL;
    int cell = piece->cell;
    int start_col = cell % 8;

    int up_left = (cell - (start_col * 9)) > start_col - (cell / 8) ? (cell - (start_col * 9)) : start_col - (cell / 8);
    int up_right = (cell - ((7 - start_col) * 7)) > start_col + (cell / 8) ? (cell - ((7 - start_col) * 7)) : start_col + (cell / 8);
    int down_left = (cell + (start_col * 7)) < 63 ? (cell + (start_col * 7)) : 63;
    int down_right = (cell + ((7 - start_col) * 9)) < 63 ? (cell + ((7 - start_col) * 9)) : 63;

    // up left
    for (int i = cell - 9; i >= up_left; i -= 9) {
        set_bit(mask, i);
        if (get_bit(board->bitboard, i)) break;
    }
    // up_right
    for (int i = cell - 7; i >= up_right; i -= 7) {
        set_bit(mask, i);
        if (get_bit(board->bitboard, i)) break;
    }
    //down_left
    for (int i = cell + 7; i <= down_left; i += 7) {
        set_bit(mask, i);
        if (get_bit(board->bitboard, i)) break;
    }
    // down_right
    for (int i = cell + 9; i <= down_right; i+=9) {
        set_bit(mask, i);
        if (get_bit(board->bitboard, i)) break;
    }
    return mask;
}

U64 get_castle_mask(Board* board, Piece* piece) {
    U64 mask = 0ULL;
    int cell = piece->cell;
    int start_row = (cell / 8) * 8;
    int end_row = start_row + 8;
    int start_col = cell % 8;
    int end_col = start_col + (8*7);
    
    for (int i = cell + 1; i < end_row; i++) {
        set_bit(mask, i);
        if (get_bit(board->bitboard, i)) break;
    }
    for (int i = cell - 1; i >= start_row; i--) {
        set_bit(mask, i);
        if (get_bit(board->bitboard, i)) break;
    }
    for (int i = cell + 8; i <= end_col; i += 8) {
        set_bit(mask, i);
        if (get_bit(board->bitboard, i)) break;
    }
    for (int i = cell - 8; i >= start_col; i -= 8) {
        set_bit(mask, i);
        if (get_bit(board->bitboard, i)) break;
    }
    return mask;
}

U64 get_knight_mask(Board* board, Piece* piece) {
    U64 mask = 0ULL;
    int cell = piece->cell;
    for (int i = -2; i <= 2; i++) {
        if (i == 0) continue;
        if (i < 0 && ((cell + i) % 8 > cell % 8)) continue;
        if (i > 0 && ((cell + i) % 8 < cell % 8)) continue;
        if (abs(i) == 1) {
            if (cell / 8 > 1) set_bit(mask, (cell + i - 16));
            if (cell / 8 < 6) set_bit(mask, (cell + i + 16));
        }
        if (abs(i) == 2) {
            if (cell / 8 > 0) set_bit(mask, (cell + i - 8));
            if (cell / 8 < 7) set_bit(mask, (cell + i + 8));
        }
    }
    return mask;
}

U64 get_king_mask(Board* board, Piece* piece) {
    U64 mask = 0ULL;
    int cell = piece->cell;
    if (cell / 8 > 0) {
        set_bit(mask, (cell - 8));
        if (cell % 8 < 7) set_bit(mask, (cell - 7));
        if (cell % 8 > 0) set_bit(mask, (cell - 9));
    }
    if (cell / 8 < 7) {
        set_bit(mask, (cell + 8));
        if (cell % 8 > 0) set_bit(mask, (cell + 7));
        if (cell % 8 < 7) set_bit(mask, (cell + 9));
    }
    if (cell % 8 > 0) set_bit(mask, (cell - 1));
    if (cell % 8 < 7) set_bit(mask, (cell + 1));

    if (piece->no_moves == 0 && board->castle_pieces[piece->c][king_side]->alive && board->castle_pieces[piece->c][king_side]->no_moves == 0) {
        set_bit(mask, board->castling_coordinates[piece->c][king_side]);
    }

    if (piece->no_moves == 0 && board->castle_pieces[piece->c][queen_side]->alive && board->castle_pieces[piece->c][queen_side]->no_moves == 0) {
        set_bit(mask, board->castling_coordinates[piece->c][queen_side]);
    }

    return mask;
}

U64 get_pawn_mask(Board* board, Piece* piece) {
    U64 mask = get_pawn_attack_mask(board, piece);
    mask |= get_pawn_move_mask(board, piece);
    return mask;
}

U64 get_pawn_attack_mask(Board* board, Piece* piece) {
    U64 attack_mask = 0ULL;
    int cell = piece->cell;
    Move* last_move = board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1];
    if (piece->c == white && cell / 8 > 0) {
        if (cell % 8 < 7 && get_bit(board->bitboard, (cell - 7))) attack_mask = (1ULL << (cell - 7)); // right
        if (cell % 8 > 0 && get_bit(board->bitboard, (cell - 9))) attack_mask |= (1ULL << (cell - 9)); // left

        // en passant
        if (cell % 8 > 0 && board->map[cell-1] && board->map[cell-1]->type == pawn && board->map[cell-1]->c == black && last_move && last_move->piece == board->map[cell-1] && last_move->from / 8 == 1  && board->map[cell-1]->alive) {
            attack_mask |= (1ULL << (cell - 9)); // left
        }
        if (cell % 8 < 7 && board->map[cell+1] && board->map[cell+1]->type == pawn && board->map[cell+1]->c == black && last_move && last_move->piece == board->map[cell+1] && last_move->from / 8 == 1 && board->map[cell+1]->alive) {
            attack_mask |= (1ULL << (cell - 7)); // right
        }
    }
    if (piece->c == black && cell / 8 < 7) {
        if (cell % 8 > 0 && get_bit(board->bitboard, (cell + 7))) attack_mask = (1ULL << (cell + 7)); // left
        if (cell % 8 < 7 && get_bit(board->bitboard, (cell + 9))) attack_mask |= (1ULL << (cell + 9)); // right

        // en passant
        if (cell % 8 > 0 && board->map[cell-1] && board->map[cell-1]->type == pawn && board->map[cell-1]->c == white && last_move && last_move->piece == board->map[cell-1] && last_move->from / 8 == 6 && board->map[cell-1]->alive) {
            attack_mask |= (1ULL << (cell + 7)); // left
        }
        if (cell % 8 < 7 && board->map[cell+1] && board->map[cell+1]->type == pawn && board->map[cell+1]->c == white && last_move && last_move->piece == board->map[cell+1] && last_move->from / 8 == 6 && board->map[cell+1]->alive) {
            attack_mask |= (1ULL << (cell + 9)); // right
        }
    }
    return attack_mask;
}

U64 get_pawn_move_mask(Board* board, Piece* piece) {
    U64 move_mask = 0ULL;
    int cell = piece->cell;
    if (piece->c == white && cell / 8 > 0 && !get_bit(board->bitboard, (cell-8))) {
        move_mask = 1ULL << (cell - 8);
        if (cell / 8 == 6 && !get_bit(board->bitboard, (cell-16))) {
            move_mask |= (1ULL << (cell - 16));
        }
    }
    if (piece->c == black && cell / 8 < 7 && !get_bit(board->bitboard, (cell+8))) {
        move_mask = 1ULL << (cell + 8);
        if (cell / 8 == 1 && !get_bit(board->bitboard, (cell+16))) {
            move_mask |= (1ULL << (cell + 16));
        }
    }
    return move_mask;
}



/* ------------------------------- */
/* -------- VISUALISATION -------- */
/* ------------------------------- */
void print_board(U64 bitboard) {
    printf("\n");
    for (int row = 0; row < 8; row++) {
        printf("  %i  ", 8 - row);
        for (int col = 0; col < 8; col++) {
            int cell = (row * 8) + col;

            printf(" %d", get_bit(bitboard, cell) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n      a b c d e f g h\n\n");
}

void print_board_pro(Board* board) {
    printf("\n");
    for (int row = 0; row < 8; row++) {
        // printf("      -----------------\n");
        printf("  %i  ", 8 - row);
        for (int col = 0; col < 8; col++) {
            int cell = (row * 8) + col;
            if (get_bit(board->bitboard, cell)) {
                wprintf(L" %lc", board->map[cell]->character);
            }
            else {
                printf(" .");
            }
        }
        printf("\n");
    }
    printf("\n      a b c d e f g h\n\n");
}

void print_pieces(Board* board) {
    for (int i = 0; i < 16; i++) {
        if (board->pieces[white][i]->alive) {
            char string[3] = {'\0'};
            square_to_string(board->pieces[white][i]->cell, string);
            printf("colour: %i; type: %i; cell: %i; square: %s\n", board->pieces[white][i]->c, board->pieces[white][i]->type, board->pieces[white][i]->cell, string);
        }
    }
    for (int i = 0; i < 16; i++) {
        if (board->pieces[black][i]->alive) {
            char string[3] = {'\0'};
            square_to_string(board->pieces[black][i]->cell, string);
            printf("colour: %i; type: %i; cell: %i; square: %s\n", board->pieces[black][i]->c, board->pieces[black][i]->type, board->pieces[black][i]->cell, string);
        }
    }
    printf("\n");

    for (int i = 0; i < 64; i++) {
        if (board->map[i] && board->map[i]->alive) {
            char string[3] = {'\0'};
            square_to_string(board->map[i]->cell, string);
            printf("colour: %i; type: %i; cell: %i; square: %s\n", board->map[i]->c, board->map[i]->type, board->map[i]->cell, string);
        }
    }
    printf("\n");
}

void print_piece(Piece* piece) {
    if (piece) {
        char string[3] = {'\0'};
        char col[10] = {'\0'};
        char type[10] = {'\0'};
        piece_to_string(piece->type, type);
        square_to_string(piece->cell, string);
        colour_to_string(piece->c, col);
        printf("Colour: %s, Type: %s, Alive: %i, square: %s, moved: %i\n", col, type, piece->alive, string, piece->no_moves);
    }
    else {
        printf("EMPTY\n");
    }
}

void append_piece_to_string(Piece* piece, char* string) {
    if (!piece) {
        return;
    }
    char square[3] = {'\0'};
    char col[10] = {'\0'};
    char type[10] = {'\0'};
    piece_to_string(piece->type, type);
    square_to_string(piece->cell, square);
    colour_to_string(piece->c, col);

    char whole[100] = {'\0'};
    strcat(whole, "Colour: ");
    strcat(whole, col);
    strcat(whole, ", Type: ");
    strcat(whole, type);
    strcat(whole, ", Alive: ");
    if (piece->alive) {
        strcat(whole, "true");
    }
    else {
        strcat(whole, "false");
    }
    strcat(whole, ", Square: ");
    strcat(whole, square);

    char moves[6] = {'\0'};
    sprintf(moves, "%i", piece->no_moves);
    strcat(whole, ", # Moves: ");
    strcat(whole, moves);
    strcat(whole, "\n");
    strcat(string, whole);
    // printf("Colour: %s, Type: %s, Alive: %i, square: %s, moved: %i\n", col, type, piece->alive, square, piece->no_moves);
}

void print_moves(Moves* moves) {
    for (int i = 0; i < moves->length; i++) {
        printf("move %i: ", i);
        print_move(moves->moves[i]);
    }
    printf("\n");
}

void colour_to_string(colour c, char* string) {
    if (c) {
        string[0] = 'B';
        string[1] = 'l';
        string[2] = 'a';
        string[3] = 'c';
        string[4] = 'k';
    }
    else {
        string[0] = 'W';
        string[1] = 'h';
        string[2] = 'i';
        string[3] = 't';
        string[4] = 'e';
    }
}

void name_to_string(name n, char* string) {
    switch (n) {
        case king:
            string[0] = 'k';
            string[1] = 'i';
            string[2] = 'n';
            string[3] = 'g';
            break;
        case queen:
            string[0] = 'q';
            string[1] = 'u';
            string[2] = 'e';
            string[3] = 'e';
            string[4] = 'n';
            break;
        case castle:
            string[0] = 'c';
            string[1] = 'a';
            string[2] = 's';
            string[3] = 't';
            string[4] = 'l';
            string[5] = 'e';
            break;
        case bishop:
            string[0] = 'b';
            string[1] = 'i';
            string[2] = 's';
            string[3] = 'h';
            string[4] = 'o';
            string[5] = 'p';
            break;
        case knight:
            string[0] = 'k';
            string[1] = 'n';
            string[2] = 'i';
            string[3] = 'g';
            string[4] = 'h';
            string[5] = 't';
            break;
        case pawn:
            string[0] = 'p';
            string[1] = 'a';
            string[2] = 'w';
            string[3] = 'n';
            break;
        default:
            break;
    }
}

void print_move(Move* move) {
    // printf("PRINTING SINGLE MOVE:\n");
    if (move == NULL) {
        return;
    }
    char from[3] = {'\0'};
    char to[3] = {'\0'};
    char promotion[10] = {'\0'};
    square_to_string(move->from, from);
    square_to_string(move->destination, to);
    name_to_string(move->promotion, promotion);
    char c[10] = {'\0'};
    colour_to_string(move->piece->c, c);
    printf("Colour: %s; Piece: %i; From: %s; To: %s; Eval: %i; Promotion: %s\n", c, move->piece->type, from, to, move->evaluation, promotion);
}

void square_to_string(square s, char* string) {
    int col = s % 8;
    int row = s / 8;
    string[0] = 'a' + col;
    string[1] = '8' - row;
}

void print_square(square s) {
    char square_string[3] = {'\0'};
    square_to_string(s, square_string);
    printf("%s", square_string);
}

void piece_to_string(name n, char* string) {
    if (n == king) {
        string[0] = 'k';
        string[1] = 'i';
        string[2] = 'n';
        string[3] = 'g';
    }
    if (n == queen) {
        string[0] = 'q';
        string[1] = 'u';
        string[2] = 'e';
        string[3] = 'e';
        string[4] = 'n';
    }
    if (n == castle) {
        string[0] = 'c';
        string[1] = 'a';
        string[2] = 's';
        string[3] = 't';
        string[4] = 'l';
        string[5] = 'e';
    }
    if (n == knight) {
        string[0] = 'k';
        string[1] = 'n';
        string[2] = 'i';
        string[3] = 'g';
        string[4] = 'h';
        string[5] = 't';
    }
    if (n == bishop) {
        string[0] = 'b';
        string[1] = 'i';
        string[2] = 's';
        string[3] = 'h';
        string[4] = 'o';
        string[5] = 'p';
    }
    if (n == pawn) {
        string[0] = 'p';
        string[1] = 'a';
        string[2] = 'w';
        string[3] = 'n';
    }
}


void print_scores(Scores* scores) {
    printf("EVALUATION: %i\n", scores->eval);
    print_moves(scores->moves);
}


/* ------------------------------- */
/* -------- MISCELLANEOUS -------- */
/* ------------------------------- */
U64 set_multiple_bits(U64 bitboard, int cells[], int length) {
    for (int i = 0; i < length; i++) {
        set_bit(bitboard, cells[i]);
    }
    return bitboard;
}

void clear_board(Board* board) {
    for (int i = 0; i < 16; i++) {
        board->pieces[white][i]->alive = false;
    }
    for (int i = 0; i < 16; i++) {
        board->pieces[black][i]->alive = false;
    }
    for (int i = 0; i < 64; i++) {
        board->map[i] = NULL;
    }
    board->bitboard = 0ULL;
}

Board* copy_board(Board* board) {
    Board* copy = calloc(1, sizeof(Board));
    copy->bitboard = board->bitboard;
    copy->hash_value = board->hash_value;
    copy->lm_length = board->lm_length;
    copy->last_moved[copy->lm_length == 0 ? 0 : copy->lm_length - 1] = board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1];
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 16; j++) {
            // printf("i: %i, j: %i, alive: %i\n", i, j, board->pieces[i][j]->alive);
            Piece* piece = calloc(1, sizeof(Piece));
            copy->pieces[i][j] = piece;
            copy->pieces[i][j]->alive = board->pieces[i][j]->alive;
            copy->pieces[i][j]->cell = board->pieces[i][j]->cell;
            copy->pieces[i][j]->type = board->pieces[i][j]->type;
            copy->pieces[i][j]->c = board->pieces[i][j]->c;
            copy->pieces[i][j]->value = board->pieces[i][j]->value;
            copy->pieces[i][j]->no_moves = board->pieces[i][j]->no_moves;
            copy->pieces[i][j]->move_func = board->pieces[i][j]->move_func;
            copy->pieces[i][j]->character = board->pieces[i][j]->character;
        }
    }
    for (int i = 0; i < 64; i++) {
        copy->map[i] = NULL;
        if (board->map[i]) {
            copy->map[i] = copy->pieces[invert_colour(board->map[i]->id / 16)][board->map[i]->id % 16];
        }
        
    }
    return copy;
}

bool compare_boards(Board* board1, Board* board2, char* location) {
    bool same = true;
    char msg[10000] = {'\0'};
    if (board1->bitboard != board2->bitboard) {
        strcat(msg, "DIFFERENCE DETECTED: bitboards\n");
        same = false;
    }
    if (board1->hash_value != board2->hash_value) {
        strcat(msg, "DIFFERENCE DETECTED: hash_value\n");
        same = false;
    }
    if (board1->lm_length != board2->lm_length) {
        strcat(msg, "DIFFERENCE DETECTED: lm_length\n");
        same = false;
    }
    if (board1->last_moved[board1->lm_length == 0 ? 0 : board1->lm_length - 1] != board2->last_moved[board2->lm_length == 0 ? 0 : board2->lm_length - 1]) {
        strcat(msg, "DIFFERENCE DETECTED: last_moved\n");
        same = false;
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 16; j++) {
            bool piece_same = true;
            if (board1->pieces[i][j]->alive != board2->pieces[i][j]->alive) {
                strcat(msg, "DIFFERENCE DETECTED PIECES: piece->alive\n");
                piece_same = false;
            }
            if (board1->pieces[i][j]->cell != board2->pieces[i][j]->cell) {
                strcat(msg, "DIFFERENCE DETECTED PIECES: piece->cell\n");
                piece_same = false;
            }
            if (board1->pieces[i][j]->type != board2->pieces[i][j]->type) {
                strcat(msg, "DIFFERENCE DETECTED PIECES: piece->type\n");
                piece_same = false;
            }
            if (board1->pieces[i][j]->c != board2->pieces[i][j]->c) {
                strcat(msg, "DIFFERENCE DETECTED PIECES: piece->c\n");
                piece_same = false;
            }
            if (board1->pieces[i][j]->value != board2->pieces[i][j]->value) {
                strcat(msg, "DIFFERENCE DETECTED PIECES: piece->value\n");
                piece_same = false;
            }
            if (board1->pieces[i][j]->no_moves != board2->pieces[i][j]->no_moves) {
                strcat(msg, "DIFFERENCE DETECTED PIECES: piece->no_moves\n");
                piece_same = false;
            }
            if (board1->pieces[i][j]->move_func != board2->pieces[i][j]->move_func) {
                strcat(msg, "DIFFERENCE DETECTED PIECES: piece->move_func\n");
                piece_same = false;
            }
            if (!piece_same) {
                append_piece_to_string(board1->pieces[i][j], msg);
                append_piece_to_string(board2->pieces[i][j], msg);
                same = false;
            }
        }
    }
    for (int i = 0; i < 64; i++) {
        bool map_same = true;
        if ((board1->map[i] && !board2->map[i]) || (!board1->map[i] && board2->map[i])) {
            strcat(msg, "DIFFERENCE DETECTED MAP: null vs non-null value\n");
            same = false;
            map_same = false;
        }
        else if (board1->map[i] && board2->map[i]) {
            bool piece_same = true;
            if (board1->map[i]->alive != board2->map[i]->alive) {
                strcat(msg, "DIFFERENCE DETECTED MAP: piece->move_func\n");
                piece_same = false;
            }
            if (board1->map[i]->cell != board2->map[i]->cell) {
                strcat(msg, "DIFFERENCE DETECTED MAP: piece->cell\n");
                piece_same = false;
            }
            if (board1->map[i]->type != board2->map[i]->type) {
                strcat(msg, "DIFFERENCE DETECTED MAP: piece->type\n");
                piece_same = false;
            }
            if (board1->map[i]->c != board2->map[i]->c) {
                strcat(msg, "DIFFERENCE DETECTED MAP: piece->c\n");
                piece_same = false;
            }
            if (board1->map[i]->value != board2->map[i]->value) {
                strcat(msg, "DIFFERENCE DETECTED MAP: piece->value\n");
                piece_same = false;
            }
            if (board1->map[i]->no_moves != board2->map[i]->no_moves) {
                strcat(msg, "DIFFERENCE DETECTED MAP: piece->no_moves\n");
                piece_same = false;
            }
            if (board1->map[i]->move_func != board2->map[i]->move_func) {
                strcat(msg, "DIFFERENCE DETECTED MAP: piece->move_func\n");
                piece_same = false;
            }
            if (!piece_same) {
                append_piece_to_string(board1->map[i], msg);
                append_piece_to_string(board2->map[i], msg);
                same = false;
                map_same = false;
            }
        }
        if (!map_same) {
            char sq[3] = {'\0'};
            square_to_string(i, sq);
            strcat(msg, "DIFFERENCE DETECTED MAP: at square ");
            strcat(msg, sq);
            strcat(msg, "\n");
            same = false;
        }
    }
    if (!same) {
        printf("FAILURE AT %s\n%s\n", location, msg);
        printf("BOARD 1:\n");
        print_board_pro(board1);
        printf("BOARD 2:\n");
        print_board_pro(board2);
    }
    return same;
}

void free_piece(Piece* piece) {
    free(piece);
}

void free_move(Move* move) {
    free_piece(move->piece);
    free(move);
}

void free_copy_board(Board* board) {
    for (int i = 0; i < MAX_COLOUR; i++) {
        for (int j = 0; j < CELLS; j++) {
            if (board->pieces[i][j]) {
                free_piece(board->pieces[i][j]);
            }
        }
    }

    // DO NOT free moves -> they are not deep copied to the board copy

    free(board);
}

void free_board(Board* board) {
    for (int i = 0; i < MAX_COLOUR; i++) {
        for (int j = 0; j < CELLS; j++) {
            if (board->pieces[i][j]) {
                free_piece(board->pieces[i][j]);
            }
        }
    }
    // all pieces in the Move* last_moved list will already be freed.
    // Therefore, just free the Move and not the contained Piece.
    for (int i = 0; i < board->lm_length; i++) {
        free(board->last_moved[i]);
    }
    free(board);
}

void free_scores(Scores* scores, int depth) {
    for (int i = scores->moves->length - 1; i >= depth; i--) {
        // do not free the piece as well
        free(scores->moves->moves[i]);
    }
    free(scores);
}
