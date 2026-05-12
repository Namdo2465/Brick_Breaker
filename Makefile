CC = clang

CFLAGS := -g $(shell pkg-config sdl2 SDL2_image --cflags)
LDFLAGS := $(shell pkg-config sdl2 SDL2_image --libs)

SOURCES = breakout.c game.c

EXECUTABLE = breakout

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)