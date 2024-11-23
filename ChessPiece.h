// Header file "ChessPiece.h"

#ifndef CHESSPIECE_H
#define CHESSPIECE_H

//#include "ChessGame.h"
#include <string>

enum PieceType {pawn, rook, knight, bishop, queen, king};
enum PieceColour {white, black};

class ChessPiece {
    // ABSTRACT CLASS
    public:
        ChessPiece(PieceColour c, PieceType t, int rank, int file);

        char getAbbrName() const;

        PieceType getType() const;
        PieceColour getColour() const;

        int getRankIndex() const;
        int getFileIndex() const;

        virtual bool isValidMovePattern(const int* coords1, const int* coords2) const;

    protected:
        char abbrName;

        PieceType type;
        PieceColour colour;

        int rankIndex;
        int fileIndex;
};

class Pawn : public ChessPiece {

    public:
        Pawn(PieceColour c, int rank, int file);

        bool isValidMovePattern(const int* coord1, const int* coord2) const override;
        bool firstMove;
        // bool canEnPassant
        // promote()
};

class Rook : public ChessPiece {

    public:
        Rook(PieceColour c, int rank, int file);

        bool isValidMovePattern(const int* coord1, const int* coord2) const override;
        bool hasMoved;
    // left_rook
};

class Knight : public ChessPiece {

    public:
        Knight(PieceColour c, int rank, int file);

        bool isValidMovePattern(const int* coord1, const int* coord2) const override;
};

class Bishop : public ChessPiece {
    
    public:
        Bishop(PieceColour c, int rank, int file);

        bool isValidMovePattern(const int* coord1, const int* coord2) const override;
};

class Queen : public ChessPiece {

    public:
        Queen(PieceColour c, int rank, int file);

        bool isValidMovePattern(const int* coord1, const int* coord2) const override;
};

class King : public ChessPiece {

    public:
        King(PieceColour c, int rank, int file);

        bool isValidMovePattern(const int* coord1, const int* coord2) const override;
    // has_moved
    // in_check
};

#endif