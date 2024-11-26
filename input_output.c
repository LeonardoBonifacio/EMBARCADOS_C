#include <stdio.h>


int main(int argc, char const *argv[])
{
    /*
    int variavel,variavel2;

    printf("Digite um numero inteiro:  ");
    scanf("%d",&variavel);

    printf("Digite outro numero inteiro:  ");
    scanf("%d",&variavel2);// tipo de dado e endereço de memoria da variavel a ser lida
    printf("\n\nA soma dos dois numeros digitados e %d\n\n",variavel + variavel2);
    

   float numero = 3.1415f;
   printf("Valor da minha variavel: %.2f\n", numero);
   

   printf("\nDigite um novo valor para a variavel: ");
   scanf("%f", &numero);
   printf("\nO valor digitado foi %.2f", numero);

   

  char sexo = 'M';

  printf("Valor da variavel sexo: %c\n", sexo);

  printf("\nDigite seu sexo: (M/F/NB)");
  scanf("%c", &sexo);
  printf("Seu sexo e: %c", sexo);

  

  char letra;
  printf("Digite uma letra: ");
  letra = getchar();

  printf("Caracter lido: %c\n", letra);
  

  char letra;

  printf("Digite 1 caracter: ");
  letra = getc(stdin);// leitura deve ser feita pela entrada padrão do computador, o teclado nesse caso

  printf("O caracter digitado foi: %c", letra);
  */
  char letra;

  printf("Digite um caracter: ");
  letra = fgetc(stdin);// ou le de um arquivo(especificando o nome) ou da entrada padrão do computador
  printf("O caracter digitado foi: %c", letra);
  return 0;
}
