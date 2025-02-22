#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "hw1.h"

int top_key[MAX_LENGTH] = {0};
int bottom_key[MAX_LENGTH] = {0};
int left_key[MAX_LENGTH] = {0};
int right_key[MAX_LENGTH] = {0};

char board[MAX_LENGTH][MAX_LENGTH] = {0};
int constraints[MAX_LENGTH][MAX_LENGTH] = {0};

int length = 5; //dims

bool invalid_board(void);
bool check_initial_keys(void);
bool check_dupes_row(int row);
bool check_dupes_col(int col);
bool check_keys(void);
bool check_complete_row(int i);
bool check_complete_col(int j);
int count_buildings(int *view, int size);
void print_board(void);
bool winning_board(void);
void start_game(void);
int count_visible(int *line, int size);
bool check_rowcol_keys(int r, int c);
int all_possible(int n);
int count_bits(int poss);
int first_candidate(int poss);
void initial_possibilities(int size);
void edge_clue_initialization(int size);
void apply_edge_constraint_rule(int size); //
void apply_constraint_propagation(int size); //
void apply_process_of_elimination(int size); //
int generate_valid__row_sequences(int row, int valid_seq[][MAX_LENGTH], int size);
void generate_valid_row_sequences_helper(int row, int col, int used, int current[], int size, int valid_seq[][MAX_LENGTH], int *count);
int generate_valid_col_sequences(int col, int valid_seq[][MAX_LENGTH], int size);
void generate_valid_col_sequences_helper(int col, int row, int used, int current[], int size, int valid_seq[][MAX_LENGTH], int *count);
void sequence_filtration_rows(int size); //
void sequence_filtration_cols(int size); //

int initialize_board(const char *initial_state, const char *keys, int size) {
    length = size;

    for (int i = 0; i < size; i++) {
        top_key[i]    = keys[i] - '0';
        bottom_key[i] = keys[size + i] - '0';
        left_key[i]   = keys[2 * size + i] - '0';
        right_key[i]  = keys[3 * size + i] - '0';
    }
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            char piece = initial_state[i * size + j];
            if (piece >= '1' && piece <= ('0' + size)){
                board[i][j] = piece;
            } else {
                board[i][j] = '-';
            }
        }
    }

    if (invalid_board()) {
        printf("Invalid initial board state.\n");
        return 0;
    }
    return 1;
}

bool invalid_board(void) {
    for (int i = 0; i < length; i++){
        if (check_dupes_row(i) || check_dupes_col(i) || !check_initial_keys()){
            return true;
        }
    }
    return false;
}

bool check_initial_keys(void) {
    int i, j;
    // Check top clues
    for (j = 0; j < length; j++){
        // if 1 then N
        if (top_key[j] == 1 && board[0][j] != '-' && board[0][j] != ('0' + length)){
            return false;
        }
        // if N then 1 thr n
        if (top_key[j] == length){
            for (i = 0; i < length; i++){
                if(board[i][j] != ('0' + i + 1) && board[i][j] != '-'){
                    return false;
                }
            }
        }
    }
    // Check bottom clues
    for (j = 0; j < length; j++){
        // if 1 then N
        if (bottom_key[j] == 1 && board[length - 1][j] != '-' && board[length - 1][j] != ('0' + length)){
            return false;
        }
        // if N then N thr 1
        if (bottom_key[j] == length){
            for (i = 0; i < length; i++){
                if(board[i][j] != ('0' + (length - i)) && board[i][j] != '-'){
                    return false;
                }
            }
        }
    }
    // Check left clues
    for (i = 0; i < length; i++){
        if (left_key[i] == 1 && board[i][0] != '-' && board[i][0] != ('0' + length)){
            return false;
        }
        // if N then 1 thr N
        if (left_key[i] == length){
            for (j = 0; j < length; j++){
                if(board[i][j] != ('0' + j + 1) && board[i][j] != '-'){
                    return false;
                }
            }
        }
    }
    // Check right clues
    for (i = 0; i < length; i++){
        if (right_key[i] == 1 && board[i][length - 1] != '-' && board[i][length - 1] != ('0' + length)){
            return false;
        }
        // if N then N thr 1
        if (right_key[i] == length){
            for (j = 0; j < length; j++){
                if(board[i][j] != ('0' + (length - j)) && board[i][j] != '-'){
                    return false;
                }
            }
        }
    }
    // filled row/col check keys
    for(i = 0; i < length; i++){
        if(!check_complete_row(i)){
            return false;
        }
    }
    for(j = 0; j < length; j++){
        if(!check_complete_col(j)){
            return false;
        }
    }
    return true;
}

