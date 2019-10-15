#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void loop();

int main()
{
    while(1)
    {
        loop();
    }
    return 0;
}

void loop()
{
    if(kbhit())
    {
        char carlu = 0;
        carlu = getch();
        if(carlu == ':')
        {
           exit(0); 
        }
        else
        {
           printf("lettre : %d\n",carlu); 
        }
    }
    
}