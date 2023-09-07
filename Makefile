all: build

build:
	g++ -c .\BFS.cpp
	g++ -c .\dstar.cpp
	g++ -c .\matriz.cpp
	g++ -Wall -I./include -L./lib  BFS.o dstar.o matriz.o main.cpp -o main -lraylib -lopengl32 -lgdi32 -lwinmm
	.\main.exe

clean:
	rm *.exe
	rm *.o