# Validation record

Authoring host: Windows, November 6, 2026. Requirements: C++17 / CMake 3.16+; Windows, macOS, Linux.

The README verification command is `ctest --test-dir build -C Release --output-on-failure`. The companion GitHub Actions workflow describes reproducible build/test checks. Merely including that workflow does not mean it ran successfully. Host-specific verification results are recorded before publication.

Manual acceptance steps are in README.md. GUI behavior, hardware sensors, permissions, audio output, and OS isolation need verification on their target platform; compilation alone cannot establish these properties.

## Authoring result

Target toolchain/platform unavailable on the Windows authoring host. Source and build configuration were reviewed, and a target-platform GitHub Actions build/check workflow is provided. No successful native build or runtime result is claimed yet.

## Observed GitHub Actions result

Passed on November 6, 2026: [build/check run](https://github.com/netcodermike/interactive-audio-synthesizer/actions/runs/42196100842).

Verified source commit: `ec8cf5c39991bf107561cbb9e9a4f602d1a52e50`.

C++ build, DSP self-test, and WAV header/duration/signal validation.

This record may be committed after the linked source-validation run; the linked SHA identifies exactly what was checked. Native GUI/hardware acceptance remains manual as described in README.md.
