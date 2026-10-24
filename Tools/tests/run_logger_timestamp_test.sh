#!/usr/bin/env bash
set -euo pipefail
repo_path="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)"
juce_path="${JUCE_PATH:-$(dirname -- "$repo_path")/JUCE}"
test_dir="${LOGGER_TEST_BUILD_DIR:-/tmp/chataigne-logger-tests}"
mkdir -p "$test_dir"
"${CXX:-g++}" -std=c++17 -pthread \
    -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1 -DJUCE_STANDALONE_APPLICATION=1 \
    -DJUCE_USE_CURL=0 -DJUCE_MODAL_LOOPS_PERMITTED=1 \
    -I"$juce_path/modules" -I"$repo_path/Modules" \
    "$repo_path/Tools/tests/logger_timestamp_test.cpp" \
    "$juce_path/modules/juce_core/juce_core.cpp" \
    "$juce_path/modules/juce_events/juce_events.cpp" \
    -ldl -lrt -o "$test_dir/logger_timestamp_test"
"$test_dir/logger_timestamp_test"
