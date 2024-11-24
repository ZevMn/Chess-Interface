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
                chessBoard[rank][file] = NULL;
                file++;
            }
        }

        else { // Must be a piece as we are told that only valid FEN strings will be received as inputs
            chessBoard[rank][file] = createChessPiece(currentCharacter, rank, file);
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
        switchTurn();
    }
    else {
        cout << "move is not valid\n";
    }

    delete [] originCoord;
    delete [] destinationCoord;
}

ChessPiece* ChessGame::getPiece(const int* coordinate) {
    return chessBoard[coordinate[0]][coordinate[1]];
}

ChessPiece* ChessGame::createChessPiece(char abbrName, int rank, int file) {

    ChessPiece* newPiece;

    switch(abbrName) {
        case 'p':
            newPiece = new Pawn(black, rank, file);
            break;
        case 'P':
            newPiece = new Pawn(white, rank, file);
            break;
        case 'r':
            newPiece = new Rook(black, rank, file);
            break;
        case 'R':
            newPiece = new Rook(white, rank, file);
            break;
        case 'n':
            newPiece = new Knight(black, rank, file);
            break;
        case 'N':
            newPiece = new Knight(white, rank, file);
            break;
        case 'b':
            newPiece = new Bishop(black, rank, file);
            break;
        case 'B':
            newPiece = new Bishop(white, rank, file);
            break;
        case 'q':
            newPiece = new Queen(black, rank, file);
            break;
        case 'Q':
            newPiece = new Queen(white, rank, file);
            break;
        case 'k':
            newPiece = new King(black, rank, file);
            blackKing = newPiece;
            break;
        case 'K':
            newPiece = new King(white, rank, file);
            whiteKing = newPiece;
            break;
        default:
            cout << "ERROR: Invalid chess piece - could not instantiate game.\n";
            exit(1);

    return newPiece;
    }
}

int* ChessGame::coordToIndex(const char * coord) {

    int* indexArray = new int[2];

    indexArray[0] = coord[0] - 'A';
    indexArray[1] = coord[1] - '1';

    return indexArray;
}

