/*
На вход подаются 2 целочисленных массива.
Каждый элемент лежит в пределах
(-2 147 483 648, 2 147 483 647)
*/


#include <stdio.h>

enum { len = 20 };

long long U(int[len], int[len]);

long long U(int arr1[len], int arr2[len]) {
    long long tmp = 0, answer = 0;
    for(int i = 0; i < len - 5; ++i) {
        tmp += (long long)arr1[i] * (long long)arr2[i];
    }
    if (tmp > 0) {
        for(int j = 0; j < len; ++j) {
            answer += (long long)arr1[j] * (long long)arr1[j];
        }
    } else {
        for(int j = len/2 - 1; j < len; ++j) {
            answer += (long long)arr2[j] * (long long)arr2[j];
        }
    }
    return answer;
}

int main(void) {
    long long a = 0;
    int X[len] = {0}, Y[len] = {0};

    for(int i = 0; i < len; ++i) {
        scanf("%d", &X[i]);
    }
    for(int i = 0; i < len; ++i) {
        scanf("%d", &Y[i]);
    }
    a = U(X, Y);
    printf ("%lld\n", a);
    return 0;
}