bool check_complete_row(int i) {
    bool complete = true;
    for(int j = 0; j < length; j++){
        if(board[i][j] == '-'){
            complete = false;
            break;
        }
    }
    if(complete){
        int lclue = left_key[i];
        int rclue = right_key[i];
        int row_values[length];
        int rev_row[length];
        for(int j = 0; j < length; j++){
            row_values[j] = board[i][j] - '0';
        }
        for(int j = 0; j < length; j++){
            rev_row[j] = row_values[length - j - 1];
        }
        // chek
        if((lclue != 0 && count_buildings(row_values, length) != lclue) ||
           (rclue != 0 && count_buildings(rev_row, length) != rclue)){
            return false;
        }
    }
    return true;
}

bool check_complete_col(int j) {
    bool complete = true;
    for(int i = 0; i < length; i++){
        if(board[i][j] == '-'){
            complete = false;
            break;
        }
    }
    if(complete){
        int tclue = top_key[j];
        int bclue = bottom_key[j];
        int col_values[length];
        int rev_col[length];
        for(int i = 0; i < length; i++){
            col_values[i] = board[i][j] - '0';
        }
        for(int i = 0; i < length; i++){
            rev_col[i] = col_values[length - i - 1];
        }
        if((tclue != 0 && count_buildings(col_values, length) != tclue) ||
           (bclue != 0 && count_buildings(rev_col, length) != bclue)){
            return false;
        }
    }
    return true;
}

bool check_dupes_row(int row) {
    int seen[MAX_LENGTH] = {0};
    for (int col = 0; col < length; col++) {
        if (board[row][col] != '-') {
            int value = board[row][col] - '0';
            if (seen[value - 1] > 0) {
                return true;
            }
            seen[value - 1]++;
        }
    }
    return false;
}

bool check_dupes_col(int col) {
    int seen[MAX_LENGTH] = {0};
    for (int row = 0; row < length; row++){
        if (board[row][col] != '-'){
            int value = board[row][col] - '0';
            if (seen[value - 1] > 0){
                return true;
            }
            seen[value - 1]++;
        }
    }
    return false;
}

void print_board(void) {
    int i, j;

    printf("   ");
    for (i = 0; i < length; i++){
        printf(" %d", top_key[i]);
    }
    printf("\n");

    printf("   ");
    for (i = 0; i < length; i++){
        printf(" v");
    }
    printf("\n");

    for (i = 0; i < length; i++){
        printf("%d > ", left_key[i]);
        for (j = 0; j < length; j++){
            printf("%c ", board[i][j]);
        }
        printf("< %d\n", right_key[i]);
    }

    printf("   ");
    for (i = 0; i < length; i++){
        printf(" ^");
    }
    printf("\n");

    printf("   ");
    for (i = 0; i < length; i++){
        printf(" %d", bottom_key[i]);
    }
    printf("\n");
}

int count_buildings(int *view, int size){
    int count = 0;
    int tallest = 0;
    for (int i = 0; i < size; i++){
        if (view[i] > tallest){
            count++;
            tallest = view[i];
        }
    }
    return count;
}

