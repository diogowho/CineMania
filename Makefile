CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -O2
LDFLAGS = 
TARGET = cinemania
SOURCES = utils.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

# Dependencies
utils.o: utils.c utils.h types.h

.PHONY: all clean
