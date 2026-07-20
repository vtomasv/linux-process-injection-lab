#!/usr/bin/env python3
"""
============================================================
SIMULACIÓN VISUAL DE RANSOMWARE
============================================================
Este script muestra una ventana de ransomware en pantalla completa
para simular el efecto visual de un ataque exitoso.

¡ESTO ES SOLO UNA SIMULACIÓN EDUCATIVA!
No cifra archivos ni causa daño real.

Se ejecuta como resultado de la "inyección" en el proceso
legítimo (mousepad) del laboratorio anterior.
============================================================
"""

import tkinter as tk
import time
import threading
import os

class RansomwareSimulation:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("") 
        self.root.configure(bg='#1a0000')
        
        # Fullscreen
        self.root.attributes('-fullscreen', True)
        self.root.attributes('-topmost', True)
        
        # Allow escape to close (for the lab)
        self.root.bind('<Escape>', lambda e: self.root.destroy())
        self.root.bind('<q>', lambda e: self.root.destroy())
        
        self.setup_ui()
        self.animate_timer()
        
    def setup_ui(self):
        # Main frame
        main_frame = tk.Frame(self.root, bg='#1a0000')
        main_frame.pack(expand=True, fill='both', padx=50, pady=30)
        
        # Skull ASCII art
        skull = """
        ░██████╗████████╗░█████╗░██████╗░██╗
        ██╔════╝╚══██╔══╝██╔══██╗██╔══██╗██║
        ╚█████╗░░░░██║░░░██║░░██║██████╔╝██║
        ░╚═══██╗░░░██║░░░██║░░██║██╔═══╝░╚═╝
        ██████╔╝░░░██║░░░╚█████╔╝██║░░░░░██╗
        ╚═════╝░░░░╚═╝░░░░╚════╝░╚═╝░░░░░╚═╝
        """
        
        skull_label = tk.Label(
            main_frame, 
            text=skull,
            font=('Courier', 14, 'bold'),
            fg='#ff0000',
            bg='#1a0000',
            justify='center'
        )
        skull_label.pack(pady=(20, 10))
        
        # Warning title
        title = tk.Label(
            main_frame,
            text="⚠️  TUS ARCHIVOS HAN SIDO CIFRADOS  ⚠️",
            font=('Helvetica', 28, 'bold'),
            fg='#ff3333',
            bg='#1a0000'
        )
        title.pack(pady=(10, 20))
        
        # Message
        message_text = """
Todos los archivos de este sistema han sido cifrados con AES-256.
Para recuperar tus archivos, debes pagar 0.5 BTC a la siguiente dirección:

    bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh

Tienes 72 horas antes de que la clave de descifrado sea eliminada permanentemente.
NO intentes recuperar los archivos por tu cuenta o serán destruidos.
        """
        
        msg_label = tk.Label(
            main_frame,
            text=message_text,
            font=('Helvetica', 14),
            fg='#ffffff',
            bg='#1a0000',
            justify='center'
        )
        msg_label.pack(pady=10)
        
        # Timer frame
        timer_frame = tk.Frame(main_frame, bg='#330000', bd=2, relief='ridge')
        timer_frame.pack(pady=20, padx=100, fill='x')
        
        timer_title = tk.Label(
            timer_frame,
            text="⏰ TIEMPO RESTANTE PARA PAGAR:",
            font=('Helvetica', 16, 'bold'),
            fg='#ffcc00',
            bg='#330000'
        )
        timer_title.pack(pady=(10, 5))
        
        self.timer_label = tk.Label(
            timer_frame,
            text="71:59:58",
            font=('Courier', 48, 'bold'),
            fg='#ff0000',
            bg='#330000'
        )
        self.timer_label.pack(pady=(5, 10))
        
        # Files "encrypted" list
        files_frame = tk.Frame(main_frame, bg='#1a0000')
        files_frame.pack(pady=10)
        
        files_title = tk.Label(
            files_frame,
            text="Archivos cifrados (muestra):",
            font=('Helvetica', 12, 'bold'),
            fg='#ff6666',
            bg='#1a0000'
        )
        files_title.pack()
        
        fake_files = [
            "📄 /home/student/Documents/proyecto_final.docx.LOCKED",
            "📄 /home/student/Documents/presupuesto_2026.xlsx.LOCKED",
            "📄 /home/student/Pictures/foto_familia.jpg.LOCKED",
            "📄 /home/student/.ssh/id_rsa.LOCKED",
            "📄 /etc/passwd.LOCKED",
        ]
        
        for f in fake_files:
            fl = tk.Label(
                files_frame,
                text=f,
                font=('Courier', 11),
                fg='#cc9999',
                bg='#1a0000'
            )
            fl.pack(anchor='w', padx=50)
        
        # Educational notice at the bottom
        notice_frame = tk.Frame(main_frame, bg='#003300', bd=2, relief='ridge')
        notice_frame.pack(pady=20, padx=50, fill='x', side='bottom')
        
        notice = tk.Label(
            notice_frame,
            text="🎓 ESTO ES UNA SIMULACIÓN EDUCATIVA - Presiona ESC o 'q' para cerrar\n"
                 "Este ransomware NO cifró archivos reales. Es parte del laboratorio MAR404.",
            font=('Helvetica', 12, 'bold'),
            fg='#00ff00',
            bg='#003300',
            justify='center'
        )
        notice.pack(pady=10)
    
    def animate_timer(self):
        """Simulates a countdown timer"""
        self.hours = 71
        self.minutes = 59
        self.seconds = 58
        self.update_timer()
    
    def update_timer(self):
        self.timer_label.config(text=f"{self.hours:02d}:{self.minutes:02d}:{self.seconds:02d}")
        self.seconds -= 1
        if self.seconds < 0:
            self.seconds = 59
            self.minutes -= 1
            if self.minutes < 0:
                self.minutes = 59
                self.hours -= 1
        self.root.after(1000, self.update_timer)
    
    def run(self):
        self.root.mainloop()

if __name__ == "__main__":
    # Set display for noVNC
    if "DISPLAY" not in os.environ:
        os.environ["DISPLAY"] = ":1"
    
    app = RansomwareSimulation()
    app.run()
