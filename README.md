# Patch Synth

[![Build and verify](https://github.com/netcodermike/interactive-audio-synthesizer/actions/workflows/verify.yml/badge.svg)](https://github.com/netcodermike/interactive-audio-synthesizer/actions/workflows/verify.yml)


An interactive synthesis workbench that renders oscillator patches through an envelope, low-pass filter, and feedback echo into standard WAV files.

**Language:** C++17 / CMake 3.16+  
**Platform:** Windows, macOS, Linux  
**Status:** First-release learning project. See implemented scope and limitations below; this is not a production-completeness claim.

## Features

- Sine, square, saw, and triangle waveforms.
- Adjustable frequency, duration, low-pass cutoff, and echo amount.
- Attack/release envelope and sample clipping protection.
- 44.1 kHz, mono, signed 16-bit little-endian PCM WAV export.
- Text patch overview and a standalone visual Web Audio patch playground.

## Requirements and quick start

Clone the repository, enter its folder, and install the language/platform tools listed above. Commands run from the repository root. No credentials are required except for Backup Vault's explicitly generated secrets.

```sh
git clone https://github.com/netcodermike/interactive-audio-synthesizer.git
cd interactive-audio-synthesizer
cmake -S . -B build
cmake --build build --config Release
# Linux/macOS:
./build/synth
# Windows (Visual Studio generator):
.\build\Release\synth.exe
```

## Verification

```sh
ctest --test-dir build -C Release --output-on-failure
```

Render a 220 Hz sine and a 440 Hz saw, open both in an audio player, and verify their durations. Lower cutoff to hear high frequencies soften. Set echo to zero and then 0.5 to compare decay. Run the self-test and inspect the RIFF/WAVE header of an exported file. Try live controls in `playground.html`.

See [validation notes](docs/VALIDATION.md) for what was actually executed during authoring. A committed CI workflow is a future check until its run is observed successful.

## Architecture and data

`main.cpp` defines `Patch`, a phase oscillator, one-pole low-pass filter, feedback echo buffer, WAV header writer, and interactive command loop. The render chain is oscillator → attack/release envelope → low-pass → echo → PCM. The delay is 11,025 samples (250 ms). `CMakeLists.txt` builds a dependency-free executable and registers the self-test. `playground.html` is a companion browser interface with visual controls and live Web Audio playback; its DSP implementation is independent of the C++ WAV renderer.

At the prompt use `wave triangle`, `freq 440`, `duration 4`, `cutoff 1200`, `echo 0.3`, `show`, `render tone.wav`, and `quit`. Supported ranges: 20–10,000 Hz pitch, 0.1–60 seconds duration, 20–20,000 Hz cutoff, 0–0.8 echo. Open `playground.html` in a modern browser for the live preview.

## Limitations and next steps

The C++ program renders files rather than streaming to an audio device. Browser preview is a separate implementation and will not sound sample-identical. Square/saw oscillators are naive and can alias at high frequencies. There is no MIDI, polyphony, editable node topology, patch persistence, or DAW plugin format. The command loop supports one command/value pair at a time and file paths without spaces. Listening volume is controlled by your player/browser.

## Troubleshooting

If CMake cannot find a compiler, install Visual Studio C++ build tools, Xcode command-line tools, or GCC. With single-config generators the executable is `build/synth`; Visual Studio places it under `build/Release`. Type `render output.wav` after editing controls; changing a control alone does not write a file. Invalid numeric values leave the previous patch intact. Use the browser's Play button to unlock audio.

## Development

Read the source entry point described above, make a focused change, run the verification command, and exercise the relevant manual scenario. [Contributing](CONTRIBUTING.md) explains bug reports and pull requests; [security notes](SECURITY.md) describe the trust assumptions. Runtime/generated files are ignored. No telemetry or paid API dependencies are included.

## License

[MIT](LICENSE), copyright 2026 netcodermike.
