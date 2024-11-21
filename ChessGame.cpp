// Implementation file "ChessGame.cpp"

#include <iostream>
#include "ChessGame.h"

using namespace std;

void ChessGame::loadState(const char * fenString) {
    cout << "A new board is loaded!\n\n";

    // DECODE FEN STRING
    // PART 1: BOARD ARRANGEMENT
    // PART 2: ACTIVE COLOUR
    // PART 3: CASTLING RIGHTS
    // PART 4: EN PASSANT SQUARES
    // PART 5: HALF-MOVE COUNTER
    // PART 6: FULL-MOVE COUNTER

    /* PART 1: BOARD ARRANGEMENT */
    int rank = 7, file = 0; // Start at rank 8, file 1
    
    int i = 0;
    while (fenString[i] != ' ') { // Iterate through the FEN string until first blank space (end of part 1)
        char current_character = fenString[i];

        if (current_character == '/') { // Move to next rank, reset file to 0
            rank--;
            file = 0;
        }

        else if (current_character > '0' && current_character < '9') { // Insert 'X' for each empty square
            for (int empty = 0; empty < (current_character - '0'); empty++) {
                chess_board[rank][file] = 'X';
                //chess_board[rank][file] = NULL;
                file++;
            }
        }

        else { // Must be a piece as we are told that only valid FEN strings will be received as inputs
            chess_board[rank][file] = current_character;
            file++;
        }
        i++; // At the end of the loop, i will hold the position of the blank space
    }

    /* PART 2: ACTIVE COLOUR */
    i++;
    if (fenString[i] == 'w') {
        white_to_play = true;
    }
    else {
        white_to_play = false;
    }

    // Leave part 3+ for now
    // ----------------------------
    // ----------------------------


}


void ChessGame::submitMove(const char *, const char *) {
    if (checkMoveValid()) {
        cout << "move is valid";
    }
    else {
        cout << "move is not valid";
    }
}

bool ChessGame::checkMoveValid() {
    return true; // ADD LOGIC HERE
}
