#!/usr/bin/env bash
set -euo pipefail

# Requires the Linux application build dependencies plus xvfb/xauth.
# Tests use the shared JUCE checkout without changing the application or CI build.
repo_path="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)"
juce_path="${JUCE_PATH:-$(dirname -- "$repo_path")/JUCE}"
test_dir="${TRIGGER_TEST_BUILD_DIR:-/tmp/chataigne-trigger-tests}"
configuration="${CONFIGURATION:-Release}"
jobs="${JOBS:-4}"
if [[ ! -f "$juce_path/modules/juce_core/juce_core.h" ]]; then
    printf 'Shared JUCE checkout not found: %s\n' "$juce_path" >&2
    exit 1
fi
mkdir -p "$test_dir/$configuration"

# Newer distributions provide WebKit 4.1 in place of the generated 4.0 package.
cat > "$test_dir/pkg-config" <<'SCRIPT'
#!/usr/bin/env bash
args=()
for arg in "$@"; do
    if [[ "$arg" == webkit2gtk-4.0 ]] && ! /usr/bin/pkg-config --exists "$arg"; then
        arg=webkit2gtk-4.1
    fi
    args+=("$arg")
done
exec /usr/bin/pkg-config "${args[@]}"
SCRIPT
chmod +x "$test_dir/pkg-config"
cat > "$test_dir/tests.mk" <<'MAKEFILE'
TEST_OBJECTS := $(filter-out $(JUCE_OBJDIR)/Main_90ebc5c2.o,$(OBJECTS_APP))
# Git for Windows may check out bundled library symlinks as text files. Prefer
# system development libraries, and link the actual versioned Servus binary.
JUCE_LDFLAGS := -L/usr/lib/x86_64-linux-gnu $(filter-out -lServus,$(JUCE_LDFLAGS)) -l:libServus.so.1.6.0
.SECONDARY: $(JUCE_OBJDIR)/timeline_trigger_integration_test.o $(JUCE_OBJDIR)/cv_set_value_trigger_integration_test.o
.PHONY: trigger-tests
trigger-tests: $(JUCE_OUTDIR)/timeline_trigger_integration_test $(JUCE_OUTDIR)/cv_set_value_trigger_integration_test

$(JUCE_OBJDIR)/%_integration_test.o: ../../Tools/tests/%_integration_test.cpp
	@mkdir -p $(@D)
	$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OUTDIR)/%_integration_test: $(TEST_OBJECTS) $(JUCE_OBJDIR)/%_integration_test.o $(JUCE_OBJDIR)/execinfo.cmd
	@mkdir -p $(@D)
	$(CXX) -o "$@" $(TEST_OBJECTS) $(JUCE_OBJDIR)/$*_integration_test.o $(JUCE_LDFLAGS) $(shell cat $(JUCE_OBJDIR)/execinfo.cmd) $(JUCE_LDFLAGS_APP) $(TARGET_ARCH)
MAKEFILE

cd "$repo_path/Builds/LinuxMakefile"
make -f Makefile -f "$test_dir/tests.mk" CONFIG="$configuration" \
    JUCE_OBJDIR="$test_dir/$configuration/objects" JUCE_OUTDIR="$test_dir/$configuration" \
    JUCE_BINDIR="$test_dir/$configuration" JUCE_LIBDIR="$test_dir/$configuration" \
    CPPFLAGS="-I$juce_path/modules" PKG_CONFIG="$test_dir/pkg-config" \
    -j"$jobs" trigger-tests

mkdir -p "$test_dir/runtime"
ln -sfn "$repo_path/External/servus/lib/linux/libServus.so.1.6.0" "$test_dir/runtime/libServus.so.6"
export LD_LIBRARY_PATH="$test_dir/runtime${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
xvfb-run -a "$test_dir/$configuration/timeline_trigger_integration_test"
xvfb-run -a "$test_dir/$configuration/cv_set_value_trigger_integration_test"
