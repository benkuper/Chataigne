#!/usr/bin/env bash
set -euo pipefail

# Requires cmake, patch, pkg-config, a C++ compiler and libavahi-client-dev.
# Cross builds may set CXX, PKG_CONFIG_LIBDIR and CMAKE_TOOLCHAIN_FILE.
repo_path="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="${SERVUS_BUILD_DIR:-/tmp/chataigne-servus-${CXX:-native}}"
output_dir="${1:-$repo_path/External/servus/lib/linux}"
mkdir -p "$build_dir"
mkdir -p "$build_dir/source"
cp -a "$repo_path/External/servus/source/." "$build_dir/source/"
patch --directory="$build_dir/source" --strip=1 --input="$repo_path/External/servus/patches/avahi-poll-failure.patch"

args=()
if [[ -n "${CMAKE_TOOLCHAIN_FILE:-}" ]]; then
    args+=("-DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE")
fi
cmake -S "$repo_path/External/servus" -B "$build_dir/build" \
    -DSERVUS_SOURCE_DIR="$build_dir/source" -DCMAKE_BUILD_TYPE=Release "${args[@]}"
cmake --build "$build_dir/build" --parallel "${JOBS:-4}"
# Remove only these known library names: Windows may check Git symlinks out as text.
mkdir -p "$output_dir"
rm -f "$output_dir/libServus.so" "$output_dir/libServus.so.6" "$output_dir/libServus.so.1.6.0"
cmake --install "$build_dir/build" --prefix "$build_dir/install"
cp -a "$build_dir/install/lib/"libServus.so* "$output_dir/"
printf 'Patched Servus installed in %s\n' "$output_dir"
