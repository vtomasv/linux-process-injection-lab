/*
 * ============================================================
 * PROCESO OBJETIVO (VÍCTIMA)
 * ============================================================
 * Este programa simula un proceso legítimo en ejecución.
 * El atacante (ptrace_inject) se adjuntará a este proceso
 * y reemplazará su flujo de ejecución con shellcode malicioso.
 *
 * Equivalente en Windows: svchost.exe, explorer.exe, etc.
 * ============================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║   PROCESO LEGÍTIMO (VÍCTIMA)                     ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║   PID: %-40d  ║\n", getpid());
    printf("║   Estado: Ejecutando tarea normal                ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");
    printf("[*] Para inyectar, ejecuta en otra terminal:\n");
    printf("    sudo ./ptrace_inject %d\n\n", getpid());
    printf("[*] Iniciando bucle de trabajo normal...\n");
    printf("    (Si la inyección es exitosa, este bucle se detendrá)\n\n");

    int counter = 0;
    while (1) {
        sleep(2);
        printf("[Target] Iteración %d - Proceso ejecutándose normalmente...\n", counter++);
    }

    return 0;
}
