# Repository Instructions

## JUCE dependency

- Never clone, copy, initialize, or place a JUCE repository inside the Chataigne workspace.
- Never add JUCE to Chataigne as a Git submodule or gitlink.
- For local builds and tooling, always use the existing shared JUCE checkout at `D:\Projects\Dev\JUCE`.
- If a generated project expects a workspace-local `JUCE` directory, point the build at the shared checkout instead. Do not create a nested clone, copy, junction, or symlink unless the user explicitly requests it.
- Do not change CI's dependency checkout strategy unless the user explicitly asks for that change.