bool check_keys() {
    //winning only**
    for (int i = 0; i < length; i++){
        int row_values[length];
        int rev_row[length];
        for (int j = 0; j < length; j++){
            if (board[i][j] != '-'){
                row_values[j] = board[i][j] - '0';
            } else {
                row_values[j] = 0;//bad
            }
        }
        for (int j = 0; j < length; j++){
            rev_row[j] = row_values[length - j - 1];
        }
        if (left_key[i] > 0 && count_buildings(row_values, length) != left_key[i]){
            return false;
        }
        if (right_key[i] > 0 && count_buildings(rev_row, length) != right_key[i]){
            return false;
        }
    }
    //col
    for (int i = 0; i < length; i++){
        int col_values[length];
        int rev_col[length];
        for (int j = 0; j < length; j++){
            if (board[j][i] != '-'){
                col_values[j] = board[j][i] - '0';
            } else {
                col_values[j] = 0;
            }
        }
        for (int j = 0; j < length; j++){
            rev_col[j] = col_values[length - j - 1];
        }
        if (top_key[i] > 0 && count_buildings(col_values, length) != top_key[i]){
            return false;
        }
        if (bottom_key[i] > 0 && count_buildings(rev_col, length) != bottom_key[i]){
            return false;
        }
    }
    return true;
}

bool winning_board(void) {
    for (int i = 0; i < length; i++){
        for (int j = 0; j < length; j++){
            if (board[i][j] == '-'){
                return false;
            }
        }
    }
    return check_keys();
}

bool check_rowcol_keys(int r, int c) {
    //4 after placement only
    bool row_complete = true;
    int row_values[length];
    for (int j = 0; j < length; j++) {
        if (board[r][j] == '-'){
            row_complete = false;
            break;
        } else {
            row_values[j] = board[r][j] - '0';
        }
    }
    if (row_complete) {
        if (left_key[r] > 0) {
            if (count_buildings(row_values, length) != left_key[r])
                return false;
        }
        if (right_key[r] > 0) {
            int rev_row[length];
            for (int j = 0; j < length; j++){
                rev_row[j] = row_values[length - j - 1];
            }
            if (count_buildings(rev_row, length) != right_key[r])
                return false;
        }
    }
    bool col_complete = true;
    int col_values[length];
    for (int i = 0; i < length; i++) {
        if (board[i][c] == '-'){
            col_complete = false;
            break;
        } else {
            col_values[i] = board[i][c] - '0';
        }
    }
    if (col_complete){
        if (top_key[c] > 0){
            if (count_buildings(col_values, length) != top_key[c]){
                return false;
            }    
        }
        if (bottom_key[c] > 0){
            int rev_col[length];
            for (int i = 0; i < length; i++){
                rev_col[i] = col_values[length - i - 1];
            }
            if (count_buildings(rev_col, length) != bottom_key[c]){
                return false;
            }
        }
    }
    return true;
}

void start_game(void) {
    char piece;
    int row, col;
    //print
    //piece
    //row, col
    //1G
    //1H
    //check
    while (true){
        print_board();

        printf("Choose a piece (1-%d) or q to quit: ", length);
        scanf(" %c", &piece);
        while ((piece < '1' || piece > ('0' + length)) && piece != 'q') {
            printf("Invalid choice. Choose a piece (1-%d) or q to quit: ", length);
            scanf(" %c", &piece);
        }
        if (piece == 'q'){
            break;
        }

        printf("Choose a row (0-%d): ", length - 1);
        scanf(" %d", &row);
        while (row < 0 || row >= length){
            printf("Invalid choice. Choose a row (0-%d): ", length - 1);
            scanf(" %d", &row);
        }

        printf("Choose a column (0-%d): ", length - 1);
        scanf(" %d", &col);
        while (col < 0 || col >= length){
            printf("Invalid choice. Choose a column (0-%d): ", length - 1);
            scanf(" %d", &col);
        }

        if (board[row][col] != '-') {
            printf("Invalid choice. That space is already occupied.\n");
            continue;
        }
        board[row][col] = piece;

        if (check_dupes_row(row) || check_dupes_col(col)){
            printf("Invalid choice. There is already a building with that height in that row or column.\n");
            board[row][col] = '-';
            continue;
        }
        
        if (!check_rowcol_keys(row, col)){
            printf("Invalid choice. You violate one of the key requirements.\n");
            board[row][col] = '-';
            continue;
        }

        if (winning_board()){
            printf("Congratulations, you have filled the board!\n");
            print_board();
            break;
        }
    }
}

