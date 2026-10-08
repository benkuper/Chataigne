# CV Values

Add a **CV Values** sequence layer, add target CV Groups in Free mode, and select
a Base Preset for each group. Create Preset Blocks or Custom Values blocks.
Right-click a block and choose **Edit Values / Animations** to edit its grouped
values, number automation, or color gradients.

The layer writes base values outside blocks. Between blocks, Base Preset returns
to base, Interpolate crosses the whole gap, and Hold keeps the preceding endpoint.
Ordinary two-block overlaps crossfade across the entire overlap. Invalid edits
are rejected; invalid imported blocks remain visible with warnings and do not
contribute. Shift-resizing stretches animation keys; trimming keeps local key
times. Block looping is disabled.

Unset custom values inherit authored history by default. **Do not change** masks
that block's contribution and permits state-dependent output. Lower visible
CV Values layers win per parameter, while a no-write contribution lets earlier
layers continue driving it. External writers and other sequences are outside
this arbitration.

`CVValuesEvaluator.h` is a JUCE-independent, double-precision evaluator. Its
immutable authored timeline returns explicit write/no-write samples and uses
supplied interpolation and animation functions. `CVValuesLayer` resolves targets
and publishes snapshots; `ChataigneSequence` coordinates priority and applies
each parameter once. Snapshots never read destination values for interpolation
or inheritance, and animation sampling creates no playback threads.

Preset entries track whether their values were authored, so automatically filled
or missing entries use parameter defaults in the timeline. This metadata survives
serialization and preset-update undo without changing ordinary preset loading.

From the repository root, run `Tools/tests/run_cv_values_evaluator_test.cmd`.
After a Debug x64 app build using the shared JUCE checkout, run
`Tools/tests/run_cv_values_integration_test.ps1`. The integration suite exercises
real sequence clocks, persistence, priority, references, typed editors, clipboard,
undo, and timeline gestures. Its optional `-FixturePath` writes a project and
rendered editor previews for inspection.
