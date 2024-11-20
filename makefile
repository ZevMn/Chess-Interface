chess: main.o chessgame.o chesspiece.o
	g++ -g main.o chessgame.o chesspiece.o -o chess

main.o: ChessMain.cpp ChessPiece.h ChessGame.h
	g++ -Wall -g -c ChessMain.cpp

chessgame.o: ChessGame.cpp ChessGame.h
	g++ -Wall -g -c ChessGame.cpp

chesspiece.o: ChessPiece.cpp ChessPiece.h
	g++ -Wall -g -c ChessPiece.cpp

clean:
	rm -f *.o chess

