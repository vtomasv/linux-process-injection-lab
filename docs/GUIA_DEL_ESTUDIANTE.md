# Guía Avanzada del Estudiante: Debugging y Análisis de Process Injection

Bienvenido al laboratorio de Threat Hunting. Esta guía te enseñará paso a paso no solo cómo ejecutar las técnicas de inyección, sino **cómo observar el comportamiento de la memoria y los procesos en tiempo real** utilizando las herramientas gráficas y de consola preinstaladas.

Para comenzar, abre tu navegador web y entra a `http://localhost:8080`. Ingresa la contraseña `infected`.
Una vez en el escritorio XFCE, abre **tres ventanas de terminal** (Terminal Emulator).

---

## Laboratorio 1: Ptrace Injection (T1055.001 / T1055.003)

En este laboratorio inyectaremos código en un proceso en ejecución y usaremos herramientas para ver el cambio exacto en la memoria.

### Paso 1: Preparación
En la **Terminal 1**, compila el código:
```bash
cd ~/lab/src/lab1_ptrace
make
```

### Paso 2: Ejecutar el proceso objetivo
En la misma **Terminal 1**, ejecuta el programa víctima:
```bash
./target
```
*Anota el PID que se muestra en pantalla.*

### Paso 3: Monitoreo en Tiempo Real (strace y htop)
Ve a la **Terminal 2**. Vamos a observar qué está haciendo el proceso antes de ser inyectado.
Ejecuta `htop`, presiona `F4` (Filter) y escribe el nombre `target`. Verás el proceso corriendo pacíficamente.
Presiona `q` para salir de htop.

Ahora, rastreemos sus llamadas al sistema (Syscalls):
```bash
strace -p <PID_DEL_TARGET>
```
Verás que el proceso está llamando repetidamente a `nanosleep` y `write` (imprimiendo en pantalla). Deja esta terminal abierta.

### Paso 4: Análisis de Memoria antes de la Inyección
Ve a la **Terminal 3**. Vamos a ver el mapa de memoria del proceso:
```bash
cat /proc/<PID_DEL_TARGET>/maps
```
Busca las líneas que tienen permisos `r-xp` (lectura y ejecución). Ese es el código legítimo del programa.

### Paso 5: ¡Inyección!
Abre una **Terminal 4** y ejecuta el script de inyección:
```bash
sudo ./ptrace_inject <PID_DEL_TARGET>
```

### Paso 6: Análisis Post-Inyección (Debugging)
1. Mira la **Terminal 1**: El programa legítimo dejó de imprimir su mensaje normal y ahora el shellcode inyectado tomó el control.
2. Mira la **Terminal 2** (`strace`): Verás que el flujo normal de `nanosleep` se interrumpió y de repente apareció una llamada a `execve("/bin/sh")`. ¡Esa es la inyección!

---

## Laboratorio 2: LD_PRELOAD (Equivalente a DLL Injection)

En este laboratorio, secuestraremos la ejecución de un comando estándar (`ls`) forzándolo a cargar nuestra librería maliciosa.

### Paso 1: Preparación
En la **Terminal 1**:
```bash
cd ~/lab/src/lab2_ldpreload
make
```

### Paso 2: Ejecución Normal vs Maliciosa
Ejecuta un `ls` normal:
```bash
ls -la
```

Ahora, ejecuta el mismo comando forzando la inyección de la librería:
```bash
LD_PRELOAD=./malicious.so ls -la
```
Verás que el mensaje malicioso se imprime **antes** de que el comando `ls` haga su trabajo.

### Paso 3: Debugging con `ltrace`
Para ver exactamente cómo la librería intercepta la ejecución, usaremos `ltrace` (Library Trace).
```bash
ltrace -S LD_PRELOAD=./malicious.so ls
```
Observa el output. Verás que la función `init()` de nuestra librería se ejecuta durante la fase de carga dinámica del sistema operativo, antes de que se llame a la función `main()` de `ls`.

---

## Laboratorio 3: Fileless Execution con memfd_create (Process Ghosting)

Esta técnica ejecuta malware directamente en la RAM sin tocar el disco.

### Paso 1: Preparación
En la **Terminal 1**:
```bash
cd ~/lab/src/lab3_memfd
make
```

