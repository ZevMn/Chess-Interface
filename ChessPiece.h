// Header file "ChessPiece.h"

#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "ChessGame.h"
#include <string>

enum pieceColour {white, black};

class ChessPiece {
    // ABSTRACT CLASS
    public:
        void setColour(pieceColour c);
        char getAbbrName() const;

        int rankIndex;
        int fileIndex;

        virtual bool isValidMovePattern(const char* coord1, const char* coord2) const = 0;

    protected:
        char abbrName;
        string fullName;

        pieceColour colour;

        virtual void makeMove();
        virtual void capture();
};

class Pawn : public ChessPiece {

    public:
        bool isValidMovePattern(const char* coord1, const char* coord2) const override;
        // promote()
    private:
        bool firstMove = true;
        // can_en_passant
};

class Rook : public ChessPiece {

    public:
        bool isValidMovePattern(const char* coord1, const char* coord2) const override;
    // has_moved
    // left_rook
};

class Knight : public ChessPiece {
    public:
        bool isValidMovePattern(const char* coord1, const char* coord2) const override;
};

class Bishop : public ChessPiece {
    public:
        bool isValidMovePattern(const char* coord1, const char* coord2) const override;
};

class Queen : public ChessPiece {
    public:
        bool isValidMovePattern(const char* coord1, const char* coord2) const override;
};

class King : public ChessPiece {
    public:
        bool isValidMovePattern(const char* coord1, const char* coord2) const override;
    // has_moved
    // in_check
};

#endif