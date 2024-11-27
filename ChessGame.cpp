// Implementation file "ChessGame.cpp"

#include <iostream>
#include "ChessGame.h"
#include "ChessPiece.h"


using namespace std;

ChessGame::ChessGame() : captureOccured(false), whiteInCheck(false), blackInCheck(false), blackKing(nullptr), whiteKing(nullptr) {
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            chessBoard[i][j] = nullptr;
        }
    }
}

ChessGame::~ChessGame() {
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            delete chessBoard[i][j];
            chessBoard[i][j] = nullptr;
        }
    }
}

PieceColour ChessGame::getTurn() {
    return turn;
}

void ChessGame::loadState(const char * fenString) {
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            if (chessBoard[i][j] != nullptr) {
                delete chessBoard[i][j];
                chessBoard[i][j] = nullptr;
            }
        }
    }
    cout << "A new board is loaded!\n\n";

    // DECODE FEN STRING
    // PART 1: BOARD ARRANGEMENT
    // PART 2: ACTIVE COLOUR
    // PART 3: CASTLING RIGHTS
    // PART 4: EN PASSANT SQUARES
    // PART 5: HALF-MOVE COUNTER
    // PART 6: FULL-MOVE COUNTER

    /* PART 1: BOARD ARRANGEMENT */
    int rank = 7, file = 0; // Start at rank 8, file 1
    
    int i = 0;
    while (fenString[i] != ' ') { // Iterate through the FEN string until first blank space (end of part 1)
        char currentCharacter = fenString[i];

        if (currentCharacter == '/') { // Move to next rank, reset file to 0
            rank--;
            file = 0;
        }

        else if (currentCharacter > '0' && currentCharacter < '9') { // Insert 'X' for each empty square
            for (int empty = 0; empty < (currentCharacter - '0'); empty++) {
                chessBoard[rank][file] = nullptr;
                file++;
            }
        }

        else { // Must be a pie>getAbbrName() ce as we are told that only valid FEN strings will be received as inputs
            chessBoard[rank][file] = createChessPiece(currentCharacter, rank, file);
            file++;
        }
        i++; // At the end of the loop, i will hold the position of the blank space

    }

    /* PART 2: ACTIVE COLOUR */
    i++;
    if (fenString[i] == 'w') {
        turn = white;
    }
    else {
        turn = black;
    }

    cout << "Finished loading.\n";
    printBoard();

    // Leave part 3+ for now
    // ----------------------------
    // ----------------------------


}

void ChessGame::submitMove(const char * coord1, const char * coord2) {

    detectGameState();

    int* originCoord = coordToIndex(coord1);
    int* destinationCoord = coordToIndex(coord2);

    if (checkMoveValid(originCoord, destinationCoord, coord1, coord2)) {
        makeMove(originCoord, destinationCoord);
        cout << turn << "'s " << getPiece(destinationCoord)->getType() << " moves from " << coord1 << " to " << coord2;
        switchTurn();
        if (captureOccured) {
            cout << " taking " << turn << "'s " << capturedPieceName;
        }
        cout << "\n\n";
        printBoard();
        cout << "\n\n";
        captureOccured = false;
    }
    else {
        cout << "Move " << coord1 << " to " << coord2 << " is not valid\n";
    }

    delete [] originCoord;
    originCoord = nullptr;
    delete [] destinationCoord;
    destinationCoord = nullptr;
}

ChessPiece* ChessGame::getPiece(const int* coordinate) {
    return chessBoard[coordinate[0]][coordinate[1]];
}

ChessPiece* ChessGame::createChessPiece(char abbrName, int rank, int file) {

    ChessPiece* newPiece = nullptr;

    switch(abbrName) {
        case 'p':
            newPiece = new Pawn(black, rank, file, *this);
            break;
        case 'P':
            newPiece = new Pawn(white, rank, file, *this);
            break;
        case 'r':
            newPiece = new Rook(black, rank, file, *this);
            break;
        case 'R':
            newPiece = new Rook(white, rank, file, *this);
            break;
        case 'n':
            newPiece = new Knight(black, rank, file, *this);
            break;
        case 'N':
            newPiece = new Knight(white, rank, file, *this);
            break;
        case 'b':
            newPiece = new Bishop(black, rank, file, *this);
            break;
        case 'B':
            newPiece = new Bishop(white, rank, file, *this);
            break;
        case 'q':
            newPiece = new Queen(black, rank, file, *this);
            break;
        case 'Q':
            newPiece = new Queen(white, rank, file, *this);
            break;
        case 'k':
            newPiece = new King(black, rank, file, *this);
            blackKing = newPiece;
            break;
        case 'K':
            newPiece = new King(white, rank, file, *this);
            whiteKing = newPiece;
            break;
        default:
            cout << "ERROR: Invalid chess piece - could not instantiate game.\n";
            exit(1);
    }
    return newPiece;
}

