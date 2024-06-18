#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

double now()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_usec * 0.000001 + t.tv_sec);
}

int main(int argc, char *argv[])
{
    long N, num, i, j, sqn;
    char *isprime;
    double st, ft;

    N = 100000;
    if (argc == 2)
        N = atol(argv[1]);

    isprime = malloc(N+1);
    if (isprime == NULL) {
        printf("Cannot allocate array\n");
        return -1;
    }
    sqn = sqrt((double)(N));

    st = now();
    #pragma omp parallel for
    for (i = 0; i <= N; i++)
        isprime[i] = 1;

    isprime[0] = 0;
    isprime[1] = 0;
    #pragma omp parallel for
    for (i = 4; i <= N; i+=2)
        isprime[i] = 0;
    #pragma omp parallel for schedule(dynamic,1) 
    for (i = 3; i <= sqn; i += 2) 
        if (isprime[i] == 1)
            for (j = i*i; j <= N; j+= i)
                isprime[j] = 0;

    num = 0;
    #pragma omp parallel for reduction(+:num)
    for (i = 2; i <= N; i++) 
        num += isprime[i];

    ft = now();

    printf("Number of primes : %ld (range 1 to %ld), time %f sec\n", num, N, ft-st);

    free(isprime);

    return 0;
}

/*
gcc -fopenmp -O3 prog1.c -o prog1.elf -lm
*/
