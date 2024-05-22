#include "test.c"

int main(void) {
    initialise();

    test_forcing_moves_2();
    
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
    Tracer* tracer;
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
        grapher = init_grapher(2, 6, white);
        tracer = init_tracer(white);
        create_graph(grapher, grapher->start, board, white);
        moves = dfs(grapher->start, tracer);
        if (moves->length == 0) {
            printf("BLACK WINS\n");
            return;
        }
        pretend_move(board, moves->moves[0]->piece, moves->moves[0]->destination);

        print_board_pro(board);

        grapher = init_grapher(2, 6, black);
        tracer = init_tracer(black);
        create_graph(grapher, grapher->start, board, black);
        moves = dfs(grapher->start, tracer);
        pretend_move(board, moves->moves[0]->piece, moves->moves[0]->destination);
        if (moves->length == 0) {
            printf("WHITE WINS\n");
            return;
        }

        print_board_pro(board);
        length = moves->length;
    }
    

}



/* ------------------------------ */
/* -------- SEARCH GRAPH -------- */
/* ------------------------------ */



void update_high_low_eval(Tracer* tracer, int* high_low_eval, int* best_index, int index) {
    if (tracer->best_eval == *high_low_eval && rand() % 2) {
        *best_index = index;
    }
    else if (tracer->mover == tracer->original_mover) {
        if (tracer->best_eval > *high_low_eval) {
            *best_index = index;
            *high_low_eval = tracer->best_eval;
        }
    }
    else {
        if (tracer->best_eval < *high_low_eval) {
            *best_index = index;
            *high_low_eval = tracer->best_eval;
        }
    }
}

Moves* dfs(GraphNode* node, Tracer* tracer) {
    if (node->move != NULL) {
        tracer->best_eval += node->move->evaluation;
    }

    if (node->i == 0) {
        Moves* moves = calloc(1, sizeof(Moves));
        moves->length = tracer->depth;
        return moves;
    }

    int high_low_eval = 2 * MAX_SCORE;
    int best_index = 0;
    if (tracer->mover == tracer->original_mover) {
        high_low_eval =  -(MAX_SCORE);
    }
    Moves* selection[node->i];
    Moves* best_moves;
    int saved = tracer->best_eval;
    for (int i = 0; i < node->i; i++) {
        tracer->depth += 1;
        tracer->mover = invert_colour(tracer->mover);
        best_moves = dfs(node->children[i], tracer);
        selection[i] = best_moves;
        tracer->mover = invert_colour(tracer->mover);
        tracer->depth -= 1;
        update_high_low_eval(tracer, &high_low_eval, &best_index, i);
        tracer->best_eval = saved;
    }
    tracer->best_eval = high_low_eval;
    best_moves = selection[best_index];
    best_moves->moves[tracer->depth] = node->children[best_index]->move;
    if (tracer->depth == 0) {
        tracer->best = best_moves;
    }
    return best_moves;
}



/* ------------------------------ */
/* ------- EVALUATE MOVES ------- */
/* ------------------------------ */

void update_graph(GraphNode* parent, Move* move) {
    GraphNode* child = calloc(1, sizeof(GraphNode));
    child->move = move;
    parent->children[parent->i] = child;
    parent->i += 1;
}

int evaluate_no_moves(Grapher* grapher, GraphNode* parent, Board* board, colour mover) {
    if (!is_check(board, mover)) {
        return 0;
    }
    int score = MAX_SCORE - ((grapher->max_depth - grapher->depth) / 2);
    // printf("MATE DETECTED: %i\n", score);
    if (mover == grapher->start_player) {
        parent->move->evaluation -= score;
        return -score;
    }
    else {
        parent->move->evaluation += score;
        return score;
    }
}

int init_score(Grapher* grapher, colour mover) {
    if (grapher->start_player == mover) {
        return -(MAX_SCORE);
    }
    else {
        return MAX_SCORE;
    }
}

int init_breadth(Grapher* grapher, colour mover) {
    if (mover == grapher->start_player || grapher->depth == grapher->max_depth - 1) {
        return MAX_BREADTH;
    }
    return grapher->max_breadth;
}

void evaluate_killed(Move* move, Piece* killed, colour mover, Grapher* grapher) {
    if (mover == grapher->start_player) {
        move->evaluation += killed->value;
    }
    else {
        move->evaluation -= killed->value;
    }
}

void calculate_depth(Grapher* grapher, int running_score, colour mover) {
    if (mover != grapher->start_player) {
        running_score = -running_score;
    }
    if (MAX_SCORE - running_score < grapher->depth) {
        // printf("previous depth: %i, new depth %i, running_score: %i\n", grapher->depth, (MAX_SCORE - running_score), running_score);
        grapher->depth = MAX_SCORE - running_score;
        if (grapher->depth == 0) {
            grapher->depth = 1;
        }
    }
}

