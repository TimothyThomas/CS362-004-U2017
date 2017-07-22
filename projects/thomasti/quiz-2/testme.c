#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define LENGTH 5

char randStr[LENGTH+1];   // statically allocated variable to hold our random string (add 1 for \0)

char inputChar()
{
    // choose a random character from ASCII table (decimal values 32 thru 126)
    char r = 32 + rand() % (126 - 32 + 1);  // random int between 32 and 126
    return r;
}

char *inputString()
{
    int i;
    for (i = 0; i < LENGTH; i++) {
        int c = 97 + rand() % (122 - 97 + 1);  // random char between ASCII 97 and 122 (lowercase alphabet)
        randStr[i] = c; 
    }
    randStr[LENGTH] = '\0';
    return randStr;
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
