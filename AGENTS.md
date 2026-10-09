# Repository Instructions

## JUCE dependency

- Never clone, copy, initialize, or place a JUCE repository inside the Chataigne workspace.
- Never add JUCE to Chataigne as a Git submodule or gitlink.
- For local builds and tooling, always use the existing shared JUCE checkout at `D:\Projects\Dev\JUCE`.
- If a generated project expects a workspace-local `JUCE` directory, point the build at the shared checkout instead. Do not create a nested clone, copy, junction, or symlink unless the user explicitly requests it.
- Do not change CI's dependency checkout strategy unless the user explicitly asks for that change.

## Publishing Chataigne releases

- Requests such as "push a new beta", "push version X", or "release a stable version" invoke the `chataigne-release` skill at `C:\Users\bkupe\.codex\skills\chataigne-release\SKILL.md`. It supports both beta and stable versions; honor an explicit version exactly.
- First set the requested version in `Chataigne.jucer` and resave it with Projucer (`--resave`) to regenerate all configured exporter projects. Verify the generated versions and include these changes in the release commit.
- The release request authorizes the full workflow: review commits since the previous release, set the Projucer version and resave all projects, update `site/releases/update.json` and `site/user/config/downloaddata.yaml` for the selected channel (including changelog and history), commit release changes, and push the exact version tag.
- Wait for the tag's CI run and all required platform package uploads to succeed before uploading either metadata file. Never publish metadata from a failed, incomplete, or unrelated branch run.
- Upload both files through the SFTP profile in `site/.vscode/sftp.json`, preserving their paths relative to `site` under its configured remote root. Keep credentials private, upload only these two files, and verify the remote contents.
- Preserve unrelated work and existing tags; do not force-push, change CI dependency strategy, or force-add the ignored website/credential tree. If the personal skill is unavailable, follow this workflow directly.
