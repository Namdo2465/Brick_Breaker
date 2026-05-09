CC = clang

CFLAGS = -g -Wall -fsanitize=address $(shell pkg-config --cflags sdl2)

LDFLAGS = $(shell pkg-config --libs sdl2) -lSDL2_image

SOURCES = breakout.c

EXECUTABLE = breakout

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)