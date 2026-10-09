#!/usr/bin/env bash
set -euo pipefail
DEST=$(cd "$(dirname "$0")" && pwd)
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT
cd "$WORK"
curl --fail --location --retry 3 -o ndi.tar.gz \
  https://downloads.ndi.tv/SDK/NDI_SDK_Linux/Install_NDI_SDK_v6_Linux.tar.gz
tar xzf ndi.tar.gz
# The SDK installer displays its license and extracts the libraries locally.
yes | PAGER=cat sh Install_NDI_SDK_v6_Linux.sh >install.log 2>&1 || test -d 'NDI SDK for Linux/lib'
for SPEC in x86_64-linux-gnu:x64 aarch64-rpi4-linux-gnueabi:arm64 arm-rpi4-linux-gnueabihf:armhf; do
  SOURCE=${SPEC%:*}
  ARCH=${SPEC#*:}
  test -d "NDI SDK for Linux/lib/$SOURCE"
  mkdir -p "$DEST/lib/linux/$ARCH"
  cp -L "NDI SDK for Linux/lib/$SOURCE/libndi.so.6" "$DEST/lib/linux/$ARCH/"
done
cp 'NDI SDK for Linux/licenses/libndi_licenses.txt' "$DEST/LICENSE-LINUX.txt"
cp 'NDI SDK for Linux/NDI SDK License Agreement.pdf' "$DEST/LICENSE-SDK6.pdf"
