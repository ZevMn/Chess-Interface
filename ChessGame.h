// Header file "ChessGame.h"

#ifndef CHESSGAME_H
#define CHESSGAME_H

class ChessGame;
class ChessPiece;

#include "ChessPiece.h"
#include <string>

const int ranks = 8, files = 8;

enum Directions {leftRank, rightRank, upFile, downFile, plusplus, minusminus, plusminus, minusplus};

class ChessGame {

    public:
        ChessGame();
        ~ChessGame();

        ChessPiece* chessBoard[ranks][files];

        void loadState(const char * fenString);
        void submitMove(const char * coord1, const char * coord2);

        ChessPiece* getPiece(const int * coordinate);

        PieceColour getTurn();

        void printBoard();

    private:
        PieceColour turn;

        bool captureOccured;
        PieceType capturedPieceName;
        
        bool whiteInCheck;
        bool blackInCheck;

        ChessPiece* blackKing;
        ChessPiece* whiteKing;
        
        // Helper functions for loadState()
        ChessPiece* createChessPiece(char abbrName, int rank, int file);

        // Helper functions for submitMove()
        void detectGameState();
        int* coordToIndex(const char * coord);
        bool checkMoveValid(const int* initCoord, const int* destCoord, const char * coord1, const char * coord2);
        void makeMove(int* initCoord, int* destCoord);
        void switchTurn();

        // Helper functions for detectGameState()
        bool anySafeSquares(ChessPiece* king);
        bool anyPiecesCanMove(); // ADD FUNCTIONALITY

        // Helper functions for makeMove()
        void deletePiece(ChessPiece* pieceToDelete);
        void modifyAttributes(ChessPiece* movedPiece);
        bool detectCheck(ChessPiece* square);

        // Helper functions for detectCheck()
        ChessPiece* findNearestNeighbour(ChessPiece* square, Directions direction);
        bool detectKnightInRange(ChessPiece* square);
        bool doesPieceSeeSquare(ChessPiece* square, ChessPiece* nearestNeighbour, Directions direction);

        void endGame(); // ADD FUNCTIONALITY
};

#endif