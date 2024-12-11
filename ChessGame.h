/* 
 * ChessGame.h - Header file for the ChessGame class 
 * containing the chessBoard and logic representing
 * a chess game.
 */

 /* 
  * Author: Zev Menachemson
  * Last Edited: 11/12/2024
  */

#ifndef CHESSGAME_H
#define CHESSGAME_H

// Forward declarations to prevent circular dependency
class ChessGame;
class ChessPiece;

#include "ChessPiece.h"

// Global constants representing the standard size of a chess board
const int ranks = 8, files = 8;


/****************************** Class ChessPiece ******************************/

class ChessGame final {

    public:
        /*
		 * Default constructor to be called for creating an instance of a chess game.
         * Initiates the ChessPiece pointers: 'blackKing' and 'whiteKing' to 'nullptr', 
         * and the booleans: pieceAtDestinationSquare, whiteInCheck and blackInCheck to false.
         * Initiates the chess board to a 2D array of 'nullptr's.
		 */
        ChessGame();

        /*
         * Default destructor deletes any heap memory used by the remaining chess pieces 
         * stored in the chess board.
         */
        ~ChessGame();

        /*
		 * Decodes a FEN string into its six components describing the state of a chess game. 
         * Initialises all the relevant parameters of that game. Detects any game in a state of 
         * check, checkmate, stalemate or a draw and outputs that to the console.
         * 
         * FEN string components: PART 1: BOARD ARRANGEMENT
         *                        PART 2: ACTIVE COLOUR
         *                        PART 3: CASTLING RIGHTS
         *                        PART 4: EN PASSANT SQUARES
         *                        PART 5: HALF-MOVE COUNTER
         *                        PART 6: FULL-MOVE COUNTER
         *
         * @param fenString The FEN string describing the state of the chess game to load
		 */
        void loadState(const char* fenString);

        void submitMove(const char* stringCoord1, const char* stringCoord2);

        ChessPiece* chessBoard[ranks][files];
        
        int* getEnPassantSquare(); // Called in Pawn->isValidMovePattern()
        bool enPassantCapture = false;

        //void printBoard();

    private:
        PieceColour turn;
        int halfMoveCounter;
        int fullMoveCounter;

        bool gameLoaded = false;
        bool endGame = false;

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

        PieceColour getTurn();
        ChessPiece* getPiece(const int* coord);
        
        // Helper functions for loadState()
        void cleanChessBoard();
        void decodePartOne(const char* fenString, int& i);
        void decodePartTwo(const char* fenString, int& i);
        void decodePartThree(const char* fenString, int& i);
        void decodePartFour(const char* fenString, int& i);
        void decodePartFive(const char* fenString, int& i);
        void decodePartSix(const char* fenString, int& i);
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
        void generalCannotMoveOutput(const PieceType pieceType, const char* stringCoord2);

        // Helper functions for detectGameState()
        bool anySafeSquares(ChessPiece* king);
        bool anyPiecesCanMove();

        // Helper functions for makeMove()
        void deletePiece(ChessPiece* &pieceToDelete);
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
        bool checkPathClear(const int* originCoord, const int* destinationCoord, const char* stringCoord2);
        bool checkCastlingValid(CastlingStatus& castlingStatus, const int* originCoord, const int* destinationCoord);

        // Helper functions for detectCheck()
        ChessPiece* findNearestNeighbour(int rank, int file, PieceColour colour, Directions direction);
        bool detectKnightInRange(int rank, int file, PieceColour colour);
        bool doesPieceSeeSquare(int rank, int file, PieceColour colour, ChessPiece* nearestNeighbour, const Directions& direction);
};

#endif