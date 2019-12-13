all: 
	g++ -Wall -I/usr/include/SDL2 -c *.cc 
	g++ -o main *.o -lSDL2_image -lSDL2 -lpthread

clean:
	rm -f *.o
	rm -f main