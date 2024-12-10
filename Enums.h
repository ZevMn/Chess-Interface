/* 
 * Enums.h - Header file for the enums used in this project.
 */

 /* 
  * Author: Zev Menachemson
  * Last Edited: 10/12/2024
  */

#ifndef ENUMS_H
#define ENUMS_H

#include <iostream>

/* 
 * Enum representing the two colours in a game of chess.
 */
enum PieceColour {white, black};

/* 
 * Enum representing the different types of chess pieces.
 */
enum PieceType {pawn, rook, knight, bishop, queen, king};

/* 
 * Enum representing the castling status of a move.
 */
enum CastlingStatus {regularMove, kingsideCastle, queensideCastle};

/* 
 * Enum representing the possible directions a chess piece can move in.
 */
enum Directions {leftRank, rightRank, upFile, downFile, plusplus, minusminus, plusminus, minusplus};

/* 
 * Overloads the << operator to output PieceColour
 * as the name of the colour.
 *
 * @param os A reference to the output stream.
 * @param type The piece colour to display.
 * @return The modified output stream reference.
 */
inline std::ostream &operator<<(std::ostream& os, PieceColour colour) {
    if (colour == white) {
        os << "White";
    }
    else {
        os << "Black";
    }
    return os;
}

/* 
 * Overloads the << operator to output PieceType 
 * as the name of the chess piece.
 *
 * @param os A reference to the output stream.
 * @param type The piece type to display.
 * @return The modified output stream reference.
 */
inline std::ostream &operator<<(std::ostream& os, PieceType type) {

    switch (type) {
        case pawn:
            os << "Pawn"; break;
        case rook:
            os << "Rook"; break;
        case knight:
            os << "Knight"; break;
        case bishop:
            os << "Bishop"; break;
        case queen:
            os << "Queen"; break;
        case king:
            os << "King"; break;
    }
    return os;
}

/* 
 * Overloads the << operator to output castlingStatus 
 * as "kingside" or "queenside".
 *
 * @param os A reference to the output stream.
 * @param castlingStatus The castling type to display.
 * @return The modified output stream reference.
 */
inline std::ostream &operator<<(std::ostream& os, CastlingStatus castlingStatus) {
    if (castlingStatus == kingsideCastle) {
        os << "kingside";
    }
    else if (castlingStatus == queensideCastle) {
        os << "queenside";
    }
    else {
        os << "\nERROR: operator<< should not be called on castlingStatus = regularMove\n";
    }

    return os;
}

#endif