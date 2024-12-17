#include <stdio.h>
#include <stdbool.h>

// Escreva um programa para converter uma temperatura de graus Celsius para graus
// Fahrenheit, sendo que: C = (F - 32)/18 
float libra = 0.0;
float pes = 0.0;
float milhas = 0.0;
float celsius = 0.0;
int escolha = 0;
bool continuar = true;

int main(){
    while (continuar)
    {
        printf("\nDeseja converter o que? ");
        printf("\n[1] libras para newtons");
        printf("\n[2] pes para metros");
        printf("\n[3] milhas para quilometros");
        printf("\n[4] Celsius para graus fahrenheit");
        printf("\n[5] encerrar o sistema\n-> ");
        scanf("%d",&escolha);

        switch (escolha)
        {
        case 1:
            printf("Digite quantas libras voce quer converter para newtons: ");
            scanf("%f",&libra);

            float newtons = libra * 4.9;

            printf("\n%.2f libras em newtons sao %.2f",libra,newtons);
            break;
        case 2:
            printf("Digite quantos pes voce quer converter para metros: ");
            scanf("%f",&pes);

            float metros = pes * 3.29;

            printf("\n%.2f pes convertidos em metros sao %.2f",pes,metros);
            break;
        case 3:
            printf("Digite quantas milhas voce quer converter para km: ");
            scanf("%f",&milhas);

            float quilometros = milhas * 1.61;

            printf("\n%.2f milhas convertidas em km sao %.2f",milhas,quilometros);
            break;
        case 4:
            printf("Digite quantos graus celsius voce quer converter para fahrenheit: ");
            scanf("%f",&celsius);

            float fahrenheit = celsius * 1.8 + 32;

            printf("\n%.2f graus celsius convertidos em fahrenheit sao %.2f",celsius,fahrenheit);
            break;
        case 5:
            continuar = false;
            break;
        default:
            printf("Opçao invalida, aperte qualquer tecla para escolher denovo: ");
            scanf("%d",&escolha);
            break;
        }
      
    }

    printf("Obrigado por usar meu conversor...");
    

    return 0;
}