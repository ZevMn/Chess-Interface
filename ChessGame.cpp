// Implementation file "ChessGame.cpp"

#include <iostream>
#include "ChessGame.h"
#include "ChessPiece.h"
#include <map>
#include <stdint.h>


using namespace std;

ChessGame::ChessGame() : pieceAtDestinationSquare(false), whiteInCheck(false), blackInCheck(false), blackKing(nullptr), whiteKing(nullptr) {
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

void ChessGame::loadState(const char* fenString) {
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            if (chessBoard[i][j] != nullptr) {
                delete chessBoard[i][j];
                chessBoard[i][j] = nullptr;
            }
        }
    }
    cout << "A new board state is loaded!\n";

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

    //printBoard();

    // Leave part 3+ for now
    // ----------------------------
    // ----------------------------

    detectGameState();
}

void ChessGame::submitMove(const char* stringCoord1, const char* stringCoord2) {

    int* originCoord = coordToIndex(stringCoord1);
    int* destinationCoord = coordToIndex(stringCoord2);

    ChessPiece* pieceToCapture = getPiece(destinationCoord);
    ChessPiece* currentKing = ((turn == white) ? whiteKing : blackKing);

    if (pieceToCapture != nullptr) {
        pieceAtDestinationSquare = true;
    }
    else {
        pieceAtDestinationSquare = false;
    }

    // FIRST DETERMINE IF MOVE IS VALID DISREGARDING STATE OF CHECK
    if (checkMoveValid(originCoord, destinationCoord, stringCoord1, stringCoord2)) {

        makeMove(originCoord, destinationCoord);

        // IF IN CHECK, THE MOVE MUST TAKE YOU OUT OF CHECK
        if ((turn == white && whiteInCheck) || (turn == black && blackInCheck)) {
            if (detectCheck(currentKing->getRankIndex(), currentKing->getFileIndex(), currentKing->getColour(), true)) {
                makeMove(destinationCoord, originCoord); // UNDO MOVE
                cout << "ERROR: Cannot make move - you are in check.";
                return;
            }
            else {
                turn == white ? whiteInCheck = false : blackInCheck = false;
            }
        }
        // IF NOT IN CHECK, YOU MUST NOT BE MOVING INTO CHECK
        else {
            if (detectCheck(currentKing->getRankIndex(), currentKing->getFileIndex(), currentKing->getColour(), true)) {
                makeMove(destinationCoord, originCoord); // UNDO MOVE
                cout << "ERROR: Cannot make move - you cannot move into check";
                return;
            }
        }

        cout << turn << "'s " << getPiece(destinationCoord)->getType() << " moves from " << stringCoord1 << " to " << stringCoord2;

        // CAPTURE LOGIC
        if (pieceAtDestinationSquare) {
            doCapture(pieceToCapture);
        }

        // REMEMBER PIECE HAS MOVED
        chessBoard[destinationCoord[0]][destinationCoord[1]]->hasMoved = true;
    }

    else {
        cout << "Move " << stringCoord1 << " to " << stringCoord2 << " is not valid";
    }

    delete [] originCoord;
    originCoord = nullptr;
    delete [] destinationCoord;
    destinationCoord = nullptr;

    detectGameState();
    //cout << "\n\n";
    //printBoard();
    //cout << "\n\n";

    switchTurn();
}

ChessPiece* ChessGame::getPiece(const int* coord) {
    return chessBoard[coord[0]][coord[1]];
}

ChessPiece* ChessGame::createChessPiece(const char& abbrName, const int& rank, const int& file) {

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

    bool checkDetected = false;

    // Detect Check
    if (turn == white ? detectCheck(blackKing->getRankIndex(), blackKing->getFileIndex(), blackKing->getColour(), true) : detectCheck(whiteKing->getRankIndex(), whiteKing->getFileIndex(), whiteKing->getColour(), true)){
        checkDetected = true;
    }
    
    // Detect Checkmate
    if (turn == black && whiteInCheck && detectCheckmate(whiteKing)) {
        cout << "\nWhite is in checkmate";
        endGame();
    }
    if (turn == white && blackInCheck && detectCheckmate(blackKing)) {
        cout << "\nBlack is in checkmate";
        endGame();
    }

    // Detect Stalemate
    if (turn == white && !blackInCheck && !anySafeSquares(blackKing) && !anyPiecesCanMove()) { // If no black pieces can move
        cout << "\nStalemate";
        endGame();
    }
    if (turn == black && !whiteInCheck && !anySafeSquares(whiteKing) && !anyPiecesCanMove()) { // If no white pieces can move
        cout << "\nStalemate";
        endGame();
    }

    // Detect draw by 50 moves
    // Detect Draw by repetition

    if (checkDetected) { // If game continues then output check message
        PieceColour oppositeTurn = ((turn == white) ? black : white);
        cout << "\n" << oppositeTurn << " is in check";
    }
}

