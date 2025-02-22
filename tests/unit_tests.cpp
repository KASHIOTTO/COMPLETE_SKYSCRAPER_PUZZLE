#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "gtest/gtest.h"

extern "C" {
#include "hw1.h"
}
void assert_arrays_equal(char exp_array[MAX_LENGTH][MAX_LENGTH], char act_array[MAX_LENGTH][MAX_LENGTH]) {
    for (int i = 0; i < MAX_LENGTH; i++) {
        for (int j = 0; j < MAX_LENGTH; j++) {
			if (exp_array[i][j] != act_array[i][j])
				ERROR("Mismatch at row %d, col %d: expected %c, got %c", i, j, exp_array[i][j], act_array[i][j]);
            ASSERT_EQ(exp_array[i][j], act_array[i][j]);
        }
    }
}

extern char board[MAX_LENGTH][MAX_LENGTH];

class solver_tests : public testing::Test { 
    void SetUp() override {
        
    }
};

// simple 4x4 that can be solved using only basic edge board initialization (i.e. when key = 1 or N)
TEST_F(solver_tests, board4x4init){
    const char* init_board = "3-12-----12323-1";
    const char* init_keys = "2132221324132122";
    char board_array[MAX_LENGTH][MAX_LENGTH] = {
    {'$', '<', 'Y', '%', 'm', '-', 'Z', 'g'},
    {'_', '|', '-', '>', ',', 'I', 'd', '3'},
    {'[', 'B', 'x', '/', '0', '+', '-', '-'},
    {':', ')', '!', 's', 'i', 'k', '/', 'S'},
    {'R', 'L', 'q', 'j', 'b', 'a', 'h', '>'},
    {'g', '2', '@', '-', 'K', 'X', '^', 'R'},
    {'_', 'y', '4', 'h', 'W', 'E', 'd', '#'},
    {'e', '$', 'l', '4', 'C', '*', 'A', 'k'},
    };
    memcpy(board, board_array, sizeof(board_array));
    solve(init_board, init_keys, 4);
    char exp_board[MAX_LENGTH][MAX_LENGTH] = {
    {'3', '4', '1', '2', 'm', '-', 'Z', 'g'},
    {'1', '2', '3', '4', ',', 'I', 'd', '3'},
    {'4', '1', '2', '3', '0', '+', '-', '-'},
    {'2', '3', '4', '1', 'i', 'k', '/', 'S'},
    {'R', 'L', 'q', 'j', 'b', 'a', 'h', '>'},
    {'g', '2', '@', '-', 'K', 'X', '^', 'R'},
    {'_', 'y', '4', 'h', 'W', 'E', 'd', '#'},
    {'e', '$', 'l', '4', 'C', '*', 'A', 'k'},
    };
    assert_arrays_equal(exp_board, board);
}

// Small square board, Can be solved with only edge and and constraint
TEST_F(solver_tests, board4x4) {
    const char *init_board = "----------------";
    const char *init_keys = "2124242121333321";
    char board_array[MAX_LENGTH][MAX_LENGTH] = {
    {'V','<','1',',','G','3','b','+'},
    {'2','b','~','+','w','G','-','C'},
    {'K','w','E','U','Y','$','^','4'},
    {'`','y','2','3','u','#','-','&'},
    {'B','i','"','2','`','?','}','z'},
    {'o','D','.','t','5','`','T','&'},
    {'F',',','=','C','e','q',':','B'},
    {')','f','?','5',']','`','~','q'},
    };    
    memcpy(board, board_array, sizeof(board_array));
    solve(init_board, init_keys, 4);
    char exp_board[MAX_LENGTH][MAX_LENGTH] = {
    {'3','4','2','1','G','3','b','+'},
    {'4','3','1','2','w','G','-','C'},
    {'1','2','4','3','Y','$','^','4'},
    {'2','1','3','4','u','#','-','&'},
    {'B','i','"','2','`','?','}','z'},
    {'o','D','.','t','5','`','T','&'},
    {'F',',','=','C','e','q',':','B'},
    {')','f','?','5',']','`','~','q'},
    };  
    assert_arrays_equal(exp_board, board);
}

