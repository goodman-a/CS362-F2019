# Makefile 
# OSU CS 362 - Software Engineering II
# A. Goodman
# Assignment #5 


CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to enter: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

# Coverage Results from BOTH Random Tests & Unit Tests
testresults: randomtestcard1.c randomtestcard2.c randomtestcard3.c unittest1.c unittest2.c unittest3.c unittest4.c unittest5.c dominion.o rngs.o 
	echo "randomtestcard1.c:" >> testresults.out
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)
	echo "randomtestcard2.c:" >> testresults.out
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)
	echo "randomtestcard3.c:" >> testresults.out
	gcc -o randomtestcard3 -g randomtestcard3.c dominion.o rngs.o $(CFLAGS)	
	echo "unittest1.c:" >> testresults.out
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)
	echo "unittest2.c:" >> testresults.out
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)
	echo "unittest3.c:" >> testresults.out
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)	
	echo "unittest4.c:" >> testresults.out
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)	
	echo "unittest5.c:" >> testresults.out
	gcc -o unittest5 -g unittest5.c dominion.o rngs.o $(CFLAGS)	

	./randomtestcard1 >> testresults.out
	./randomtestcard2 >> testresults.out
	./randomtestcard3 >> testresults.out
	./unittest1 >> testresults.out
	./unittest2 >> testresults.out
	./unittest3 >> testresults.out
	./unittest4 >> testresults.out
	./unittest5 >> testresults.out

	gcov -c -b -f dominion.c >> testresults.out
	cat dominion.c.gcov >> testresults.out

# Coverage Results ONLY from Random Tests
randomtestresults: randomtestcard1.c randomtestcard2.c randomtestcard3.c dominion.o rngs.o 
	echo "randomtestcard1.c:" >> testresults.out
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)
	echo "randomtestcard2.c:" >> testresults.out
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)
	echo "randomtestcard3.c:" >> testresults.out
	gcc -o randomtestcard3 -g randomtestcard3.c dominion.o rngs.o $(CFLAGS)	

	./randomtestcard1 >> testresults.out
	./randomtestcard2 >> testresults.out
	./randomtestcard3 >> testresults.out

	gcov -c -b -f dominion.c >> testresults.out
	cat dominion.c.gcov >> testresults.out

# Coverage Results ONLY from Unit Tests
unittestresults: unittest1.c unittest2.c unittest3.c unittest4.c unittest5.c dominion.o rngs.o 
	echo "unittest1.c:" >> testresults.out
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)
	echo "unittest2.c:" >> testresults.out
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)
	echo "unittest3.c:" >> testresults.out
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)	
	echo "unittest4.c:" >> testresults.out
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)	
	echo "unittest5.c:" >> testresults.out
	gcc -o unittest5 -g unittest5.c dominion.o rngs.o $(CFLAGS)	

	./unittest1 >> testresults.out
	./unittest2 >> testresults.out
	./unittest3 >> testresults.out
	./unittest4 >> testresults.out
	./unittest5 >> testresults.out

	gcov -c -b -f dominion.c >> testresults.out
	cat dominion.c.gcov >> testresults.out


# Compiling Individual Tests for Working and Debugging
randomtestcard1: randomtestcard1.c dominion.o rngs.o
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o rngs.o
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)

randomtestcard3: randomtestcard3.c dominion.o rngs.o
	gcc -o randomtestcard3 -g randomtestcard3.c dominion.o rngs.o $(CFLAGS)	

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)

unittest5: unittest5.c dominion.o rngs.o
	gcc -o unittest5 -g unittest5.c dominion.o rngs.o $(CFLAGS)



# Compile for Playing & Full Test Results
all: playdom player testresults

clean:
	rm -f *.o playdom player testDrawCard randomtestcard1 *.exe *.gcov *.gcda *.gcno *.so *.out 
