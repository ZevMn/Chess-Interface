// Implementation file "ChessPiece.cpp"

#include "ChessPiece.h"
#include "ChessGame.h"
#include <iostream>
#include <cmath>

using namespace std;

/* CHESS PIECE */
ChessPiece::ChessPiece(PieceColour c, PieceType t, int rank, int file) : colour(c), type(t), rankIndex(rank), fileIndex(file), hasMoved(false) {}

char ChessPiece::getAbbrName() const {
    return abbrName;
}

PieceType ChessPiece::getType() const {
    return type;
}

PieceColour ChessPiece::getColour() const {
    return colour;
}

int ChessPiece::getRankIndex() const {
    return rankIndex;
}

int ChessPiece::getFileIndex() const {
    return fileIndex;
}

/* PAWN */
Pawn::Pawn(PieceColour c, int rank, int file) : ChessPiece(c, pawn, rank, file) {}

bool Pawn::isValidMovePattern(const int* coords1, const int* coords2) const {

    if (coords2[1] == coords1[1]) { // If moving along a rank only
        if (!hasMoved && (coords2[0] == coords1[0] + 2)) {
            return true;
        }
        if (coords2[0] == coords1[0] + 1) {
            return true;
        }
    }
    return false;
}

// ROOK
Rook::Rook(PieceColour c, int rank, int file) : ChessPiece(c, rook, rank, file) {}

bool Rook::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (coord2[0] == coord1[0] || coord2[1] == coord1[1]) { // If moving along a rank or a file
        return true;
    }
    return false;
}

// KNIGHT
Knight::Knight(PieceColour c, int rank, int file) : ChessPiece(c, knight, rank, file) {}

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
Bishop::Bishop(PieceColour c, int rank, int file) : ChessPiece(c, bishop, rank, file) {}

bool Bishop::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (abs(coord2[1] - coord1[1]) == abs(coord2[0] - coord1[0])) {
        return true;
    }
    return false;
}

// QUEEN
Queen::Queen(PieceColour c, int rank, int file) : ChessPiece(c, queen, rank, file) {}

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
King::King(PieceColour c, int rank, int file) : ChessPiece(c, king, rank, file) {}

bool King::isValidMovePattern(const int* coord1, const int* coord2) const {

    if ((abs(coord2[0] - coord1[0]) < 2) && (abs(coord2[1] - coord1[1]) < 2)) {
        return true;
    }
    return false;
}