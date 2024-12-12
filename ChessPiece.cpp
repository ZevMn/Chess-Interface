/* 
 * ChessPiece.cpp - Implementation file for the ChessPiece abstract class 
 * and the sub-classes representing the different chess pieces
 * and their behaviour in a chess game.
 */

 /* 
  * Author: Zev Menachemson
  * Last Edited: 12/12/2024
  */

#include "ChessPiece.h"
#include "ChessGame.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;


/****************************** ChessPiece - Member Function Definitions ******************************/

/* CONSTRUCTOR */
ChessPiece::ChessPiece(PieceColour colour, PieceType type, int rank, int file, ChessGame& chessGame) 
: colour(colour), type(type), rankIndex(rank), fileIndex(file), chessGame(chessGame) {}

/* VIRTUAL DEFAULT DESTRUCTOR */
ChessPiece::~ChessPiece() {}

/* OVERLOADING THE << OPERATOR FOR 'ChessPiece*' */
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

/* DETERMINES IF A MOVE IS GEOMETRICALLY VALID BY CALLING isValidMovementPattern FOR A GIVEN CHESS PIECE */
bool ChessPiece::checkMovementPattern(const int* originCoord, const int* destinationCoord, const char* stringCoord2) const {
    if (!isValidMovePattern(originCoord, destinationCoord)) {
        cout << colour << "'s " << type << " cannot move to " << stringCoord2 << "!\n";
        return false;
    }
    return true;
}

/* GETTER FOR 'abbrName' */
char ChessPiece::getAbbrName() const {
    return abbrName;
}

/* GETTER FOR 'type' */
PieceType ChessPiece::getType() const {
    return type;
}

/* GETTER FOR 'colour' */
PieceColour ChessPiece::getColour() const {
    return colour;
}

/* GETTER FOR 'rank'' */
int ChessPiece::getRankIndex() const {
    return rankIndex;
}

/* GETTER FOR 'file' */
int ChessPiece::getFileIndex() const {
    return fileIndex;
}

/* GETTER FOR 'unitMoves' */
std::vector<std::vector<int>> ChessPiece::getUnitMoves() const {
    return unitMoves;
}


/****************************** Pawn - Member Function Definitions ******************************/

/* CONSTRUCTOR */
Pawn::Pawn(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, pawn, rank, file, chessGame) {
    if (c == white) { // Pawn capture logic is dealt with further in ChessGame class
        unitMoves = {{1, 0}, {1, 1}, {1, -1}};
    }
    else {
        unitMoves = {{-1, 0}, {-1, 1}, {-1, -1}};
    }
}

/* DETERMINES IF A MOVE IS GEOMETRICALLY VALID FOR A PAWN */
bool Pawn::isValidMovePattern(const int* coords1, const int* coords2) const {

    int oldRank = coords1[0];
    int oldFile = coords1[1];
    int newRank = coords2[0];
    int newFile = coords2[1];

    int advance = newRank - oldRank;

    if (oldFile == newFile) { // Move without capture
        if (colour == white && (advance == 1 || (oldRank == 1 && advance == 2))) {
            return true;
        }
        if (colour == black && (advance == -1 || (oldRank == 6 && advance == -2))) {
            return true;
        }
    }

    if (abs(oldFile - newFile) == 1) { // Capture or en passant
        if ((colour == white) && (advance == 1) && (chessGame.chessBoard[newRank][newFile] != nullptr)) {
            return true;
        }
        if ((colour == black) && (advance == -1) && (chessGame.chessBoard[newRank][newFile] != nullptr)) {
            return true;
        }
        if (chessGame.getEnPassantSquare()[0] != -1 && newRank == chessGame.getEnPassantSquare()[0] && newFile == chessGame.getEnPassantSquare()[1]) {
            chessGame.enPassantCapture = true;
            return true;
        }
    }

    return false;
}


/****************************** Rook - Member Function Definitions ******************************/

/* CONSTRUCTOR */
Rook::Rook(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, rook, rank, file, chessGame) {
    unitMoves = {{1,0}, {-1,0}, {0,1}, {0,-1}};
}

/* DETERMINES IF A MOVE IS GEOMETRICALLY VALID FOR A ROOK */
bool Rook::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (coord2[0] == coord1[0] || coord2[1] == coord1[1]) { // If moving along a rank or a file
        return true;
    }
    return false;
}


/****************************** Knight - Member Function Definitions ******************************/

/* CONSTRUCTOR */
Knight::Knight(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, knight, rank, file, chessGame) {
    unitMoves = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}};
}

/* DETERMINES IF A MOVE IS GEOMETRICALLY VALID FOR A KNIGHT */
bool Knight::isValidMovePattern(const int* coord1, const int* coord2) const {

    if ((abs(coord2[1] - coord1[1]) == 1) && (abs(coord2[0] - coord1[0]) == 2)) {
        return true;
    }
    if ((abs(coord2[1] - coord1[1]) == 2) && (abs(coord2[0] - coord1[0]) == 1)) {
        return true;
    }
    return false;        
}


/****************************** Bishop - Member Function Definitions ******************************/

/* CONSTRUCTOR */
Bishop::Bishop(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, bishop, rank, file, chessGame) {
    unitMoves = {{1,1}, {-1,-1}, {1,-1}, {-1,1}};
}

/* DETERMINES IF A MOVE IS GEOMETRICALLY VALID FOR A BISHOP */
bool Bishop::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (abs(coord2[1] - coord1[1]) == abs(coord2[0] - coord1[0])) {
        return true;
    }
    return false;
}


/****************************** Queen - Member Function Definitions ******************************/

/* CONSTRUCTOR */
Queen::Queen(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, queen, rank, file, chessGame) {
    unitMoves = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,-1}, {1,-1}, {-1,1}};
}

/* DETERMINES IF A MOVE IS GEOMETRICALLY VALID FOR A QUEEN */
bool Queen::isValidMovePattern(const int* coord1, const int* coord2) const {

    if (coord2[0] == coord1[0] || coord2[1] == coord1[1]) { // Rook logic
        return true;
    }
    if (abs(coord2[1] - coord1[1]) == abs(coord2[0] - coord1[0])) { // Bishop Logic
        return true;
    }
    return false;
    
}


/****************************** King - Member Function Definitions ******************************/

/* CONSTRUCTOR */
King::King(PieceColour c, int rank, int file, ChessGame& chessGame) : ChessPiece(c, king, rank, file, chessGame) {
    unitMoves = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,-1}, {1,-1}, {-1,1}};
}

/* DETERMINES IF A MOVE IS GEOMETRICALLY VALID FOR A KING */
bool King::isValidMovePattern(const int* coord1, const int* coord2) const {

    if ((abs(coord2[0] - coord1[0]) < 2) && (abs(coord2[1] - coord1[1]) < 2)) {
        return true;
    }
    return false;
}