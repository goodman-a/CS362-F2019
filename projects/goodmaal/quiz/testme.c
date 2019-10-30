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

char *inputString()
{
    // TODO: rewrite this function
    char randStr[6];
    int i = 0;

    char vowels[] = {'a','e','i','o','u'};
    randStr[0] = 'r';
    //randStr[1] = 'e';
    for(i=1; i<5; i++)
    {
      //note: z=122, t=116, e=191, a=97 
      randStr[i] = (rand()%(116-101+1))+101;
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
