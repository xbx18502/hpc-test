#include <iostream>
#include <vector>
#include <omp.h>

int main(){
    double t = 0.0;
    int* a = (int*)malloc(sizeof(int)*1000);
    for(int i=0;i<1000;i++){
        a[i]=i;
    }
    #pragma omp parallel for reduction(+:t)
    for (int i = 0; i < 1000; i++) {
        t += a[i];
    }

    printf("%f\n",t);


}