# Laboratorio Avanzado de Process Injection en Linux (MAR404)

Este repositorio contiene un entorno Docker aislado y laboratorios prácticos para demostrar técnicas avanzadas de inyección de procesos y malware "fileless" en sistemas Linux. 

Estas técnicas son los equivalentes directos a las metodologías utilizadas por APTs en Windows (T1055).

## Requisitos Previos
- Docker
- Docker Compose

## Inicio Rápido

1. Clona este repositorio o extrae el archivo.
2. Inicia el entorno:
   ```bash
   docker-compose up -d
   ```
3. Accede a la interfaz gráfica a través de tu navegador:
   **http://localhost:8080**
4. Contraseña de VNC: `infected`

## Estructura de los Laboratorios

El código fuente de los laboratorios se encuentra en la carpeta `src/`. Las guías paso a paso están en la carpeta `docs/`.

- **Lab 1: Ptrace Injection** (`src/lab1_ptrace`)
  - Equivalente a *Classic Process Injection* (T1055.001) y *Thread Execution Hijacking* (T1055.003).
  - Demuestra cómo un proceso puede adjuntarse a otro, modificar sus registros y escribir shellcode directamente en su memoria de ejecución.

- **Lab 2: LD_PRELOAD Injection** (`src/lab2_ldpreload`)
  - Equivalente a *DLL Injection* (T1055.001).
  - Demuestra cómo forzar a un proceso legítimo a cargar una librería compartida maliciosa (.so) antes que las librerías estándar.

- **Lab 3: Fileless Execution (memfd_create)** (`src/lab3_memfd`)
  - Equivalente a *Process Ghosting / Hollowing* (T1055.012).
  - Demuestra cómo crear un archivo anónimo directamente en RAM, escribir un binario en él y ejecutarlo, evadiendo la detección basada en disco.

## Instrucciones para los Alumnos
Abre la terminal dentro del entorno web (noVNC) y sigue las instrucciones detalladas en `docs/GUIA_DEL_ESTUDIANTE.md`.
