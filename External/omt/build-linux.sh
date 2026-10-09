#!/usr/bin/env bash
# Build the native OMT runtime on the Ubuntu 22.04 release baseline.
set -euo pipefail
ARCH=${1:-x64}
case "$ARCH" in
  x64) RID=linux-x64; CXX=clang++; FLAGS=(-msse4.2 -mssse3 -mlzcnt); AVX=(-mavx2 -mbmi -mlzcnt) ;;
  arm64) RID=linux-arm64; CXX=clang++; FLAGS=(--target=aarch64-linux-gnu -march=armv8-a+simd); AVX=() ;;
  *) echo "OMT NativeAOT supports x64 and arm64; ARM32 is unavailable." >&2; exit 1 ;;
esac
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
DEST=$SCRIPT_DIR/lib/linux/$ARCH
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT
fetch() {
  local REPO=$1 COMMIT=$2
  mkdir "$WORK/$REPO"
  curl --fail --location --retry 3 "https://github.com/openmediatransport/$REPO/archive/$COMMIT.tar.gz" |
    tar xz --strip-components=1 -C "$WORK/$REPO"
}
fetch libomt bb4b67be8e5964f1017592b9fc5406495e704e5d
fetch libomtnet 029ef4e925c68973f3289eb0c7ecfd40a161166f
fetch libvmx a1828cb438823ebb777dcad20f9f56b633ae8db4
# Discovery is optional: upstream otherwise aborts without an Avahi daemon.
git -C "$WORK/libomtnet" apply "$SCRIPT_DIR/optional-avahi.patch"
mkdir -p "$DEST"
pushd "$WORK/libvmx/src" >/dev/null
if [ "$ARCH" = x64 ]; then
  "$CXX" -O3 -std=c++17 -fdeclspec -fPIC "${FLAGS[@]}" -c vmxcodec.cpp vmxcodec_x86.cpp
  "$CXX" -O3 -std=c++17 -fdeclspec -fPIC "${FLAGS[@]}" "${AVX[@]}" -c vmxcodec_avx2.cpp
else
  "$CXX" -O3 -std=c++17 -fdeclspec -fPIC "${FLAGS[@]}" -c vmxcodec.cpp vmxcodec_arm.cpp
fi
"$CXX" "${FLAGS[@]}" -shared -Wl,-soname,libvmx.so -Wl,-rpath,'$ORIGIN' ./*.o -o "$DEST/libvmx.so"
popd >/dev/null
dotnet build "$WORK/libomtnet/libomtnet.csproj" -c Release
dotnet publish "$WORK/libomt/libomt.csproj" -r "$RID" -c Release \
  -p:StripSymbols=false -p:InvariantGlobalization=true -o "$WORK/publish"
cp "$WORK/publish/libomt.so" "$DEST/"
if [ "$ARCH" = arm64 ]; then
  aarch64-linux-gnu-strip --strip-unneeded "$DEST/libomt.so"
else
  strip --strip-unneeded "$DEST/libomt.so"
fi
cp "$WORK/libvmx/LICENSE.txt" "$DEST/VMX-LICENSE.txt"
echo "Built OMT runtime: $DEST"
