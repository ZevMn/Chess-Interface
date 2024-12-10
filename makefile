chess: ChessMain.o ChessGame.o ChessPiece.o
	g++ -g ChessMain.o ChessGame.o ChessPiece.o -o chess

ChessMain.o: ChessMain.cpp ChessPiece.h ChessGame.h Enums.h
	g++ -Wall -g -c ChessMain.cpp

ChessGame.o: ChessGame.cpp ChessGame.h Enums.h
	g++ -Wall -g -c ChessGame.cpp

ChessPiece.o: ChessPiece.cpp ChessPiece.h Enums.h
	g++ -Wall -g -c ChessPiece.cpp

clean:
	rm -f *.o chess

