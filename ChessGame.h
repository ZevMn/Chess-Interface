// Header file "ChessGame.h"

#ifndef CHESSGAME_H
#define CHESSGAME_H

class ChessGame;
class ChessPiece;

#include "ChessPiece.h"
#include <string>

const int ranks = 8, files = 8;

enum Directions {leftRank, rightRank, upFile, downFile, plusplus, minusminus, plusminus, minusplus};

enum CastlingStatus {regularMove, kingsideCastle, queensideCastle};

class ChessGame final {

    public:
        ChessGame();
        ~ChessGame();

        ChessPiece* chessBoard[ranks][files];

        void loadState(const char* fenString);
        void submitMove(const char* stringCoord1, const char* stringCoord2);

        ChessPiece* getPiece(const int* coord);

        PieceColour getTurn();
        
        int* getEnPassantSquare();
        bool enPassantCapture = false;

        void printBoard();

    private:
        PieceColour turn;
        int halfMoveCounter;
        int fullMoveCounter;

        bool pieceAtDestinationSquare;
        PieceType capturedPieceName;
        
        bool whiteInCheck;
        bool blackInCheck;

        CastlingStatus castlingStatus = regularMove;

        int emptySquare[2];
        int enPassantSquare[2];

        ChessPiece* blackKing;
        ChessPiece* whiteKing;

        bool whiteCanCastleKingside;
        bool whiteCanCastleQueenside;
        bool blackCanCastleKingside;
        bool blackCanCastleQueenside;
        
        // Helper functions for loadState()
        ChessPiece* createChessPiece(const char& abbrName, const int& rank, const int& file);

        // Helper functions for submitMove()
        void detectGameState();
        int* coordToIndex(const char* coord);
        bool checkMoveValid(const int* originCoord, const int* destinationCoord, const char* stringCoord1, const char* stringCoord2);
        void makeMove(const int* originCoord, const int* destinationCoord);
        void doCapture(ChessPiece* pieceToCapture);
        void switchTurn();
        void castle(const int* originCoord, const int* destinationCoord);
        bool regularMoveLogic(const int* originCoord, const int* destinationCoord);
        void toggleCastlingFlags(const ChessPiece* pieceAtOrigin, const int* originCoord);

        // Helper functions for detectGameState()
        bool anySafeSquares(ChessPiece* king);
        bool anyPiecesCanMove();

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
        bool checkCastlingValid(CastlingStatus& castlingStatus, const int* originCoord, const int* destinationCoord);

        // Helper functions for detectCheck()
        ChessPiece* findNearestNeighbour(int rank, int file, PieceColour colour, Directions direction);
        bool detectKnightInRange(int rank, int file, PieceColour colour);
        bool doesPieceSeeSquare(int rank, int file, PieceColour colour, ChessPiece* nearestNeighbour, const Directions& direction);

        void endGame();
};

#endif