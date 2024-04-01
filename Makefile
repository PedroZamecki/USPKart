all: game
	@echo "DONE"

INCLUDE=./include
SRC=./src

#Compiladores
CXX=g++
CC=gcc 

FLAGS  = -I$(INCLUDE)

game: game.cpp
	$(CXX) -O3 game.cpp $(FLAGS) $(SRC)/glad.c -lm -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_mixer -lGL -lGLU -ldl -o game

clean:
	$(RM) *~ *.o game -f
