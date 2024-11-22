// Implementation file "ChessPiece.cpp"

#include "ChessPiece.h"
#include "ChessGame.h"
#include <iostream>
#include <cmath>

using namespace std;

/* CHESS PIECE */
void ChessPiece::setColour(pieceColour c) {
    colour = c;
}

char ChessPiece::getAbbrName() const {
    return abbrName;
}

/* PAWN */
bool Pawn::isValidMovePattern(const char* coord1, const char* coord2) const {

    if (coord2[1] == coord1[1]) { // If moving along a rank only
        if (firstMove && (coord2[0] == coord1[0] + 2)) {
            firstMove = false;
            return true;
        }
        if (coord2[0] == coord1[0] + 1) {
            return true;
        }
    }
    return false;
}

// ROOK
bool Rook::isValidMovePattern(const char* coord1, const char* coord2) const {

    if (coord2[0] == coord1[0] || coord2[1] == coord1[1]) { // If moving along a rank or a file
            return true;
    }
    return false;
}

// KNIGHT
bool Knight::isValidMovePattern(const char* coord1, const char* coord2) const {

    if ((abs(coord2[1] - coord1[1]) == 1) && (abs(coord2[0] - coord1[0]) == 2)) {
        return true;
    }
    if ((abs(coord2[1] - coord1[1]) == 2) && (abs(coord2[0] - coord1[0]) == 1)) {
        return true;
    }
    return false;        
}

// BISHOP
bool Bishop::isValidMovePattern(const char* coord1, const char* coord2) const {

    if (abs(coord2[1] - coord1[1]) == abs(coord2[0] - coord1[0])) {
        return true;
    }
    return false;
}

// QUEEN
bool Queen::isValidMovePattern(const char* coord1, const char* coord2) const {

    if (coord2[0] == coord1[0] || coord2[1] == coord1[1]) { // Rook logic
            return true;
    }
    if (abs(coord2[1] - coord1[1]) == abs(coord2[0] - coord1[0])) { // Bishop Logic
        return true;
    }
    return false;
    
}

// KING
bool King::isValidMovePattern(const char* coord1, const char* coord2) const {

    if ((abs(coord2[0] - coord1[0]) < 2) && (abs(coord2[1] - coord1[1]) < 2)) {
        return true;
    }
    return false;
}