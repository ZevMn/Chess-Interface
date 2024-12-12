// Implementation file "ChessGame.cpp"

#include <iostream>
#include "ChessGame.h"
#include "ChessPiece.h"
#include <cmath>
#include <map>
#include <stdint.h>


using namespace std;


/****************************** ChessGame - Member Function Definitions ******************************/

/* CONSTRUCTOR */
ChessGame::ChessGame() : pieceAtDestinationSquare(false), whiteInCheck(false), blackInCheck(false), blackKing(nullptr), whiteKing(nullptr) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            chessBoard[rank][file] = nullptr;
        }
    }
}

/* DESTRUCTOR */
ChessGame::~ChessGame() {
    cleanChessBoard();
}

void ChessGame::loadState(const char* fenString) {

    cleanChessBoard(); // Clear any previously loaded chess game
    endGame = false; // Indicates that a game is in progress

    /* DECODE FEN STRING */
    int i = 0;
    decodePartOne(fenString, i); // PART 1: BOARD ARRANGEMENT
    decodePartTwo(fenString, i); // PART 2: ACTIVE COLOUR
    decodePartThree(fenString, i); // PART 3: CASTLING RIGHTS

    i++;
    if (fenString[i] != '\0') { // If full notation FEN string is input

        decodePartFour(fenString, i); // PART 4: EN PASSANT SQUARES
        decodePartFive(fenString, i); // PART 5: HALF-MOVE COUNTER
        decodePartSix(fenString, i); // PART 6: FULL-MOVE COUNTER
    }

    gameLoaded = true;
    cout << "A new board state is loaded!\n";
    //printBoard();

    // Simulate the opponent finishing their turn to detect if board loaded in a state of check/checkmate/stalemate/draw
    turn = (turn == white ? black : white); 
    detectGameState();
    turn = (turn == white ? black : white);
}

void ChessGame::cleanChessBoard() {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {

            if (chessBoard[rank][file] != nullptr) {
                delete chessBoard[rank][file];
                chessBoard[rank][file] = nullptr;
            }
        }
    }
}

void ChessGame::decodePartOne(const char* fenString, int& i) {
    int rank = 7, file = 0; // Start at 8th rank and the A-file
    
    while (fenString[i] != ' ') { // Iterate through the FEN string until first blank space (end of part 1)
        
        char currentCharacter = fenString[i];

        if (currentCharacter == '/') { // Move to next rank, reset file to 0
            rank--;
            file = 0;
        }
        else if (currentCharacter > '0' && currentCharacter < '9') { // Skip empty squares
            for (int empty = 0; empty < (currentCharacter - '0'); empty++) {
                file++;
            }
        }
        else { // Must be a piece as we are told that only valid FEN strings will be received as inputs
            chessBoard[rank][file] = createChessPiece(currentCharacter, rank, file);
            file++;
        }
        i++; // At the end of the loop, i will hold the position of the first blank space
    }
}

void ChessGame::decodePartTwo(const char* fenString, int& i) {
    i++;
    turn = (fenString[i] == 'w' ? white : black);
    i++; // i will hold the position of the second blank space
}

void ChessGame::decodePartThree(const char* fenString, int& i) {
    whiteCanCastleKingside = false;
    whiteCanCastleQueenside = false;
    blackCanCastleKingside = false;
    blackCanCastleQueenside = false;

    i++;
    while (fenString[i] != ' ') {
        if (fenString[i] == 'K') {
            whiteCanCastleKingside = true;
        }
        if (fenString[i] == 'Q') {
            whiteCanCastleQueenside = true;
        }
        if (fenString[i] == 'k') {
            blackCanCastleKingside = true;
        }
        if (fenString[i] == 'q') {
            blackCanCastleQueenside = true;
        }
        // if fenString[i] = '-', then nothing happens
        i++; // At the end of the loop, i will hold the position of the third blank space
    }
}

void ChessGame::decodePartFour(const char* fenString, int& i) {
    if (fenString[i] == '-') {
        i++;
    }
    else { // Only one en passant square can exist at any one time
        const char enPassantCoord[2] = {(static_cast<char>(toupper(fenString[i]))), fenString[i+1]};

        int* temporaryEnPassantSquare = coordToIndex(enPassantCoord);

        // Store as an attribute of ChessGame
        enPassantSquare[0] = temporaryEnPassantSquare[0];
        enPassantSquare[1] = temporaryEnPassantSquare[1];

        delete [] temporaryEnPassantSquare;
        temporaryEnPassantSquare = nullptr;

        i += 2; // i will hold the position of the fourth blank space
    }
}

