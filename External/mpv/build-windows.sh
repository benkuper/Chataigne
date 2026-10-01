#!/usr/bin/env bash
# Cross-build the bundled Windows 10/11 x64 runtime on Linux or WSL.
# All dependencies are static; the result needs only Windows system DLLs.
set -euo pipefail

script_dir=$(cd -- "$(dirname -- "$0")" && pwd)
work_dir=${1:?Usage: build-windows.sh /absolute/build-directory [jobs]}
[[ "$work_dir" = /* && "$work_dir" != / ]] || { echo 'Use an absolute build directory.' >&2; exit 1; }
mkdir -p "$work_dir"
work_dir=$(cd -- "$work_dir" && pwd)
jobs=${2:-8}
prefix="$work_dir/prefix"
target=x86_64-w64-mingw32
export CC=$target-gcc-posix CXX=$target-g++-posix AR=$target-ar RANLIB=$target-ranlib
export CFLAGS='-O2 -ffunction-sections -fdata-sections'
export CXXFLAGS="$CFLAGS"
export LDFLAGS='-Wl,--gc-sections -static-libgcc -static-libstdc++'
export PKG_CONFIG_LIBDIR="$prefix/lib/pkgconfig"
export PKG_CONFIG_PATH=
export SOURCE_DATE_EPOCH=1780272000
for tool in "$CC" "$CXX" meson ninja cmake nasm git python3 pkg-config autoreconf; do
    command -v "$tool" >/dev/null || { echo "Missing tool: $tool (see README.md)" >&2; exit 1; }
done
mkdir -p "$prefix" "$work_dir/src" "$work_dir/build" "$work_dir/dist/mpv-licenses"

# Exact commits, including the dependencies' own pinned submodules.
python3 - "$script_dir/windows-sources.json" "$work_dir/src" <<'PY'
import json, pathlib, subprocess, sys
for name, source in json.load(open(sys.argv[1])).items():
    dest = pathlib.Path(sys.argv[2]) / name
    if not (dest / '.git').exists():
        subprocess.run(['git', 'init', '-q', str(dest)], check=True)
        subprocess.run(['git', '-C', str(dest), 'remote', 'add', 'origin',
                        'https://github.com/' + source['repo'] + '.git'], check=True)
    current = subprocess.run(['git', '-C', str(dest), 'rev-parse', 'HEAD'],
                             capture_output=True, text=True)
    if current.stdout.strip() != source['commit']:
        subprocess.run(['git', '-C', str(dest), 'fetch', '-q', '--depth=1',
                        'origin', source['commit']], check=True)
        subprocess.run(['git', '-C', str(dest), 'checkout', '-q', '--detach',
                        source['commit']], check=True)
    subprocess.run(['git', '-C', str(dest), 'submodule', 'update', '--init',
                    '--recursive', '--depth=1'], check=True)
PY

# mpv's pinned build files tie D3D11 decoding helpers to the optional D3D11
# renderer. Keep hardware decoding available with our OpenGL-only renderer.
for patch in "$script_dir"/patches/*.patch; do
    if git -C "$work_dir/src/mpv" apply --check "$patch" 2>/dev/null; then
        git -C "$work_dir/src/mpv" apply "$patch"
    else
        git -C "$work_dir/src/mpv" apply --reverse --check "$patch"
    fi
done

cat > "$work_dir/cross.ini.next" <<EOF
[binaries]
c = '$CC'
cpp = '$CXX'
ar = '$AR'
strip = '$target-strip'
windres = '$target-windres'
dlltool = '$target-dlltool'
pkg-config = 'pkg-config'
[host_machine]
system = 'windows'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'little'
[properties]
needs_exe_wrapper = true
[built-in options]
buildtype = 'release'
default_library = 'static'
prefer_static = true
auto_features = 'disabled'
c_args = ['-O2', '-ffunction-sections', '-fdata-sections']
cpp_args = ['-O2', '-ffunction-sections', '-fdata-sections']
c_link_args = ['-Wl,--gc-sections', '-static-libgcc', '-static-libstdc++', '-static']
cpp_link_args = ['-Wl,--gc-sections', '-static-libgcc', '-static-libstdc++', '-static']
EOF
if cmp -s "$work_dir/cross.ini.next" "$work_dir/cross.ini"; then
    rm "$work_dir/cross.ini.next"
else
    mv "$work_dir/cross.ini.next" "$work_dir/cross.ini"
fi

meson_build() {
    local name=$1; shift
    echo "Building $name"
    if [[ ! -f "$work_dir/build/$name/build.ninja" ]]; then
        meson setup "$work_dir/build/$name" "$work_dir/src/$name" \
            --cross-file "$work_dir/cross.ini" --prefix "$prefix" --libdir lib "$@"
    fi
    meson compile -C "$work_dir/build/$name" -j "$jobs"
    meson install -C "$work_dir/build/$name"
}
cmake_build() {
    local name=$1; shift
    echo "Building $name"
    cmake -S "$work_dir/src/$name" -B "$work_dir/build/$name" -G Ninja \
        -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER="$CC" -DCMAKE_CXX_COMPILER="$CXX" \
        -DCMAKE_RC_COMPILER="$target-windres" -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_INSTALL_LIBDIR=lib -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5 "$@"
    cmake --build "$work_dir/build/$name" -j "$jobs"
    cmake --install "$work_dir/build/$name"
}

cmake_build zlib -DZLIB_BUILD_EXAMPLES=OFF
meson_build freetype -Dzlib=external -Dharfbuzz=disabled
meson_build fribidi -Dtests=false -Ddocs=false -Dbin=false
meson_build harfbuzz -Dfreetype=enabled -Dtests=disabled -Dutilities=disabled \
    -Dsubset=disabled -Draster=disabled -Dvector=disabled -Dgpu=disabled
meson_build libass -Ddirectwrite=enabled -Dasm=enabled
meson_build dav1d -Denable_tools=false -Denable_tests=false
cmake_build opus -DOPUS_BUILD_TESTING=OFF -DOPUS_BUILD_PROGRAMS=OFF
meson_build lcms2 -Dtests=disabled -Dutils=false

if [[ ! -f "$work_dir/build/libvpx/Makefile" ]]; then
    (cd "$work_dir/build"; mkdir -p libvpx; cd libvpx
     CROSS="$target-" "$work_dir/src/libvpx/configure" --target=x86_64-win64-gcc \
         --prefix="$prefix" --disable-shared --enable-static --enable-vp9-highbitdepth \
         --disable-vp8-encoder --disable-vp9-encoder --disable-examples --disable-tools \
         --disable-docs --disable-unit-tests)
fi
make -C "$work_dir/build/libvpx" -j "$jobs"
make -C "$work_dir/build/libvpx" install
make -C "$work_dir/src/nv-codec-headers" PREFIX="$prefix" install

# Retain all built-in decoders/demuxers, network playback, filters, alpha and
# hardware acceleration. PNG/MJPEG encoders and SPDIF are used by mpv itself.
if [[ ! -f "$work_dir/build/ffmpeg/Makefile" ]]; then
    (cd "$work_dir/build/ffmpeg" 2>/dev/null || { mkdir -p "$work_dir/build/ffmpeg"; cd "$work_dir/build/ffmpeg"; }
     "$work_dir/src/ffmpeg/configure" --prefix="$prefix" --libdir="$prefix/lib" \
         --target-os=mingw32 --arch=x86_64 --enable-cross-compile --cross-prefix="$target-" \
         --cc="$CC" --cxx="$CXX" --pkg-config=pkg-config --pkg-config-flags=--static \
         --enable-static --disable-shared --disable-debug --disable-doc --disable-programs \
         --disable-autodetect --disable-avdevice --disable-encoders --disable-muxers \
         --enable-encoder=png,mjpeg --enable-muxer=spdif --enable-zlib --enable-schannel \
         --enable-libdav1d --enable-libvpx --enable-libopus --enable-ffnvcodec \
         --enable-nvdec --enable-cuda --enable-d3d11va --enable-dxva2 \
         --extra-cflags="$CFLAGS" --extra-cxxflags="$CXXFLAGS" \
         --extra-ldflags="$LDFLAGS" --extra-libs='-lpthread -lstdc++')
fi
make -C "$work_dir/build/ffmpeg" -j "$jobs"
make -C "$work_dir/build/ffmpeg" install

meson_build libplacebo -Ddemos=false -Dtests=false -Dopengl=enabled -Dlcms=enabled \
    -Dvulkan=disabled -Dshaderc=disabled -Dglslang=disabled -Dd3d11=disabled

meson_build mpv -Ddefault_library=shared -Dlibmpv=true -Dcplayer=false -Dtests=false \
    -Dgpl=false -Dbuild-date=false -Dgl=enabled -Dplain-gl=enabled -Dgl-win32=enabled \
    -Dgl-dxinterop=enabled -Dgl-dxinterop-d3d9=enabled -Dd3d-hwaccel=enabled \
    -Dd3d9-hwaccel=enabled -Dcuda-hwaccel=enabled -Dcuda-interop=enabled \
    -Dlcms2=enabled -Dzlib=enabled -Dwin32-threads=enabled -Dlua=disabled

cp "$work_dir/build/mpv/libmpv-2.dll" "$work_dir/dist/"
"$target-strip" --strip-unneeded "$work_dir/dist/libmpv-2.dll"
# Reject accidental dependence on a MinGW runtime or any extra vendor DLL.
"$target-objdump" -p "$work_dir/dist/libmpv-2.dll" > "$work_dir/dist/mpv-licenses/imports.txt"
if grep -Ei 'DLL Name:.*(libgcc|libstdc\+\+|libwinpthread|libav|libplacebo|libass|libdav1d|libopus|libvpx)' \
    "$work_dir/dist/mpv-licenses/imports.txt"; then
    echo 'The runtime is not self-contained.' >&2; exit 1
fi
cp "$script_dir/windows-sources.json" "$script_dir/build-windows.sh" "$work_dir/dist/mpv-licenses/"
cp -a "$script_dir/patches" "$work_dir/dist/mpv-licenses/"
python3 - "$work_dir" <<'PY'
import hashlib, json, pathlib, shutil, sys
root = pathlib.Path(sys.argv[1])
licenses = root / 'dist/mpv-licenses'
for source in (root / 'src').iterdir():
    dest = licenses / source.name
    dest.mkdir(exist_ok=True)
    for file in source.rglob('*'):
        if '.git' in file.parts or not file.is_file():
            continue
        if file.name.upper().startswith(('LICENSE', 'COPYING', 'COPYRIGHT', 'FTL.TXT')):
            target = dest / file.relative_to(source)
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(file, target)
mpv = root / 'dist/libmpv-2.dll'
(licenses / 'runtime.json').write_text(json.dumps({
    'sha256': hashlib.sha256(mpv.read_bytes()).hexdigest(), 'bytes': mpv.stat().st_size,
    'platform': 'Windows 10/11 x64', 'mpv': 'v0.41.0-dev-ga1f50f2c3',
    'ffmpeg': '8.1.3', 'profile': 'Chataigne bundled playback',
}, indent=2) + '\n')
PY
# Keep exact corresponding sources separately from the small installer.
tar --exclude=.git -C "$work_dir" -cJf "$work_dir/dist/mpv-sources.tar.xz" src
echo "Runtime: $work_dir/dist/libmpv-2.dll"
cat "$work_dir/dist/mpv-licenses/runtime.json"
