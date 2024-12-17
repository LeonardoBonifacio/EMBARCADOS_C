#include <stdio.h>

// Uma revendedora de carros usados paga a seus funcionários vendedores, um
// salário fixo por mês, uma comissão, também fixa, para cada carro vendido e mais 5% do valor
// das vendas por eles efetuadas. Escreva um programa que calcule o salário de um funcionário
// que vendeu 5 carros de R$19.999,00, 2 de R$28.750,00 e 1 de R$46.700,00. O salário fixo é de
// R$1.500,00 e a comissão fixa é de R$99,95


int main(){
    const float salario_fixo = 1500.00;
    const float comissao = 99.95; // por carro vendido + 5% do valor das vendas por ele efetuado
    float total_vendas = 5 * 19999 + 2 * 28750 + 46700;
    float salario_total = salario_fixo + (comissao * 8) + total_vendas * 5/100;
    printf("%f",total_vendas);
    printf("salario_total: %f",salario_total);

}