int calculate_score(Grapher* grapher, colour mover, int new_score, int running_score) {
    if (grapher->start_player == mover && new_score > running_score) {
        return new_score;
    }
    if (grapher->start_player != mover && new_score < running_score) {
        return new_score;
    }
    return running_score;
}


int take_train_heuristic(Board* board, GraphNode* node) {
    // just the previous move
    return 0;
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

int count_moves(Board* board, colour mover) {
    int count = 0;
    for (int i=0; i<16; i++) {
        if (board->pieces[mover][i]->alive) {
            U64 mask = board->pieces[mover][i]->move_func(board, board->pieces[mover][i]);
            count += count_bitboard(mask);
        }
    }
    return count;
}

int initiative_heuristic(Board* board, GraphNode* node, colour mover, Grapher* grapher) {
    // Do you care if it's legal or not at this point??
    int score = 0;
    if (mover == grapher->start_player) {
        score += count_moves(board, mover);
    }
    else {
        score -= count_moves(board, mover);
    }
    return score;
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

int evaluate_position(Board* board, GraphNode* node, colour mover, Grapher* grapher) {
    int score = 0;
    score += king_safety_heuristic(board, node, mover, grapher);
    score += initiative_heuristic(board, node, mover, grapher);   
    score += measure_points(board, mover);
    return score;    
}

void reorder_scores(Scores* scores, int start, int new_high, int new_index) {
    int saved;
    int saved_index;
    int i;
    for (i = start; i < scores->breadth; i++) {
        saved = scores->highest[start];
        saved_index = scores->indices[start];
        scores->highest[i] = new_high;
        scores->indices[i] = new_index;
        new_high = saved;
        new_index = saved_index;
    }
    if (i < scores->max_breadth) {
        scores->breadth += 1;
    }
}

void update_scores(Scores* scores, int score, int new_index) {
    int i;
    for (i = 0; i < scores->breadth; i++) {
        if (score > scores->highest[i]) {
            reorder_scores(scores, i, score, new_index);
            return;
        }
    }
    if (i < scores->max_breadth) {
        scores->indices[i] = new_index;
        scores->highest[i] = score;
        scores->breadth += 1;
    }
}

Scores* init_scores(int length) {
    Scores* scores = calloc(length, sizeof(Scores));
    scores->max_breadth = MAX_BRANCH;
    return scores;
}

int create_graph(Grapher* grapher, GraphNode* parent, Board* board, colour mover) {
    
    if (grapher->depth == grapher->base) {
        board->leaves += 1;
        return 0;
    }
    
    Moves* moves = get_all_moves_for_colour(board, mover);

    if (moves->length == 0) {
        return evaluate_no_moves(grapher, parent, board, mover);
    }

    // int breadth = init_breadth(grapher, mover);
    int running_score = init_score(grapher, mover);
    int saved_depth = grapher->depth;
    bool looked_ahead = false;

    // You need to make a hashing function...
    Scores* scores = init_scores(moves->length);

    // print_moves(moves);

    for (int i = 0; i < moves->length; i++) {
        Move* move = moves->moves[i];
        Piece* killed = pretend_move(board, move->piece, move->destination);
        int score = evaluate_position(board, parent->children[parent->i-1], mover, grapher);
        update_scores(scores, score, i);
        undo_pretend_move(board, move->piece, killed, move->from);
    }

    // print_scores(scores, moves);

    for (int i = 0; i < scores->breadth; i++) {
        Move* move = moves->moves[scores->indices[i]];
        Piece* killed = pretend_move(board, move->piece, move->destination);
        update_graph(parent, move);
        if (killed) {
            evaluate_killed(move, killed, mover, grapher);
        }
        grapher->depth -= 1;
        grapher->value_limit += 1;
        int new_score = create_graph(grapher, parent->children[parent->i-1], board, invert_colour(mover));
        grapher->value_limit -= 1;
        grapher->depth += 1;

        running_score = calculate_score(grapher, mover, new_score, running_score);
        calculate_depth(grapher, running_score, mover);
        looked_ahead = true;
        undo_pretend_move(board, move->piece, killed, move->from);
    }
    
    free(scores);
    if (!looked_ahead) {
        board->leaves += 1;
        return 0;
    }
    grapher->depth = saved_depth;
    return running_score;
}




/* -------------------------------- */
/* ------- SEARCH ALL MOVES ------- */
/* -------------------------------- */

void get_all_moves_for_piece(Board* board, Piece* piece, Moves* moves) {

    // print_piece(piece);
    U64 mask = piece->move_func(board, piece);
    // print_board(mask);
    // printf("\n");
    for (int i = 0; i < CELLS; i++) {
        if (mask % 2 == 1) {
            // bit found on the bit board
            if (is_move_legal(board, piece, i)) {
                bool allowed = true;
                if (piece->no_moves == 0 && piece->type == king) {
                    if ((i == g1 || i == g8 || i == c1 || i == c8) && is_check(board, piece->c)) {
                        allowed = false;
                    }
                    if ((i == g1 || i == g8) && (!is_move_legal(board, piece, piece->cell + 1))) {
                        allowed = false;
                    }
                    if ((i == c1 || i == c8) && (!is_move_legal(board, piece, piece->cell - 1))) {
                        allowed = false;
                    }
                }
                if (allowed) {
                    moves->moves[moves->length] = calloc(1, sizeof(Move));
                    moves->moves[moves->length]->piece = piece;
                    moves->moves[moves->length]->destination = i;
                    moves->moves[moves->length]->from = piece->cell;
                    moves->length += 1;
                }
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

void check_castle(Board* board, Piece* piece, square to, square from) {
    // CASTLE
    if (piece->no_moves == 0 && piece->type == king && ((to == g1 && from == e1) || (to == g8 && from == e8))) {
        execute_move(board, board->pieces[piece->c][CASTLE_2(piece->c)], board->pieces[piece->c][CASTLE_2(piece->c)]->cell - 2);
    }
    else if (piece->no_moves == 0 && piece->type == king && ((to == c1 && from == e1) || (to == c8 && from == e8))) {
        execute_move(board, board->pieces[piece->c][CASTLE_1(piece->c)], board->pieces[piece->c][CASTLE_1(piece->c)]->cell + 3);
    }

    // REVERSE
    else if (piece->type == king && ((to == e1 && from == g1) || (to == e8 && from == g8))) {
        execute_move(board, board->pieces[piece->c][CASTLE_2(piece->c)], board->pieces[piece->c][CASTLE_2(piece->c)]->cell + 2);
        board->pieces[piece->c][CASTLE_2(piece->c)]->no_moves = 0;
    }
    else if (piece->type == king && ((to == e1 && from == c1) || (to == e8 && from == c8))) {
        execute_move(board, board->pieces[piece->c][CASTLE_1(piece->c)], board->pieces[piece->c][CASTLE_1(piece->c)]->cell - 3);
        board->pieces[piece->c][CASTLE_1(piece->c)]->no_moves = 0;
    }
}

void execute_move(Board* board, Piece* piece, square to) {
    pop_bit(board->bitboard, piece->cell);
    set_bit(board->bitboard, to);
    board->map[piece->cell] = NULL;
    square from = piece->cell;
    piece->cell = to;
    if (board->map[to]) {
        board->map[to]->alive = false;
    }
    board->map[to] = piece;
    board->last_moved = piece;

    check_castle(board, piece, to, from);
}

Piece* pretend_move(Board* board, Piece* piece, square to) {
    board->counter += 1;    
    pop_bit(board->bitboard, piece->cell);
    set_bit(board->bitboard, to);
    board->map[piece->cell] = NULL;
    square from = piece->cell;
    piece->cell = to;
    Piece* killed = board->map[to];
    if (killed) {
        killed->alive = false;
    }
    board->map[to] = piece;
    board->last_moved = piece;
    check_castle(board, piece, to, from);
    piece->no_moves += 1;
    return killed;
}

void undo_pretend_move(Board* board, Piece* original, Piece* killed, square original_from) {
    execute_move(board, original, original_from);
    if (killed) {
        killed->alive = true;
        execute_move(board, killed, killed->cell);
    }
    original->no_moves -= 1;
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
    Piece* killed = pretend_move(board, piece, to);
    bool legal = true;
    if (killed && killed->c == piece->c) {
        legal = false;
    }
    else if (is_check(board, piece->c)) {
        legal = false;
    }
    undo_pretend_move(board, piece, killed, from);
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

/* -------------------------------- */
/* -------- INITIALISATION -------- */
/* -------------------------------- */

Board* init_board(void) {
    Board* board = calloc(1, sizeof(Board));
    board->bitboard = 0ULL;
    int cells[32] = {a8, b8, c8, d8, e8, f8, g8, h8, a7, b7, c7, d7, e7, f7, g7, h7, a2, b2, c2, d2, e2, f2, g2, h2, a1, b1, c1, d1, e1, f1, g1, h1};
    board->bitboard = set_multiple_bits(board->bitboard, cells, 32);

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
    Moves* track = calloc(1, sizeof(Moves));
    Moves* best = calloc(1, sizeof(Moves));
    tracer->tracer = track;
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
            char square_string[5] = {'\0'};
            char colour_string[10] = {'\0'};
            char piece_string[10] = {'\0'};
            colour_to_string(c, colour_string);
            square_to_string(sq, square_string);
            piece_to_string(p, piece_string);
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

    Piece* castle1 = board->pieces[piece->c][CASTLE_1(piece->c)];
    Piece* castle2 = board->pieces[piece->c][CASTLE_2(piece->c)];

    if (piece->no_moves == 0 && castle1->no_moves == 0 && castle1->alive) {
        if (piece->c == black && get_bit(board->bitboard, d8) == 0 && get_bit(board->bitboard, c8) == 0 && get_bit(board->bitboard, b8) == 0) {
            set_bit(mask, c8);
        }
        if (piece->c == white && get_bit(board->bitboard, d1) == 0 && get_bit(board->bitboard, c1) == 0 && get_bit(board->bitboard, b1) == 0) {
            set_bit(mask, c1);
        }
    }
    // printf("outside\n");
    // printf("%i, %i, %i", piece->no_moves, castle2->no_moves, castle2->alive);
    if (piece->no_moves == 0 && castle2->no_moves == 0 && castle2->alive) {
        // printf("maybe...\n");
        if (piece->c == black && get_bit(board->bitboard, f8) == 0 && get_bit(board->bitboard, g8) == 0) {
            set_bit(mask, g8);
        }
        if (piece->c == white && get_bit(board->bitboard, f1) == 0 && get_bit(board->bitboard, g1) == 0) {
            // printf("wow!\n");
            set_bit(mask, g1);
        }
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
    if (piece->c == white && cell / 8 > 0) {
        if (cell % 8 < 7 && get_bit(board->bitboard, (cell - 7))) attack_mask = (1ULL << (cell - 7)); // right
        if (cell % 8 > 0 && get_bit(board->bitboard, (cell - 9))) attack_mask |= (1ULL << (cell - 9)); // left

        // en passant
        if (cell % 8 < 7 && get_bit(board->bitboard, (cell - 1)) && board->map[cell-1] &&  board->map[cell-1]->type == pawn && board->map[cell-1]->c == black && board->last_moved == board->map[cell-1] && board->map[cell-1]->alive) {
            attack_mask |= (1ULL << (cell - 1)); // right
        }
        
        if (cell % 8 > 0 && get_bit(board->bitboard, (cell + 1)) && board->map[cell+1] && board->map[cell+1]->type == pawn && board->map[cell+1]->c == black && board->last_moved == board->map[cell+1] && board->map[cell+1]->alive) {
            attack_mask |= (1ULL << (cell + 1)); // left
        }
        

    }
    if (piece->c == black && cell / 8 < 7) {
        if (cell % 8 > 0 && get_bit(board->bitboard, (cell + 7))) attack_mask = (1ULL << (cell + 7)); // left
        if (cell % 8 < 7 && get_bit(board->bitboard, (cell + 9))) attack_mask |= (1ULL << (cell + 9)); // right

        // en passant
        if (cell % 8 > 0 && get_bit(board->bitboard, (cell - 1)) && board->map[cell-1] && board->map[cell-1]->type == pawn && board->map[cell-1]->c == white && board->last_moved == board->map[cell-1] && board->map[cell-1]->alive) {
            attack_mask |= (1ULL << (cell - 1)); // left
        }
        
        if (cell % 8 < 7 && get_bit(board->bitboard, (cell + 1)) && board->map[cell+1] && board->map[cell+1]->type == pawn && board->map[cell+1]->c == white && board->last_moved == board->map[cell+1] && board->map[cell+1]->alive) {
            attack_mask |= (1ULL << (cell + 1)); // right
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
    printf("PRINTING MOVES: length %i\n", moves->length);
    int i = 0;
    // for (int i = 0; i < moves->length; i++) {
    while (moves->moves[i]) {
        char from[3] = {'\0'};
        char to[3] = {'\0'};
        square_to_string(moves->moves[i]->from, from);
        square_to_string(moves->moves[i]->destination, to);
        printf("[MOVE %i] Colour: %i; Type: %i; From: %s; To: %s; Eval: %i\n", i, moves->moves[i]->piece->c, moves->moves[i]->piece->type, from, to, moves->moves[i]->evaluation);
        i += 1;
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

void print_move(Move* move) {
    // printf("PRINTING SINGLE MOVE:\n");
    if (move == NULL) {
        return;
    }
    char from[3] = {'\0'};
    char to[3] = {'\0'};
    square_to_string(move->from, from);
    square_to_string(move->destination, to);
    char c[10] = {'\0'};
    colour_to_string(move->piece->c, c);
    printf("Colour: %s; Piece: %i; From: %s; To: %s; Eval: %i\n", c, move->piece->type, from, to, move->evaluation);
}

void square_to_string(square s, char* string) {
    int col = s % 8;
    int row = s / 8;
    string[0] = 'a' + col;
    string[1] = '8' - row;
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

void print_scores(Scores* scores, Moves* moves) {
    for (int i = 0; i < scores->breadth; i++) {
        print_move(moves->moves[scores->indices[i]]);
        printf("score: %i\n", scores->highest[i]);
    }
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

