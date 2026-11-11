#!/usr/bin/env bash
set -euo pipefail

# Apply to both traditional Ubuntu sources and deb822 sources in newer images.
# Keep the distribution, components and signature verification unchanged.
for source in /etc/apt/sources.list /etc/apt/sources.list.d/*.list /etc/apt/sources.list.d/*.sources; do
    if [ -f "$source" ]; then
        sed -i -E 's#https?://([a-z0-9.-]+\.)?ubuntu\.com/ubuntu#https://mirrors.mit.edu/ubuntu#g' "$source"
    fi
done

# GitHub runners can use mirror+file sources instead of literal repository URLs.
for mirror_list in /etc/apt/apt-mirrors.txt /etc/apt/apt-mirrors-security.txt; do
    if [ -f "$mirror_list" ]; then
        printf '%s\n' 'https://mirrors.mit.edu/ubuntu' > "$mirror_list"
    fi
done
