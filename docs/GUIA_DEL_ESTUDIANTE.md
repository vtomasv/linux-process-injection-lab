# Guía del Estudiante: Laboratorios de Process Injection

Bienvenido al entorno de pruebas. Abre el emulador de terminal (Terminal Emulator) en el escritorio para comenzar.

## Laboratorio 1: Ptrace Injection (Equivalente a T1055.001 / T1055.003)

En este laboratorio usaremos `ptrace` para adjuntarnos a un proceso legítimo en ejecución, modificar su puntero de instrucción (RIP) e inyectar un shellcode malicioso en su memoria.

1. **Compilar el código:**
   ```bash
   cd ~/lab/src/lab1_ptrace
   make
   ```

2. **Ejecutar el proceso objetivo (Víctima):**
   Abre una terminal y ejecuta:
   ```bash
   ./target
   ```
   *Toma nota del PID que aparece en pantalla.*

3. **Inyectar el código (Atacante):**
   Abre una **segunda terminal** y ejecuta el inyector pasando el PID del objetivo:
   ```bash
   sudo ./ptrace_inject <PID_DEL_TARGET>
   ```

4. **Análisis:**
   - Observa la terminal del proceso objetivo. Verás que el flujo de ejecución fue secuestrado y ahora está ejecutando el shellcode (en este caso, abriendo una shell `/bin/sh` o cerrándose abruptamente si el shellcode termina).
   - Puedes usar `strace -p <PID>` antes de la inyección para ver cómo se comportaba el proceso legítimo.

---

## Laboratorio 2: LD_PRELOAD (Equivalente a DLL Injection)

En Windows, los atacantes fuerzan la carga de una DLL maliciosa. En Linux, podemos usar la variable de entorno `LD_PRELOAD` para forzar a cualquier comando a cargar nuestra librería compartida (`.so`) antes que `libc`.

1. **Compilar la librería maliciosa:**
   ```bash
   cd ~/lab/src/lab2_ldpreload
   make
   ```

2. **Ejecutar un comando legítimo con inyección:**
   Ejecuta cualquier comando inofensivo, por ejemplo `ls` o `cat`, pero anteponiendo nuestra librería:
   ```bash
   LD_PRELOAD=./malicious.so ls -la
   ```

3. **Análisis:**
   - Verás que **antes** de que `ls` liste los archivos, se ejecuta nuestro código malicioso (que imprime un mensaje y ejecuta `id`).
   - Esto ocurre porque definimos la función `init()` con `__attribute__((constructor))`, lo que hace que se ejecute inmediatamente al cargar la librería en memoria.

---

## Laboratorio 3: Fileless Execution con memfd_create (Process Ghosting)

Los atacantes modernos evitan tocar el disco duro para evadir el antivirus. En Linux, `memfd_create` permite crear un archivo directamente en la memoria RAM.

1. **Compilar el código:**
   ```bash
   cd ~/lab/src/lab3_memfd
   make
   ```

2. **Ejecutar el inyector Fileless:**
   ```bash
   ./memfd_exec ./dummy_payload
   ```

3. **Análisis Forense:**
   Abre una **nueva terminal** mientras el payload sigue corriendo.
   
   - Busca el proceso malicioso. Notarás que se ha disfrazado como un proceso del kernel:
     ```bash
     ps aux | grep kworker
     ```
   - Revisa de dónde se está ejecutando el binario:
     ```bash
     ls -l /proc/<PID_DEL_PAYLOAD>/exe
     ```
   - Verás algo como: `lrwxrwxrwx ... /proc/1234/exe -> /memfd:kworker/u4:2 (deleted)`.
   - El `(deleted)` indica que el archivo no existe en el disco duro, vive enteramente en la memoria RAM, lo que lo hace invisible para escaneos de disco tradicionales.