### Paso 2: Ejecutar el inyector Fileless
```bash
./memfd_exec ./dummy_payload
```

### Paso 3: Análisis Forense de Memoria
Ve a la **Terminal 2**. El malware está corriendo, pero se ha disfrazado.
Vamos a buscarlo en los procesos:
```bash
ps aux | grep kworker
```
Verás un proceso llamado `kworker/u4:2`. Parece un proceso legítimo del kernel de Linux, pero es nuestro malware. Toma nota de su PID.

### Paso 4: Descubriendo el engaño en `/proc`
Los Threat Hunters usan el sistema de archivos `/proc` para descubrir estas anomalías.
Ejecuta:
```bash
ls -l /proc/<PID_DEL_MALWARE>/exe
```
El resultado será algo como:
`lrwxrwxrwx 1 student student 0 ... /proc/<PID>/exe -> /memfd:kworker/u4:2 (deleted)`

**¿Qué significa esto?**
El enlace simbólico `exe` apunta al archivo ejecutable original. Al decir `(deleted)` y apuntar a `/memfd:`, nos confirma que este proceso no se ejecutó desde el disco duro, sino desde un archivo anónimo creado en la memoria RAM. ¡Acabas de cazar un malware fileless!

---

## Laboratorio 4: Simulación Visual (Phishing + Ransomware)

Este laboratorio es altamente visual y simula un ataque completo del mundo real: un usuario recibe un correo de phishing, abre un adjunto malicioso y este inyecta código en un proceso legítimo para desplegar un Ransomware en toda la pantalla.

### Paso 1: Ejecutar el Simulador de Correo
En la **Terminal 1**, navega a la carpeta del Lab 4 y ejecuta el simulador:
```bash
cd ~/lab/src/lab4_phishing_gui
python3 email_simulator.py
```

### Paso 2: Caer en la trampa
1. Presiona `ENTER` para abrir el correo sospechoso de "Recursos Humanos".
2. Lee los indicadores de phishing marcados en rojo.
3. Presiona `1` para simular que el usuario decide abrir el archivo adjunto.

### Paso 3: Observar el Ataque Visual
- Verás que se abre el **bloc de notas legítimo de Linux (`mousepad`)** mostrando un supuesto error al leer el PDF. El usuario pensará "oh, el archivo está dañado" y lo dejará abierto o intentará cerrarlo.
- **¡Pero la inyección ya ocurrió!** En segundo plano, el adjunto malicioso inyectó una librería en `mousepad`.
- Unos segundos después, **la pantalla completa se pondrá roja con un mensaje de Ransomware**.
- *(Nota: No te asustes, es una simulación. No cifrará tus archivos reales. Puedes presionar `ESC` o `q` en cualquier momento para cerrar la pantalla roja).*

### Paso 4: Análisis Forense (Threat Hunting)
Deja la pantalla roja de ransomware abierta (o muévela a un lado si tu gestor de ventanas lo permite, aunque está en pantalla completa, puedes abrir otra terminal con `Ctrl+Alt+T` o cambiar de espacio de trabajo).

En la **Terminal 2**, vamos a cazar al proceso malicioso:

**A. Ver el árbol de procesos:**
```bash
htop
```
*(Presiona `F5` para activar la vista de árbol. Busca `mousepad`. Verás que el script de python del ransomware está "colgando" como hijo directo de `mousepad`. ¡El bloc de notas está ejecutando el ransomware!)*. Presiona `q` para salir.

**B. Ver los archivos abiertos por el proceso legítimo:**
Primero, obtén el PID de `mousepad`:
```bash
ps aux | grep mousepad
```
Luego usa `lsof` (reemplaza `<PID>`):
```bash
lsof -p <PID>
```
Busca en la lista de archivos. Deberías ver que `mousepad` tiene cargada una librería sospechosa llamada `libmalware.so` que normalmente no debería estar allí.

**C. Revisar la memoria mapeada:**
```bash
cat /proc/<PID>/maps | grep libmalware
```
Esto confirma que la librería maliciosa fue inyectada en el espacio de memoria del proceso legítimo, logrando evadir la detección inicial al esconderse dentro de un programa confiable.

Para terminar, vuelve a la pantalla del Ransomware y presiona `ESC` para cerrarla.
