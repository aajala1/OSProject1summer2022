p1: player1.cpp my_const.h
	g++ -g player1.cpp -o p1

p2: player2.cpp my_const.h
	g++ -g player2.cpp -o p2

clean:
	rm p1 p2

zip:
	zip named_pipe.zip *.cpp *.h Makefile