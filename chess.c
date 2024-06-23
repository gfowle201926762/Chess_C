#include "test.c"

int main(void) {
    initialise();

    // test_pawn_en_passant_3();
    // test_pawn_en_passant_6();
    test();

    // test_18_june_2024();

    // test_forcing_moves_1();
    // test_dont_be_stupid();
    // test_forcing_moves_2();
    // test_forcing_moves_3();
    // test_puzzle_fork();
    // test_puzzle_win_queen();
    // test_forcing_moves();
    
    // play_game();

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

Scores* evaluate_no_moves(Grapher* grapher, GraphNode* parent, Board* board, colour mover) {
    if (!is_check(board, mover)) {
        // STALEMATE
        parent->move->evaluation = 0;
        return init_scores(parent, reverse_depth(grapher));
    }
    parent->move->evaluation = MAX_SCORE - ((grapher->max_depth - grapher->depth) / 2);
    return init_scores(parent, reverse_depth(grapher));
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

int king_safety_heuristic(Board* board, GraphNode* node, colour mover, Grapher* grapher) {
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

void free_scores(Scores* scores) {
    // TODO
}

void update_graph(GraphNode* parent, Move* move) {
    GraphNode* child = calloc(1, sizeof(GraphNode));
    child->move = move;
    parent->children[parent->i] = child;
    parent->i += 1;
}

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

Scores* init_scores(GraphNode* node, int depth) {
    Scores* scores = calloc(1, sizeof(Scores));
    scores->moves = calloc(1, sizeof(Moves));
    scores->eval = node->move->evaluation;
    scores->moves->moves[depth - 1] = node->move;
    scores->moves->length = depth;
    return scores;
}

Moves* get_best_moves(Board* board, Moves* moves, colour mover, int max_breadth) {
    for (int i = 0; i < moves->length; i++) {
        Move* move = moves->moves[i];
        Piece* killed = pretend_move(board, move);
        move->evaluation = evaluate_position(board, mover);
        undo_pretend_move(board, move, killed);
    }

    return get_best_scores(moves, max_breadth);
}

bool init_original_mover(GraphNode* node, Grapher* grapher) {
    if (node->move) {
        return node->move->piece->c != grapher->start_player;
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

Scores* create_graph(Grapher* grapher, GraphNode* parent, Board* board, colour mover, int prune) {
    
    if (grapher->depth == grapher->base) {
        board->leaves += 1;
        parent->move->evaluation = evaluate_position(board, mover);
        return init_scores(parent, reverse_depth(grapher));
    }
    
    Moves* moves = get_all_moves_for_colour(board, mover);
    if (moves->length == 0) {
        return evaluate_no_moves(grapher, parent, board, mover);
    }

    Moves* best_moves = get_best_moves(board, moves, mover, grapher->max_breadth);
    Scores* best_scores;
    bool original_mover = init_original_mover(parent, grapher);
    int limit = init_limit(original_mover);

    for (int i = 0; i < best_moves->length; i++) {
        Move* move = best_moves->moves[i];

        name save_type = move->piece->type;
        int save_value = move->piece->value;
        U64 (*save_move_func)(Board*, Piece*) = move->piece->move_func;
        U64 hash = board->hash_value;
        Piece* last_moved = board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1];
        int lm = board->lm_length;
        U64 test_board = board->bitboard;
        
        Piece* killed = pretend_move(board, move);
        // board->last_moved = last_moved;
        hash_move_piece(board, move, killed);
        hash_change_colour(board);
        if (move->piece->no_moves == 0 && move->piece->type == king && ((move->destination == g1 && move->from == e1) || (move->destination == g8 && move->from == e8))) {
            hash_castle(board, mover, king_side);
        }
        else if (move->piece->no_moves == 0 && move->piece->type == king && ((move->destination == c1 && move->from == e1) || (move->destination == c8 && move->from == e8))) {
            hash_castle(board, mover, queen_side);
        }

        // board->last_moved = move->piece;

        update_graph(parent, move);

        // if (grapher->depth == grapher->max_depth) {
        //     // printf("\ndepth: %i\n", grapher->depth);
        //     print_move(move);
        // }

        grapher->depth -= 1;
        Scores* scores = create_graph(grapher, parent->children[parent->i-1], board, invert_colour(mover), limit);
        grapher->depth += 1;

        hash_move_piece(board, move, killed);
        hash_change_colour(board);
        // REVERSE
        undo_pretend_move(board, move, killed);
        if (move->piece->no_moves == 0 && move->piece->type == king && ((move->destination == g1 && move->from == e1) || (move->destination == g8 && move->from == e8))) {
            hash_castle(board, mover, king_side);
        }
        else if (move->piece->no_moves == 0 && move->piece->type == king && ((move->destination == c1 && move->from == e1) || (move->destination == c8 && move->from == e8))) {
            hash_castle(board, mover, queen_side);
        }

        assert(test_board == board->bitboard);
        assert(move->piece->type == save_type);
        assert(move->piece->value == save_value);
        assert(move->piece->move_func == save_move_func);
        assert(board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1] == last_moved);
        assert(lm == board->lm_length);
        if (hash != board->hash_value) {
            print_board_pro(board);
            print_move(move);
            printf("\n");
            assert(false);
        }
        assert(hash == board->hash_value);

        if ((original_mover && scores->eval > limit) || (!original_mover && scores->eval < limit)) {
            limit = scores->eval;
            best_scores = scores;
        }
        else {
            free_scores(scores);
        }
        if (prune != MAX_SCORE && prune != -MAX_SCORE && ((original_mover && scores->eval > prune) || (!original_mover && scores->eval < prune))) {
            // printf("prune: %i, scores->eval: %i, original_mover? %i\n", prune, scores->eval, original_mover);
            break;
        }
    }
    if (parent->move != NULL) {
        best_scores->moves->moves[reverse_depth(grapher) - 1] = parent->move;
    }
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
                    print_piece(piece);
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

Piece* handle_en_passant(Board* board, Move* move) {
    // must be a pawn which is attacking
    if (move->piece->type != pawn || move->from % 8 == move->destination % 8) {
        return NULL;
    }
    // must be moving into an empty square -- guaranteed en passant.
    if (board->map[move->destination] == NULL) {
        Piece* piece = board->map[move->piece->c == white ? move->destination + 8 : move->destination - 8];
        piece->alive = false;
        board->map[move->piece->c == white ? move->destination + 8 : move->destination - 8] = NULL;
        pop_bit(board->bitboard, (move->piece->c == white ? move->destination + 8 : move->destination - 8));
        return piece;
    }
    return NULL;
}

Piece* pretend_move(Board* board, Move* move) {
    // A move has already been generated. It is guaranteed to be legal. Execute the move.
    // Must handle castling and en passant.
    add_last_moved(board, move->piece);
    move->piece->no_moves += 1;
    if (move->castle) {
        move_single_piece(board, move->piece, move->destination, none);
        move_single_piece(board, board->castle_pieces[move->piece->c][move->castle_side], board->to_castle_coords[move->piece->c][move->castle_side], none);
        return NULL;
    }
    Piece* killed = handle_en_passant(board, move);
    if (killed) {
        move_single_piece(board, move->piece, move->destination, move->promotion);
        return killed;
    }
    return move_single_piece(board, move->piece, move->destination, move->promotion);
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

void add_last_moved(Board* board, Piece* piece) {
    board->last_moved[board->lm_length] = piece;
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
    square from = piece->cell;
    Piece* killed = move_single_piece(board, piece, to, none);
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
        move_single_piece(board, killed, to, none);
    }
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
        return CASTLE_1(c);
    }
    return CASTLE_2(c);
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

// Not so easy to implement with history dependent information...
// Or perhaps even easier than you thought?

Transposition* get(HashTable* hashtable, U64 hash_value) {
    return hashtable->transpositions[hash_value % HASH_TABLE_SIZE];
}

void put(HashTable* hashtable, U64 hash_value, int eval, int depth) {
    int index = hash_value % HASH_TABLE_SIZE;
    if (hashtable->transpositions[index] && hashtable->transpositions[index]->depth < depth) {
        // keep (assuming a shallower depth is more valuable)
        return;
    }
    if (hashtable->transpositions[index] == NULL) {
        Transposition* transposition = calloc(1, sizeof(Transposition));
        hashtable->transpositions[index] = transposition;
    }
    hashtable->transpositions[index]->hash_value = hash_value;
    hashtable->transpositions[index]->eval = eval;
    hashtable->transpositions[index]->depth = depth;
}

void hash_move_piece(Board* board, Move* move, Piece* killed) {
    board->hash_value ^= board->keys_position[move->piece->c][move->promotion == none ? move->piece->type : pawn][move->from];
    if (killed) {
        board->hash_value ^= board->keys_position[killed->c][killed->type][move->destination];
    }
    board->hash_value ^= board->keys_position[move->piece->c][move->promotion == none ? move->piece->type : move->promotion][move->destination];
    if (board->lm_length) {
        board->hash_value ^= board->keys_last_moved[board->last_moved[board->lm_length - 1]->cell];
    }
    board->hash_value ^= board->keys_last_moved[move->destination];
}

void hash_change_colour(Board* board) {
    board->hash_value ^= board->key_mover;
}

void hash_castle(Board* board, colour mover, castle_type type) {
    board->hash_value ^= board->keys_castling[mover][type];
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

    board->castle_pieces[white][king_side] = board->pieces[white][CASTLE_2(white)];
    board->castling_coordinates[white][king_side] = g1;
    board->to_castle_coords[white][king_side] = f1;
    board->from_castle_coords[white][king_side] = h1;

    board->castle_pieces[white][queen_side] = board->pieces[white][CASTLE_1(white)];
    board->castling_coordinates[white][queen_side] = c1;
    board->to_castle_coords[white][queen_side] = d1;
    board->from_castle_coords[white][queen_side] = a1;

    board->castle_pieces[black][king_side] = board->pieces[black][CASTLE_2(black)];
    board->castling_coordinates[black][king_side] = g8;
    board->to_castle_coords[black][king_side] = f8;
    board->from_castle_coords[black][king_side] = h8;

    board->castle_pieces[black][queen_side] = board->pieces[black][CASTLE_1(black)];
    board->castling_coordinates[black][queen_side] = c8;
    board->to_castle_coords[black][queen_side] = d8;
    board->from_castle_coords[black][queen_side] = a8;
    return board;
}

Grapher* init_grapher(int breadth, int depth, colour start_player) {
    Grapher* grapher = calloc(1, sizeof(Grapher));
    GraphNode* node = calloc(1, sizeof(GraphNode));
    grapher->start = node;
    grapher->max_breadth = breadth;
    grapher->max_depth = depth * 2;
    grapher->depth = depth * 2;
    grapher->start_player = start_player;
    return grapher;
}

Tracer* init_tracer(colour mover) {
    Tracer* tracer = calloc(1, sizeof(Tracer));
    Moves* best = calloc(1, sizeof(Moves));
    tracer->best = best;
    tracer->original_mover = mover;
    tracer->mover = mover;
    return tracer;
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


    // Piece* castle1 = board->pieces[piece->c][CASTLE_1(piece->c)];
    // Piece* castle2 = board->pieces[piece->c][CASTLE_2(piece->c)];

    // if (piece->no_moves == 0 && castle1->no_moves == 0 && castle1->alive) {
    //     if (piece->c == black && get_bit(board->bitboard, d8) == 0 && get_bit(board->bitboard, c8) == 0 && get_bit(board->bitboard, b8) == 0) {
    //         set_bit(mask, c8);
    //     }
    //     if (piece->c == white && get_bit(board->bitboard, d1) == 0 && get_bit(board->bitboard, c1) == 0 && get_bit(board->bitboard, b1) == 0) {
    //         set_bit(mask, c1);
    //     }
    // }
    // if (piece->no_moves == 0 && castle2->no_moves == 0 && castle2->alive) {
    //     if (piece->c == black && get_bit(board->bitboard, f8) == 0 && get_bit(board->bitboard, g8) == 0) {
    //         set_bit(mask, g8);
    //     }
    //     if (piece->c == white && get_bit(board->bitboard, f1) == 0 && get_bit(board->bitboard, g1) == 0) {
    //         set_bit(mask, g1);
    //     }
    // }

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
    if (piece->c == white && cell / 8 > 0) {
        if (cell % 8 < 7 && get_bit(board->bitboard, (cell - 7))) attack_mask = (1ULL << (cell - 7)); // right
        if (cell % 8 > 0 && get_bit(board->bitboard, (cell - 9))) attack_mask |= (1ULL << (cell - 9)); // left

        // en passant
        if (cell % 8 > 0 && board->map[cell-1] &&  board->map[cell-1]->type == pawn && board->map[cell-1]->c == black && board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1] == board->map[cell-1] && board->map[cell-1]->alive) {
            attack_mask |= (1ULL << (cell - 9)); // left
            // printf("EN PASSANT 1\n");
        }
        if (cell % 8 < 7 && board->map[cell+1] && board->map[cell+1]->type == pawn && board->map[cell+1]->c == black && board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1] == board->map[cell+1] && board->map[cell+1]->alive) {
            attack_mask |= (1ULL << (cell - 7)); // right
            // printf("EN PASSANT 2\n");
        }

        // en passant
        // if (cell % 8 < 7 && get_bit(board->bitboard, (cell - 1)) && board->map[cell-1] &&  board->map[cell-1]->type == pawn && board->map[cell-1]->c == black && board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1] == board->map[cell-1] && board->map[cell-1]->alive) {
        //     attack_mask |= (1ULL << (cell - 1)); // right
        // }
        
        // if (cell % 8 > 0 && get_bit(board->bitboard, (cell + 1)) && board->map[cell+1] && board->map[cell+1]->type == pawn && board->map[cell+1]->c == black && board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1] == board->map[cell+1] && board->map[cell+1]->alive) {
        //     attack_mask |= (1ULL << (cell + 1)); // left
        // }
    }
    if (piece->c == black && cell / 8 < 7) {
        if (cell % 8 > 0 && get_bit(board->bitboard, (cell + 7))) attack_mask = (1ULL << (cell + 7)); // left
        if (cell % 8 < 7 && get_bit(board->bitboard, (cell + 9))) attack_mask |= (1ULL << (cell + 9)); // right

        // en passant
        if (cell % 8 > 0 && board->map[cell-1] && board->map[cell-1]->type == pawn && board->map[cell-1]->c == white && board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1] == board->map[cell-1] && board->map[cell-1]->alive) {
            attack_mask |= (1ULL << (cell + 7)); // left
            // printf("EN PASSANT 3\n");
        }
        
        if (cell % 8 < 7 && board->map[cell+1] && board->map[cell+1]->type == pawn && board->map[cell+1]->c == white && board->last_moved[board->lm_length == 0 ? 0 : board->lm_length - 1] == board->map[cell+1] && board->map[cell+1]->alive) {
            attack_mask |= (1ULL << (cell + 9)); // right
            // printf("EN PASSANT 4\n");
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
        square_to_string(piece->cell, string);
        printf("Colour: %i, Type: %i, Alive: %i, square: %s, moved: %i\n", piece->c, piece->type, piece->alive, string, piece->no_moves);
    }
    else {
        printf("EMPTY\n");
    }
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

void print_tracer(Tracer* tracer) {
    printf("EVALUATION: %i\n", tracer->best_eval);
    print_moves(tracer->best);
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
    Board* copy = init_board();
    copy->bitboard = board->bitboard;
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 16; j++) {
            // printf("i: %i, j: %i, alive: %i\n", i, j, board->pieces[i][j]->alive);
            copy->pieces[i][j]->alive = board->pieces[i][j]->alive;
            copy->pieces[i][j]->cell = board->pieces[i][j]->cell;
            copy->pieces[i][j]->type = board->pieces[i][j]->type;
            copy->pieces[i][j]->c = board->pieces[i][j]->c;
            copy->pieces[i][j]->value = board->pieces[i][j]->value;
               
        }
    }
    for (int i = 0; i < 64; i++) {
        copy->map[i] = NULL;
        if (board->map[i]) {
            int id = board->map[i]->id;
            copy->map[i] = copy->pieces[1-(id / 16)][id % 16];
        }
    }
    return copy;
}

bool compare_boards(Board* board1, Board* board2) {
    if (board1->bitboard != board2->bitboard) {
        return false;
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 16; j++) {
            if (board1->pieces[i][j]->alive != board2->pieces[i][j]->alive) {
                assert(false);
            }
            if (board1->pieces[i][j]->cell != board2->pieces[i][j]->cell) {
                assert(false);
            }
            if (board1->pieces[i][j]->type != board2->pieces[i][j]->type) {
                assert(false);
            }
            if (board1->pieces[i][j]->c != board2->pieces[i][j]->c) {
                assert(false);
            }
            if (board1->pieces[i][j]->value != board2->pieces[i][j]->value) {
                assert(false);
            }
        }
    }
    for (int i = 0; i < 64; i++) {
        if (board1->map[i] && board2->map[i]) {
            if (board1->map[i]->alive != board2->map[i]->alive) {
                char string[3] = {'\0'};
                square_to_string(i, string);
                printf("square: %s, board alive: %i, copied board alive: %i\n", string, board1->map[i]->alive, board2->map[i]->alive);
                assert(false);
            }
            if (board1->map[i]->cell != board2->map[i]->cell) {
                assert(false);
            }
            if (board1->map[i]->type != board2->map[i]->type) {
                assert(false);
            }
            if (board1->map[i]->c != board2->map[i]->c) {
                assert(false);
            }
            if (board1->map[i]->value != board2->map[i]->value) {
                assert(false);
            }
        }
        if (board1->map[i] == NULL && board2->map[i] != NULL) {
            assert(false);
        }
        if (board2->map[i] == NULL && board1->map[i] != NULL) {
            assert(false);
        }
    }
    return true;
}

