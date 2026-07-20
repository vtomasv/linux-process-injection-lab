FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV USER=student
ENV HOME=/home/$USER
ENV VNC_PORT=5900
ENV NOVNC_PORT=8080

# Install core utilities, development tools, and GUI
RUN apt-get update && apt-get install -y \
    xfce4 xfce4-goodies \
    tightvncserver \
    novnc \
    websockify \
    python3 python3-pip \
    build-essential \
    gdb \
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
    radare2 \
    nasm \
    xterm \
    firefox \
    && rm -rf /var/lib/apt/lists/*

# Create user with sudo privileges
RUN useradd -m -s /bin/bash $USER && \
    echo "$USER ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# Setup VNC password (password: infected)
RUN mkdir -p $HOME/.vnc && \
    echo "infected" | vncpasswd -f > $HOME/.vnc/passwd && \
    chmod 600 $HOME/.vnc/passwd && \
    chown -R $USER:$USER $HOME/.vnc

# Copy supervisor config
COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

# Allow ptrace for non-child processes in the container (simulated via cap-add in compose, but we set sysctl if possible)
# Note: sysctl requires privileged mode, handled in docker-compose.yml

WORKDIR /home/student/lab
EXPOSE 8080

CMD ["/usr/bin/supervisord", "-c", "/etc/supervisor/conf.d/supervisord.conf"]
