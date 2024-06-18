#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>  

double now()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_usec * 0.000001 + t.tv_sec);
}

int main(int argc, char *argv[])
{
    long N, num, i, j, sqn, p, k, kstart, kend;
    char *isprime;
    int *baseprimes;
    int numbaseprimes, b;
    double st, ft;

    N = 100000;
    b = 10000;
    if (argc > 1) 
      N = atol(argv[1]);
    if (argc > 2)
      b = atoi(argv[2]);

    isprime = malloc(N+1);
    if (isprime == NULL) {
        printf("Cannot allocate array\n");
        return -1;
    }
    sqn = sqrt(N);
    baseprimes = (int *)malloc(sqn * sizeof(int));
    if (baseprimes == NULL) {
        printf("Cannot allocate array\n");
        return -1;
    }

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
            for (j = i*i; j <= sqn; j += i)
                isprime[j] = 0;

    
    j = 0;
    for (i = 0; i <= sqn; i++)
        if (isprime[i] == 1)
            baseprimes[j++] = i;
    numbaseprimes = j;
    

    #pragma omp parallel for private(i,j, p, kstart, kend, k) 
    for (i = sqn + 1; i <= N; i += b)  {
        for (j = 0; j < numbaseprimes; j++) {
            p = baseprimes[j];
            if (p*p <= i + b) {
                if (i < p*p)
                    kstart = p*p;
                else 
                    kstart = ((i + p - 1) / p) * p;
                if (kstart % 2 == 0)
                    kstart += p;
                kend = (i + b) < N ? i + b : N;

                for (k = kstart; k <= kend; k += p) 
                    isprime[k] = 0;
            }
        }
    }

    num = 0;
    #pragma omp parallel for reduction(+:num)
    for (i = 2; i <= N; i++) 
        num += isprime[i];

    ft = now();

    printf("Number of primes : %ld (range 1 to %ld), time %f sec\n", num, N, ft-st);

    free(isprime);
    free(baseprimes);

    return 0;
}
