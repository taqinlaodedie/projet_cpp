all: 
	g++ -Wall -std=c++0x -I/usr/include/SDL2 -c *.cc 
	g++ -o main *.o -lSDL2_image -lSDL2 -lSDL2_ttf

clean:
	rm -f *.o
	rm -f main