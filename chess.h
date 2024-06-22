#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>

#define CELLS 64
#define MAX_COLOUR 2
#define MAX_PIECE_TYPE 7
#define MAX_ACTUAL_PIECE_TYPE 6
#define MAX_CASTLING_OPTIONS 2
#define HASH_TABLE_SIZE 1000000000
#define MAX_PROMOTABLE_PIECES 4
#define MAX_REPETITIONS 3
#define U64 unsigned long long
#define MOVES_SIZE 100
#define MAX_SCORE 100000
#define SAVED_SIZE 100
#define MAX_BREADTH 10000
#define MAX_BRANCH 10

#define KING_VALUE 1000
#define QUEEN_VALUE 900
#define CASTLE_VALUE 500
#define BISHOP_VALUE 300
#define KNIGHT_VALUE 300
#define PAWN_VALUE 100

int WHITE_KINGS = 1;
int WHITE_QUEENS = 1;
int WHITE_CASTLES = 2;
int WHITE_BISHOPS = 2;
int WHITE_KNIGHTS = 2;
int WHITE_PAWNS = 8;
int BLACK_KINGS = 1;
int BLACK_QUEENS = 1;
int BLACK_CASTLES = 2;
int BLACK_BISHOPS = 2;
int BLACK_KNIGHTS = 2;
int BLACK_PAWNS = 8;

// bitboard macros
#define get_bit(bitboard, cell) (bitboard & (1ULL << cell))
#define set_bit(bitboard, cell) (bitboard |= (1ULL << cell))
#define pop_bit(bitboard, cell) (get_bit(bitboard, cell) ? (bitboard ^= (1ULL << cell)) : 0)
#define invert_colour(c) (1 - c)
#define KING_INDEX(c) (c == black ? 4 : 12)
#define CASTLE_1(c) (c == black ? 0 : 8)
#define CASTLE_2(c) (c == black ? 7 : 15)
#define KNIGHT_1(c) (c == black ? 1 : 9)
#define KNIGHT_2(c) (c == black ? 6 : 14)
#define BISHOP_1(c) (c == black ? 2 : 10)
#define BISHOP_2(c) (c == black ? 5 : 13)
#define QUEEN_INDEX(c) (c == black ? 3 : 11)
#define PAWN_INDEX(c, n) (c == black ? (8 + n) : (n))

/*
how to flip an array:
Just invert the row number...
*/

int KING_EVAL[64] = {
  -25, -25, -25, -25, -25, -25, -25, -25,
  -25, -25, -25, -25, -25, -25, -25, -25,
  -25, -25, -25, -25, -25, -25, -25, -25,
  -25, -25, -25, -25, -25, -25, -25, -25,
  -25, -25, -25, -25, -25, -25, -25, -25,
  -25, -25, -25, -25, -25, -25, -25, -25,
  -25, -25, -25, -25, -25, -25, -25, -25,
  5, 15, 10, -15, -15, -15, 15, 5
};

int PAWN_EVAL[64] = {
  20, 20, 20, 20, 20, 20, 20, 20,
  10, 10, 10, 10, 10, 10, 10, 10,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 15, 15, 0, 0, 0,
  0, 0, 0, 10, 10, 0, 0, 0,
  0, 0, 0, 5, 5, 0, 0, 0,
  0, 0, 0, -25, -25, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};



enum square {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
};
typedef enum square square;

enum file {
    a, b, c, d, e, f, g, h
};
typedef enum file file;

enum colour {
    white, black
};
typedef enum colour colour;

// none name is just for promotions so you don't have to pass NULL in
enum name {
    none, king, queen, castle, bishop, knight, pawn
};
typedef enum name name;

enum castle_type {
    king_side, queen_side
};
typedef enum castle_type castle_type;

struct Board;

struct Piece {
    int id;
    int cell;
    name type;
    colour c;
    int value;
    U64 (*move_func)(struct Board*, struct Piece*);
    int (*index_func)(colour c, int i);
    bool alive;
    int no_moves;
    wchar_t character;
};
typedef struct Piece Piece;

struct Transposition {
    U64 hash_value;
    int eval;
    int depth;
};
typedef struct Transposition Transposition;

struct HashTable {
    // index is the hashtable's key (hash_value % HASH_TABLE_SIZE)
    // the value is the U64 hash (and eval) -> check whether actually match
    Transposition* transpositions[HASH_TABLE_SIZE];
};
typedef struct HashTable HashTable;

struct Board {
    U64 bitboard;
    U64 hash_value;
    U64 keys_position[MAX_COLOUR][MAX_PIECE_TYPE][CELLS];
    U64 keys_castling[MAX_COLOUR][MAX_CASTLING_OPTIONS];
    U64 keys_last_moved[CELLS]; // just need a last moved position... allows for any number of pawns.
    U64 keys_repetitions[MAX_REPETITIONS];
    U64 key_mover;
    U64 last_positions[MOVES_SIZE];
    Piece* map[CELLS];
    Piece* pieces[MAX_COLOUR][CELLS];
    Piece* last_moved[MOVES_SIZE];
    int lm_length;
    int max_pieces[MAX_COLOUR];
    name promotable_pieces[MAX_PROMOTABLE_PIECES];
    name valid_pieces[MAX_ACTUAL_PIECE_TYPE];
    int counter;
    int leaves;
};
typedef struct Board Board;

