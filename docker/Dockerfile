FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV USER=student
ENV HOME=/home/$USER
ENV VNC_PORT=5900
ENV NOVNC_PORT=8080

# ============================================================
# FASE 1: Instalar paquetes base del sistema, GUI y herramientas
# ============================================================
RUN apt-get update && apt-get install -y \
    xfce4 \
    xfce4-terminal \
    mousepad \
    tightvncserver \
    novnc \
    websockify \
    python3 \
    python3-pip \
    python3-tk \
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
    bsdmainutils \
    binutils \
    file \
    lsof \
    && rm -rf /var/lib/apt/lists/*

# ============================================================
# FASE 2: Crear usuario con privilegios sudo
# ============================================================
RUN useradd -m -s /bin/bash $USER && \
    echo "$USER ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# ============================================================
# FASE 3: Configurar VNC (contraseña: infected)
# ============================================================
RUN mkdir -p $HOME/.vnc && \
    echo "infected" | vncpasswd -f > $HOME/.vnc/passwd && \
    chmod 600 $HOME/.vnc/passwd && \
    chown -R $USER:$USER $HOME/.vnc

# ============================================================
# FASE 4: Copiar configuración de supervisor
# ============================================================
COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

# ============================================================
# FASE 5: Configurar el entorno de trabajo
# ============================================================
WORKDIR /home/student/lab

EXPOSE 8080

CMD ["/usr/bin/supervisord", "-c", "/etc/supervisor/conf.d/supervisord.conf"]
