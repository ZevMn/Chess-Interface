// Header file "ChessGame.h"

#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "ChessPiece.h"

const int ranks = 8, files = 8;

class ChessGame {

    public:
        void loadState(const char * fenString);
        void submitMove(const char *, const char *);

    private:
        char chess_board[ranks][files];
        // ChessPiece* chess_board[ranks][files];
        bool white_to_play;

        bool checkMoveValid();

        // std::map<const char *, char> chess_board;
        // chess_board["A1"] = 'N';
        // chess_board["A2"] = 'N';
};

#endif