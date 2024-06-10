#include "chess.h"


int main(void) {
    initialise();

    test(); // all other test here are experimental and not contained within this one.

    // test_forcing_moves();
    // test_forcing_moves_1();
    // test_forcing_moves_2();
    // test_forcing_moves_3();
    // test_puzzle_fork();
    // test_puzzle_win_queen();
    // test_take_train();
    // test_board_notation();

    

    
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


void take_train_heuristic(Board* board, GraphNode* node) {
    // just the previous move

}

void evaluate_position(Board* board, GraphNode* node) {
    // heuristics based
    // king safety
    // controlling squares
    // initiative -- make threatening moves
    // take chain (heuristic, not actual)
    
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

    int breadth = init_breadth(grapher, mover);
    int running_score = init_score(grapher, mover);
    int saved_depth = grapher->depth;
    bool looked_ahead = false;

    for (int i = 0; i < moves->length; i++) {
        
        Move* move = moves->moves[i];
        bool taken = false;
        if (board->map[move->destination]) {
            taken = true;
        }

        // (board->map[move->destination] && board->map[move->destination]->value > grapher->value_limit) || 
        if (moves->length <= breadth) {
            update_graph(parent, move);
            Piece* killed = pretend_move(board, move->piece, move->destination);
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
        
    }
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
                if (piece->no_moves == 0 && piece->type == king && (i == g1 || i == g8 || i == c1 || i == c8)) {
                     if (is_check(board, piece->c)) {
                        allowed = false;
                     }
                }
                if (piece->no_moves == 0 && piece->type == king && (i == g1 || i == g8)) {
                     if (!is_move_legal(board, piece, piece->cell + 1)) {
                        allowed = false;
                     }
                }
                if (piece->no_moves == 0 && piece->type == king && (i == c1 || i == c8)) {
                     if (!is_move_legal(board, piece, piece->cell - 1)) {
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
                piece->value = 5;
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
                piece->value = 3;
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
                piece->value = 3;
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
                piece->value = 9;
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
                piece->value = 64;
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
            piece->value = 1;
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



/* ----------------------------- */
/* ---------- TESTING ---------- */
/* ----------------------------- */

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
    Grapher* grapher = init_grapher(2, 3, white);
    int score = create_graph(grapher, grapher->start, board, white);
    assert(test_board == board->bitboard);
    Tracer* tracer = init_tracer(white);
    dfs(grapher->start, tracer);

    assert(score == 0);
    assert(tracer->best_eval == 5);
    assert(tracer->best->length == 3);
    assert(tracer->best->moves[0]->from == e3);
    assert(tracer->best->moves[0]->destination == e8);
    assert(tracer->best->moves[0]->piece->c == white);
    assert(tracer->best->moves[0]->piece->type == queen);

}

void test_forcing_moves_2() {
    Board* board = set_board_notation("bkg6 bcd6 wqe3 ");
    print_board_pro(board);

    U64 test_board = board->bitboard;
    Grapher* grapher = init_grapher(2, 3, white);
    int score = create_graph(grapher, grapher->start, board, white);
    assert(test_board == board->bitboard);
    Tracer* tracer = init_tracer(white);
    dfs(grapher->start, tracer);

    assert(score == 0);
    assert(tracer->best_eval == 5);
    assert(tracer->best->length == 3);
    assert(tracer->best->moves[0]->from == e3);
    assert(tracer->best->moves[0]->destination == g3);
    assert(tracer->best->moves[0]->piece->c == white);
    assert(tracer->best->moves[0]->piece->type == queen);

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
    assert_evals_match_best_eval(tracer);
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
    assert_evals_match_best_eval(tracer);

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
    assert_evals_match_best_eval(tracer);
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
    assert_evals_match_best_eval(tracer);
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
    assert_evals_match_best_eval(tracer);
}

void assert_evals_match_best_eval(Tracer* tracer) {
    int evals = 0;
    for (int i = 0; i < tracer->best->length; i++) {
        evals += tracer->best->moves[i]->evaluation;
    }
    assert(evals == tracer->best_eval);
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
    assert_evals_match_best_eval(tracer);
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




