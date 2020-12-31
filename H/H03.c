#include <stdio.h>

double avg(int);

double avg(int n) {
    double sum = 0, x, res;
    for(int i = 0; i < n; ++i) {
        scanf("%lf", &x);
        sum += x;
    }

    res = sum / n;
    return res;
}

int main(void)  {
    int n;
    scanf("%d", &n);
    printf("%lf\n", avg(n));
    return 0;
}
