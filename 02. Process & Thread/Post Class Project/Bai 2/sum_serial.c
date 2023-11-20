#include <stdio.h>
#include <stdlib.h>

int main(int argv, char* argc[])
{
    int MAX_NUMBER = atoi(argc[1]);
    unsigned long long sum = 0;

    for(int i=1; i<=MAX_NUMBER; i++)
        sum += i;

    printf("SUM: %lld\n", sum);

    return 0;
}