void ChessGame::detectGameState() {
    
    // Detect Checkmate
    if (turn == white && whiteInCheck && !anySafeSquares(whiteKing)) {
        cout << "White is in checkmate\n";
        endGame();
    }
    if (turn == black && blackInCheck && !anySafeSquares(blackKing)) {
        cout << "Black is in checkmate\n";
        endGame();
    }

    // Detect Stalemate
    if (turn == white && !whiteInCheck && !anySafeSquares(whiteKing) && !anyPiecesCanMove()) {
        cout << "Stalemate\n";
        endGame();
    }
    if (turn == black && !blackInCheck && !anySafeSquares(blackKing) && !anyPiecesCanMove()) {
        cout << "Stalemate\n";
        endGame();
    }

    // Detect draw by 50 moves
    // Detect Draw by repetition
    cout << "Game continues\n";
}

bool ChessGame::anySafeSquares(ChessPiece* king) {
    // Get coords of king
    int rank = king->getRankIndex();
    int file = king->getFileIndex();
    int originCoord[2] = {rank, file};

    int kingMoves[8][2] = {{1, 1}, {1, 0}, {0, 1}, {-1, -1}, {-1, 0}, {0, -1}, {1, -1}, {-1, 1}};

    for (int move=0; move < 8; move++) {
        int newRank = rank + kingMoves[move][0];
        int newFile = file + kingMoves[move][1];

        if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) { // Boundary checks
            int destinationCoord[2] = {newRank, newFile};

            // check adjacent square empty
            if (chessBoard[newRank][newFile] == nullptr) {

                // move the king to that square
                makeMove(originCoord, destinationCoord);

                // if not in check, move back and return true
                if (!detectCheck(king)) {
                    makeMove(destinationCoord, originCoord);
                    return true;
                }

                // else just move it back
                makeMove(destinationCoord, originCoord);
            }
        }
    }
    return false;
}

bool ChessGame::anyPiecesCanMove() {

    // Go through all white pieces
    // Return true if any of them can move

    return true;
    //return false;
}

int* ChessGame::coordToIndex(const char * coord) {

    int* indexArray = new int[2];

    indexArray[1] = coord[0] - 'A'; // files are denoted by letters
    indexArray[0] = coord[1] - '1'; // ranks are deonated by numbers

    if (indexArray[0] < 0 || indexArray[0] > 7 || indexArray[1] < 0 || indexArray[1] > 7) {
        cout << "ERROR: Coordinate out of bounds.\n";
        exit(1); // REVISIT THIS
    }

    return indexArray;
}

