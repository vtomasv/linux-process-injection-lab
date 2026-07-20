# Laboratorio Avanzado de Process Injection en Linux (MAR404)

Este repositorio contiene un entorno Docker aislado con interfaz gráfica (noVNC) y laboratorios prácticos para demostrar técnicas avanzadas de inyección de procesos en sistemas Linux. Estas técnicas son los equivalentes directos a las metodologías utilizadas por APTs en Windows (MITRE ATT&CK T1055).

## Requisitos Previos

- Docker Desktop (Windows/Mac) o Docker Engine (Linux)
- Docker Compose

## Inicio Rápido

```bash
# 1. Clonar el repositorio
git clone https://github.com/vtomasv/linux-process-injection-lab.git
cd linux-process-injection-lab

# 2. Construir e iniciar el entorno
docker-compose up -d --build

# 3. Acceder a la interfaz gráfica
# Abre tu navegador en: http://localhost:8080
# Contraseña VNC: infected
```

## Estructura del Repositorio

```
linux-process-injection-lab/
├── docker/
│   ├── Dockerfile              # Imagen con GUI, compiladores y herramientas
│   └── supervisord.conf        # Configuración de servicios (VNC, noVNC, XFCE)
├── docker-compose.yml          # Orquestación con privilegios SYS_PTRACE
├── src/
│   ├── lab1_ptrace/            # T1055.001/003: Inyección clásica con ptrace
│   │   ├── target.c            # Proceso víctima
│   │   ├── ptrace_inject.c     # Inyector de shellcode (multi-arch)
│   │   └── Makefile
│   ├── lab2_ldpreload/         # T1055.001: DLL Injection equivalente
│   │   ├── malicious.c         # Librería maliciosa (.so)
│   │   └── Makefile
│   ├── lab3_memfd/             # T1055.012: Fileless / Process Ghosting
│   │   ├── memfd_exec.c        # Ejecutor en memoria
│   │   ├── dummy_payload.c     # Payload de demostración
│   │   └── Makefile
│   └── lab4_phishing_gui/      # Simulación Visual: Phishing + Ransomware
│       ├── email_simulator.py  # Simulador de correo (phishing)
│       ├── ransomware_gui.py   # GUI de ransomware (pantalla completa)
│       ├── injector.c          # Librería .so que lanza el ransomware
│       ├── launch_attack.sh    # Script que ejecuta el ataque completo
│       └── Makefile
├── docs/
│   └── GUIA_DEL_ESTUDIANTE.md  # Instrucciones paso a paso con debugging
└── README.md
```

## Laboratorios Disponibles

| Lab | Nombre | Técnica MITRE | Descripción |
|-----|--------|---------------|-------------|
| 1 | Ptrace Injection | T1055.001 / T1055.003 | Inyección de shellcode via ptrace (equivalente a WriteProcessMemory + CreateRemoteThread) |
| 2 | LD_PRELOAD | T1055.001 | Carga forzada de librería maliciosa (equivalente a DLL Injection) |
| 3 | memfd_create | T1055.012 | Ejecución fileless desde RAM (equivalente a Process Ghosting) |
| 4 | Phishing + Ransomware | T1566.001 + T1055 + T1486 | Simulación visual completa: correo → adjunto → inyección → ransomware en pantalla |

## Herramientas Preinstaladas en el Contenedor

| Herramienta | Propósito |
|-------------|-----------|
| `gcc` / `make` | Compilación de los programas en C |
| `gdb` | Debugger para inspeccionar registros y memoria |
| `strace` | Rastreo de llamadas al sistema (syscalls) |
| `ltrace` | Rastreo de llamadas a librerías dinámicas |
| `htop` | Monitor de procesos en tiempo real (visual, con árbol) |
| `lsof` | Listar archivos abiertos por un proceso |
| `objdump` / `readelf` | Análisis de binarios ELF |
| `hexdump` | Visualización hexadecimal de memoria |
| `mousepad` | Editor de texto (proceso legítimo víctima en Lab 4) |
| `python3-tk` | GUI para la simulación de ransomware |

## Instrucciones para los Alumnos

Una vez dentro del entorno gráfico (noVNC), abre el emulador de terminal y sigue las instrucciones detalladas en `docs/GUIA_DEL_ESTUDIANTE.md`.

**IMPORTANTE:** Todo el código debe compilarse y ejecutarse **dentro del contenedor Docker**, no en tu sistema operativo local (macOS/Windows).

Para entrar al contenedor desde tu terminal local:
```bash
docker exec -it injection-lab /bin/bash
```

## Detener el Entorno

```bash
docker-compose down
```
