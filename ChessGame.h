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
         * @param fenString The FEN string describing the state of the chess game to load.
		 */
        void loadState(const char* fenString);

        void submitMove(const char* stringCoord1, const char* stringCoord2);
        // NB: Castling is accepted as an input when the king is requested to move two squares along a rank in either direction

        ChessPiece* chessBoard[ranks][files];
        
        int* getEnPassantSquare(); // Called in Pawn->isValidMovePattern()
        bool enPassantCapture = false;

        //void printBoard();

    private:

        PieceColour turn; // The active colour on the board (i.e. the player whose turn it is).

        int halfMoveCounter; // The number of moves in a chess game since the last capture or pawn advance.
        int fullMoveCounter; // The number of full moves (only incremented when white begins a turn).

        bool gameLoaded = false; // Indicates whether a game has loaded; prevents submitting moves if set to false.
        bool endGame = false; // Indicates whether a chess game is in progress.

        bool pieceAtDestinationSquare; // Indicates whether there is a piece at the dstination square in submitMove().
        PieceType capturedPieceName; // Retains the name of a captured piece for output to console.
        
        bool whiteInCheck; // Indicates whether the white king is in check
        bool blackInCheck; // Indicates whether the black king is in check

        CastlingStatus castlingStatus = regularMove; // Indicates whether a move is attempting to castle kingside/queenside or not at all

        int enPassantSquare[2] = {-1, -1}; // Contains the coordinates of an en passant square, defined as a square directly behind any pawn that has advanced two ranks the previous turn.
        // NB: If no en passant squares exist, enPassantSquare[0] = -1.

        ChessPiece* blackKing; // A pointer to the black king
        ChessPiece* whiteKing; // A pointer to the white king

        bool whiteCanCastleKingside; // Indicates kingside castling rights for white
        bool whiteCanCastleQueenside; // Indicates queenside castling rights for white
        bool blackCanCastleKingside; // Indicates kingside castling rights for black
        bool blackCanCastleQueenside; // Indicates queenside castling rights for black
        

        /************************** HELPER FUNCTIONS FOR loadState() **************************/

        /*
		 * Iterates through chessBoard (2D array of ChessPiece*). Deletes any memory allocated on the heap and resets all
         * elements to 'nullptr'.
		 */
        void cleanChessBoard();

        /*
		 * Decodes part 1 of a FEN string representing the arrangement of pieces on the chess board.
         *
         * @param fenString The FEN string describing the state of the chess game to load.
         * @param i A reference to the index of the current position being read in the FEN string.
		 */
        void decodePartOne(const char* fenString, int& i);

        /*
		 * Decodes part 2 of a FEN string representing the active colour and stores this in 'turn'.
         *
         * @param fenString The FEN string describing the state of the chess game to load.
         * @param i A reference to the index of the current position being read in the FEN string.
		 */
        void decodePartTwo(const char* fenString, int& i);

        /*
		 * Decodes part 3 of a FEN string representing the castling rights of the players, storing this 
         * in four boolean attributes: 'whiteCanCastleKingside', 'whiteCanCastleQueenside', 'blackCanCastleKingside' 
         * and 'blackCanCastleQueenside'.
         *
         * @param fenString The FEN string describing the state of the chess game to load.
         * @param i A reference to the index of the current position being read in the FEN string.
		 */
        void decodePartThree(const char* fenString, int& i);

        /*
		 * Decodes part 4 of a FEN string representing the en passant status of any pawns and storing this
         * in 'enPassantSquare[2]'. An en passant square is defined as a square directly behind any pawn 
         * that has advanced two ranks the previous turn.
         *
         * @param fenString The FEN string describing the state of the chess game to load.
         * @param i A reference to the index of the current position being read in the FEN string.
		 */
        void decodePartFour(const char* fenString, int& i);

        /*
		 * Decodes part 5 of a FEN string representing the number of half moves, defined as the 
         * number of moves in a chess game since the last capture or pawn advance.
         *
         * @param fenString The FEN string describing the state of the chess game to load.
         * @param i A reference to the index of the current position being read in the FEN string.
		 */
        void decodePartFive(const char* fenString, int& i);

        /*
		 * Decodes part 6 of a FEN string representing the number of full moves, defined as the 
         * number of turns that each player has had (incremented when white begins a turn).
         *
         * @param fenString The FEN string describing the state of the chess game to load.
         * @param i A reference to the index of the current position being read in the FEN string.
		 */
        void decodePartSix(const char* fenString, int& i);

        /*
         * Creates a chess piece with the relevant attributes using heap memory and intialises a 
         * pointer to that piece at a given position on the chess board.
         * 
		 * @param abbrName A const reference to the character representing the chess piece in FEN string notation.
         * @param rank A const reference to the rank occupied by the piece.
         * @param file A const reference to the file occupied by the piece.
		 */
        ChessPiece* createChessPiece(const char& abbrName, const int& rank, const int& file);


        /************************** HELPER FUNCTIONS FOR submitMove() **************************/

        ChessPiece* getPiece(const int* coord);
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