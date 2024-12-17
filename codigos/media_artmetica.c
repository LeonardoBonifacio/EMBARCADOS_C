#include <stdio.h>

// Escreva um programa que calcule a média aritmética de todos os números entre 13 e 25. 


int main(){
    int soma = 0;
    int count = 0;
   for (int i = 13; i < 26; i++)
   {
      soma += i;
      count++;
   }
   int media = soma/count;

   printf("A media aritmetica entre todos os numero de 13 a 26 e: %d", media);

   13
}