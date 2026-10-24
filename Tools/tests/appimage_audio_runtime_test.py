"""Linux regression tests for host JACK/PipeWire selection in the AppImage.

Run with python3 Tools/tests/appimage_audio_runtime_test.py (requires gcc/binutils).
The tiny ELF fixtures model libmpv's transitive audio dependencies without an
audio server; the production bundling functions and AppRun select the backend.
"""

import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import textwrap
import unittest


REPO = Path(__file__).resolve().parents[2]
WORKFLOW = Path(os.environ.get("WORKFLOW_PATH", REPO / ".github/workflows/build.yml")).read_text()


def functions(name):
    return [textwrap.dedent(match).replace("${{ matrix.gnu_type }}-readelf", "readelf")
            for match in re.findall(r"^          " + name + r"\(\) \{\n.*?^          \}",
                                    WORKFLOW, re.MULTILINE | re.DOTALL)]


def shell(script, directory, **environment):
    return subprocess.run(["bash", "-eu", "-o", "pipefail", "-c", script], cwd=directory,
                          env={**os.environ, **environment}, capture_output=True, text=True)


class AppImageAudioRuntimeTest(unittest.TestCase):
    def test_dependency_policies(self):
        policies = functions("is_forbidden_lib")
        self.assertEqual(len(policies), 3)  # x64, ARM packaging and ARM smoke test
        forbidden = ["libjack.so.0", "libjacknet.so.0", "libjackserver.so.0",
                     "libpipewire-0.3.so.0", "libpulse.so.0", "libc.so.6"]
        for index, policy in enumerate(policies):
            for library in forbidden + ["libmpv.so.2", "libmosquitto.so.1"]:
                with self.subTest(policy=index, library=library):
                    result = shell(policy + '\nis_forbidden_lib "$LIBRARY"', REPO, LIBRARY=library)
                    self.assertEqual(result.returncode, 0 if library in forbidden else 1, result.stderr)

    def test_packaged_app_uses_host_audio_clients(self):
        policies = functions("is_forbidden_lib")
        for index in range(2):  # Exercise both architectures' dependency walkers.
            with self.subTest(packager=index), tempfile.TemporaryDirectory(prefix="chataigne-audio-") as temp:
                root = Path(temp)
                appdir = root / "AppDir"
                bundled = appdir / "usr/lib"
                binary_dir = appdir / "usr/bin"
                build = root / "build"
                host = root / "host-audio"
                for directory in (bundled, binary_dir, build, host):
                    directory.mkdir(parents=True)

                # A bundled JACK implementation cannot connect to PipeWire; the
                # host's replacements return a different marker for the probe.
                for soname, function in (("libjack.so.0", "jack_backend"),
                                         ("libpipewire-0.3.so.0", "pipewire_backend")):
                    for directory, marker in ((build, 1), (host, 2)):
                        source = directory / (function + ".c")
                        source.write_text(f"int {function}(void) {{ return {marker}; }}\n")
                        subprocess.run(["gcc", "-shared", "-fPIC", str(source),
                                        "-Wl,-soname," + soname, "-o", str(directory / soname)], check=True)

                mpv_source = build / "mpv.c"
                mpv_source.write_text("int jack_backend(void); int pipewire_backend(void);\n"
                                      "int mpv_backend(void) { return jack_backend() + pipewire_backend(); }\n")
                subprocess.run(["gcc", "-shared", "-fPIC", str(mpv_source), "-L" + str(build),
                                "-l:libjack.so.0", "-l:libpipewire-0.3.so.0", "-Wl,-soname,libmpv.so.2",
                                "-o", str(build / "libmpv.so.2")], check=True)
                probe_source = build / "probe.c"
                probe_source.write_text("int mpv_backend(void); int main(void) { return mpv_backend() == 4 ? 0 : 1; }\n")
                subprocess.run(["gcc", str(probe_source), "-L" + str(build), "-l:libmpv.so.2",
                                "-Wl,-rpath-link," + str(build), "-o", str(binary_dir / "audio-probe")], check=True)

                shutil.copyfile(build / "libmpv.so.2", bundled / "libmpv.so.2")
                snippets = [functions(name)[index] for name in
                            ("copy_glob", "find_matching_so_path", "copy_needed_from_target")]
                package = policies[index] + "\n" + "\n".join(snippets) + "\n" + r'''
SEARCH_DIRS=("$BUILD_LIBS")
copy_needed_from_target "$APPDIR/usr/bin/audio-probe"
copy_needed_from_target "$APPDIR/usr/lib/libmpv.so.2"
'''
                result = shell(package, root, APPDIR=str(appdir), BUILD_LIBS=str(build))
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertFalse((bundled / "libjack.so.0").exists(), "JACK must stay on the host")
                self.assertFalse((bundled / "libpipewire-0.3.so.0").exists(), "PipeWire must stay on the host")

                launcher_dir = REPO / "Builds/LinuxMakefile/Chataigne.AppDir"
                (appdir / "AppRun").write_text((launcher_dir / "AppRun").read_text())
                shutil.copyfile(launcher_dir / "usr/lib/exec_wrapper.so", bundled / "exec_wrapper.so")
                (appdir / "probe.desktop").write_text("[Desktop Entry]\nExec=audio-probe\n")
                # pw-jack supplies its replacement JACK library via this variable.
                result = shell('bash "$APPDIR/AppRun"', root, APPDIR=str(appdir), LD_LIBRARY_PATH=str(host))
                self.assertEqual(result.returncode, 0, "AppRun must preserve host audio selection: " + result.stderr)

    def test_packaging_rejects_stale_audio_libraries(self):
        guards = re.findall(r"^          for FORBIDDEN in .*?^          done", WORKFLOW,
                            re.MULTILINE | re.DOTALL)
        self.assertEqual(len(guards), 2)
        for guard in guards:
            for library in ("libjack.so.0", "libjacknet.so.0", "libjackserver.so.0", "libpipewire-0.3.so.0"):
                with self.subTest(library=library), tempfile.TemporaryDirectory() as temp:
                    bundled = Path(temp) / "usr/lib"
                    bundled.mkdir(parents=True)
                    (bundled / library).touch()
                    result = shell(textwrap.dedent(guard), temp, APPDIR=temp)
                    self.assertNotEqual(result.returncode, 0, "stale audio client must fail packaging")


if __name__ == "__main__":
    unittest.main()
