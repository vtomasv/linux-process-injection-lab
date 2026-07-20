FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV USER=student
ENV HOME=/home/$USER
ENV VNC_PORT=5900
ENV NOVNC_PORT=8080

# ============================================================
# FASE 1: Instalar paquetes base del sistema y GUI
# ============================================================
RUN apt-get update && apt-get install -y \
    xfce4 \
    xfce4-terminal \
    tightvncserver \
    novnc \
    websockify \
    python3 python3-pip \
    build-essential \
    gcc \
    g++ \
    make \
    gdb \
    gdbserver \
    strace \
    ltrace \
    htop \
    vim \
    nano \
    git \
    wget \
    curl \
    sudo \
    supervisor \
    x11vnc \
    xvfb \
    net-tools \
    procps \
    nasm \
    xterm \
    hexdump \
    binutils \
    file \
    lsof \
    && rm -rf /var/lib/apt/lists/*

# ============================================================
# FASE 2: Instalar herramientas de análisis adicionales
# ============================================================
RUN apt-get update && apt-get install -y \
    linux-tools-generic \
    elfutils \
    patchelf \
    && rm -rf /var/lib/apt/lists/* || true

# ============================================================
# FASE 3: Crear usuario con privilegios sudo
# ============================================================
RUN useradd -m -s /bin/bash $USER && \
    echo "$USER ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# ============================================================
# FASE 4: Configurar VNC (contraseña: infected)
# ============================================================
RUN mkdir -p $HOME/.vnc && \
    echo "infected" | vncpasswd -f > $HOME/.vnc/passwd && \
    chmod 600 $HOME/.vnc/passwd && \
    chown -R $USER:$USER $HOME/.vnc

# ============================================================
# FASE 5: Copiar configuración de supervisor
# ============================================================
COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

# ============================================================
# FASE 6: Configurar el entorno de trabajo
# ============================================================
WORKDIR /home/student/lab

# Deshabilitar ptrace_scope dentro del contenedor (permite inyección entre procesos)
RUN echo "kernel.yama.ptrace_scope = 0" >> /etc/sysctl.conf || true

EXPOSE 8080

CMD ["/usr/bin/supervisord", "-c", "/etc/supervisor/conf.d/supervisord.conf"]
