CC = gcc
CFLAGS = -Wall -g -c
LDFLAGS = -ljpeg

EXECS = mandel movie
OBJS = jpegrw.o mandel.o mandelmovie.o

all: $(EXECS)

mandel: jpegrw.o mandel.o
	$(CC) -o $@ $^ $(LDFLAGS)

movie: jpegrw.o mandelmovie.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(EXECS) *.d