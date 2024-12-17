#include <stdio.h>

int main() {
    int total_segundos = 67702556;

    int horas = total_segundos / 3600;
    int minutos = (total_segundos % 3600) / 60;
    int segundos = total_segundos % 60;

    printf("%d horas, %d minutos e %d segundos\n", horas, minutos, segundos);

    return 0;
}