// ******************** PART 2********************

int all_possible(int n) {
    return ((1 << (n + 1)) - (1 << 1));
}

int count_bits(int poss) {
    int count = 0;
    while (poss) {
        count += poss & 1;
        poss >>= 1;
    }
    return count;
}

int first_candidate(int poss) {
    for (int i = 1; i <= length; i++) {
        if (poss & (1 << i)){
            return i;
        }
    }
    return -1;
}

void initial_possibilities(int size) {
     for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (board[i][j] != '-'){
                int d = board[i][j] - '0';
                constraints[i][j] = 1 << d;
            } 
            else{
                constraints[i][j] = all_possible(size);
            }
        }
    }
}

void edge_clue_initialization(int size) {
    //t 
     for (int j = 0; j < size; j++){
        if (top_key[j] == 1 && board[0][j] == '-'){
            board[0][j] = '0' + size;
            constraints[0][j] = 1 << size;
        }
        if (top_key[j] == size){
            for (int i = 0; i < size; i++){
                board[i][j] = '0' + (i + 1);
                constraints[i][j] = 1 << (i + 1);
            }
        }
    }
    //b
    for (int j = 0; j < size; j++){
        if (bottom_key[j] == 1 && board[size - 1][j] == '-') {
            board[size - 1][j] = '0' + size;
            constraints[size - 1][j] = 1 << size;
        }
        if (bottom_key[j] == size){
            for (int i = 0; i < size; i++){
                board[size - 1 - i][j] = '0' + (i + 1);
                constraints[size - 1 - i][j] = 1 << (i + 1);
            }
        }
    }
    //l
    for (int i = 0; i < size; i++){
        if (left_key[i] == 1 && board[i][0] == '-'){
            board[i][0] = '0' + size;
            constraints[i][0] = 1 << size;
        }
        if (left_key[i] == size){
            for (int j = 0; j < size; j++){
                board[i][j] = '0' + (j + 1);
                constraints[i][j] = 1 << (j + 1);
            }
        }
    }
    // r
    for (int i = 0; i < size; i++){
        if (right_key[i] == 1 && board[i][size - 1] == '-'){
            board[i][size - 1] = '0' + size;
            constraints[i][size - 1] = 1 << size;
        }
        if (right_key[i] == size){
            for (int j = 0; j < size; j++){
                board[i][size - 1 - j] = '0' + (j + 1);
                constraints[i][size - 1 - j] = 1 << (j + 1);
            }
        }
    }
}

void apply_edge_constraint_rule(int size) {
    // N - c + 2 + d
    int candidate, bit, i, j, d;
    for (j = 0; j < size; j++){
        int c = top_key[j];
        if (c > 1 && c < size){
            for (i = 0; i < size; i++){
                // i is distance from top
                d = i;
                for (candidate = size - c + 2 + d; candidate <= size; candidate++){
                    bit = 1 << candidate;
                    if (constraints[i][j] & bit){
                        constraints[i][j] &= ~bit; // remove  possibility
                    }
                }
            }
        }
    }
    for (j = 0; j < size; j++){
        int c = bottom_key[j];
        if (c > 1 && c < size){
            for (i = 0; i < size; i++){
                d = (size - 1 - i);
                for (candidate = size - c + 2 + d; candidate <= size; candidate++){
                    bit = 1 << candidate;
                    if (constraints[i][j] & bit){
                        constraints[i][j] &= ~bit;
                    }
                }
            }
        }
    }
    for (i = 0; i < size; i++){
        int c = left_key[i];
        if (c > 1 && c < size){
            for (j = 0; j < size; j++){
                d = j;
                for (candidate = size - c + 2 + d; candidate <= size; candidate++){
                    bit = 1 << candidate;
                    if (constraints[i][j] & bit){
                        constraints[i][j] &= ~bit;
                    }
                }
            }
        }
    }
    for (i = 0; i < size; i++){
        int c = right_key[i];
        if (c > 1 && c < size){
            for (j = 0; j < size; j++){
                d = (size - 1 - j);
                for (candidate = size - c + 2 + d; candidate <= size; candidate++){
                    bit = 1 << candidate;
                    if (constraints[i][j] & bit){
                        constraints[i][j] &= ~bit;
                    }
                }
            }
        }
    }
}

