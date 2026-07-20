#!/usr/bin/env python3
"""
============================================================
SIMULADOR DE CORREO ELECTRÓNICO (PHISHING)
============================================================
Este script simula la bandeja de entrada de un empleado que
recibe un correo de phishing con un adjunto malicioso.

Cuando el alumno "abre" el adjunto, se ejecuta el payload
que simula una inyección de proceso con efecto visual.

MITRE ATT&CK:
  - T1566.001: Phishing: Spearphishing Attachment
  - T1204.002: User Execution: Malicious File
  - T1055: Process Injection
============================================================
"""

import os
import sys
import time
import subprocess

# Colores ANSI para la terminal
RED = "\033[91m"
GREEN = "\033[92m"
YELLOW = "\033[93m"
BLUE = "\033[94m"
CYAN = "\033[96m"
WHITE = "\033[97m"
BOLD = "\033[1m"
RESET = "\033[0m"

def clear_screen():
    os.system('clear')

def print_slow(text, delay=0.02):
    for char in text:
        sys.stdout.write(char)
        sys.stdout.flush()
        time.sleep(delay)
    print()

def show_inbox():
    clear_screen()
    print(f"""
{BLUE}╔══════════════════════════════════════════════════════════════════╗
║                    📧  CORREO CORPORATIVO                        ║
║                    Empresa: TechCorp S.A.                        ║
╠══════════════════════════════════════════════════════════════════╣
║  Bandeja de Entrada (3 mensajes nuevos)                         ║
╠══════════════════════════════════════════════════════════════════╣{RESET}
{WHITE}║                                                                  ║
║  1. {GREEN}[Leído]{WHITE}   De: soporte@techcorp.cl                           ║
║              Asunto: Actualización de contraseña WiFi            ║
║                                                                  ║
║  2. {GREEN}[Leído]{WHITE}   De: juan.perez@techcorp.cl                        ║
║              Asunto: Reunión de equipo mañana 10:00              ║
║                                                                  ║
║  3. {YELLOW}[NUEVO]{RED}   De: rrhh-liquidaciones@techcorp.cl                 ║
║              Asunto: ⚠️  Liquidación de Sueldo - Junio 2026      ║
║              📎 Adjunto: Liquidacion_Junio_2026.pdf.bin          ║{RESET}
{BLUE}║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝{RESET}
""")

def show_email():
    clear_screen()
    print(f"""
{BLUE}╔══════════════════════════════════════════════════════════════════╗
║  📧 Mensaje #3                                                   ║
╠══════════════════════════════════════════════════════════════════╣{RESET}
{WHITE}║  De:      rrhh-liquidaciones@techcorp.cl                         ║
║  Para:    empleado@techcorp.cl                                   ║
║  Fecha:   19 de Julio de 2026, 08:45                             ║
║  Asunto:  ⚠️  Liquidación de Sueldo - Junio 2026                 ║
╠══════════════════════════════════════════════════════════════════╣
║                                                                  ║
║  Estimado/a colaborador/a,                                       ║
║                                                                  ║
║  Adjuntamos su liquidación de sueldo correspondiente al mes      ║
║  de Junio 2026. Por favor revise los montos y confirme           ║
║  recepción respondiendo este correo.                             ║
║                                                                  ║
║  📎 Adjunto: {YELLOW}Liquidacion_Junio_2026.pdf.bin{WHITE}                     ║
║     Tamaño: 245 KB                                              ║
║                                                                  ║
║  Saludos cordiales,                                              ║
║  Departamento de Recursos Humanos                                ║
║  TechCorp S.A.                                                   ║
╠══════════════════════════════════════════════════════════════════╣{RESET}
{RED}║  ⚠️  INDICADORES DE PHISHING (para el analista):                  ║
║  • Extensión doble: .pdf.bin (ejecutable disfrazado)             ║
║  • Dominio sospechoso: rrhh-liquidaciones (no es rrhh@)          ║
║  • Urgencia implícita en el asunto                               ║
║  • Sin firma digital corporativa                                 ║{RESET}
{BLUE}╚══════════════════════════════════════════════════════════════════╝{RESET}
""")

