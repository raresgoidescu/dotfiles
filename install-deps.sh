#!/usr/bin/env bash

set -e # exit on any error

if [ "$EUID" -eq 0 ]; then
    echo "Please run as a non-root user with sudo"
    exit 1
fi

# Update system packages
sudo pacman -Syu --noconfirm

# Install base development tools
sudo pacman -S --noconfirm base-devel git wget curl vim

# Install Yay (AUR Helper)
# Clone yay from AUR
git clone https://aur.archlinux.org/yay.git
cd yay
makepkg -si --noconfirm
cd ..
rm -rf yay

# Install essential development packages
yay -S --noconfirm \
    gcc clang \
    make cmake \
	gdb valgrind strace ltrace \
    python \
    nodejs npm \
    rust cargo

# System utilities
yay -S --noconfirm htop tmux zsh

# Network and security tools
yay -S --noconfirm networkmanager openssh openssl nmap net-tools wireshark-cli

# Development environments
yay -S --noconfirm code docker docker-compose

# Enable Services
systemctl_enable_services() {
    services=(
        NetworkManager
        sshd
        firewalld
        docker
        systemd-timesyncd
        systemd-journald
        fstrim.timer
        power-profiles-daemon
        pipewire
        polkit
    )

    for service in "${services[@]}"; do
        sudo systemctl enable "$service"
        sudo systemctl start "$service"
    done
}

# Configure Docker
sudo usermod -aG docker $USER

