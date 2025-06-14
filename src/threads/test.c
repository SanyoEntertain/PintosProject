#include <stdio.h>
#include "fixed_point.h"

int main(){
    int n = 13;
    int fp = n_to_fp(n);

    printf("%d", n);
    printf("%d" ,fp);
    return 0;
}