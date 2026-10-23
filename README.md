# Convolution Reverb

A JUCE audio plugin (VST3 / AU / Standalone) that applies the acoustics of a real space to audio by convolving it with an impulse response (IR).

**Current state:** a stereo passthrough plugin that builds and loads. All the reverb work is yours. Look for `TODO` comments in `Source/`.

---

## 1. One-time setup

You need **Git**, **CMake 3.22+**, and a **C++ compiler**.

| OS | Install |
|----|---------|
| macOS | Xcode from the App Store, then `xcode-select --install` (this also gives you Git). CMake: `brew install cmake` or cmake.org |
| Windows | Visual Studio 2022 Community with the **"Desktop development with C++"** workload. Git for Windows from git-scm.com. CMake from cmake.org (tick "add to PATH") |
| Linux | `gcc`/`clang`, `cmake`, `git`, plus the packages in `JUCE/docs/Linux Dependencies.md` |

Check everything in a **new** terminal:

```bash
git --version
cmake --version     # 3.22 or newer
```

Tell Git who you are (once per machine):

```bash
git config --global user.name  "Your Name"
git config --global user.email "you@example.com"
```

## 2. Get the code

JUCE is included as a git submodule (pinned to JUCE 9.0.4), so clone **recursively**:

```bash
git clone --recursive <repo-url>
cd ConvolutionReverb
```

Forgot `--recursive` and the `JUCE/` folder is empty? Run `git submodule update --init`.

## 3. Build

```bash
cmake -B build
cmake --build build
```

The first build compiles all of JUCE and takes a few minutes. After that it's fast.

To work in an IDE instead:
- **Xcode:** `cmake -B build -G Xcode`, then open `build/CONVOLUTION_REVERB.xcodeproj`
- **Visual Studio:** File → Open → Folder, and pick this folder
- **CLion / VS Code:** open the folder; both detect `CMakeLists.txt`

Built plugins end up in `build/ConvolutionReverb_artefacts/`.

## 4. Run it

- **Standalone:** run the app in `build/ConvolutionReverb_artefacts/<config>/Standalone/`. Quickest check, no DAW needed.
- **In a DAW:** copy the `.vst3` (or `.component` on macOS) into your system plugin folder, or set `COPY_PLUGIN_AFTER_BUILD TRUE` in `CMakeLists.txt`. Rescan plugins and put it on an audio track.

✅ **Milestone 1 is done when every member has the passthrough plugin loading on their own machine.** If it works for you on an OS nobody else has documented yet, add your exact steps to this README.

---

## Team workflow

- Never push directly to `main`. Create a branch per feature: `git checkout -b feature/dry-wet-mix`
- Open a pull request on GitHub and have at least one teammate review it before merging.
- Pull `main` often: `git pull`, then `git merge main` into your branch.
- Don't commit `build/` or personal IDE files (`.gitignore` already handles this).
- Don't commit machine-specific paths (e.g. `C:\Users\you\...`).

## Project layout

```
CMakeLists.txt          Build config. Add new .cpp files to target_sources here.
JUCE/                   The JUCE framework (submodule, don't edit)
Source/
  PluginProcessor.*     Audio processing + parameters (audio thread)
  PluginEditor.*        The plugin window (UI thread)
IRs/                    Shared test impulse responses
```

---

## Suggested milestones

Verify each step works before moving on. It's much easier to find a bug when only one thing changed.

1. **Passthrough builds and loads for everyone.** (Starting point, done.)
2. **Hardcoded IR.** Load one IR file and convolve the input with it. 100% wet is fine.
3. **Dry/wet mix** as a real parameter, with a slider in the UI.
4. **Load IR from the UI** with a file chooser. Save the IR path in the plugin state so DAW sessions reopen correctly.
5. **Your own features.** Some ideas: pre-delay, low/high-cut on the wet signal, IR trim/reverse/stretch, decay shaping, output gain, waveform display of the IR.

## Where to start with the DSP

**JUCE classes to read up on** (docs at https://docs.juce.com):
- `juce::dsp::Convolution`: partitioned FFT convolution with background IR loading
- `juce::dsp::ProcessSpec`, `juce::dsp::AudioBlock`, `juce::dsp::ProcessContextReplacing`: how all `juce::dsp` objects plug into `prepareToPlay` / `processBlock`
- `juce::dsp::DryWetMixer`, `juce::dsp::DelayLine`, `juce::dsp::IIR::Filter` / `juce::dsp::StateVariableTPTFilter`
- `juce::AudioProcessorValueTreeState`: parameters and slider attachments
- `juce::FileChooser`, `juce::AudioFormatManager`: picking and reading audio files

**Theory worth understanding** (even if JUCE does it for you):
- What an impulse response is, and why convolving with it reproduces a space
- Direct (time-domain) convolution and why its cost is O(N·M)
- FFT convolution: overlap-add / overlap-save
- Uniformly partitioned convolution, and the trade-off between latency and long IRs
- Non-uniform partitioning: how real-world convolvers get both

**Real-time rules** (the audio thread must never wait):
- No `new`/`malloc`, no file reading, no locks inside `processBlock()`
- Allocate and prepare in `prepareToPlay()`
- Pass values from UI to audio thread with atomics (APVTS parameters already are)

**Gotchas specific to reverbs:**
- `getTailLengthSeconds()` should return the IR length, or hosts may cut off the tail when bouncing.
- If your processing adds latency, report it with `setLatencySamples()`.
- IR files may be at a different sample rate than the session. Find out how your convolution handles that.
- Long IRs can sum to very loud output. Normalise, and test with your monitors turned down.

**Stretch goal:** write your own uniformly-partitioned convolver and compare it against `juce::dsp::Convolution`, both for sound (null test) and CPU use.
