// Header file "ChessGame.h"

#ifndef CHESSGAME_H
#define CHESSGAME_H

class ChessGame;
class ChessPiece;

#include "ChessPiece.h"
#include <string>

const int ranks = 8, files = 8;

enum Directions {leftRank, rightRank, upFile, downFile, plusplus, minusminus, plusminus, minusplus};

class ChessGame final {

    public:
        ChessGame();
        ~ChessGame();

        ChessPiece* chessBoard[ranks][files];

        void loadState(const char* fenString);
        void submitMove(const char* stringCoord1, const char* stringCoord2);

        ChessPiece* getPiece(const int* coord);

        PieceColour getTurn();

        void printBoard();

    private:
        PieceColour turn;

        bool pieceAtDestinationSquare;
        PieceType capturedPieceName;
        
        bool whiteInCheck;
        bool blackInCheck;
        bool whiteInCheckmate;
        bool blackInCheckmate;

        int emptySquare[2];

        ChessPiece* blackKing;
        ChessPiece* whiteKing;
        
        // Helper functions for loadState()
        ChessPiece* createChessPiece(const char& abbrName, const int& rank, const int& file);

        // Helper functions for submitMove()
        void detectGameState();
        int* coordToIndex(const char* coord);
        bool checkMoveValid(const int* originCoord, const int* destinationCoord, const char* stringCoord1, const char* stringCoord2);
        void makeMove(const int* originCoord, const int* destinationCoord);
        void doCapture(ChessPiece* pieceToCapture);
        void switchTurn();

        // Helper functions for detectGameState()
        bool anySafeSquares(ChessPiece* king);
        bool anyPiecesCanMove(); // ADD FUNCTIONALITY

        // Helper functions for makeMove()
        void deletePiece(ChessPiece* pieceToDelete);
        bool detectCheck(int rank, int file, PieceColour colour, bool lookingAtKing);
        bool detectCheckmate(ChessPiece* king);
        bool pieceCanBlock(ChessPiece* king);
        bool attemptBlockCheck(ChessPiece* piece);

        // Helper functions for checkMoveValid()
        bool checkCorrectTurn(ChessPiece* pieceAtOrigin);
        bool checkPieceExists(ChessPiece* pieceAtOrigin, const char* stringCoord1);
        bool checkCoordinatesValid(const int* originCoord, const int* destinationCoord);
        bool checkPieceMoves(const int* originCoord, const int* destinationCoord);
        bool checkNoFriendlyCapture(ChessPiece* pieceAtDestination);
        bool checkPathClear(const int* originCoord, const int* destinationCoord);

        // Helper functions for detectCheck()
        ChessPiece* findNearestNeighbour(int rank, int file, PieceColour colour, Directions direction);
        bool detectKnightInRange(int rank, int file, PieceColour colour);
        bool doesPieceSeeSquare(int rank, int file, PieceColour colour, ChessPiece* nearestNeighbour, const Directions& direction);

        void endGame(); // ADD FUNCTIONALITY
};

#endif