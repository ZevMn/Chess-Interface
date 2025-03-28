#include"ChessGame.h"

#include <iostream>

using std::cout;

int main() {

	cout << "========================\n";
	cout << "Testing the Chess Engine\n";
	cout << "========================\n\n";

	ChessGame cg;
	cg.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	cout << '\n';

	cg.submitMove("D7", "D6");
	cout << '\n';

	cg.submitMove("D4", "H6");
	cout << '\n';

	cg.submitMove("D2", "D4");
	cout << '\n';

	cg.submitMove("F8", "B4");
	cout << '\n';

	cout << "=========================\n";
	cout << "Alekhine vs. Vasic (1931)\n";
	cout << "=========================\n\n";

	cg.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	cout << '\n';

	cg.submitMove("E2", "E4");
	cg.submitMove("E7", "E6");
	cout << '\n';

	cg.submitMove("D2", "D4");
	cg.submitMove("D7", "D5");
	cout << '\n';

	cg.submitMove("B1", "C3");
	cg.submitMove("F8", "B4");
	cout << '\n';

	cg.submitMove("F1", "D3");
	cg.submitMove("B4", "C3");
	cout << '\n';

	cg.submitMove("B2", "C3");
	cg.submitMove("H7", "H6");
	cout << '\n';

	cg.submitMove("C1", "A3");
	cg.submitMove("B8", "D7");
	cout << '\n';

	cg.submitMove("D1", "E2");
	cg.submitMove("D5", "E4");
	cout << '\n';

	cg.submitMove("D3", "E4");
	cg.submitMove("G8", "F6");
	cout << '\n';

	cg.submitMove("E4", "D3");
	cg.submitMove("B7", "B6");
	cout << '\n';

	cg.submitMove("E2", "E6");
	cg.submitMove("F7", "E6");
	cout << '\n';

	cg.submitMove("D3", "G6");
	cout << '\n';

	// =========================================
	// EXTRA TESTS..............................
	// =========================================

    cout << "========================\n";
    cout << "Testing Stalemate 1\n";
    cout << "========================\n\n";

    cg.loadState("7k/5Q2/6K1/8/8/8/8/8 b - - 0 1");
    cout << '\n';

    cg.submitMove("H8", "H7"); // Black King moves, leading to stalemate
    cout << '\n';

	cout << "========================\n";
    cout << "Testing Stalemate 2\n";
    cout << "========================\n\n";

    cg.loadState("7k/4Q3/6K1/8/8/8/8/8 w - - 0 1");
    cout << '\n';

    cg.submitMove("E7", "F7");
	cg.submitMove("H8", "H7");
    cout << '\n';
	

    cout << "========================\n";
    cout << "Testing Castling\n";
    cout << "========================\n\n";

    cg.loadState("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    cout << '\n';

    cg.submitMove("E1", "G1"); // White kingside castling
    cg.submitMove("E8", "C8"); // Black queenside castling
    cout << '\n';


    cout << "========================\n";
    cout << "Testing En Passant\n";
    cout << "========================\n\n";

    cg.loadState("4k3/8/8/3Pp3/8/8/8/4K3 w - e6 0 1");
    cout << '\n';

    cg.submitMove("D5", "E6"); // En passant capture
    cout << '\n';

	cout << "=====================================\n";
    cout << "Testing Escape Checkmate by capture\n";
    cout << "=====================================\n\n";

    cg.loadState("7k/8/8/8/8/8/8/6QK w - - 0 1");
    cout << '\n';

    cg.submitMove("G1", "G7"); // Queen moves to put king in check
	cg.submitMove("H8", "G7"); // King takes queen leading to draw by insufficient material (not implemented)
	cout << "\n";





	return 0;
}