bool ChessGame::checkMoveValid(const int* initCoord, const int* destCoord, const char * coord1, const char * coord2) {

    // NOT IN CHECK
    if ((turn == white && whiteInCheck) || (turn == black && blackInCheck)) {
        cout << "ERROR: Cannot make move - you are in check.\n";
        return false;
    }

    // If in check
    // Do move
    // If (detectCheck()) 
    // Undo move
    // return false;

    // ARE THE COORDINATES VALID
    for (int i=0; i<2; i++) {
        if (initCoord[i] < 0 || initCoord[i] > 7 || destCoord[i] < 0 || destCoord[i] > 7) {
            cout << "ERROR: Cannot make move - invalid coordinatesm\n";
            return false;
        }
    }

    // IS THERE A PIECE AT THE POSITION TO MOVE
    ChessPiece* pieceAtOrigin = getPiece(initCoord);
    if (pieceAtOrigin == nullptr) {
        cout << "There is no piece at position " << coord1 << "!\n";
        return false;
    }

    // IS IT THE PLAYERS TURN TO MOVE
    if (pieceAtOrigin->getColour() != turn) {
        cout << "It is not " << turn << "'s turn to move!\n";
        return false;
    }

    // IS THE PIECE ACTUALLY MOVING
    if (initCoord[0] == destCoord[0] && initCoord[1] == destCoord[1]) {
        cout << "ERROR: Cannot make move - piece must move from current square\n";
        return false;
    }

    // CHECK THERE IS NO SAME COLOUR PIECE AT DESTINATION
    ChessPiece* pieceAtDestination = getPiece(destCoord);
    if (pieceAtDestination != nullptr && pieceAtDestination->getColour() == pieceAtOrigin->getColour()) {
        cout << "ERROR: Cannot make move - you cannot move to a square already occupied by one of your pieces.\n";
        return false;
    }

    // INVALID PIECE MOVEMENT PATTERN
    if (!pieceAtOrigin->isValidMovePattern(initCoord, destCoord)) {
        cout << turn << "'s " << pieceAtOrigin->getType() << " cannot move to " << coord2 << "!\n";
        return false;
    }

    // NO PIECE IN THE WAY (IF NOT KNIGHT)
    if (pieceAtOrigin->getType() != knight) {
        
        if (initCoord[0] == destCoord[0]) { // If travelling along the same rank

            for (int i=1; i < abs(destCoord[0]-initCoord[0]);i++) {
                if ((destCoord[0] > initCoord[0]) && (chessBoard[initCoord[0] + i][initCoord[1]] != nullptr)) {
                        cout << "ERROR: 1\n";
                        return false;
                }
                else if ((destCoord[0] < initCoord[0]) && (chessBoard[initCoord[0] - i][initCoord[1]] != nullptr)) {
                        cout << "ERROR: 2\n";
                        return false;
                }
            }
        }

        else if (initCoord[1] == destCoord[1]) { // If travelling along the same file
        
            for (int i=1; i < abs(destCoord[1]-initCoord[1]);i++) {
                if ((destCoord[1] > initCoord[1]) && (chessBoard[initCoord[0]][initCoord[1] + i] != nullptr)) {
                        cout << "ERROR: 3\n";
                        return false;
                }
                else if ((destCoord[1] < initCoord[1]) && (chessBoard[initCoord[0]][initCoord[1] - i] != nullptr)) {
                        cout << "ERROR: 4\n";
                        return false;
                }
            }
        }

        else if (abs(destCoord[0] - initCoord[0]) == abs(destCoord[1] - initCoord[1])) { // If travelling along a diagonal

            for (int i=1; i < abs(destCoord[0]-initCoord[0]); i++) {

                if ((destCoord[0] > initCoord[0]) && (destCoord[1] > initCoord[1])) { // (rank,file) = (+,+)
                    if (chessBoard[initCoord[0]+i][initCoord[1]+i] != nullptr) {
                        cout << "ERROR: 5\n";
                        return false;
                    }
                }
                if ((destCoord[0] < initCoord[0]) && (destCoord[1] < initCoord[1])) { // (rank,file) = (-,-)
                    if (chessBoard[initCoord[0]-i][initCoord[1]-i] != nullptr) {
                        cout << "ERROR: 6\n";
                        return false;
                    }
                }
                if ((destCoord[0] > initCoord[0]) && (destCoord[1] < initCoord[1])) { // (rank,file) = (+,-)
                    if (chessBoard[initCoord[0]+i][initCoord[1]-i] != nullptr) {
                        cout << "ERROR: 7\n";
                        return false;
                    }
                }
                if ((destCoord[0] < initCoord[0]) && (destCoord[1] > initCoord[1])) { // (rank,file) = (-,+)
                    if (chessBoard[initCoord[0]-i][initCoord[1]+i] != nullptr) {
                        cout << "ERROR: 8\n";
                        return false;
                    }
                }
            }
        }
    }

    // NOT MOVING INTO CHECK

    return true;
}

void ChessGame::makeMove(int* initCoord, int* destCoord) {

    ChessPiece* ptrAtDestination = chessBoard[destCoord[0]][destCoord[1]]; // Retain a pointer to the piece at the destination square

    chessBoard[destCoord[0]][destCoord[1]] = chessBoard[initCoord[0]][initCoord[1]]; // Make the move
    chessBoard[initCoord[0]][initCoord[1]] = nullptr;

    if (ptrAtDestination != nullptr) { // If a capture has occured, delete the piece from memory
        captureOccured = true;
        capturedPieceName = ptrAtDestination->getType();
        deletePiece(ptrAtDestination);
        ptrAtDestination = nullptr;
    }

    modifyAttributes(chessBoard[destCoord[0]][destCoord[1]]);

    if (turn == white) { // Detect if move has put opponent in check
        blackInCheck = detectCheck(blackKing);
    }
    else {
        whiteInCheck = detectCheck(whiteKing);
    }
}

void ChessGame::switchTurn() {
    turn = (turn == white) ? black : white;
}

void ChessGame::deletePiece(ChessPiece* pieceToDelete) {
    delete pieceToDelete;
    pieceToDelete = nullptr;
}

void ChessGame::modifyAttributes(ChessPiece* movedPiece) {
    movedPiece->hasMoved = true;
}

bool ChessGame::detectCheck(ChessPiece* square) {

    bool detected = false;
    
    if (detectKnightInRange(square)) {
        detected = true;
    }

    if (doesPieceSeeSquare(square, findNearestNeighbour(square, leftRank), leftRank)) {
        detected = true;
    }

    if (detected) {
        (square->getColour() == white) ? whiteInCheck = true : blackInCheck = true;
    }

    return detected;
}

