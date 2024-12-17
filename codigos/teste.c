#include <stdio.h>

int main(){
   int a, b, pow = 1;

   printf("Digite o valor da base: ");
   scanf("%d",&a);

   printf("Digite o valor do expoente: ");
   scanf("%d",&b);

   for (int i = 0; i < b; i++)
   {
    pow = pow * a;
   }

   printf("%d", pow);
   
    
}