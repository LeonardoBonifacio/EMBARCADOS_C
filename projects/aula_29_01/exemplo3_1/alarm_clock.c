#include <stdio.h>
#include "pico/stdlib.h"

// Função de callback que será chamada pelo alarme
int64_t alarm_callback(alarm_id_t id, void *user_data) {
    uint32_t start_ms = to_ms_since_boot(get_absolute_time());
    printf("tempo absoluto do alarm: %d \n", start_ms);
    // Pode retornar um valor em microssegundos para retornar no futuro.
    return 0;
}

int main() {

    stdio_init_all();

    // Chama alarm_callback com temporização de 5000 ms
    alarm_id_t alarm = add_alarm_in_ms(5000, alarm_callback, NULL, false);

    if (!alarm) {
        printf("Failed to add timer\n");
    }
    
    while(1){
        sleep_ms(1000);
        uint32_t start_ms = to_ms_since_boot(get_absolute_time());
        printf("tempo absoluto - rotina While %d \n", start_ms);
    }
}