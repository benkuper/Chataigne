#!/usr/bin/env bash
set -euo pipefail
repo_path="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)"
test_dir="${SERVUS_TEST_BUILD_DIR:-/tmp/chataigne-servus-tests}"
library_dir="${SERVUS_LIBRARY_DIR:-$repo_path/External/servus/lib/linux}"
mkdir -p "$test_dir"
"${CXX:-g++}" -std=c++11 -Wall -Wextra -Werror \
    -I"$repo_path/External/servus/include" \
    "$repo_path/Tools/tests/servus_avahi_failure_test.cpp" \
    -L"$library_dir" -Wl,--export-dynamic -l:libServus.so.1.6.0 \
    $(pkg-config --cflags --libs avahi-client) -ldl \
    -o "$test_dir/servus_avahi_failure_test"
# Use a real runtime symlink even when Git checked bundled symlinks out as text.
ln -sfn "$library_dir/libServus.so.1.6.0" "$test_dir/libServus.so.6"
export LD_LIBRARY_PATH="$test_dir${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
ulimit -c 0
for scenario in success quit poll-error client-failure registering browser-failure resolver-failure announcement-error; do
    timeout 10 "$test_dir/servus_avahi_failure_test" "$scenario"
done