bool ChessGame::anyPiecesCanMove() {

    // Iterate through the chess board
    for (int ranks = 0; ranks < 8; ranks++) {
        for (int files = 0; files < 8; files++) {

            ChessPiece* pieceToMove = chessBoard[ranks][files];

            if ((pieceToMove != nullptr) && (pieceToMove->getColour() != turn) && (pieceToMove->getType() != king)) {

                int rank = pieceToMove->getRankIndex();
                int file = pieceToMove->getFileIndex();
                int originCoord[2] = {rank, file};

                // Iterate through unit moves
                for (uint32_t move = 0; move < pieceToMove->getUnitMoves().size(); move++) {
                    int newRank = rank + pieceToMove->getUnitMoves()[move][0];
                    int newFile = file + pieceToMove->getUnitMoves()[move][1];

                    // Pawn logic
                    int destinationCoord[2] = {newRank, newFile};
                    if (pieceToMove->getType() == pawn) {
                        return pieceToMove->isValidMovePattern(originCoord, destinationCoord);
                    }

                    // Other pieces logic
                    ChessPiece* newSquare = chessBoard[newRank][newFile];
                    if ((newSquare == nullptr) || (newSquare->getColour() == turn)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool ChessGame::detectCheckmate(ChessPiece* king) { // This function is only called if in check

    if(!anySafeSquares(king)) {
        return !pieceCanBlock(king);
    }
    return false;
}

bool ChessGame::anySafeSquares(ChessPiece* king) {
    // Get coordinates of king
    int rank = king->getRankIndex();
    int file = king->getFileIndex();

    // Iterate through all possible king moves
    for (int move = 0; move < 8; move++) {
        int newRank = rank + king->getUnitMoves()[move][0];
        int newFile = file + king->getUnitMoves()[move][1];

        if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) { // Boundary checks

            // Check destination square not in check, and is either empty or occupied by an enemy piece
            if (!detectCheck(newRank, newFile, king->getColour(), false) && (chessBoard[newRank][newFile] == nullptr || chessBoard[newRank][newFile]->getColour() != king->getColour())) {
                return true;
            }
        }
    }
    return false;
}

int* ChessGame::coordToIndex(const char* coord) {

    int* indexArray = new int[2];

    indexArray[1] = coord[0] - 'A'; // files are denoted by letters
    indexArray[0] = coord[1] - '1'; // ranks are deonated by numbers

    if (indexArray[0] < 0 || indexArray[0] > 7 || indexArray[1] < 0 || indexArray[1] > 7) {
        cout << "ERROR: Coordinate out of bounds.\n";
        exit(1); // REVISIT THIS
    }

    return indexArray;
}

bool ChessGame::checkMoveValid(const int* originCoord, const int* destinationCoord, const char* stringCoord1, const char* stringCoord2) {

    ChessPiece* pieceAtOrigin = getPiece(originCoord);
    ChessPiece* pieceAtDestination = getPiece(destinationCoord);

    if (!checkPieceExists(pieceAtOrigin, stringCoord1) || !checkCorrectTurn(pieceAtOrigin)) {
        return false;
    }
    if (!checkCoordinatesValid(originCoord, destinationCoord) || !checkPieceMoves(originCoord, destinationCoord)) {
        return false;
    }
    if (pieceAtDestination != nullptr && !checkNoFriendlyCapture(pieceAtDestination)) {
        return false;
    }
    if(!pieceAtOrigin->checkMovementPattern(originCoord, destinationCoord, stringCoord2)) {
        return false;
    }
    if ((pieceAtOrigin->getType() != knight) && !checkPathClear(originCoord, destinationCoord)) {
        return false;
    }    

    return true;
}

bool ChessGame::checkCorrectTurn(ChessPiece* pieceAtOrigin) {
    if (pieceAtOrigin->getColour() != turn) {
        cout << "It is not " << pieceAtOrigin->getColour() << "'s turn to move!\n";
        return false;
    }
    return true;
}

bool ChessGame::checkPieceExists(ChessPiece* pieceAtOrigin, const char* stringCoord1) {
    if (pieceAtOrigin == nullptr) {
        cout << "There is no piece at position " << stringCoord1 << "!\n";
        return false;
    }
    return true;
}

bool ChessGame::checkCoordinatesValid(const int* originCoord, const int* destinationCoord) {
    for (int i=0; i<2; i++) {
        if (originCoord[i] < 0 || originCoord[i] > 7 || destinationCoord[i] < 0 || destinationCoord[i] > 7) {
            cout << "ERROR: Cannot make move - invalid coordinatesm\n";
            return false;
        }
    }
    return true;
}

bool ChessGame::checkPieceMoves(const int* originCoord, const int* destinationCoord) {
    if (originCoord[0] == destinationCoord[0] && originCoord[1] == destinationCoord[1]) {
        cout << "ERROR: Cannot make move - piece must move from current square\n";
        return false;
    }
    return true;
}

bool ChessGame::checkNoFriendlyCapture(ChessPiece* pieceAtDestination) {
    if (pieceAtDestination->getColour() == turn) {
        cout << "ERROR: Cannot make move - you cannot move to a square already occupied by one of your pieces.\n";
        return false;
    }
    return true;
}

bool ChessGame::checkPathClear(const int* originCoord, const int* destinationCoord) {

    if (originCoord[0] == destinationCoord[0]) { // If travelling along the same rank

        for (int i=1; i < abs(destinationCoord[0]-originCoord[0]);i++) {
            if ((destinationCoord[0] > originCoord[0]) && (chessBoard[originCoord[0] + i][originCoord[1]] != nullptr)) {
                cout << "ERROR: 1\n";
                return false;
            }
            else if ((destinationCoord[0] < originCoord[0]) && (chessBoard[originCoord[0] - i][originCoord[1]] != nullptr)) {
                cout << "ERROR: 2\n";
                    return false;
            }
        }
    }

    else if (originCoord[1] == destinationCoord[1]) { // If travelling along the same file
            
        for (int i=1; i < abs(destinationCoord[1]-originCoord[1]);i++) {
            if ((destinationCoord[1] > originCoord[1]) && (chessBoard[originCoord[0]][originCoord[1] + i] != nullptr)) {
                cout << "ERROR: 3\n";
                return false;
            }
            else if ((destinationCoord[1] < originCoord[1]) && (chessBoard[originCoord[0]][originCoord[1] - i] != nullptr)) {
                cout << "ERROR: 4\n";
                return false;
            }
        }
    }

    else if (abs(destinationCoord[0] - originCoord[0]) == abs(destinationCoord[1] - originCoord[1])) { // If travelling along a diagonal

        for (int i=1; i < abs(destinationCoord[0]-originCoord[0]); i++) {
            if ((destinationCoord[0] > originCoord[0]) && (destinationCoord[1] > originCoord[1])) { // (rank,file) = (+,+)
                if (chessBoard[originCoord[0]+i][originCoord[1]+i] != nullptr) {
                    cout << "ERROR: 5\n";
                    return false;
                }
            }
            if ((destinationCoord[0] < originCoord[0]) && (destinationCoord[1] < originCoord[1])) { // (rank,file) = (-,-)
                if (chessBoard[originCoord[0]-i][originCoord[1]-i] != nullptr) {
                    cout << "ERROR: 6\n";
                    return false;
                }
            }
            if ((destinationCoord[0] > originCoord[0]) && (destinationCoord[1] < originCoord[1])) { // (rank,file) = (+,-)
                if (chessBoard[originCoord[0]+i][originCoord[1]-i] != nullptr) {
                    cout << "ERROR: 7\n";
                    return false;
                }
            }
            if ((destinationCoord[0] < originCoord[0]) && (destinationCoord[1] > originCoord[1])) { // (rank,file) = (-,+)
                if (chessBoard[originCoord[0]-i][originCoord[1]+i] != nullptr) {
                    cout << "ERROR: 8\n";
                    return false;
                }
            }
        }
    }
    return true;
}

void ChessGame::makeMove(const int* originCoord, const int* destinationCoord) {

    chessBoard[destinationCoord[0]][destinationCoord[1]] = chessBoard[originCoord[0]][originCoord[1]]; // Make the move
    chessBoard[originCoord[0]][originCoord[1]] = nullptr;
}

void ChessGame::doCapture(ChessPiece* pieceToCapture) {
    cout << " taking " << pieceToCapture->getColour() << "'s " << pieceToCapture->getType();

    pieceAtDestinationSquare = false;
    deletePiece(pieceToCapture);
    pieceToCapture = nullptr;
}

void ChessGame::switchTurn() {
    cout << "\n";
    turn = (turn == white) ? black : white;
}

void ChessGame::deletePiece(ChessPiece* pieceToDelete) {
    delete pieceToDelete;
    pieceToDelete = nullptr;
}

bool ChessGame::detectCheck(int rank, int file, PieceColour colour, bool lookingAtKing) {

    bool detected = false;
    
    if (detectKnightInRange(rank, file, colour)) {
        detected = true;
    }

    Directions directions[] = {leftRank, rightRank, upFile, downFile, plusplus, minusminus, plusminus, minusplus};

    for (const Directions& direction : directions) {
        if (doesPieceSeeSquare(rank, file, colour, findNearestNeighbour(rank, file, colour, direction), direction)) {
            detected = true;
            break;
        }
    }

    if (lookingAtKing) {
        if (detected) {
            colour == white ? whiteInCheck = true : blackInCheck = true;
        }
        else {
            colour == white ? whiteInCheck = false : blackInCheck = false;
        }
    }

    return detected;
}

bool ChessGame::pieceCanBlock(ChessPiece* king) {

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if ((chessBoard[rank][file] != nullptr) && (chessBoard[rank][file]->getType() != king->getType()) && (chessBoard[rank][file]->getColour() == king->getColour())) {
                if (attemptBlockCheck(chessBoard[rank][file])) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ChessGame::attemptBlockCheck(ChessPiece* piece) {
    // Try all the moves for that piece, test if blocking was succesful and undo

    int rank = piece->getRankIndex();
    int file = piece->getFileIndex();
    int originCoords[2] = {rank, file};

    for (uint32_t move = 0; move < piece->getUnitMoves().size(); move++) {
        if (piece->getType() == rook || piece->getType() == bishop || piece->getType() == queen) {
            for (int multiplier = 1; multiplier <= 8; multiplier++) {
                int newRank = rank + multiplier*piece->getUnitMoves()[move][0];
                int newFile = file + multiplier*piece->getUnitMoves()[move][1];
                int destinationCoords[2] = {newRank, newFile};
                if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) { // Boundary checks
                    if (chessBoard[newRank][newFile] == nullptr) {
                        makeMove(originCoords, destinationCoords);
                        if ((turn == black && !detectCheck(whiteKing->getRankIndex(), whiteKing->getFileIndex(), whiteKing->getColour(), true)) || (turn == white && !detectCheck(blackKing->getRankIndex(), blackKing->getFileIndex(), blackKing->getColour(), true))) {
                            makeMove(destinationCoords, originCoords);
                            return true;
                        }
                        makeMove(destinationCoords, originCoords);
                    }
                }
            }
        }
        else { // pawn, knight, king
            int newRank = rank + piece->getUnitMoves()[move][0];
            int newFile = file + piece->getUnitMoves()[move][1];
            int destinationCoords[2] = {newRank, newFile};

            if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) { // Boundary checks
                if (chessBoard[newRank][newFile] == nullptr) {
                    makeMove(originCoords, destinationCoords);
                    if ((turn == black && !detectCheck(whiteKing->getRankIndex(), whiteKing->getFileIndex(), whiteKing->getColour(), true)) || (turn == white && !detectCheck(blackKing->getRankIndex(), blackKing->getFileIndex(), blackKing->getColour(), true))) {
                        makeMove(destinationCoords, originCoords);
                        return true;
                    } 
                    makeMove(destinationCoords, originCoords);    
                }      
            }
        }
    }
    return false;
}

ChessPiece* ChessGame::findNearestNeighbour(int rank, int file, PieceColour colour, Directions direction) {

    switch (direction) {
        case leftRank:
            if (file == 0) {
                return nullptr;
                break;
            }
            for (int newFile = file - 1; newFile >= 0; newFile--) {
                if (chessBoard[rank][newFile] != nullptr) {
                    return chessBoard[rank][newFile];
                }
            }
            break;

        case rightRank:
            if (file == 7) {
            return nullptr;
            break;
            }
            for (int newFile = file + 1; newFile < 8; newFile++) {
                if (chessBoard[rank][newFile] != nullptr) {
                    return chessBoard[rank][newFile];
                }
            }
            break;

        case upFile:
            if (rank == 7) {
                return nullptr;
                break;
            }
            for (int newRank = rank + 1; newRank < 8; newRank++) {
                if (chessBoard[newRank][file] != nullptr) {
                    return chessBoard[newRank][file];
                }
            }
            break;

        case downFile:
            if (rank == 0) {
                return nullptr;
                break;
            }
            for (int newRank = rank - 1; newRank >= 0; newRank--) {
                if (chessBoard[newRank][file] != nullptr) {
                    return chessBoard[newRank][file];
                }
            }
            break;

        case plusplus:
            if (rank == 7 || file == 7) {
                return nullptr;
                break;
            }
            for (int i=1; i+max(rank, file) < 8; i++) {
                if (chessBoard[rank+i][file+i] != nullptr) {
                    return chessBoard[rank+i][file+i];
                }
            }
            break;

        case minusminus:
            if (rank == 0 || file == 0) {
                return nullptr;
                break;
            }
            for (int i=1; min(rank, file)-i >= 0; i++) {
                if (chessBoard[rank-i][file-i] != nullptr) {
                    return chessBoard[rank-i][file-i];
                }
            }
            break;

        case plusminus:
            if (rank == 7 || file == 0) {
                return nullptr;
                break;
            }
            for (int i=1; (rank + i < 8) && (file - i >= 0); i++) {
                if (chessBoard[rank+i][file-i] != nullptr) {
                    return chessBoard[rank+i][file-i];
                }
            }
            break;

        case minusplus:
            if (rank == 0 || file == 7) {
                return nullptr;
                break;
            }
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

bool ChessGame::detectKnightInRange(int rank, int file, PieceColour colour) {

    int knightMoves[8][2] = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}};

    for (int move = 0; move < 8; move++) {
        int newRank = rank + knightMoves[move][0];
        int newFile = file + knightMoves[move][1];

        if ((newRank >= 0) && (newRank < 8) && (newFile >= 0) && (newFile < 8)) { // Boundary checks
            ChessPiece* possibleKnight = chessBoard[newRank][newFile];
            if ((possibleKnight != nullptr) && (possibleKnight->getType() == knight) && (possibleKnight->getColour() != colour)) {
                return true;
            }
        }
    }
    return false;
}

bool ChessGame::doesPieceSeeSquare(int rank, int file, PieceColour colour, ChessPiece* nearestNeighbour, const Directions& direction) {

    if (nearestNeighbour != nullptr) {
        if (colour == nearestNeighbour->getColour()) { // Check if piece is friendly
            return false;
        }

        PieceType pieceName = nearestNeighbour->getType();

        if (pieceName == queen) {
            return true;
        }

        if ((pieceName == king) && (min(abs(nearestNeighbour->getRankIndex() - rank), abs(nearestNeighbour->getFileIndex() - file)) == 1)) {
            return true;
        }

        if ((pieceName == pawn)) {
            if (((colour == white) && (nearestNeighbour->getRankIndex() == rank + 1)) ||
                ((colour == black) && (nearestNeighbour->getRankIndex() == rank - 1))) {
                if (abs(nearestNeighbour->getFileIndex() - file) == 1) {
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

// void ChessGame::printBoard() {

//     for (int rank=7; rank>=0; rank--) {
//         cout << rank + 1 << "   ";
//         for (int file=0; file<8; file++) {
//             if (chessBoard[rank][file] == nullptr) {
//                 cout << "    |    |    ";
//             }
//             else {
//                 cout << " |" << chessBoard[rank][file] << "| ";
//             }
//         }
//         cout << "\n" << "----------------------------------------------------------------------------------------------------------------------" << "\n";
//     }
//     cout <<             "      A              B              C              D              E              F              G              H      " << "\n";
// }

// void ChessGame::printBoard() {
//     // Unicode symbols for chess pieces
//     const std::map<std::pair<PieceType, PieceColour>, std::string> pieceSymbols = {
//         {{king, white}, "♔"}, {{queen, white}, "♕"}, {{rook, white}, "♖"},
//         {{bishop, white}, "♗"}, {{knight, white}, "♘"}, {{pawn, white}, "♙"},
//         {{king, black}, "♚"}, {{queen, black}, "♛"}, {{rook, black}, "♜"},
//         {{bishop, black}, "♝"}, {{knight, black}, "♞"}, {{pawn, black}, "♟"}
//     };

//     std::cout << "   a  b  c  d  e  f  g  h\n"; // Column labels
//     std::cout << "  +--+--+--+--+--+--+--+--+\n"; // Top border

//     for (int rank = 7; rank >= 0; rank--) {
//         std::cout << (rank + 1) << " |"; // Row label
//         for (int file = 0; file < 8; file++) {
//             ChessPiece* piece = chessBoard[rank][file];
//             if (piece == nullptr) {
//                 std::cout << "  |"; // Empty square
//             } else {
//                 auto type = piece->getType();
//                 auto colour = piece->getColour();
//                 std::cout << pieceSymbols.at({type, colour}) << " |";
//             }
//         }
//         std::cout << " " << (rank + 1) << "\n"; // Row label on the right
//         std::cout << "  +--+--+--+--+--+--+--+--+\n"; // Row separator
//     }

//     std::cout << "   a  b  c  d  e  f  g  h\n"; // Column labels again
// }


void ChessGame::endGame() {
    exit(1);
}