//Can be solved without part 3
TEST_F(solver_tests, board4x4v1_withValues){
    const char* init_board = "3-2-431-1-4--1-4";
    const char* init_keys = "2124242121333321";
    char board_array[MAX_LENGTH][MAX_LENGTH] = {
    {'$', '<', 'Y', '%', 'm', '-', 'Z', 'g'},
    {'_', '|', '-', '>', ',', 'I', 'd', '3'},
    {'[', 'B', 'x', '/', '0', '+', '-', '-'},
    {':', ')', '!', 's', 'i', 'k', '/', 'S'},
    {'R', 'L', 'q', 'j', 'b', 'a', 'h', '>'},
    {'g', '2', '@', '-', 'K', 'X', '^', 'R'},
    {'_', 'y', '4', 'h', 'W', 'E', 'd', '#'},
    {'e', '$', 'l', '4', 'C', '*', 'A', 'k'},
    };
    memcpy(board, board_array, sizeof(board_array));
    solve(init_board, init_keys, 4);
    char exp_board[MAX_LENGTH][MAX_LENGTH] = {
    {'3', '4', '2', '1', 'm', '-', 'Z', 'g'},
    {'4', '3', '1', '2', ',', 'I', 'd', '3'},
    {'1', '2','4', '3', '0', '+', '-', '-'},
    {'2', '1', '3', '4', 'i', 'k', '/', 'S'},
    {'R', 'L', 'q', 'j', 'b', 'a', 'h', '>'},
    {'g', '2', '@', '-', 'K', 'X', '^', 'R'},
    {'_', 'y', '4', 'h', 'W', 'E', 'd', '#'},
    {'e', '$', 'l', '4', 'C', '*', 'A', 'k'},
    };
    assert_arrays_equal(exp_board, board);

}

// simple 4x4 that can be solved using only part 2
TEST_F(solver_tests, board4x4zeros){
    const char* init_board = "---2-----12-23-1";
    const char* init_keys = "0130201324130120";
    char board_array[MAX_LENGTH][MAX_LENGTH] = {
    {'$', '<', 'Y', '%', 'm', '-', 'Z', 'g'},
    {'_', '|', '-', '>', ',', 'I', 'd', '3'},
    {'[', 'B', 'x', '/', '0', '+', '-', '-'},
    {':', ')', '!', 's', 'i', 'k', '/', 'S'},
    {'R', 'L', 'q', 'j', 'b', 'a', 'h', '>'},
    {'g', '2', '@', '-', 'K', 'X', '^', 'R'},
    {'_', 'y', '4', 'h', 'W', 'E', 'd', '#'},
    {'e', '$', 'l', '4', 'C', '*', 'A', 'k'},
    };
    memcpy(board, board_array, sizeof(board_array));
    solve(init_board, init_keys, 4);
    char exp_board[MAX_LENGTH][MAX_LENGTH] = {
    {'3', '4', '1', '2', 'm', '-', 'Z', 'g'},
    {'1', '2', '3', '4', ',', 'I', 'd', '3'},
    {'4', '1', '2', '3', '0', '+', '-', '-'},
    {'2', '3', '4', '1', 'i', 'k', '/', 'S'},
    {'R', 'L', 'q', 'j', 'b', 'a', 'h', '>'},
    {'g', '2', '@', '-', 'K', 'X', '^', 'R'},
    {'_', 'y', '4', 'h', 'W', 'E', 'd', '#'},
    {'e', '$', 'l', '4', 'C', '*', 'A', 'k'},
    };
    assert_arrays_equal(exp_board, board);
}

// hard 4x4 that can be solved only with part 3
TEST_F(solver_tests, board4x4hard){
    const char* init_board = "3---------------";
    const char* init_keys = "0100201320132120";
    char board_array[MAX_LENGTH][MAX_LENGTH] = {
    {'$', '<', 'Y', '%', 'm', '-', 'Z', 'g'},
    {'_', '|', '-', '>', ',', 'I', 'd', '3'},
    {'[', 'B', 'x', '/', '0', '+', '-', '-'},
    {':', ')', '!', 's', 'i', 'k', '/', 'S'},
    {'R', 'L', 'q', 'j', 'b', 'a', 'h', '>'},
    {'g', '2', '@', '-', 'K', 'X', '^', 'R'},
    {'_', 'y', '4', 'h', 'W', 'E', 'd', '#'},
    {'e', '$', 'l', '4', 'C', '*', 'A', 'k'},
    };
    memcpy(board, board_array, sizeof(board_array));
    solve(init_board, init_keys, 4);
    char exp_board[MAX_LENGTH][MAX_LENGTH] = {
    {'3', '4', '1', '2', 'm', '-', 'Z', 'g'},
    {'1', '2', '3', '4', ',', 'I', 'd', '3'},
    {'4', '1', '2', '3', '0', '+', '-', '-'},
    {'2', '3', '4', '1', 'i', 'k', '/', 'S'},
    {'R', 'L', 'q', 'j', 'b', 'a', 'h', '>'},
    {'g', '2', '@', '-', 'K', 'X', '^', 'R'},
    {'_', 'y', '4', 'h', 'W', 'E', 'd', '#'},
    {'e', '$', 'l', '4', 'C', '*', 'A', 'k'},
    };
    assert_arrays_equal(exp_board, board);
}

