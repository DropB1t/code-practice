#include "stdio.h"

void diff_abs(float, float);

int main(){
    float a,b;
    scanf("%f",&a);
    scanf("%f",&b);
    diff_abs(a-b,b-a);
    return 0;
}

void diff_abs(float a, float b){
    printf("%.2f\n",a);
    printf("%.2f\n",b);
    return;
}