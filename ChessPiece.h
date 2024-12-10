/* 
 * ChessPiece.h - Header file for the ChessPiece abstract class 
 * and the sub-classes representing the different chess pieces
 * and their behaviour in a chess game.
 */

 /* 
  * Author: Zev Menachemson
  * Last Edited: 10/12/2024
  */

#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <string>
#include <vector>

class ChessGame; // Forward declaration to prevent circular dependency

/* 
 * Enum representing the different types of chess pieces.
 */
enum PieceType {pawn, rook, knight, bishop, queen, king};

/* 
 * Overloads the << operator to output PieceType 
 * as the name of the chess piece.
 *
 * @param os A reference to the output stream.
 * @param type The piece type to display.
 * @return The modified output stream reference.
 */
std::ostream &operator<<(std::ostream& os, PieceType type);

/* 
 * Enum representing the two colours in a game of chess.
 */
enum PieceColour {white, black};

/* 
 * Overloads the << operator to output PieceColour
 * as the name of the colour.
 *
 * @param os A reference to the output stream.
 * @param type The piece colour to display.
 * @return The modified output stream reference.
 */
std::ostream &operator<<(std::ostream& os, PieceColour colour);

#include "ChessGame.h"

/****************************** Class ChessPiece ******************************/

/* 
 * Abstract base class representing a chess piece.
 */
class ChessPiece {
    public:
        /* 
		 * Parameterised constructor to be called for creating an instance of a chess piece.
         * Initiates the piece with a colour, type, rank, file and a reference to the
         * associated chess game instance.
		 *
		 * @param colour The colour of the chess piece.
		 * @param type The type of chess piece.
         * @param rank The rank of the square occupied by the chess piece.
         * @param file The file of the square occupied by the chess piece.
         * @param chessGame The chess game that the chess piece belongs to.
		 */
        ChessPiece(PieceColour colour, PieceType type, int rank, int file, ChessGame& chessGame);

        /* 
		 * Virtual destructor
		 */
        virtual ~ChessPiece();

        /* MEMBER FUNCTIONS: */

        /* 
         * Determines if a move is geometrically valid by calling isValidMovePattern()
         * individual to the specific piece. Outputs a message to notify the player if the 
         * move is not valid.
         *
         * @param originCoord The coordinates of the initial square occupied by the piece (zero indexed).
         * @param destinationCoord The coordinates of the destination square (zero indexed).
         * @param stringCoord2 The string literal coordinate of the destination square (e.g. "A1") .
         * 
         * @return 'true' if the move is geometrically valid, otherwise 'false'. 
         */
        bool checkMovementPattern(const int* originCoord, const int* destinationCoord, const char* stringCoord2) const;

        /* 
         * Pure virtual function to determine if a move is geometrically valid. Overriden by
         * every piece type to provide individual piece movement logic.
         *
         * @param originCoord The coordinates of the initial square occupied by the piece (zero indexed).
         * @param destinationCoord The coordinates of the destination square (zero indexed).
         * 
         * @return 'true' if the move is geometrically valid, otherwise 'false'. 
         */
        virtual bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const = 0;

        /* Overloads the << operator to output ChessPiece* as the 
         * full name of the chess piece, specified by colour and type
         * (e.g. "White Pawn").
         * 
         * @param os A reference to the output stream.
         * @param chessPiece The pointer to the chess piece to display.
         * @return The modified output stream reference.
         */
        friend std::ostream &operator<<(std::ostream& os, ChessPiece* chessPiece);

        /* GETTER FUNCTIONS: */

        /* 
         * Getter function for 'abbrName' attribute in ChessPiece class.
         *
         * @return The character representing the chess piece in 
         * FEN string notation.
         */
        char getAbbrName() const;

        /* 
         * Getter function for 'type' attribute in ChessPiece class.
         * 
         * @return The type of chess piece.
         */
        PieceType getType() const;

        /*
         * Getter function for 'colour' attribute in ChessPiece class.
         * 
         * @return The colour of the chess piece.
         */
        PieceColour getColour() const;

        /* 
         * Getter function for 'rank' attribute in ChessPiece class.
         * 
         * @return The index of the rank occupied by the chess piece.
         */
        int getRankIndex() const;

        /* 
         * Getter function for 'file' attribute in ChessPiece class.
         * 
         * @return The index of the file occupied by the chess piece.
         */
        int getFileIndex() const;

        /* 
         * Getter function for 'unitMoves' attribute in ChessPiece class.
         * 
         * @return The set of unit moves (the complete set of shortest legal moves 
         * in a given direction).
         */
        std::vector<std::vector<int>> getUnitMoves() const;

    protected:
        /* ATTRIBUTES: */

        char abbrName; // The character representing the chess piece in FEN string notation.

        PieceColour colour; // The colour of the chess piece (white or black)
        PieceType type; // The type of the chess piece (pawn, rook, bishop, knight, king or queen)

        std::vector<std::vector<int>> unitMoves; // The complete set of shortest legal moves in a given direction).
        // NB: For knight, pawn and king this is the set of all legal moves.

        int rankIndex; // The index of the rank occupied by a chess piece (zero indexed).
        int fileIndex; // The index of the file occupied by a chess piece (zero indexed).

        ChessGame& chessGame; // A reference to the chess game that the piece belongs to.
};

class Pawn : public ChessPiece {

    public:
        Pawn(PieceColour colour, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
        // bool canEnPassant
        // promote()
};

class Rook : public ChessPiece {

    public:
        Rook(PieceColour colour, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

class Knight : public ChessPiece {

    public:
        Knight(PieceColour colour, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

class Bishop : public ChessPiece {
    
    public:
        Bishop(PieceColour colour, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

class Queen : public ChessPiece {

    public:
        Queen(PieceColour colour, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

class King : public ChessPiece {

    public:
        King(PieceColour colour, int rank, int file, ChessGame& chessGame);

        bool isValidMovePattern(const int* originCoord, const int* destinationCoord) const override;
};

#endif