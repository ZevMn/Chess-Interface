// Implementation file "ChessGame.cpp"

#include <iostream>
#include "ChessGame.h"
#include "ChessPiece.h"

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
        turn = white;
    }
    else {
        turn = black;
    }

    // Leave part 3+ for now
    // ----------------------------
    // ----------------------------


}

void ChessGame::submitMove(const char * coord1, const char * coord2) {

    int* originCoord = coordToIndex(coord1);
    int* destinationCoord = coordToIndex(coord2);

    if (checkMoveValid(originCoord, destinationCoord)) {
        makeMove(originCoord, destinationCoord);
    }
    else {
        cout << "move is not valid\n";
    }
}

ChessPiece* ChessGame::createChessPiece(char abbrName) {

    ChessPiece* newPiece;

    switch(abbrName) {
        case 'p':
            newPiece = new Pawn(black);
            break;
        case 'P':
            newPiece = new Pawn(white);
            break;
        case 'r':
            newPiece = new Rook(black);
            break;
        case 'R':
            newPiece = new Rook(white);
            break;
        case 'n':
            newPiece = new Knight(black);
            break;
        case 'N':
            newPiece = new Knight(white);
            break;
        case 'b':
            newPiece = new Bishop(black);
            break;
        case 'B':
            newPiece = new Bishop(white);
            break;
        case 'q':
            newPiece = new Queen(black);
            break;
        case 'Q':
            newPiece = new Queen(white);
            break;
        case 'k':
            newPiece = new King(black);
            break;
        case 'K':
            newPiece = new King(white);
            break;
        default:
            cout << "ERROR: Invalid chess piece - could not instantiate game.\n";
            exit(1);

    return newPiece;
    }
}

bool ChessGame::checkMoveValid(const int* initCoord, const int* destCoord) {

    // ARE THE COORDINATES VALID
    for (int i=0; i<2; i++) {
        if (initCoord[i] < 0 || initCoord[i] > 7 || destCoord[i] < 0 || destCoord[i] > 7) {
            cout << "ERROR: Cannot make move - invalid coordinatesm\n";
            return false;
        }
    }

    // IS THERE A PIECE OF CORRECT COLOUR TO MOVE
    ChessPiece* pieceAtOrigin = getPiece(initCoord);
    if (pieceAtOrigin == nullptr || pieceAtOrigin->getColour() != turn) {
        cout << "ERROR: Cannot make move - you have not selected a valid piece to movem\n";
        return false;
    }

    // IS THE PIECE ACTUALLY MOVING
    if (initCoord[0] == destCoord[0] && initCoord[1] == destCoord[1]) {
        cout << "ERROR: Cannot make move - piece must move from current squarem\n";
        return false;
    }

    // CHECK THERE IS NO SAME COLOUR PIECE AT DESTINATION
    ChessPiece* pieceAtDestination = getPiece(destCoord);
    if (pieceAtDestination != NULL && pieceAtDestination->getColour() != pieceAtOrigin->getColour()) {
        cout << "ERROR: Cannot make move - you cannot move to a square already occupied by one of your pieces.\n";
        return false;
    }

    // NOT IN CHECK
    if ((turn == white && whiteInCheck) || (turn == black && blackInCheck)) {
        cout << "ERROR: Cannot make move - you are in check.\n";
        return false;
    }

    // NO PIECE IN THE WAY (IF NOT KNIGHT)
    if (pieceAtOrigin->getType() != knight) {
        
        if (initCoord[0] == destCoord[0]) { // If travelling along the same rank

            for (int i=1; i <= abs(destCoord[0]-initCoord[0]);i++) {
                if ((destCoord[0] > initCoord[0]) && (chessBoard[initCoord[0] + i][initCoord[1]] != nullptr)) {
                        return false;
                }
                else if ((destCoord[0] < initCoord[0]) && (chessBoard[initCoord[0] - i][initCoord[1]] != nullptr)) {
                        return false;
                }
            }
        }

        else if (initCoord[1] == destCoord[1]) { // If travelling along the same file
        
            for (int i=1; i <= abs(destCoord[1]-initCoord[1]);i++) {
                if ((destCoord[1] > initCoord[1]) && (chessBoard[initCoord[0]][initCoord[1] + i] != nullptr)) {
                        return false;
                }
                else if ((destCoord[1] < initCoord[1]) && (chessBoard[initCoord[0]][initCoord[1] - i] != nullptr)) {
                        return false;
                }
            }
        }

        else if (abs(destCoord[0] - initCoord[0]) == abs(destCoord[1] - initCoord[1])) { // If travelling along a diagonal

            for (int i=1; i <= abs(destCoord[0]-initCoord[0]); i++) {

                if ((destCoord[0] > initCoord[0]) && (destCoord[1] > initCoord[1])) { // (rank,file) = (+,+)
                    if (chessBoard[initCoord[0]+i][initCoord[1]+i] != nullptr) {
                        return false;
                    }
                }
                if ((destCoord[0] < initCoord[0]) && (destCoord[1] < initCoord[1])) { // (rank,file) = (-,-)
                    if (chessBoard[initCoord[0]-i][initCoord[1]-i] != nullptr) {
                        return false;
                    }
                }
                if ((destCoord[0] > initCoord[0]) && (destCoord[1] < initCoord[1])) { // (rank,file) = (+,-)
                    if (chessBoard[initCoord[0]+i][initCoord[1]-i] != nullptr) {
                        return false;
                    }
                }
                if ((destCoord[0] < initCoord[0]) && (destCoord[1] > initCoord[1])) { // (rank,file) = (-,+)
                    if (chessBoard[initCoord[0]-i][initCoord[1]+i] != nullptr) {
                        return false;
                    }
                }
            }
        }
    }

    // NOT MOVING INTO CHECK
    
    return pieceAtOrigin->isValidMovePattern(initCoord, destCoord);
}

int* ChessGame::coordToIndex(const char * coord) {

    int indexArray[2];

    //int* indexArray = new int[2];

    indexArray[0] = coord[0] - 'A';
    indexArray[1] = coord[1] - '1';

    return indexArray;
}

ChessPiece* ChessGame::getPiece(const int* coordinate) {
    return chessBoard[coordinate[0]][coordinate[1]];
}

void ChessGame::makeMove(int* initCoord, int* destCoord) {

    ChessPiece* ptrAtOrigin = chessBoard[initCoord[0]][initCoord[1]];
    ChessPiece* ptrAtDestination = chessBoard[destCoord[0]][destCoord[1]];

    if (ptrAtDestination != nullptr) {
            deletePiece(ptrAtDestination);
        }
        ptrAtDestination = ptrAtOrigin;
        ptrAtOrigin = nullptr;
}

void deletePiece(ChessPiece* pieceToDelete) {
    delete pieceToDelete;
}

void ChessGame::detectCheck(ChessPiece* square) {
    // @param sqaure Item in chessBoard
    // If rook sees square along file or rank -> oppositeColourInCheck = true
    // If bishop sees square along diagonal -> oppositeColourInCheck = true
    // If queen sees square along file, rank or diagonal -> oppositeColourInCheck = true
    // If King within one square in any direction -> oppositeColourInCheck = true
    // If pawn diagonally adjacent -> If pawn validMove -> oppositeColourInCheck
    // If knight within (+-1, +-2) or (+-2,+-1) -> oppositeColourInCheck
}