def execute_payload():
    print(f"\n{YELLOW}[*] Descargando adjunto: Liquidacion_Junio_2026.pdf.bin ...{RESET}")
    time.sleep(1)
    print(f"{YELLOW}[*] Guardando en /tmp/Liquidacion_Junio_2026.pdf.bin ...{RESET}")
    time.sleep(0.5)
    print(f"{GREEN}[+] Descarga completada.{RESET}")
    time.sleep(0.5)
    
    print(f"\n{WHITE}[*] Abriendo documento...{RESET}")
    time.sleep(1)
    
    # Launch the legitimate notepad (mousepad) with a decoy file
    decoy_path = "/tmp/liquidacion_error.txt"
    with open(decoy_path, "w") as f:
        f.write("ERROR: No se pudo renderizar el documento PDF.\n")
        f.write("Por favor contacte a soporte técnico.\n")
        f.write("Código de error: 0x80070005\n")
    
    # Open the text editor (legitimate process)
    env = os.environ.copy()
    env["DISPLAY"] = ":1"
    notepad_proc = subprocess.Popen(
        ["mousepad", decoy_path],
        env=env,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )
    
    print(f"{WHITE}[*] Documento abierto en el visor (PID: {notepad_proc.pid}){RESET}")
    time.sleep(2)
    
    # Now simulate the malicious injection happening in the background
    print(f"\n{RED}{'='*60}")
    print(f"  ⚠️  ACTIVIDAD MALICIOSA EN SEGUNDO PLANO (invisible al usuario)")
    print(f"{'='*60}{RESET}")
    time.sleep(1)
    
    print(f"{RED}[MALWARE] Proceso legítimo capturado: mousepad (PID: {notepad_proc.pid}){RESET}")
    time.sleep(0.5)
    print(f"{RED}[MALWARE] Adjuntándose al proceso via ptrace...{RESET}")
    time.sleep(0.5)
    print(f"{RED}[MALWARE] Inyectando payload en memoria del proceso...{RESET}")
    time.sleep(0.5)
    print(f"{RED}[MALWARE] Ejecutando ransomware desde el contexto de mousepad...{RESET}")
    time.sleep(1)
    
    # Launch the ransomware GUI from the context of this simulation
    script_dir = os.path.dirname(os.path.abspath(__file__))
    ransomware_script = os.path.join(script_dir, "ransomware_gui.py")
    
    subprocess.Popen(
        ["python3", ransomware_script],
        env=env,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )
    
    time.sleep(2)
    print(f"\n{RED}{'='*60}")
    print(f"  💀 RANSOMWARE DESPLEGADO EN PANTALLA")
    print(f"  El proceso mousepad (PID: {notepad_proc.pid}) fue el vector.")
    print(f"{'='*60}{RESET}")
    
    print(f"\n{CYAN}{'='*60}")
    print(f"  🔍 EJERCICIO PARA EL ESTUDIANTE:")
    print(f"  Abre otra terminal y ejecuta los siguientes comandos")
    print(f"  para investigar qué está pasando:")
    print(f"")
    print(f"  1. htop  (presiona F5 para modo árbol)")
    print(f"  2. ps aux | grep -E 'mousepad|python|ransom'")
    print(f"  3. ls -l /proc/{notepad_proc.pid}/exe")
    print(f"  4. cat /proc/{notepad_proc.pid}/maps | grep rwx")
    print(f"  5. lsof -p {notepad_proc.pid}")
    print(f"{'='*60}{RESET}\n")

def main():
    show_inbox()
    input(f"\n{WHITE}Presiona ENTER para abrir el mensaje #3 (el sospechoso)...{RESET}")
    
    show_email()
    print(f"\n{YELLOW}¿Qué deseas hacer?{RESET}")
    print(f"  1. Abrir el adjunto (simular que el usuario cae en el phishing)")
    print(f"  2. Reportar como sospechoso (acción correcta)")
    print(f"  3. Salir")
    
    choice = input(f"\n{WHITE}Selecciona una opción [1-3]: {RESET}")
    
    if choice == "1":
        print(f"\n{RED}[!] El usuario ha caído en el phishing...{RESET}")
        execute_payload()
    elif choice == "2":
        print(f"\n{GREEN}[✓] ¡Correcto! Has identificado el phishing.{RESET}")
        print(f"{GREEN}    En un SOC real, este correo sería enviado al equipo de IR.{RESET}")
        print(f"{GREEN}    Pero para este laboratorio, selecciona la opción 1 para ver el ataque.{RESET}")
    else:
        print(f"\n{WHITE}Saliendo...{RESET}")
        sys.exit(0)

if __name__ == "__main__":
    main()
