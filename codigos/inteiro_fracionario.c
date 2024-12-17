#include <stdio.h>

// Escreva um programa que, dado um número do tipo double, consiga separar a
// parte inteira da parte fracionária. Exemplo: Número: 237.48, Parte inteira: 237, Parte
// fracionária: 0.48 
float numero = 0.0;

int main(){
    printf("Digite um numero quebrado para a separacao da parte fracinaria da inteira: ");
    scanf("%f",&numero);
    int inteira = numero;
    double fracionaria = numero - inteira;
    printf("\nParte inteira -> %d",inteira);
    printf("\nParte fracionaria -> %.2f", fracionaria);
    return 0;
}