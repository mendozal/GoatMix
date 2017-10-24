/*
  ==============================================================================

    MobileMixPluginInstanceEditor.h
    Created: 7 Oct 2017 5:17:49pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "MobileMixPluginInstance.h"

class MobileMixPluginInstanceEditor :
    public AudioProcessorEditor
{
protected:
    MobileMixPluginInstanceEditor(MobileMixPluginInstance& processor);
    virtual ~MobileMixPluginInstanceEditor() {}

    /** Convenience methods + definitions for creating attachment points for
        editor UI elements.
    */
    typedef std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> MMButtonAttachment;
    typedef std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> MMComboBoxAttachment;
    typedef std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> MMSliderAttachment;

    MMButtonAttachment createButtonAttachment(String parameterID, Button& buttonToControl);
    MMComboBoxAttachment createComboBoxAttachment(String parameterID, ComboBox& comboBoxToControl);
    MMSliderAttachment createSliderAttachment(String parameterID, Slider& sliderToControl);

    MobileMixPluginInstance& processor;

private:
    MMButtonAttachment bypassButtonAttachment;
    ToggleButton bypassButton;
};