// 7x7 board with zero-keys, requires part 3
TEST_F(solver_tests, board7x7_zeros) {
    const char* init_board = "------------52----4--24-5-------2---5---3---31---";
    const char* init_keys = "2102042004403123041000432031";
    char board_array[MAX_LENGTH][MAX_LENGTH] = {
    {'@', 'X', 'o', 'N', '&', '>', 'p', 'd'},
    {'Z', 'r', '{', 'y', '[', '5', '2', 'q'},
    {'g', '~', '1', '&', '4', 'B', 'F', ')'},
    {'2', '4', '_', '5', 'U', 'O', '/', '8'},
    {'X', 'B', 'l', 'x', '2', 'b', '_', 'q'},
    {'F', '5', 'H', 'N', 'T', '3', 'w', 'h'},
    {'2', 'i', '3', '1', '#', '{', '2', 'K'},
    {'|', 'J', '*', '*', '4', 'u', 'j', 'U'},
    };
    memcpy(board, board_array, sizeof(board_array));
    solve(init_board, init_keys, 7);
    char exp_board[MAX_LENGTH][MAX_LENGTH] = {
    {'5', '7', '1', '2', '3', '4', '6', 'd'},
    {'3', '1', '4', '7', '6', '5', '2', 'q'},
    {'1', '2', '7', '3', '4', '6', '5', ')'},
    {'2', '4', '6', '5', '1', '7', '3', '8'},
    {'7', '3', '5', '6', '2', '1', '4', 'q'},
    {'6', '5', '2', '4', '7', '3', '1', 'h'},
    {'4', '6', '3', '1', '5', '2', '7', 'K'},
    {'|', 'J', '*', '*', '4', 'u', 'j', 'U'},
    }; 
    assert_arrays_equal(exp_board, board);
}

//8x8 board requires part 3
TEST_F(solver_tests, board8x8init){
    const char* init_board = "--------------4---1----3-----4---2--3---4--------1-74-6----6--2-";
    const char* init_keys = "14423323431332321322334233241332";
    char board_array[MAX_LENGTH][MAX_LENGTH] = {
    {'~', 'W', '-', 'I', 'd', '7', ']', 'k'},
    {'Q', 'J', 'q', ',', 'o', 'U', '4', 'F'},
    {'C', 'B', '1', 'O', 'l', '-', 'G', '3'},
    {'+', '-', 'S', 'W', '1', '4', '%', 'h'},
    {'L', '2', 'Y', 'W', '3', 'm', 'w', 'B'},
    {'4', 'm', 't', '4', '<', '?', '_', '{'},
    {'x', '1', '2', '7', '4', 'h', '6', '{'},
    {'p', '@', 'g', '6', 't', 'C', '2', '"'},
    };
    memcpy(board, board_array, sizeof(board_array));
    solve(init_board, init_keys, 8);
    char exp_board[MAX_LENGTH][MAX_LENGTH] = {
    {'8', '3', '2', '1', '5', '6', '7', '4'},
    {'2', '5', '3', '8', '7', '1', '4', '6'},
    {'7', '6', '1', '4', '2', '5', '8', '3'},
    {'1', '8', '7', '2', '6', '4', '3', '5'},
    {'6', '2', '4', '5', '3', '7', '1', '8'},
    {'4', '7', '6', '3', '8', '2', '5', '1'},
    {'3', '1', '5', '7', '4', '8', '6', '2'},
    {'5', '4', '8', '6', '1', '3', '2', '7'},
    };
    assert_arrays_equal(exp_board, board);

}