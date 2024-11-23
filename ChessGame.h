// Header file "ChessGame.h"

#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "ChessPiece.h"
#include <string>

const int ranks = 8, files = 8;

enum Directions {leftRank, rightRank, upFile, downFile, plusplus, minusminus, plusminus, minusplus};

class ChessGame {

    public:
        ChessPiece* chessBoard[ranks][files];

        //void createPieces();
        void loadState(const char * fenString); //DONE FOR NOW
        void submitMove(const char * coord1, const char * coord2);
        ChessPiece* getPiece(const int * coordinate);

    private:
        //IGNORE: char chess_board[ranks][files];
        PieceColour turn;
        bool whiteInCheck = false;
        bool blackInCheck = false;

        ChessPiece* createChessPiece(char abbrName, int rank, int file);
        int* coordToIndex(const char * coord);
        bool checkMoveValid(const int* initCoord, const int* destCoord);
        void makeMove(int* initCoord, int* destCoord);
        bool detectCheck(ChessPiece* square);
        ChessPiece* findNearestNeighbour(ChessPiece* square, Directions direction);
        bool detectKnightInRange(ChessPiece* square);
        bool doesPieceSeeSquare(ChessPiece* square, ChessPiece* nearestNeighbour, Directions direction);
        void deletePiece(ChessPiece* pieceToDelete);
};

#endif