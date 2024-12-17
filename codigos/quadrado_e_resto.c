#include <stdio.h>
#include <math.h>

// - Escreva um programa em C que, dados dois números inteiros, imprima:
// • raiz quadrada da soma dos números;
// • resto da divisão do primeiro pelo segundo-

int primeiro = 0;
int segundo = 0;

int main(){
    printf("Digite o primeiro numero: ");
    scanf("%d",&primeiro);

    printf("\nDigite o segundo numero: ");
    scanf("%d",&segundo);

    printf("A raiz quadrada da soma dos numero digitados e: %.2f",sqrt(primeiro + segundo));
    return 0;
}