ChessPiece* ChessGame::findNearestNeighbour(ChessPiece* square, Directions direction) {
    int rank = square->getRankIndex();
    int file = square->getFileIndex();

    switch (direction) {
        case leftRank:
            for (int i=rank-1; i >= 0; i--) {
                if (chessBoard[i][file] != nullptr) {
                    return chessBoard[i][file];
                }
            }
            break;

        case rightRank:
            for (int i=rank+1; i < 8; i++) {
                if (chessBoard[i][file] != nullptr) {
                    return chessBoard[i][file];
                }
            }
            break;

        case upFile:
            for (int i=file+1; i < 8; i++) {
                if (chessBoard[rank][i] != nullptr) {
                    return chessBoard[rank][i];
                }
            }
            break;

        case downFile:
            for (int i=file-1; i >= 0; i--) {
                if (chessBoard[rank][i] != nullptr) {
                    return chessBoard[rank][i];
                }
            }
            break;

        case plusplus:
            for (int i=1; i+max(rank, file) < 8; i++) {
                if (chessBoard[rank+i][file+i] != nullptr) {
                    return chessBoard[rank+i][file+i];
                }
            }
            break;

        case minusminus:
            for (int i=1; min(rank, file)-i >= 0; i++) {
                if (chessBoard[rank-i][file-i] != nullptr) {
                    return chessBoard[rank-i][file-i];
                }
            }
            break;

        case plusminus:
            for (int i=1; (rank + i < 8) && (file - i >= 0); i++) {
                if (chessBoard[rank+i][file-i] != nullptr) {
                    return chessBoard[rank+i][file-i];
                }
            }
            break;

        case minusplus:
            for (int i=1; (rank - i >= 0) && (file + i < 8); i++) {
                if (chessBoard[rank-i][file+i] != nullptr) {
                    return chessBoard[rank-i][file+i];
                }
            }
            break;

        default:
            cout << "ERROR: Please input a valid direction.\n";
            return nullptr;
    }
    return nullptr;
}

bool ChessGame::detectKnightInRange(ChessPiece* square) {
    int rank = square->getRankIndex();
    int file = square->getFileIndex();

    int knightMoves[8][2] = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}};

    for (int move=0; move < 8; move++) {
        int newRank = rank + knightMoves[move][0];
        int newFile = file + knightMoves[move][1];

        if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) { // Boundary checks
            ChessPiece* possibleKnight = chessBoard[newRank][newFile];
            if (possibleKnight && (possibleKnight->getType() == knight) && (possibleKnight->getColour() != square->getColour())) {
                return true;
            }
        }
    }
    return false;
}

bool ChessGame::doesPieceSeeSquare(ChessPiece* square, ChessPiece* nearestNeighbour, Directions direction) {

    if (nearestNeighbour != nullptr) {
        if (square->getColour() == nearestNeighbour->getColour()) { // Return false if piece is friendly
            return false;
        }

        PieceType pieceName = nearestNeighbour->getType();

        if (pieceName == queen) {
            return true;
        }

        if ((pieceName == king) && (min(abs(nearestNeighbour->getRankIndex() - square->getRankIndex()), abs(nearestNeighbour->getFileIndex() - square->getFileIndex())) == 1)) {
            return true;
        }

        if ((pieceName == pawn)) {
            if ((square->getColour() == white) && (nearestNeighbour->getRankIndex() == square->getRankIndex()+1)) {
                if (abs(nearestNeighbour->getFileIndex() - square->getFileIndex()) == 1) {
                    return true;
                }
            }
            if ((square->getColour() == black) && (nearestNeighbour->getRankIndex() == square->getRankIndex()-1)) {
                if (abs(nearestNeighbour->getFileIndex() - square->getFileIndex()) == 1) {
                    return true;
                }
            }
        }

        switch (direction) {
            case leftRank:
            case rightRank:
            case upFile:
            case downFile:
                if (pieceName == rook) {
                    return true;
                }
                break;

            case plusplus:
            case minusminus:
            case plusminus:
            case minusplus:
                if (pieceName == bishop) {
                    return true;
                }
                break;

            default:
                return false;
        }
    }
    return false;
}

void ChessGame::printBoard() {

    for (int rank=7; rank>=0; rank--) {
        cout << rank + 1 << "   ";
        for (int file=0; file<8; file++) {
            if (chessBoard[rank][file] == nullptr) {
                cout << "    |    |    ";
            }
            else {
                cout << " |" << chessBoard[rank][file] << "| ";
            }
        }
        cout << "\n" << "----------------------------------------------------------------------------------------------------------------------" << "\n";
    }
    cout <<             "      A              B              C              D              E              F              G              H      " << "\n";
}

void ChessGame::endGame() {
    exit(1);
}