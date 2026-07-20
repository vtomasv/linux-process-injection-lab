#include <stdio.h>
#include <unistd.h>

int main() {
    printf("\n[!!!] PAYLOAD EJECUTADO DESDE MEMORIA (FILELESS) [!!!]\n");
    printf("[*] Mi PID es: %d\n", getpid());
    printf("[*] Revisa 'htop' o 'ps aux'. Verás que mi nombre parece legítimo ('kworker/u4:2').\n");
    printf("[*] Revisa 'ls -l /proc/%d/exe'. Verás que apunta a un archivo borrado en memoria (memfd).\n\n", getpid());
    
    while(1) {
        sleep(5);
    }
    return 0;
}
