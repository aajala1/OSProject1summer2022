p1: prog1.cpp my_const.h
	g++ -g prog1.cpp -o p1

p2: prog2.cpp my_const.h
	g++ -g prog2.cpp -o p2

clean:
	rm p1 p2

zip:
	zip named_pipe.zip *.cpp *.h Makefile