void apply_constraint_propagation(int size) {
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (count_bits(constraints[i][j]) == 1) {
                int fixed = constraints[i][j];
                int lastPiece = first_candidate(fixed);
                if(board[i][j] == '-'){// nedd to assign
                    board[i][j] = lastPiece + '0';//check order***
                }
                //do ro w
                for (int k = 0; k < size; k++){
                    if (k != j && (constraints[i][k] & fixed)){
                        constraints[i][k] &= ~fixed;
                    }
                }
                // do cols
                for (int k = 0; k < size; k++){
                    if (k != i && (constraints[k][j] & fixed)){
                        constraints[k][j] &= ~fixed;
                    }
                }
            }
        }
    }
}

void apply_process_of_elimination(int size) {
    int k;
    //k row
    for (int i = 0; i < size; i++){
        for (k = 1; k <= size; k++){
            int count = 0, col_index = -1;
            for (int j = 0; j < size; j++){
                if (constraints[i][j] & (1 << k)){
                    count++;
                    col_index = j;
                }
            }
            if (count == 1 && count_bits(constraints[i][col_index]) > 1){
                constraints[i][col_index] = 1 << k;
                board[i][col_index] = '0' + k;
            }
        }
    }
    //k col
    for (int j = 0; j < size; j++){
        for (k = 1; k <= size; k++){
            int count = 0, row_index = -1;
            for (int i = 0; i < size; i++){
                if (constraints[i][j] & (1 << k)){
                    count++;
                    row_index = i;
                }
            }
            if (count == 1 && count_bits(constraints[row_index][j]) > 1){
                constraints[row_index][j] = 1 << k;
                board[row_index][j] = '0' + k;
            }
        }
    }
}

// ******************** PART 3********************
void generate_valid_row_sequences_helper(int row, int col, int used, int current[], int size, int valid_seq[][MAX_LENGTH], int *count) {
    if (col == size){
        if (left_key[row] != 0) {
            int visible = count_buildings(current, size);
            if (visible != left_key[row]){
                return;
            }
        }
        if (right_key[row] != 0){
            int rev[MAX_LENGTH];
            for (int j = 0; j < size; j++){
                rev[j] = current[size - j - 1];
            }
            if (count_buildings(rev, size) != right_key[row]){
                return;
            }
        }
        for (int j = 0; j < size; j++){
            valid_seq[*count][j] = current[j];
        }
        (*count)++;
        return;
    }
    if (board[row][col] != '-'){
        int k = board[row][col] - '0';
        if (used & (1 << k)){
            return;
        }
        if (!(constraints[row][col] & (1 << k))){
            return;
        }
        current[col] = k;
        generate_valid_row_sequences_helper(row, col+1, used | (1 << k), current, size, valid_seq, count);
    } 
    else{
        for (int k = 1; k <= size; k++){
            if (!(constraints[row][col] & (1 << k))){
                continue;
            }
            if (used & (1 << k)){
                continue;
            }
            current[col] = k;
            generate_valid_row_sequences_helper(row, col+1, used | (1 << k), current, size, valid_seq, count);
        }
    }
}

int generate_valid__row_sequences(int row, int valid_seq[][MAX_LENGTH], int size) {
    int count = 0;
    int current[MAX_LENGTH];
    generate_valid_row_sequences_helper(row, 0, 0, current, size, valid_seq, &count);
    return count;
}

