/*
  ==============================================================================

    GainProcessor.cpp
    Created: 15 Sep 2017 11:01:31am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "GainProcessor.h"
#include "GainProcessorEditor.h"

GainProcessor::GainProcessor(AudioProcessor& _parentProcessor) :
    SubProcessor(_parentProcessor)
{
}

GainProcessor::~GainProcessor()
{
}

AudioProcessorEditor* GainProcessor::createEditor()
{
    return new GainProcessorEditor(*this);
}

const String GainProcessor::getName() const
{
    return TRANS("Gain");
}