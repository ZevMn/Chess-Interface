// Header file "ChessGame.h"

#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "ChessPiece.h"
#include <string>

const int ranks = 8, files = 8;

class ChessGame {

    public:
        ChessPiece* chessBoard[ranks][files];

        //void createPieces();
        void loadState(const char * fenString); //DONE FOR NOW
        void submitMove(const char * coord1, const char * coord2);
        ChessPiece* getPiece(string destination);

    private:
        //IGNORE: char chess_board[ranks][files];
        bool whiteToPlay;
        bool whiteInCheck = false;
        bool blackInCheck = false;

        ChessPiece* createChessPiece(char abbrName); //DONE
        int* coordToIndex(const char * coord);
        bool checkMoveValid(int * initCoord, int * destCoord);
        void outputMove();
        void deletePiece(ChessPiece* pieceToDelete);
};

#endif