void generate_valid_col_sequences_helper(int col, int row, int used, int current[], int size, int valid_seq[][MAX_LENGTH], int *count) {
    if (row == size){
        if (top_key[col] != 0){
            int visible = count_buildings(current, size);
            if (visible != top_key[col]){
                return;
            }
        }
        if (bottom_key[col] != 0){
            int rev[MAX_LENGTH];
            for (int i = 0; i < size; i++){
                rev[i] = current[size - i - 1];
            }
            if (count_buildings(rev, size) != bottom_key[col]){
                return;
            }
        }
        for (int i = 0; i < size; i++){
            valid_seq[*count][i] = current[i];
        }
        (*count)++;
        return;
    }
    if (board[row][col] != '-'){
        int k = board[row][col] - '0';
        if (used & (1 << k)){
            return;
        }
        if (!(constraints[row][col] & (1 << k))){
            return;
        }
        current[row] = k;
        generate_valid_col_sequences_helper(col, row+1, used | (1 << k), current, size, valid_seq, count);
    } 
    else{
        for (int k = 1; k <= size; k++){
            if (!(constraints[row][col] & (1 << k))){
                continue;
            }
            if (used & (1 << k)){
                continue;
            }
            current[row] = k;
            generate_valid_col_sequences_helper(col, row+1, used | (1 << k), current, size, valid_seq, count);
        }
    }
}

int generate_valid_col_sequences(int col, int valid_seq[][MAX_LENGTH], int size) {
    int count = 0;
    int current[MAX_LENGTH];
    generate_valid_col_sequences_helper(col, 0, 0, current, size, valid_seq, &count);
    return count;
}

void sequence_filtration_rows(int size) {
    int valid_seq[MAX_SEQUENCE_CAP][MAX_LENGTH];

    for (int i = 0; i < size; i++){
        bool complete = true;
        for (int j = 0; j < size; j++){
            if (board[i][j] == '-'){
                complete = false;
                break;
            }
        }
        if (complete){
            continue;
        }
        int count = generate_valid__row_sequences(i, valid_seq, size);
        if (count == 0){
            continue;
        }
        for (int j = 0; j < size; j++){
            int unionPoss = 0;
            for (int l = 0; l < count; l++){
                int digit = valid_seq[l][j];
                unionPoss |= (1 << digit);
            }
            int old = constraints[i][j];
            int newPoss = constraints[i][j] & unionPoss;
            if (newPoss != old){
                constraints[i][j] = newPoss;
                if (count_bits(newPoss) == 1 && board[i][j] == '-'){
                    board[i][j] = '0' + first_candidate(newPoss);
                }
            }
        }
    }
}

void sequence_filtration_cols(int size) {
    int valid_seq[MAX_SEQUENCE_CAP][MAX_LENGTH];

    for (int j = 0; j < size; j++){
        bool complete = true;
        for (int i = 0; i < size; i++){
            if (board[i][j] == '-'){
                complete = false;
                break;
            }
        }
        if (complete){
            continue;
        }
        int count = generate_valid_col_sequences(j, valid_seq, size);
        if (count == 0){
            continue;
        }
        for (int i = 0; i < size; i++){
            int unionPoss = 0;
            for (int l = 0; l < count; l++){
                int digit = valid_seq[l][i];
                unionPoss |= (1 << digit);//u or 1 l d
            }
            int old = constraints[i][j];
            int newPoss = constraints[i][j] & unionPoss;
            if (newPoss != old){
                constraints[i][j] = newPoss;
                if (count_bits(newPoss) == 1 && board[i][j] == '-'){
                    board[i][j] = '0' + first_candidate(newPoss);
                }
            }
        }
    }
}

int solve(const char *initial_state, const char *keys, int size) {
    if (!initialize_board(initial_state, keys, size)){
        return 0;  
    }
    initial_possibilities(size);
    edge_clue_initialization(size);
    apply_edge_constraint_rule(size);

    
    do {
        apply_constraint_propagation(size); 
        apply_process_of_elimination(size);
        apply_edge_constraint_rule(size); 
        sequence_filtration_rows(size); 
        sequence_filtration_cols(size); 
    } while (!winning_board());

    print_board();
    return 1;
}
