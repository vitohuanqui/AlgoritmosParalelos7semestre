#include <iostream>
#include <random>
#include <time.h>

using namespace std;

#define MAX 1000
double A[MAX][MAX];
double X[MAX];
double Y[MAX];

int main()
{
    for (int i=0; i<MAX;i++){
        Y[i] = 0;
        X[i] = rand()%99;
        for (int j=0; j< MAX; j++){
            A[i][j] = rand()%99;
        }
    }
// 1654
    clock_t inicio, fin;
    inicio = clock();
    for (int i=0; i<MAX;i++){
        for (int j=0; j< MAX; j++)
            Y[i]+= A[i][j]*X[j];
    }

    fin = clock();
    cout<<(double)(fin - inicio) / CLOCKS_PER_SEC<<endl;
    inicio = clock();
    double acum = 0;
    for (int i=0; i<MAX;i++){
        for (int j=0; j< MAX; j++)
            acum+= A[i][j]*X[j];
        Y[i] = acum;
    }

    fin = clock();
    cout<<(double)(fin - inicio) / CLOCKS_PER_SEC<<endl;

// 1661

    inicio = clock();
    for (int j = 0; j<MAX; j++){
        for (int i =0; i<MAX; i++)
            Y[i]+=A[i][j]*X[j];
    }
    fin = clock();
    cout<<(double)(fin - inicio) / CLOCKS_PER_SEC<<endl;
    return 0;
}
