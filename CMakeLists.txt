cmake_minimum_required(VERSION 3.22)

project(CONVOLUTION_REVERB VERSION 0.1.0)

# JUCE lives in the JUCE/ folder as a git submodule, pinned to a release tag.
# If this line fails, you cloned without --recursive. Run:
#   git submodule update --init
add_subdirectory(JUCE)

juce_add_plugin(ConvolutionReverb
    COMPANY_NAME "CHANGE ME"                    # TODO: your group / team name
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE               # Set TRUE to auto-install into your system plugin folder
    PLUGIN_MANUFACTURER_CODE Grpx               # TODO: 4 chars, at least one upper-case letter
    PLUGIN_CODE Cvrb                            # 4 chars, exactly one upper-case letter
    FORMATS AU VST3 Standalone                  # AU is only built on macOS
    PRODUCT_NAME "Convolution Reverb"
    NEEDS_WEB_BROWSER FALSE
    NEEDS_CURL FALSE)

target_sources(ConvolutionReverb
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp)

# Add new .cpp files to the list above as you create them,
# e.g. Source/DSP/MyConvolver.cpp

target_compile_definitions(ConvolutionReverb
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(ConvolutionReverb
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp                          # juce::dsp::Convolution, DryWetMixer, filters, etc.
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
