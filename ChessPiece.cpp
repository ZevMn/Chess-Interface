// Implementation file "ChessPiece.cpp"

#include "ChessPiece.h"
#include "ChessGame.h"
#include <iostream>
#include <cmath>

using namespace std;

/* CHESS PIECE */
char ChessPiece::getAbbrName() const {
    return abbrName;
}

/* PAWN */
Pawn::Pawn(PieceColour c) : colour(c) {}

bool Pawn::isValidMovePattern(const int* coords1, const int* coords2) const {

    if (coords2[1] == coords1[1]) { // If moving along a rank only
        if (firstMove && (coords2[0] == coords1[0] + 2)) {
            firstMove = false;
            return true;
        }
        if (coords2[0] == coords1[0] + 1) {
            return true;
        }
    }
    return false;
}

// ROOK
Rook::Rook(PieceColour c) : colour(c) {}

bool Rook::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (coord2[0] == coord1[0] || coord2[1] == coord1[1]) { // If moving along a rank or a file
        hasMoved = true;
        return true;
    }
    return false;
}

// KNIGHT
Knight::Knight(PieceColour c) : colour(c) {}

bool Knight::isValidMovePattern(const int* coord1, const int* coord2) const {

    if ((abs(coord2[1] - coord1[1]) == 1) && (abs(coord2[0] - coord1[0]) == 2)) {
        return true;
    }
    if ((abs(coord2[1] - coord1[1]) == 2) && (abs(coord2[0] - coord1[0]) == 1)) {
        return true;
    }
    return false;        
}

// BISHOP
Bishop::Bishop(PieceColour c) : colour(c) {}

bool Bishop::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (abs(coord2[1] - coord1[1]) == abs(coord2[0] - coord1[0])) {
        return true;
    }
    return false;
}

// QUEEN
Queen::Queen(PieceColour c) : colour(c) {}

bool Queen::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (coord2[0] == coord1[0] || coord2[1] == coord1[1]) { // Rook logic
        return true;
    }
    if (abs(coord2[1] - coord1[1]) == abs(coord2[0] - coord1[0])) { // Bishop Logic
        return true;
    }
    return false;
    
}

// KING
King::King(PieceColour c) : colour(c) {}

bool King::isValidMovePattern(const int* coord1, const int* coord2) const {

    if ((abs(coord2[0] - coord1[0]) < 2) && (abs(coord2[1] - coord1[1]) < 2)) {
        return true;
    }
    return false;
}