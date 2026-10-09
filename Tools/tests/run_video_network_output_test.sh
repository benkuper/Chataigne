#!/usr/bin/env bash
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
JUCE_DIR=${JUCE_DIR:-"$ROOT/../JUCE"}
ARCH=${VIDEO_ARCH:-x64}
CORE_SOURCE="$JUCE_DIR/modules/juce_core/juce_core.cpp"
LINK_FLAGS=(-ldl -lrt -lz)
if [ "$(uname -s)" = Darwin ]; then
  CORE_SOURCE="$JUCE_DIR/modules/juce_core/juce_core.mm"
  LINK_FLAGS=(-framework Foundation -framework IOKit -framework Security -lz)
fi
OUT="$ROOT/Binaries/validation/video-network-$ARCH"
mkdir -p "$OUT"
${CXX:-g++} -std=c++17 -O1 -pthread -DCHATAIGNE_VIDEO_RUNTIME_ONLY=1 \
  -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1 -DJUCE_USE_CURL=0 -DJUCE_WEB_BROWSER=0 \
  -I"$JUCE_DIR/modules" -I"$ROOT/Source" \
  "$ROOT/Tools/tests/video_network_output_test.cpp" \
  "$CORE_SOURCE" "${LINK_FLAGS[@]}" -o "$OUT/test"
if [ "$(uname -s)" = Darwin ]; then
  export NDI_RUNTIME_DIR_V6="$ROOT/External/ndi/lib/osx"
  export CHATAIGNE_OMT_RUNTIME_DIR="$ROOT/External/omt/lib/osx"
else
  export NDI_RUNTIME_DIR_V6="$ROOT/External/ndi/lib/linux/$ARCH"
  export CHATAIGNE_OMT_RUNTIME_DIR="$ROOT/External/omt/lib/linux/$ARCH"
  export LD_LIBRARY_PATH="$NDI_RUNTIME_DIR_V6:$CHATAIGNE_OMT_RUNTIME_DIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
fi
"$OUT/test"
