ifeq ($(OS),Windows_NT)
    RM = del /Q
    TARGET = cinemania.exe
    ifeq ($(SHELL),sh.exe)
        RM = del /Q
    endif
else
    RM = rm -f
    TARGET = cinemania
endif

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -O2
LDFLAGS = 
SOURCES = main.c utils.c movie.c fileio.c display.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)

main.o: main.c types.h movie.h display.h fileio.h utils.h
utils.o: utils.c utils.h types.h
movie.o: movie.c movie.h types.h utils.h
fileio.o: fileio.c fileio.h types.h utils.h movie.h
display.o: display.c display.h types.h utils.h movie.h

.PHONY: all clean
