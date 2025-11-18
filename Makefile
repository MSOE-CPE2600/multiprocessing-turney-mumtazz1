CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=-ljpeg


all: mandel movie

mandel: jpegrw.o mandel.o
    $(CC) -o mandel jpegrw.o mandel.o

movie: jpegrw.o mandelmovie.o
    $(CC) -o movie jpegrw.o mandelmovie.o


clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<
