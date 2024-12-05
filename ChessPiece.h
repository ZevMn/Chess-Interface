// Header file "ChessPiece.h"

#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <string>
#include <vector>

class ChessGame;

enum PieceType {pawn, rook, knight, bishop, queen, king};
std::ostream &operator<<(std::ostream& os, PieceType type);


enum PieceColour {white, black};
std::ostream &operator<<(std::ostream& os, PieceColour colour);

#include "ChessGame.h"

class ChessPiece {
    // ABSTRACT CLASS
    public:
        ChessPiece(PieceColour c, PieceType t, int rank, int file, ChessGame& chessGame);
        virtual ~ChessPiece();

        bool hasMoved;

        char getAbbrName() const;

        PieceType getType() const;
        PieceColour getColour() const;

        int getRankIndex() const;
        int getFileIndex() const;

        std::vector<std::vector<int>> getUnitMoves() const;

        bool checkMovementPattern(const int* originCoord, const int* destinationCoord, const char* stringCoord2) const;
        virtual bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const = 0;

        friend std::ostream &operator<<(std::ostream& os, ChessPiece* chessPiece);

    protected:
        char abbrName;
        std::string name;

        PieceColour colour;
        PieceType type;

        std::vector<std::vector<int>> unitMoves; // NB: For knight, pawn and king this is the set of all legal moves

        int rankIndex;
        int fileIndex;

        ChessGame& chessGame;
};

class Pawn : public ChessPiece {

    public:
        Pawn(PieceColour c, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
        // bool canEnPassant
        // promote()
};

class Rook : public ChessPiece {

    public:
        Rook(PieceColour c, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

class Knight : public ChessPiece {

    public:
        Knight(PieceColour c, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

class Bishop : public ChessPiece {
    
    public:
        Bishop(PieceColour c, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

class Queen : public ChessPiece {

    public:
        Queen(PieceColour c, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

class King : public ChessPiece {

    public:
        King(PieceColour c, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

#endif