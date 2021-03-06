/*
  ==============================================================================

    GMCompressorPlugin.cpp
    Created: 15 Sep 2017 11:01:31am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "GMCompressorPlugin.h"
#include "GMCompressorPluginEditor.h"
#include "../GUI/ValueStringFuncs.h"

GMCompressorPlugin::GMCompressorPlugin(AudioProcessorValueTreeState& state) :
    GMPluginInstance(state)
{
}

void GMCompressorPlugin::registerParameters()
{
    // Add parameters here via createParameter...
    paramAttack = state.createAndAddParameter(
        addPrefixToParameterName("Attack"),
        addPrefixToParameterName("Attack"),
        ValueStringFuncs::Milliseconds::unit,
        NormalisableRange<float>(5.0f, 500.0f, 1.0f),
        5.0f,
        ValueStringFuncs::Milliseconds::valueToText,
        ValueStringFuncs::Generic::textToValue);

    paramRelease = state.createAndAddParameter(
        addPrefixToParameterName("Release"),
        addPrefixToParameterName("Release"),
        ValueStringFuncs::Milliseconds::unit,
        NormalisableRange<float>(5.0f, 500.0f, 1.0f),
        5.0f,
        ValueStringFuncs::Milliseconds::valueToText,
        ValueStringFuncs::Generic::textToValue);

    paramRatio = state.createAndAddParameter(
        addPrefixToParameterName("Ratio"),
        addPrefixToParameterName("Ratio"),
        ": 1",
        NormalisableRange<float>(1.0f, 20.0f, 1.0f),
        1.0f,
        [](float value){ return ValueStringFuncs::Generic::valueToText(value, ": 1", 0); },
        ValueStringFuncs::Generic::textToValue);

    paramThreshold = state.createAndAddParameter(
        addPrefixToParameterName("Threshold"),
        addPrefixToParameterName("Threshold"),
        ValueStringFuncs::Gain::unit,
        ValueStringFuncs::Gain::range,
        ValueStringFuncs::Gain::defaultValue,
        ValueStringFuncs::Gain::valueToText,
        ValueStringFuncs::Gain::textToValue);

    NormalisableRange<float> makeupRange(ValueStringFuncs::Gain::range);
    makeupRange.end = 3.0f;
    paramMakeupGain = state.createAndAddParameter(
        addPrefixToParameterName("Makeup Gain"),
        addPrefixToParameterName("Makeup Gain"),
        ValueStringFuncs::Gain::unit,
        makeupRange,
        ValueStringFuncs::Gain::defaultValue,
        ValueStringFuncs::Gain::valueToText,
        ValueStringFuncs::Gain::textToValue);


    paramDryWet = state.createAndAddParameter(
        addPrefixToParameterName("Dry/Wet"),
        addPrefixToParameterName("Dry/Wet"),
        ValueStringFuncs::Percent::unit,
        ValueStringFuncs::Percent::range,
        0.5f,
        ValueStringFuncs::Percent::valueToText,
        ValueStringFuncs::Percent::textToValue);
}

GMPluginInstanceEditor* GMCompressorPlugin::createGoatMixEditor()
{
    return new GMCompressorPluginEditor(*this);
}

void GMCompressorPlugin::prepareToPlayDerived(double sampleRate, int maximumExpectedSamplesPerBlock)
{
    dsp::ProcessSpec spec
    {
        sampleRate,
        static_cast<uint32>(maximumExpectedSamplesPerBlock),
        static_cast<uint32>(getTotalNumInputChannels())
    };

    meter.prepare(spec);
    compressor.prepare(spec);
    makeup.prepare(spec);
    dryWet.prepare(spec);
}

void GMCompressorPlugin::processBlockDerived(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    compressor.params->attack = getUnnormalizedValue(paramAttack);
    compressor.params->release = getUnnormalizedValue(paramRelease);
    compressor.params->ratio = getUnnormalizedValue(paramRatio);
    compressor.params->threshold = getUnnormalizedValue(paramThreshold);
    makeup.params->gain = getUnnormalizedValue(paramMakeupGain);
    dryWet.params->wetDry = getUnnormalizedValue(paramDryWet);

    dryWet.copyDrySignal(buffer);
    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
    meter.process(context);
    compressor.process(context);
    makeup.process(context);
    dryWet.process(context);
}
