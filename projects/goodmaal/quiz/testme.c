/*
 * Course: OSU CS 362 - Software Engineering II
 * Assignment: Random Testing Quiz
 * Author: Alexander Goodman
 * Due Date: 10 November 2019
 * 
 * file: testme.c
 * description: 
 * 
 */ 

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    // TODO: rewrite this function
    // ascii characters go from 32 (space) to 126 (~)
    int randVal = (rand()%(126-32+1))+32;
    //char randChar = randVal + '0';

    return randVal;
}

char RandomLetter()
{
   /*
    // a = 81
    // e = 127
    // h = 61
    // i = 70
    // n = 67
    // o = 75
    // r = 60
    // s = 63
    // t = 90

    */

    // value combination a=[0-80], e=[81-207], t=[208-297], common=[298-697], uncommon[698-999]
    char common[] = {'h', 'i', 'n', 'o', 'r', 's'};
    char uncommon[] = {'b','c','d','f','g','j','k','l','m','p','q','u','v','w','x','y','z'};

    int randomVal = (rand()%(1000-0))+0;
    char c;

    if(randomVal < 81)
    {
      c = 'a';
    }
    else if(randomVal >=81 && randomVal < 208)
    {
      c = 'e';
    }
    else if (randomVal >= 208 && randomVal < 298)
    {
      c = 't';
    }
    else if(randomVal >= 298 && randomVal < 698)
    {
      c = common[(rand()%(6-0))+0];
    }
    else
    {
      c = uncommon[(rand()%(17-0))+0] ; 
    }

    return c;

}

char *inputString()
{
    // TODO: rewrite this function
    char randStr[6];
    int i = 0;

    //randStr[0] = 'r';
    //randStr[1] = 'e';
    for(i=0; i<5; i++)
    {
      // Range that includes the lowest letter, c, and highest, t)
      // note: z=122, t=116, e=191, a=97 
      //randStr[i] = (rand()%(116-101+1))+101; 

      // Using a Weighted Random Generator based off of frequency in the english language:
      // source: https://en.wikipedia.org/wiki/Letter_frequency
      randStr[i] = RandomLetter();

    }

    randStr[5] = '\0'; //null terminator
  
    // Store Random String
    char* randomstring;
    randomstring = randStr;
 
    // Store Set String -- allows to hit condition
    //char* randomstring = "reset";

  
   return randomstring;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    //state = 9;
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);
    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
