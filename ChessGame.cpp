// Implementation file "ChessGame.cpp"

#include <iostream>
#include "ChessGame.h"
#include "ChessPiece.h"

using namespace std;

// void ChessGame::createPieces() {
//     // Store the pieces (on the heap) as pointers inside an array
//     // Delete the pieces when they are removed from the board
//     // KEY: Pawns {0,...,7}; Rooks {8,9}; Knights {10,11}; Bishops {12,13}; Queen {14}; King {15}

//     ChessPiece* chessPieceArray[2][16]; // white = 0, black = 1
//     for (int i=0; i < 8; i++) {
//         chessPieceArray[white][i] = new Pawn;
//         chessPieceArray[black][i] = new Pawn;
//     }
    
//     chessPieceArray[white][8] = new Rook;
//     chessPieceArray[black][8] = new Rook;
//     chessPieceArray[white][9] = new Rook;
//     chessPieceArray[black][9] = new Rook;

//     chessPieceArray[white][10] = new Knight;
//     chessPieceArray[black][10] = new Knight;
//     chessPieceArray[white][11] = new Knight;
//     chessPieceArray[black][11] = new Knight;

//     chessPieceArray[white][12] = new Bishop;
//     chessPieceArray[black][12] = new Bishop;
//     chessPieceArray[white][13] = new Bishop;
//     chessPieceArray[black][13] = new Bishop;

//     chessPieceArray[white][14] = new Queen;
//     chessPieceArray[black][14] = new Queen;
//     chessPieceArray[white][15] = new King;
//     chessPieceArray[black][15] = new King;
// }

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
        char currentCharacter = fenString[i];

        if (currentCharacter == '/') { // Move to next rank, reset file to 0
            rank--;
            file = 0;
        }

        else if (currentCharacter > '0' && currentCharacter < '9') { // Insert 'X' for each empty square
            for (int empty = 0; empty < (currentCharacter - '0'); empty++) {
                //IGNORE: chess_board[rank][file] = 'X';
                chessBoard[rank][file] = NULL;
                file++;
            }
        }

        else { // Must be a piece as we are told that only valid FEN strings will be received as inputs
            //IGNORE: chess_board[rank][file] = current_character;
            chessBoard[rank][file] = createChessPiece(currentCharacter);
            file++;
        }
        i++; // At the end of the loop, i will hold the position of the blank space
    }

    /* PART 2: ACTIVE COLOUR */
    i++;
    if (fenString[i] == 'w') {
        whiteToPlay = true;
    }
    else {
        whiteToPlay = false;
    }

    // Leave part 3+ for now
    // ----------------------------
    // ----------------------------


}

void ChessGame::submitMove(const char * coord1, const char * coord2) {

    int* originCoord = coordToIndex(coord1);
    int* destinationCoord = coordToIndex(coord2);

    ChessPiece* ptrAtOrigin = chessBoard[originCoord[0]][originCoord[1]];
    ChessPiece* ptrAtDestination = chessBoard[destinationCoord[0]][destinationCoord[1]];

    if (checkMoveValid(originCoord, destinationCoord)) {
        outputMove();

        if (ptrAtDestination != nullptr) {
            deletePiece(ptrAtDestination);
        }
        ptrAtDestination = ptrAtOrigin;
        ptrAtOrigin = nullptr;
    }
    else {
        cout << "move is not valid\n";
    }
}

ChessPiece* ChessGame::createChessPiece(char abbrName) {

    ChessPiece* newPiece;

    switch(abbrName) {
        case 'p':
        case 'P':
            newPiece = new Pawn;
            break;
        case 'r':
        case 'R':
            newPiece = new Rook;
            break;
        case 'n':
        case 'N':
            newPiece = new Knight;
            break;
        case 'b':
        case 'B':
            newPiece = new Bishop;
            break;
        case 'q':
        case 'Q':
            newPiece = new Queen;
            break;
        case 'k':
        case 'K':
            newPiece = new King;
            break;
        default:
            cout << "ERROR: Invalid chess piece - could not instantiate game.\n";
            exit(1);
    }
    
    if (abbrName > 'a' && abbrName < 's') {
        newPiece->setColour(white);
    }
    else {
        newPiece->setColour(black);
    }

    return newPiece;
}

bool ChessGame::checkMoveValid() {
    // ARE THE COORDINATES VALID
    // IS THE PIECE MOVING
    // NO SAME COLOUR PIECE AT DESTINATION
    ChessPiece* pieceAtDestination = getPiece(destination);
    if (pieceAtDestination != NULL && pieceAtDestination->colour != colour) {
        return false;
    }

    // NOT IN CHECK
    return whiteToPlay ? !whiteInCheck : !blackInCheck;

    // NO PIECE IN THE WAY (IF NOT KNIGHT)
    // NOT MOVING INTO CHECK
    return true; // ADD LOGIC HERE
}

int* coordToIndex(const char * coord) {

    int* indexArray = new int[2];

    indexArray[0] = coord[0] - 'A';
    indexArray[1] = coord[1] - '1';

    return indexArray; // REMEMBER TO DELETE
}

bool checkMoveValid(int * initCoord, int * destCoord) {

}
