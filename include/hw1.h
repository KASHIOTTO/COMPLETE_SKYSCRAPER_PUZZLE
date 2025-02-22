#ifndef __HW1_H
#define __HW1_H

#include <stdio.h>
#include <stdbool.h>

#define INFO(...) do {fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0)
#define ERROR(...) do {fprintf(stderr, "[          ] [ ERR  ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0)

#define MAX_LENGTH 8
#define MAX_SEQUENCE_CAP  40320

//Required:
int initialize_board(const char *initial_state, const char *keys, int size);
int solve(const char *initial_state, const char *keys, int size);

//Part 1
bool invalid_board(void);
void print_board(void);
bool check_initial_keys(void);
bool check_dupes_col(int col);
bool check_dupes_row(int row);
int count_buildings(int *view, int size);
int count_visible(int *line, int size);
bool check_keys(void);
bool winning_board(void);
bool check_rowcol_keys(int r, int c);
bool check_complete_row(int i);
bool check_complete_col(int j);
void start_game(void);

//Part 2
int all_possible(int n);
int count_bits(int poss);
int first_candidate(int poss);
void initial_possibilities(int size);
void edge_clue_initialization(int size);
void apply_edge_constraint_rule(int size);
void apply_constraint_propagation(int size);
void apply_process_of_elimination(int size);

//Part 3
int generate_valid__row_sequences(int row, int valid_seq[][MAX_LENGTH], int size);
void generate_valid_row_sequences_helper(int row, int col, int used, int current[], int size, int valid_seq[][MAX_LENGTH], int *count);
int generate_valid_col_sequences(int col, int valid_seq[][MAX_LENGTH], int size);
void generate_valid_col_sequences_helper(int col, int row, int used, int current[], int size, int valid_seq[][MAX_LENGTH], int *count);
void sequence_filtration_rows(int size);
void sequence_filtration_cols(int size);

#endif // HW1_H
