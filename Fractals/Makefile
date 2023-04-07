CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags --libs gl glfw3 glew bobgl`
LDLIBS = `pkg-config --cflags --libs gl glfw3 glew bobgl`

SRC = helper.c window.c
OBJ = ${SRC:.c=.o}
EXE = fractals

all: build run clean

build: comp link

comp:
	$(CC) $(CFLAGS) $(SRC) -c $(CFLAGS)

link:
	$(CC) $(OBJ) -o $(EXE) $(CFLAGS)

run:
	./$(EXE)

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} ${EXE}
# END
