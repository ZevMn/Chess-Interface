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

        /* 
         * Accepts a move in the active chess game. If invalid or illegal, outputs the appropriate
         * message. If legal, makes the move, updates the game status and outputs the appropriate message.
         * 
         * NB: Moving the king two squares along a file is interpreted as castling.
         * 
         * @param stringCoord1 The string literal letter-integer coordinates (e.g. "A1") of the piece to move.
         * @param stringCoord2 The string literal letter-integer coordinates (e.g. "B2") of the destination square.
         */
        void submitMove(const char* stringCoord1, const char* stringCoord2);

        ChessPiece* chessBoard[ranks][files];
        
        /*
         * Getter function for enPassantSquare, called in Pawn-> isValidMovePattern()
         *
         * @return An integer array of length two containing zero-indexed coordinates of an en passant
         * square on the chess board.
         */
        int* getEnPassantSquare();
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

        /*
         * Converts chess board coordinates from letter-integer format to zero-indexed integers.
         *
         * @param stringCoord A string literal giving the chess coordinates (e.g. "A1") of a square on the chess board.
         * 
         * @return An integer array of length two containing zero-indexed coordinates of a piece on the chess board.
         */
        int* coordToIndex(const char* stringCoord);

        /*
         * Obtains a pointer to a chess piece at a given position on the chess board.
         *
         * @param coord An integer array of length two containing zero-indexed coordinates of a piece on the chess board.
         * 
         * @return A pointer to the chess piece at the specified position on the chess board.
         */
        ChessPiece* getPiece(const int* coord);

        /*
         * Determines whether a move is valid disregarding state of check.
         *
         * Checks whether: 1) The coordinates are valid
         *                 2) A piece exists at the origin square
         *                 3) The piece belongs to the active colour
         *                 4) The piece is being moved
         *                 5) There is a friendly piece at the destination square
         *                 6) The player is attempting to castle (indicated by moving the king two squares along a rank)
         *                 7) The movement pattern is valid for the specified piece
         *                 8) The path is clear (unless the piece is a knight)
         *
         * @param originCoord An integer array of length two containing zero-indexed coordinates of the piece to move.
         * @param destinationCoord An integer array of length two containing zero-indexed coordinates of the destination square.
         * @param stringCoord1 The string literal letter-integer coordinates (e.g. "A1") of the piece to move.
         * @param stringCoord2 The string literal letter-integer coordinates (e.g. "B2") of the destination square.
         * 
         * @return true if the move is valid disregarding state of check; false otherwise.
         */
        bool checkMoveValid(const int* originCoord, const int* destinationCoord, const char* stringCoord1, const char* stringCoord2);
        
        /* HELPER FUNCTIONS FOR checkMoveValid() within submitMove() */

        /*
         * Determines whether a given set of coordinates for a move are in-bounds.
         *
         * @param originCoord An integer array of length two containing zero-indexed coordinates of the piece to move.
         * @param destinationCoord An integer array of length two containing zero-indexed coordinates of the destination square.
         * 
         * @return true if the coordinates are in-bounds; false otherwise.
         */
        bool checkCoordinatesValid(const int* originCoord, const int* destinationCoord);

        /*
         * Determines whether a piece exists at the origin square of a move.
         *
         * @param pieceAtOrigin A pointer to the chess piece occupying the origin square (nullptr if square is empty).
         * @param stringCoord1 The string literal letter-integer coordinates (e.g. "A1") of the piece to move.
         * @return true if the square is occupied; false otherwise.
         */
        bool checkPieceExists(ChessPiece* pieceAtOrigin, const char* stringCoord1);

        /*
         * Determines whether the piece being moved belongs to the active colour.
         *
         * @param pieceAtOrigin A pointer to the chess piece occupying the origin square (nullptr if square is empty).
         * 
         * @return true if the piece belongs to the active colour; false otherwise.
         */
        bool checkCorrectTurn(ChessPiece* pieceAtOrigin);

        /*
         * Determines whether the piece is being moved.
         *
         * @param originCoord An integer array of length two containing zero-indexed coordinates of the piece to move.
         * @param destinationCoord An integer array of length two containing zero-indexed coordinates of the destination square.
         * 
         * @return true if the piece is being moved; false otherwise.
         */
        bool checkPieceMoves(const int* originCoord, const int* destinationCoord);

        /*
         * Determines whether the destination square is occupied by a friendly piece.
         *
         * @param pieceAtDestination A pointer to the chess piece occupying the destination square 
         * (nullptr if square is empty).
         * 
         * @return false if the destination square is occupied by a friendly piece; true otherwise.
         */
        bool checkNoFriendlyCapture(ChessPiece* pieceAtDestination);

        /*
         * Determines whether an attempt to castle is valid (identified by movement of the king 
         * two squares along a file).
         *
         * @param castlingStatus A reference to the castling status of a move (in this case, either 
         * kingside or queenside).
         * @param originCoord An integer array of length two containing zero-indexed coordinates of the piece to move.
         * @param destinationCoord An integer array of length two containing zero-indexed coordinates of the destination square.
         * 
         * @return true if the attempt to castle is legal; false otherwise.
         */
        bool checkCastlingValid(CastlingStatus& castlingStatus, const int* originCoord, const int* destinationCoord);

        /*
         * Determines (for pawns, rooks, bishops and queens) whether the squares between the origin and destination
         * square are clear.
         *
         * @param originCoord An integer array of length two containing zero-indexed coordinates of the piece to move.
         * @param destinationCoord An integer array of length two containing zero-indexed coordinates of the destination square.
         * @param stringCoord2 The string literal letter-integer coordinates (e.g. "B2") of the destination square.
         * 
         * @return true if the path between the origin and destination square is clear; false otherwise.
         */
        bool checkPathClear(const int* originCoord, const int* destinationCoord, const char* stringCoord2);

        /*
         * Performs a castling move.
         *
         * @param originCoord An integer array of length two containing zero-indexed coordinates of the king to move.
         * @param destinationCoord An integer array of length two containing zero-indexed coordinates of the king's destination square.
         *
         */
        void castle(const int* originCoord, const int* destinationCoord);

        /*
         * Executes a move in a chess game and reassigns the pointers to the kings stored in ChessGame class
         * if the king has moved.
         * 
         * @param originCoord An integer array of length two containing zero-indexed coordinates of the piece to move.
         * @param destinationCoord An integer array of length two containing zero-indexed coordinates of the destination square.
         */
        void makeMove(const int* originCoord, const int* destinationCoord);

        /*
         * Considers whether the player is currently in check and whether they are moving into check, and determines
         * whether a submitted move is legal in light of those considerations.
         * 
         * @param originCoord An integer array of length two containing zero-indexed coordinates of the piece to move.
         * @param destinationCoord An integer array of length two containing zero-indexed coordinates of the destination square.
         * 
         * @return true if the move is legal; false otherwise.
         */
        bool regularMoveLogic(const int* originCoord, const int* destinationCoord);

        /* 
         * Detects whether a given square or a player's king is in check. 
         *
         * @param rank The rank of a given square on the chess board (may or may not contain a king).
         * @param file The file of a given square on the chess board (may or may not contain a king).
         * @param colour The colour of the player under threat.
         * @param lookingAtKing true indicates that we are detecting if a player's king is in check;
         * false indicates that we are detecting whether a given square is under threat.
         * 
         * @return true if check is detected; false otherwise.
         */
        bool detectCheck(const int &rank, const int &file, const PieceColour &colour, const bool lookingAtKing);

        /* 
         * Detects whether an enemy knight is in range of a given square. 
         *
         * @param rank A const reference to the rank of a given square on the chess board.
         * @param file A const reference to the file of a given square on the chess board.
         * @param colour A const reference to the colour of the player under threat (opposite to the knight to detect).
         * 
         * @return true if a knight in range of the square is detected; false otherwise.
         */
        bool detectKnightInRange(const int &rank, const int &file, const PieceColour &colour);

        /* 
         * Detects whether a nearest neighbour piece can 'see' a square (i.e. would it be able
         * to capture an enemy piece at that square).
         *
         * @param rank A const reference to the rank of a given square on the chess board.
         * @param file A const reference to the file of a given square on the chess board.
         * @param colour A const reference to the colour of the player under threat.
         * @param nearestNeighbour A pointer to the nearest neighbour chess piece relative to 
         * a square along a particular direction.
         * @param direction A const reference to the direction along which the nearest neighbour 
         * is positioned relative to the given square.
         * 
         * @return true if the nearest neighbour 'sees' the square; false otherwise.
         */
        bool doesPieceSeeSquare(const int &rank, const int &file, const PieceColour &colour, const ChessPiece* nearestNeighbour, const Directions &direction);

        /**/
        ChessPiece* findNearestNeighbour(const int &rank, const int &file, const PieceColour &colour, const Directions &direction);

        void detectGameState();
        void doCapture(ChessPiece* pieceToCapture);
        void switchTurn();
        void toggleCastlingFlags(const ChessPiece* pieceAtOrigin, const int* originCoord);
        void generalCannotMoveOutput(const PieceType pieceType, const char* stringCoord2);

        bool anySafeSquares(ChessPiece* king);
        bool anyPiecesCanMove();

        void deletePiece(ChessPiece* &pieceToDelete);
        bool detectCheckmate(ChessPiece* king);
        bool pieceCanBlock(ChessPiece* king);
        bool attemptBlockCheck(ChessPiece* piece);
};

#endif