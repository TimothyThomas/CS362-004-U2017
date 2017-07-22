CFLAGS = -Wall -lm

testme: testme.c
	gcc -o testme testme.c $(CFLAGS)

run: testme
	./testme

all: testme run 

clean:
	rm -f *.o testme
