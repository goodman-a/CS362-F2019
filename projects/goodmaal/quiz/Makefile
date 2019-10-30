# Makefile 
# OSU CS 362 - Software Engineering II
# A. Goodman
# Assignment: random quiz


CFLAGS= -Wall -fpic -coverage -lm -std=c99

testmecoverage:
	echo "testme.c:" >> testmeresults.out
	gcc -o testme -g testme.c $(CFLAGS)
	./testme >> testmeresults.out
	gcov -c -b -f testme.c >> testmeresults.out
	cat testme.c gcov >> testmeresults.out

testme:
	gcc -o testme -g testme.c $(CFLAGS)	
	./testme

all: testme

clean:
	rm -f testme *.o *.exe  *.gcov *.gcda *.gcno *.so *.out 
