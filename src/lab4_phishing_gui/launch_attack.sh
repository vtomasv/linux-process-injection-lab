#!/bin/bash
# ============================================================
# SCRIPT DE ATAQUE COMPLETO (Lab 4)
# ============================================================
# Este script simula lo que haría el adjunto malicioso:
# 1. Abre un documento señuelo en mousepad (proceso legítimo)
# 2. Inyecta la librería maliciosa via LD_PRELOAD
# 3. La librería lanza el ransomware desde el contexto de mousepad
#
# El estudiante debe ejecutar este script y luego usar
# herramientas forenses para descubrir la cadena de ataque.
# ============================================================

export DISPLAY=:1

echo ""
echo "╔══════════════════════════════════════════════════════════════╗"
echo "║  💀 SIMULACIÓN DE ATAQUE: Adjunto Malicioso                 ║"
echo "╠══════════════════════════════════════════════════════════════╣"
echo "║  Vector: Correo de phishing con adjunto .pdf.bin            ║"
echo "║  Técnica: T1055.001 (DLL/SO Injection via LD_PRELOAD)       ║"
echo "║  Impacto: T1486 (Ransomware)                                ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo ""

# Paso 1: Crear el documento señuelo
echo "[1/4] Creando documento señuelo..."
cat > /tmp/liquidacion_junio_2026.txt << 'EOF'
═══════════════════════════════════════════════
    LIQUIDACIÓN DE SUELDO - JUNIO 2026
    TechCorp S.A.
═══════════════════════════════════════════════

ERROR: No se pudo renderizar el documento PDF.

Código de error: 0x80070005 (ACCESS_DENIED)
Módulo: PDFRenderer.dll

Por favor contacte a soporte técnico:
soporte@techcorp.cl

═══════════════════════════════════════════════
EOF
sleep 1

# Paso 2: Compilar la librería maliciosa
echo "[2/4] Preparando payload (compilando librería maliciosa)..."
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
gcc -shared -fPIC -o /tmp/libmalware.so "$SCRIPT_DIR/injector.c" 2>/dev/null
if [ $? -ne 0 ]; then
    echo "[!] Error compilando. Asegúrate de tener gcc instalado."
    exit 1
fi
sleep 1

# Paso 3: Lanzar mousepad con la librería inyectada
echo "[3/4] Abriendo 'documento' en el visor (mousepad + inyección)..."
echo "      → LD_PRELOAD=/tmp/libmalware.so mousepad /tmp/liquidacion_junio_2026.txt"
LD_PRELOAD=/tmp/libmalware.so mousepad /tmp/liquidacion_junio_2026.txt &
MOUSEPAD_PID=$!
sleep 1

# Paso 4: Informar al estudiante
echo "[4/4] Ataque lanzado."
echo ""
echo "╔══════════════════════════════════════════════════════════════╗"
echo "║  ⏳ En 3 segundos aparecerá el ransomware en pantalla...    ║"
echo "║     (La librería inyectada en mousepad lo ejecutará)        ║"
echo "╠══════════════════════════════════════════════════════════════╣"
echo "║                                                              ║"
echo "║  🔍 EJERCICIO: Abre OTRA terminal y ejecuta:                ║"
echo "║                                                              ║"
echo "║  1. Ver el árbol de procesos:                                ║"
echo "║     ps auxf | grep -A2 mousepad                             ║"
echo "║                                                              ║"
echo "║  2. Ver la librería inyectada:                               ║"
echo "║     cat /proc/$MOUSEPAD_PID/maps | grep libmalware           ║"
echo "║                                                              ║"
echo "║  3. Ver archivos abiertos por mousepad:                      ║"
echo "║     lsof -p $MOUSEPAD_PID                                    ║"
echo "║                                                              ║"
echo "║  4. Ver las librerías cargadas:                              ║"
echo "║     cat /proc/$MOUSEPAD_PID/maps | grep '\.so'               ║"
echo "║                                                              ║"
echo "║  5. Detectar la anomalía con strace:                         ║"
echo "║     strace -p $MOUSEPAD_PID -e trace=write                   ║"
echo "║                                                              ║"
echo "║  PID de mousepad: $MOUSEPAD_PID                              ║"
echo "║  Para cerrar el ransomware: presiona ESC en su ventana       ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo ""

# Wait for mousepad to finish
wait $MOUSEPAD_PID 2>/dev/null
