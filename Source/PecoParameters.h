/*
  ==============================================================================

    PecoParameters.h
    Created: 8 Jun 2021 8:26:31pm
    Author:  Nicholas Berriochoa

  ==============================================================================
*/

#pragma once

#define MIN_DELAY 6000 // 10 ms
#define ONE_SECOND 48000
#define MAX_DELAY 96000 // 2 seconds

enum PecoParameters
{
    WetDryL = 0,
    FeedbackL,
    DistControlL,
    WetDryR,
    FeedbackR,
    DistControlR,
    Delay,
    DistType,
    Delay2,
    Delay3,
    TotalNumParameters
};

static juce::String PecoParameterID[TotalNumParameters] = {
    "WetDryL",
    "FeedbackL",
    "DistControlL",
    "WetDryR",
    "FeedbackR",
    "DistControlR",
    "Delay",
    "DistType",
    "Delay2",
    "Delay3"
};

static float PecoParameterMin[TotalNumParameters] = {
    0.0f,
    0.01f,
    1.0f,
    0.0f,
    0.1f,
    1.0f,
    MIN_DELAY,
    0.0f,
    MIN_DELAY,
    MIN_DELAY
};

static float PecoParameterMax[TotalNumParameters] = {
    1.0f,
    0.99f,
    15.0f,
    1.0f,
    0.99f,
    15.0f,
    MAX_DELAY,
    1,
    MAX_DELAY,
    MAX_DELAY
};

static float PecoParameterDefault[TotalNumParameters] = {
    0.5f,
    0.5f,
    1.0f,
    0.5f,
    0.5f,
    1.0f,
    ONE_SECOND,
    0,
    ONE_SECOND,
    ONE_SECOND
};