void ChessGame::decodePartFive(const char* fenString, int& i) {
    i++;
    for (int count = 0; fenString[i] != ' '; count++) {
        halfMoveCounter += (fenString[i]-'0') * pow(10, count); // string -> base 10 integer
        i++; // At the end of the loop, i will hold the position of the fifth blank space
    }
}

void ChessGame::decodePartSix(const char* fenString, int& i) {
    i++;
    for (int count = 0; fenString[i] != '\0'; count++) { // Loop until end of FEN string
        fullMoveCounter += (fenString[i]-'0') * pow(10, count); // string -> base 10 integer
        i++;
    }
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


void ChessGame::submitMove(const char* stringCoord1, const char* stringCoord2) {

    // DEFENSIVE PROGRAMMING
    if (endGame) { // Detect whether game is still in progress
        cout << "\nGame is already over\n";
        gameLoaded = false;
        return;
    }
    if (!gameLoaded) { // Detect whether a game has been loaded
        cout << "\nA game has not been loaded\n";
        return;
    }

    // CONVERT STRING LITERAL CHESS COORDINATES TO INTEGERS (ZERO INDEXED)
    int* originCoord = coordToIndex(stringCoord1);
    int* destinationCoord = coordToIndex(stringCoord2);

    // OBTAIN POINTERS TO THE PIECES AT THE ORIGIN AND DESTINATION SQUARES
    ChessPiece* pieceAtOrigin = getPiece(originCoord);
    ChessPiece* pieceAtDestination = getPiece(destinationCoord);

    // INDICATE WHETHER THE DESTINATION SQUARE IS OCCUPIED
    if (pieceAtDestination != nullptr) {
        pieceAtDestinationSquare = true;
    }
    else {
        pieceAtDestinationSquare = false;
    }

    // FIRST DETERMINE WHETHER MOVE IS VALID DISREGARDING STATE OF CHECK
    if (checkMoveValid(originCoord, destinationCoord, stringCoord1, stringCoord2)) {

        // VALIDATE CASTLING
        if (castlingStatus != regularMove) { // castlingStatus is assigned in checkMoveValid()
            castle(originCoord, destinationCoord);
            cout << turn << " castles " << castlingStatus << "\n";
            castlingStatus = regularMove; // Reset castlingStatus
        }
        // VALIDATE REGULAR MOVE
        else if (!regularMoveLogic(originCoord, destinationCoord)) {
            cout << "Move " << stringCoord1 << " to " << stringCoord2 << " is not valid\n";
            return;
        }
        // REGULAR MOVE HAS BEEN VALIDATED
        else {
            cout << turn << "'s " << getPiece(destinationCoord)->getType() << " moves from " << stringCoord1 << " to " << stringCoord2;
        }

        toggleCastlingFlags(pieceAtOrigin, originCoord);

        // CAPTURE LOGIC
        if (pieceAtDestinationSquare) { // This will never be true for castling or en passant
            doCapture(pieceAtDestination);
        }
        else if (enPassantCapture) {
            int pawnCapturedRank = (turn == white ? 4 : 3);
            doCapture(chessBoard[pawnCapturedRank][destinationCoord[1]]);
        }

        // SET EN PASSANT SQUARE FOR NEXT TURN
        if (pieceAtOrigin->getType() == pawn && (abs(originCoord[0]-destinationCoord[0]) == 2)) {
            int offset = (turn == white ? -1 : 1);
            enPassantSquare[0] = originCoord[0] + offset;
            enPassantSquare[1] = originCoord[1];
        }
        else {
            enPassantSquare[0] = -1; // Indicates no en passant square present
        }

        if (pieceAtOrigin->getType() == pawn || pieceAtDestinationSquare) {
            // Reset half move counter if capture or pawn advance occured this turn
            halfMoveCounter = -1; // This will be incremented to zero just below
        }
        halfMoveCounter++;

        detectGameState();
        switchTurn();

        if (turn == white) {
            fullMoveCounter++;
        }
    }
    else {
        cout << "Move " << stringCoord1 << " to " << stringCoord2 << " is not valid\n";
    }

    delete [] originCoord;
    originCoord = nullptr;
    delete [] destinationCoord;
    destinationCoord = nullptr;

    // cout << "\n\n";
    // printBoard();
    // cout << "\n\n";
}

int* ChessGame::coordToIndex(const char* stringCoord) {

    int* indexArray = new int[2];

    indexArray[1] = stringCoord[0] - 'A'; // files are denoted by letters
    indexArray[0] = stringCoord[1] - '1'; // ranks are deonated by numbers

    return indexArray;
}

ChessPiece* ChessGame::getPiece(const int* coord) {
    return chessBoard[coord[0]][coord[1]];
}

bool ChessGame::checkMoveValid(const int* originCoord, const int* destinationCoord, const char* stringCoord1, const char* stringCoord2) {

    ChessPiece* pieceAtOrigin = getPiece(originCoord);
    ChessPiece* pieceAtDestination = getPiece(destinationCoord);

    if (!checkCoordinatesValid(originCoord, destinationCoord) || !checkPieceExists(pieceAtOrigin, stringCoord1)) {
        return false;
    }
    if (!checkCorrectTurn(pieceAtOrigin) || !checkPieceMoves(originCoord, destinationCoord)) {
        return false;
    }
    if (pieceAtDestination != nullptr && !checkNoFriendlyCapture(pieceAtDestination)) {
        return false;
    }
    // Check whether the player is attempting to castle (before checking the movement pattern)
    if ((pieceAtOrigin->getType() == king) && (originCoord[0] - destinationCoord[0] == 0) && (abs(originCoord[1] - destinationCoord[1]) == 2)) {
        return checkCastlingValid(castlingStatus, originCoord, destinationCoord);
    }
    if(!pieceAtOrigin->checkMovementPattern(originCoord, destinationCoord, stringCoord2)) {
        return false;
    }
    if ((pieceAtOrigin->getType() != knight) && !checkPathClear(originCoord, destinationCoord, stringCoord2)) {
        return false;
    }

    return true;
}

bool ChessGame::checkCoordinatesValid(const int* originCoord, const int* destinationCoord) {
    for (int i = 0; i < 2; i++) {
        if (originCoord[i] < 0 || originCoord[i] > 7 || destinationCoord[i] < 0 || destinationCoord[i] > 7) {
            cout << "Cannot make move - Coordinate out of bounds\n";
            return false;
        }
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

bool ChessGame::checkCorrectTurn(ChessPiece* pieceAtOrigin) {
    if (pieceAtOrigin->getColour() != turn) {
        cout << "It is not " << pieceAtOrigin->getColour() << "'s turn to move!\n";
        return false;
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

bool ChessGame::checkCastlingValid(CastlingStatus& castlingStatus, const int* originCoord, const int* destinationCoord) {

    // Set enum to kingside or queenside castle
    castlingStatus = (originCoord[1] < destinationCoord[1]) ? kingsideCastle : queensideCastle;

    // Player can castle by default. This is toggled elsewhere if respective rook or king have moved
    if (turn == white) {
        if ((castlingStatus == kingsideCastle && !whiteCanCastleKingside) || (castlingStatus == queensideCastle && !whiteCanCastleQueenside)) {
            cout << "You cannot castle if you have moved your king or rook\n"; 
            return false;
        }
    }
    if (turn == black) {
        if ((castlingStatus == kingsideCastle && !blackCanCastleKingside) || (castlingStatus == queensideCastle && !blackCanCastleQueenside)) {
            cout << "You cannot castle if you have moved your king or rook\n";
            return false;
        }
    }
    
    // Cannot castle out of check
    if ((turn == white && whiteInCheck) || (turn == black && blackInCheck)) {
        cout << "Cannot castle - " << turn << " is in check\n";
        return false;
    }
    
    // Cannot castle through check or other pieces
    int jump;
    jump = (castlingStatus == kingsideCastle) ? 1 : -1;
    int rank = originCoord[0];
    int file = originCoord[1] + jump;

    int count = 0;
    while (chessBoard[rank][file] == nullptr) {
        count++;
        if (detectCheck(rank, file, turn, false)) { // Check if square in check
            cout << "You cannot castle through check\n";
            return false;
        }
        file += jump;
    }    
    if (count != (castlingStatus == kingsideCastle ? 2 : 3)) {
        cout << "You cannot castle - there are pieces in the way\n";
        return false;
    }

    return true;
}

bool ChessGame::checkPathClear(const int* originCoord, const int* destinationCoord, const char* stringCoord2) {
    // NB: No boundary checks necessary as originCoord and destinationCoord will
    //     already have been validated when this function is called.

    if (originCoord[0] == destinationCoord[0]) { // If travelling along the same rank

        for (int i=1; i < abs(destinationCoord[1]-originCoord[1]); i++) { // Excludes destination square
            // Check path clear to the right
            if ((destinationCoord[1] > originCoord[1]) && (chessBoard[originCoord[0]][originCoord[1] + i] != nullptr)) {
                cout << "Path is not clear - ";
                generalCannotMoveOutput(getPiece(originCoord)->getType(), stringCoord2);
                return false;
            }
            // Check path clear to the left
            if ((destinationCoord[1] < originCoord[1]) && (chessBoard[originCoord[0]][originCoord[1] - i] != nullptr)) {
                cout << "Path is not clear - ";
                generalCannotMoveOutput(getPiece(originCoord)->getType(), stringCoord2);
                    return false;
            }
        }
    }

    else if (originCoord[1] == destinationCoord[1]) { // If travelling along the same file
            
        for (int i=1; i < abs(destinationCoord[0]-originCoord[0]); i++) { // Excludes destination square
            // Check path clear upwards
            if ((destinationCoord[0] > originCoord[0]) && (chessBoard[originCoord[0] + i][originCoord[1]] != nullptr)) {
                cout << "Path is not clear - ";
                generalCannotMoveOutput(getPiece(originCoord)->getType(), stringCoord2);
                return false;
            }
            // Check path clear downwards
            if ((destinationCoord[0] < originCoord[0]) && (chessBoard[originCoord[0] - i][originCoord[1]] != nullptr)) {
                cout << "Path is not clear - ";
                generalCannotMoveOutput(getPiece(originCoord)->getType(), stringCoord2);
                return false;
            }
        }
    }

    else if (abs(destinationCoord[0] - originCoord[0]) == abs(destinationCoord[1] - originCoord[1])) { // If travelling along a diagonal

        for (int i=1; i < abs(destinationCoord[0]-originCoord[0]); i++) {
            if ((destinationCoord[0] > originCoord[0]) && (destinationCoord[1] > originCoord[1])) { // (rank,file) = (+,+)
                if (chessBoard[originCoord[0]+i][originCoord[1]+i] != nullptr) {
                    cout << "Path is not clear - ";
                    generalCannotMoveOutput(getPiece(originCoord)->getType(), stringCoord2);
                    return false;
                }
            }
            if ((destinationCoord[0] < originCoord[0]) && (destinationCoord[1] < originCoord[1])) { // (rank,file) = (-,-)
                if (chessBoard[originCoord[0]-i][originCoord[1]-i] != nullptr) {
                    cout << "Path is not clear - ";
                    generalCannotMoveOutput(getPiece(originCoord)->getType(), stringCoord2);
                    return false;
                }
            }
            if ((destinationCoord[0] > originCoord[0]) && (destinationCoord[1] < originCoord[1])) { // (rank,file) = (+,-)
                if (chessBoard[originCoord[0]+i][originCoord[1]-i] != nullptr) {
                    cout << "Path is not clear - ";
                    generalCannotMoveOutput(getPiece(originCoord)->getType(), stringCoord2);
                    return false;
                }
            }
            if ((destinationCoord[0] < originCoord[0]) && (destinationCoord[1] > originCoord[1])) { // (rank,file) = (-,+)
                if (chessBoard[originCoord[0]-i][originCoord[1]+i] != nullptr) {
                    cout << "Path is not clear - ";
                    generalCannotMoveOutput(getPiece(originCoord)->getType(), stringCoord2);
                    return false;
                }
            }
        }
    }
    return true;
}

void ChessGame::castle(const int* originCoord, const int* destinationCoord) {

    makeMove(originCoord, destinationCoord); // Move the king

    int rookRank = (turn == white) ? 0 : 7;
    int rookFile = (castlingStatus == kingsideCastle) ? 7 : 0;
    int rookOriginCoord[2] = {rookRank, rookFile};
            
    int rookNewFile = (castlingStatus == kingsideCastle) ? 5 : 3;
    int rookDestinationCoord[2] = {rookRank, rookNewFile};

    makeMove(rookOriginCoord, rookDestinationCoord); // Move the rook
}

void ChessGame::makeMove(const int* originCoord, const int* destinationCoord) {

    if (chessBoard[originCoord[0]][originCoord[1]] != nullptr) { // Safety check

        chessBoard[destinationCoord[0]][destinationCoord[1]] = chessBoard[originCoord[0]][originCoord[1]]; // Make the move
        chessBoard[originCoord[0]][originCoord[1]] = nullptr;

        if (chessBoard[destinationCoord[0]][destinationCoord[1]]->getType() == king) {
            (turn == white ? whiteKing : blackKing) = chessBoard[destinationCoord[0]][destinationCoord[1]];
        }
    }
}

bool ChessGame::regularMoveLogic(const int* originCoord, const int* destinationCoord) {

    makeMove(originCoord, destinationCoord);
    ChessPiece* currentKing = ((turn == white) ? whiteKing : blackKing);

    // IF IN CHECK, THE MOVE MUST TAKE YOU OUT OF CHECK
    if ((turn == white && whiteInCheck) || (turn == black && blackInCheck)) {
        if (detectCheck(currentKing->getRankIndex(), currentKing->getFileIndex(), currentKing->getColour(), true)) {
            makeMove(destinationCoord, originCoord); // UNDO MOVE
            cout << "Cannot make move - you are in check.";
            return false;
        }
        else {
            turn == white ? whiteInCheck = false : blackInCheck = false;
        }
    }
    // IF NOT IN CHECK, YOU MUST NOT BE MOVING INTO CHECK
    else {
        if (detectCheck(currentKing->getRankIndex(), currentKing->getFileIndex(), currentKing->getColour(), true)) {
            makeMove(destinationCoord, originCoord); // UNDO MOVE
            cout << "Cannot make move - you cannot move into check";
            return false;
        }
    } 
    return true;
}

bool ChessGame::detectCheck(const int &rank, const int &file, const PieceColour &colour, const bool lookingAtKing) {

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

bool ChessGame::detectKnightInRange(const int &rank, const int &file, const PieceColour &colour) {

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

bool ChessGame::doesPieceSeeSquare(const int &rank, const int &file, const PieceColour &colour, const ChessPiece* nearestNeighbour, const Directions& direction) {

    if (nearestNeighbour != nullptr) {
        if (colour == nearestNeighbour->getColour()) { // Check if piece is friendly
            return false;
        }

        PieceType pieceName = nearestNeighbour->getType();

        // A queen can see infinitely far in all directions
        if (pieceName == queen) {
            return true;
        }

        // A king can see one square in any direction
        if ((pieceName == king) && (min(abs(nearestNeighbour->getRankIndex() - rank), abs(nearestNeighbour->getFileIndex() - file)) == 1)) {
            return true;
        }

        // A pawn can see one square diagonally
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
                if (pieceName == rook) { // rooks can see infinitely far along ranks and files
                    return true;
                }
                break;
            case plusplus:
            case minusminus:
            case plusminus:
            case minusplus:
                if (pieceName == bishop) { // bishops can see infinitely far along diagonals
                    return true;
                }
                break;
            default:
                return false;
        }
    }
    return false;
}

ChessPiece* ChessGame::findNearestNeighbour(const int &rank, const int &file, const PieceColour &colour, const Directions &direction) {

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

void ChessGame::toggleCastlingFlags(const ChessPiece* pieceAtOrigin, const int* originCoord) {

    if (pieceAtOrigin->getType() == king) {
        if (turn == white) {
            whiteCanCastleKingside = false;
            whiteCanCastleQueenside = false;
        }
        else {
            blackCanCastleKingside = false;
            blackCanCastleQueenside = false;
        }
    }
    else if (pieceAtOrigin->getType() == rook) {
        if (turn == white && originCoord[1] == 0) {
            whiteCanCastleKingside = false;                    
        }
        else if (turn == white && originCoord[1] == 7) {
            whiteCanCastleQueenside = false;
        }
        else if (turn == black && originCoord[1] == 0) {
            blackCanCastleKingside = false;
        }
        else if (turn == black && originCoord[1] == 7) {
            blackCanCastleQueenside = false;
        }
    }
}

void ChessGame::detectGameState() {

    bool checkDetected = false;

    // DETECT CHECK
    if (detectCheck(blackKing->getRankIndex(), blackKing->getFileIndex(), black, true) || detectCheck(whiteKing->getRankIndex(), whiteKing->getFileIndex(), white, true)) {
        checkDetected = true;
    }
    
    // DETECT CHECKMATE
    if (turn == black && whiteInCheck && detectCheckmate(whiteKing)) {
        cout << "\nWhite is in checkmate";
        endGame = true;
    }
    if (turn == white && blackInCheck && detectCheckmate(blackKing)) {
        cout << "\nBlack is in checkmate";
        endGame = true;
    }

    // DETECT STALEMATE
    if (turn == white && !blackInCheck && !anySafeSquares(blackKing) && !anyPiecesCanMove()) { // If no black pieces can move
        cout << "\nEnd of game - Stalemate";
        endGame = true;
    }
    if (turn == black && !whiteInCheck && !anySafeSquares(whiteKing) && !anyPiecesCanMove()) { // If no white pieces can move
        cout << "\nEnd of game - Stalemate";
        endGame = true;
    }

    // DETECT DRAW BY 50-MOVE RULE
    if (!endGame && halfMoveCounter == 100) {
        cout << "\nEnd of game - draw by 50-move rule\n";
        endGame = true;
    }

    // DETECT DRAW BY REPETITION

    if (!endGame && checkDetected) { // If game continues then output check message
        PieceColour oppositeTurn = ((turn == white) ? black : white);
        cout << "\n" << oppositeTurn << " is in check";
    }
}

bool ChessGame::anyPiecesCanMove() {

    // Iterate through the chess board
    for (int ranks = 0; ranks < 8; ranks++) {
        for (int files = 0; files < 8; files++) {

            ChessPiece* pieceToMove = chessBoard[ranks][files];

            // If (non-king) piece belongs to opponent
            if ((pieceToMove != nullptr) && (pieceToMove->getColour() != turn) && (pieceToMove->getType() != king)) {

                // Iterate through unit moves
                for (uint32_t move = 0; move < pieceToMove->getUnitMoves().size(); move++) {
                    int newRank = ranks + pieceToMove->getUnitMoves()[move][0];
                    int newFile = files + pieceToMove->getUnitMoves()[move][1];

                    int originCoord[2] = {ranks, files};
                    int destinationCoord[2] = {newRank, newFile};
                    if(pieceToMove->isValidMovePattern(originCoord, destinationCoord)) {
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

int* ChessGame::getEnPassantSquare() {
    return enPassantSquare;
}

void ChessGame::generalCannotMoveOutput(const PieceType pieceType, const char* stringCoord2) {
    cout << turn << "'s " << pieceType << " cannot move to " << stringCoord2 << "!\n";
}

void ChessGame::doCapture(ChessPiece* pieceToCapture) {
    cout << " taking " << pieceToCapture->getColour() << "'s " << pieceToCapture->getType();

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (chessBoard[i][j] == pieceToCapture) {
                chessBoard[i][j] = nullptr; // Avoid dangling pointer in chessBoard
                break;
            }
        }
    }

    if (enPassantCapture) {
        cout << " via en passant";
        enPassantCapture = false;
    }
    else {
        pieceAtDestinationSquare = false;
    }

    deletePiece(pieceToCapture);
}

void ChessGame::switchTurn() {
    cout << "\n";
    turn = (turn == white) ? black : white;
}

void ChessGame::deletePiece(ChessPiece* &pieceToDelete) {
    delete pieceToDelete;
    pieceToDelete = nullptr;
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

    for (uint32_t move = 0; move < piece->getUnitMoves().size(); move++) { // Iterate through all possible moves for the piece

        if (piece->getType() == rook || piece->getType() == bishop || piece->getType() == queen) {
            for (int multiplier = 1; multiplier <= 8; multiplier++) { // For pieces that can move any multiple of squares in their allowed directions

                int newRank = rank + multiplier*piece->getUnitMoves()[move][0];
                int newFile = file + multiplier*piece->getUnitMoves()[move][1];
                int destinationCoords[2] = {newRank, newFile};

                if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) { // Boundary checks
                    if (chessBoard[newRank][newFile] == nullptr) { // If square empty
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
//         {{king, black}, "♔"}, {{queen, black}, "♕"}, {{rook, black}, "♖"},
//         {{bishop, black}, "♗"}, {{knight, black}, "♘"}, {{pawn, black}, "♙"},
//         {{king, white}, "♚"}, {{queen, white}, "♛"}, {{rook, white}, "♜"},
//         {{bishop, white}, "♝"}, {{knight, white}, "♞"}, {{pawn, white}, "♟"}
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