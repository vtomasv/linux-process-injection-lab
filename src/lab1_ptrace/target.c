#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("==========================================\n");
    printf("[Target] Proceso Legítimo Iniciado\n");
    printf("[Target] PID: %d\n", getpid());
    printf("==========================================\n");
    printf("[Target] Esperando inyección de código...\n");
    
    int counter = 0;
    while (1) {
        sleep(2);
        printf("[Target] Ejecutando tarea normal... (Iteración %d)\n", counter++);
    }
    
    return 0;
}