bool ChessGame::checkMoveValid(const int* initCoord, const int* destCoord) {

    // NOT IN CHECK
    if ((turn == white && whiteInCheck) || (turn == black && blackInCheck)) {
        cout << "ERROR: Cannot make move - you are in check.\n";
        return false;
    }

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

void ChessGame::makeMove(int* initCoord, int* destCoord) {

    ChessPiece* ptrAtDestination = chessBoard[destCoord[0]][destCoord[1]]; // Retain a pointer to the piece at the destination square

    chessBoard[destCoord[0]][destCoord[1]] = chessBoard[initCoord[0]][initCoord[1]]; // Make the move
    chessBoard[initCoord[0]][initCoord[1]] = nullptr;

    if (ptrAtDestination != nullptr) { // If a capture has occured, delete the piece from memory
            deletePiece(ptrAtDestination);
        }

    modifyAttributes(chessBoard[destCoord[0]][destCoord[1]]);

    if (turn == white) { // Detect if move has put opponent in check
        blackInCheck = detectCheck(blackKing);
    }
    else {
        whiteInCheck = detectCheck(whiteKing);
    }
}

void ChessGame::switchTurn() {
    turn = (turn == white) ? black : white;
}

void deletePiece(ChessPiece* pieceToDelete) {
    delete pieceToDelete;
}

void modifyAttributes(ChessPiece* movedPiece) {
    movedPiece->hasMoved = true;
}

bool ChessGame::detectCheck(ChessPiece* square) {

    bool detected = false;
    
    if (detectKnightInRange(square)) {
        detected = true;
    }

    if (doesPieceSeeSquare(square, findNearestNeighbour(square, leftRank), leftRank)) {
        detected = true;
    }

    if (detected) {
        (square->getColour() == white) ? whiteInCheck = true : blackInCheck = true;
    }

    return detected;
}

ChessPiece* ChessGame::findNearestNeighbour(ChessPiece* square, Directions direction) {
    int rank = square->getRankIndex();
    int file = square->getFileIndex();

    switch (direction) {
        case leftRank:
            for (int i=rank-1; i >= 0; i--) {
                if (chessBoard[i][file] != nullptr) {
                    return chessBoard[i][file];
                }
            }
            break;

        case rightRank:
            for (int i=rank+1; i < 8; i++) {
                if (chessBoard[i][file] != nullptr) {
                    return chessBoard[i][file];
                }
            }
            break;

        case upFile:
            for (int i=file+1; i < 8; i++) {
                if (chessBoard[rank][i] != nullptr) {
                    return chessBoard[rank][i];
                }
            }
            break;

        case downFile:
            for (int i=file-1; i >= 0; i--) {
                if (chessBoard[rank][i] != nullptr) {
                    return chessBoard[rank][i];
                }
            }
            break;

        case plusplus:
            for (int i=1; i+max(rank, file) < 8; i++) {
                if (chessBoard[rank+i][file+i] != nullptr) {
                    return chessBoard[rank+i][file+i];
                }
            }
            break;

        case minusminus:
            for (int i=1; min(rank, file)-i >= 0; i++) {
                if (chessBoard[rank-i][file-i] != nullptr) {
                    return chessBoard[rank-i][file-i];
                }
            }
            break;

        case plusminus:
            for (int i=1; (rank + i < 8) && (file - i >= 0); i++) {
                if (chessBoard[rank+i][file-i] != nullptr) {
                    return chessBoard[rank+i][file-i];
                }
            }
            break;

        case minusplus:
            for (int i=1; (rank - i >= 0) && (file + i < 8); i++) {
                if (chessBoard[rank-i][file+i] != nullptr) {
                    return chessBoard[rank-i][file+i];
                }
            }
            break;

        default:
            cout << "ERROR: Please input a valid direction.\n";
            return nullptr;
    }
    return nullptr;
}

bool ChessGame::detectKnightInRange(ChessPiece* square) {
    int rank = square->getRankIndex();
    int file = square->getFileIndex();

    int knightMoves[8][2] = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}};

    for (int move=0; move < 8; move++) {
        int newRank = rank + knightMoves[move][0];
        int newFile = file + knightMoves[move][1];

        if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) { // Boundary checks
            ChessPiece* possibleKnight = chessBoard[newRank][newFile];
            if (possibleKnight && (possibleKnight->getType() == knight) && (possibleKnight->getColour() != square->getColour())) {
                return true;
            }
        }
    }
    return false;
}

bool ChessGame::doesPieceSeeSquare(ChessPiece* square, ChessPiece* nearestNeighbour, Directions direction) {

    if (square->getColour() == nearestNeighbour->getColour()) { // Return false if piece is friendly
        return false;
    }

    PieceType pieceName = nearestNeighbour->getType();

    if (pieceName == queen) {
        return true;
    }

    if ((pieceName == king) && (min(abs(nearestNeighbour->getRankIndex() - square->getRankIndex()), abs(nearestNeighbour->getFileIndex() - square->getFileIndex())) == 1)) {
        return true;
    }

    if ((pieceName == pawn)) {
        if ((square->getColour() == white) && (nearestNeighbour->getRankIndex() == square->getRankIndex()+1)) {
            if (abs(nearestNeighbour->getFileIndex() - square->getFileIndex()) == 1) {
                return true;
            }
        }
        if ((square->getColour() == black) && (nearestNeighbour->getRankIndex() == square->getRankIndex()-1)) {
            if (abs(nearestNeighbour->getFileIndex() - square->getFileIndex()) == 1) {
                return true;
            }
        }
    }

    switch (direction) {
        case leftRank:
        case rightRank:
        case upFile:
        case downFile:
            if (pieceName == rook) {
                return true;
            }
            break;

        case plusplus:
        case minusminus:
        case plusminus:
        case minusplus:
            if (pieceName == bishop) {
                return true;
            }
            break;

        default:
            return false;
    }
}

void ChessGame::printBoard() {

    for (int rank=0; rank<8; rank++) {
        for (int file=0; file<8; file++) {
            cout << " " << chessBoard[rank][file] << " ";
        }
        cout << "\n";
    }
}