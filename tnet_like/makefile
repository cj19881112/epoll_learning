CFLAGS=-g
LDFLAGS=-lpthread

all : clean main

main : $(shell ls *.cpp *.h)
	g++ -o $@ $(CFLAGS) $(LDFLAGS) $^

clean :
	rm main -f

