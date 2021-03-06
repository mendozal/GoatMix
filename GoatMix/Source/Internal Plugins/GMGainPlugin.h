/*
  ==============================================================================

    GMGainPlugin.h
    Created: 15 Sep 2017 11:01:31am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../Core/GMPluginInstance.h"
#include "../GUI/GoniometerSource.h"
#include "../GUI/MaxdBLabel.h"
#include "../DSP/VariableDelayLineResampling.h"
#include "../DSP/VariableDelayLinePSOLA.h"
#include "../DSP/Gain.h"
#include "../DSP/InvertPhase.h"
#include "../DSP/Pan.h"
#include "../DSP/MultiWindowedMeter.h"

class GMGainPlugin :
    public GMPluginInstance
{
public:
    GMGainPlugin(AudioProcessorValueTreeState& state);
    ~GMGainPlugin() {}

    void registerParameters() override;

    GMPluginInstanceEditor* createGoatMixEditor() override;
    const String getName() const override { return "Gain"; }
    const String getDisplayName() const override { return UIStrings::Gain::displayName; }

    void prepareToPlayDerived(double sampleRate, int maximumExpectedSamplesPerBlock) override;
    void processBlockDerived(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;

    GoniometerSource goniometerSource;
    MultiWindowedMeter meterSource;

    AudioProcessorParameterWithID* paramGain;
    AudioProcessorParameterWithID* paramGainL;
    AudioProcessorParameterWithID* paramGainR;
    AudioProcessorParameterWithID* paramPan;
    AudioProcessorParameterWithID* paramPhaseDelayL;
    AudioProcessorParameterWithID* paramPhaseDelayR;
    AudioProcessorParameterWithID* paramPhaseInvertL;
    AudioProcessorParameterWithID* paramPhaseInvertR;

private:
    Gain gainL;
    Gain gainR;
    Gain gainCenter;
    Pan pan;
    VariableDelayLinePSOLA delayL;
    VariableDelayLinePSOLA delayR;
    InvertPhase invertPhaseL;
    InvertPhase invertPhaseR;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GMGainPlugin)
};
