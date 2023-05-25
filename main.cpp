#include <iostream>
#include <string>
#include <stdlib.h>

void task1()
{
    int num = 4494;
    char binary[sizeof(int) * 8 + 1];
    int it = 0;
    for (unsigned i = (1 <<  - 1); i > 0; i = i / 2)
        binary[it++] = (num & i) ? '1' : '0';
    binary[it] = '\0';
    printf("task1 = %s\n", binary);
}

void RemoveDups(char* str, int size)
{
    int j = 0;
    for(int i = 0; i < size; ++i)
    {
        if(str[i] == str[i+1])
            continue;
        str[++j] = str[i+1];
    }
}   

void task2()
{
    const int SIZE = 14;
    char data[SIZE] = "AAA BBB   AAA"; // have no \0 then i use size
    RemoveDups(data, SIZE);
    printf("task2 = %s\n", data); // "A B A"
}

int main()
{ 
    task1();
    task2();
    return 0;
}