struct Move {
    Piece* piece;
    square destination;
    square from;
    int evaluation;
    name promotion;
};
typedef struct Move Move;

struct Moves {
    Move* moves[MOVES_SIZE];
    int length;
};
typedef struct Moves Moves;


struct Tracer {
    Moves* best;

    int best_eval;
    colour original_mover;
    colour mover;
    int depth;
};
typedef struct Tracer Tracer;

struct GraphNode {
    struct GraphNode* children[MOVES_SIZE];
    Move* move;
    int i;
};
typedef struct GraphNode GraphNode;

struct Grapher {
    GraphNode* start;
    int depth;
    int max_depth;
    int max_breadth;
    int base;
    double value_limit;
    colour start_player;
};
typedef struct Grapher Grapher;

struct Scores {
    Moves* moves;
    int eval;
};
typedef struct Scores Scores;




U64 set_multiple_bits(U64 bitboard, int cells[], int length);

// Piece movement functions
U64 get_pawn_attack_mask(Board* board, Piece* piece);
U64 get_pawn_move_mask(Board* board, Piece* piece);
U64 get_pawn_mask(Board* board, Piece* piece);
U64 get_king_mask(Board* board, Piece* piece);
U64 get_knight_mask(Board* board, Piece* piece);
U64 get_castle_mask(Board* board, Piece* piece);
U64 get_bishop_mask(Board* board, Piece* piece);
U64 get_queen_mask(Board* board, Piece* piece);

// Testing
void test();
void test_moves();
void test_legality();
void test_pawn_attacks();
void test_pawn_moves();
void test_king_moves();
void test_knight_moves();
void test_castle_moves();
void test_bishop_moves();
void test_queen_moves();
void test_check_1();
void test_check_2();
void test_get_all_moves_1();
void test_get_all_moves_2();
void test_move_selection();
void test_detect_mate_1();
void test_detect_mate_2();
void test_mate_in_four();
void test_move_logic();
void test_move_logic_mate_in_four();
void test_moves_board_setup_1();
void test_moves_board_setup_2();
bool compare_boards(Board* board1, Board* board2);
Board* copy_board(Board* board);
Board* board_setup_m4_2();
void test_mate_detection();
void test_mate_in_four_2();
void test_mate_in_four_3();
void test_castling_1();
void test_castling_2();
Board* forcing_move_setup();
void test_forcing_moves();
void test_board_notation();
void test_puzzle_win_queen();
void test_take_train();
void test_puzzle_fork();
void test_forcing_moves_1();
void test_forcing_moves_2();
void test_forcing_moves_3();

// Initialisation
Board* init_board(void);
void set_board(Board* board);
Board* set_board_notation(char* s);
Scores* init_scores(GraphNode* node, int depth);
int init_limit(bool original_mover);

// Miscellaneous
void clear_board(Board* board);
Board* board_setup_1();


// Visualisation
void print_board(U64 bitboard);
void print_board_pro(Board* board);
void print_pieces(Board* board);
void print_moves(Moves* moves);
void square_to_string(square s, char* string);
void print_move(Move* move);
void print_piece(Piece* piece);
void print_tracer(Tracer* tracer);
void colour_to_string(colour c, char* string);
void piece_to_string(name n, char* string);
void print_scores(Scores* scores);
void print_square(square s);

// Move legal logic
void execute_move(Board* board, Piece* piece, square to, name promotion);
Piece* pretend_move(Board* board, Piece* piece, square to, name promotion);
// Piece* pretend_move(Board* board, Move* move);
bool is_check(Board* board, colour c);
bool is_move_legal(Board* board, Piece* piece, square to);
void undo_pretend_move(Board* board, Piece* original, Piece* killed, square original_from, name promotion);
// void undo_pretend_move(Board* board, Move* move, Piece* killed);
void hash_move_piece(Board* board, Move* move, Piece* killed);
void hash_change_colour(Board* board);
void hash_castle(Board* board, colour mover, castle_type type);

// Get all moves
void get_all_moves_for_piece(Board* board, Piece* piece, Moves* moves);
Moves* get_all_moves_for_colour(Board* board, colour c);

// Evaluate moves
int reverse_depth(Grapher* grapher);
Scores* create_graph(Grapher* grapher, GraphNode* parent, Board* board, colour mover, int prune);
// int create_graph(Grapher* grapher, GraphNode* parent, Board* board, colour mover);
Grapher* init_grapher(int breadth, int depth, colour start_player);


void play_game();
void initialise();