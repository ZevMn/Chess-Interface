// Header file "ChessPiece.h"

#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "ChessGame.h"

class ChessPiece {
    // ABSTRACT CLASS

    // name
    // colour
    // position

    // is_valid_move
    // capture
    // move
};

class Pawn : public ChessPiece {
    // can_en_passant
    // first_move

    // promote
};

class Rook : public ChessPiece {
    // has_moved
    // left_rook
};

class Knight : public ChessPiece {
}

class Bishop : public ChessPiece {
}

class Queen : public ChessPiece {
};

class King : public ChessPiece {
    // has_moved
    // in_check
};

#endif