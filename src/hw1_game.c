#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw1.h"

int main(int argc, char **argv) {
    assert(argc == 4); //we want a size, board state, and a list of keys
	initialize_board(argv[2], argv[3], (int)strtoul(argv[1], NULL, 10));	
	
    return 0;
}