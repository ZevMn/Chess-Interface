// Implementation file "ChessPiece.cpp"

#include "ChessPiece.h"
#include "ChessGame.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

/* CHESS PIECE */
ChessPiece::ChessPiece(PieceColour c, PieceType t, int rank, int file, ChessGame& chessGame) 
: hasMoved(false), colour(c), type(t), rankIndex(rank), fileIndex(file), chessGame(chessGame) {}

ChessPiece::~ChessPiece() {}

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

std::vector<std::vector<int>> ChessPiece::getUnitMoves() const {
    return unitMoves;
}

bool ChessPiece::checkMovementPattern(const int* originCoord, const int* destinationCoord, const char* stringCoord2) const {
    if (!isValidMovePattern(originCoord, destinationCoord)) {
        cout << colour << "'s " << type << " cannot move to " << stringCoord2 << "!\n";
        return false;
    }
    return true;
}

std::ostream &operator<<(std::ostream& os, ChessPiece* chessPiece) {

    if (chessPiece->colour == white) {
        os << "white ";
    }
    else {
        os << "black ";
    }

    switch (chessPiece->type) {
        case pawn:
            os << "pawn"; break;
        case rook:
            os << "rook"; break;
        case knight:
            os << "knight"; break;
        case bishop:
            os << "bishop"; break;
        case queen:
            os << "queen"; break;
        case king:
            os << "king"; break;
        default:
            os << "X"; break;
    }
    return os;
}

std::ostream &operator<<(std::ostream& os, PieceType type) {

    switch (type) {
        case pawn:
            os << "Pawn"; break;
        case rook:
            os << "Rook"; break;
        case knight:
            os << "Knight"; break;
        case bishop:
            os << "Bishop"; break;
        case queen:
            os << "Queen"; break;
        case king:
            os << "King"; break;
    }
    return os;
}

std::ostream &operator<<(std::ostream& os, PieceColour colour) {
    if (colour == white) {
        os << "White";
    }
    else {
        os << "Black";
    }
    return os;
}

/* PAWN */
Pawn::Pawn(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, pawn, rank, file, chessGame) {
    if (c == white) { // DEAL WITH SPECIAL CASE OF PAWN CAPTURE SOMEHOW
        unitMoves = {{1, 0}, {1, 1}, {1, -1}};
    }
    else {
        unitMoves = {{-1, 0}, {-1, 1}, {-1, -1}};
    }
}

bool Pawn::isValidMovePattern(const int* coords1, const int* coords2) const {

    int oldRank = coords1[0];
    int oldFile = coords1[1];
    int newRank = coords2[0];
    int newFile = coords2[1];

    int advance = newRank - oldRank;

    if (oldFile == newFile) {
        if (colour == white) {
            if (advance == 1 || (!hasMoved && advance == 2)) {
                return true;
            }
        }
        else if (advance == -1 || (!hasMoved && advance == -2)) {
            return true;
        }
    }

    if (abs(oldFile - newFile) == 1) {
        if ((colour == white) && (advance == 1) && (chessGame.chessBoard[newRank][newFile] != nullptr)) {
            return true;
        }
        if ((colour == black) && (advance == -1) && (chessGame.chessBoard[newRank][newFile] != nullptr)) {
            return true;
        }
    }

    return false;
}

// ROOK
Rook::Rook(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, rook, rank, file, chessGame) {
    unitMoves = {{1,0}, {-1,0}, {0,1}, {0,-1}};

    if ((chessGame.blackRookExists && c == black) || (chessGame.whiteRookExists && c == white)) {
        secondRook = true;
    }
}

bool Rook::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (coord2[0] == coord1[0] || coord2[1] == coord1[1]) { // If moving along a rank or a file
        return true;
    }
    return false;
}

// KNIGHT
Knight::Knight(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, knight, rank, file, chessGame) {
    unitMoves = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}};
}

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
Bishop::Bishop(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, bishop, rank, file, chessGame) {
    unitMoves = {{1,1}, {-1,-1}, {1,-1}, {-1,1}};
}

bool Bishop::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (abs(coord2[1] - coord1[1]) == abs(coord2[0] - coord1[0])) {
        return true;
    }
    return false;
}

// QUEEN
Queen::Queen(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, queen, rank, file, chessGame) {
    unitMoves = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,-1}, {1,-1}, {-1,1}};
}

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
King::King(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, king, rank, file, chessGame) {
    unitMoves = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,-1}, {1,-1}, {-1,1}};
}

bool King::isValidMovePattern(const int* coord1, const int* coord2) const {

    if ((abs(coord2[0] - coord1[0]) < 2) && (abs(coord2[1] - coord1[1]) < 2)) {
        return true;
    }
    return false;
}