/*
  ==============================================================================

   This file is part of the JUCE examples.
   Copyright (c) 2017 - ROLI Ltd.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

#pragma once


//==============================================================================
/* This component scrolls a continuous waveform showing the audio that's
   coming into whatever audio inputs this object is connected to.
*/
class LiveScrollingAudioDisplay  : public AudioVisualiserComponent,
                                   public AudioIODeviceCallback
{
public:
    LiveScrollingAudioDisplay()  : AudioVisualiserComponent (1) // set number of channels
    {
        setSamplesPerBlock (256);
        setBufferSize (1024);
    }
    float gain = 10.0f; // amplify signal

    //==============================================================================
    void audioDeviceAboutToStart (AudioIODevice*) override
    {
        clear();
    }

    void audioDeviceStopped() override
    {
        clear();
    }

    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels,
                                int numberOfSamples) override
    {
		//// multiple channels (not working)
		//int numChannels =  numInputChannels;
		//float *inputSamples = (float*)malloc(numChannels * sizeof(float)); 
		//for (int chan = 0; chan < numChannels; ++chan) {
		//	inputSamples[chan] = 0.0f;
		//}
  //      for (int i = 0; i < numberOfSamples; ++i)
  //      {

		//	// output input channel
		//	int chan;
		//	for (chan = 0; chan < numInputChannels; ++chan)
		//		if (const float* inputChannel = inputChannelData[chan])
		//			inputSamples[chan] = inputChannel[i];
		//	        inputSamples[chan] *= 10.0f; // boost the level to make it more easily visible.
		//			pushSample (&inputSamples[chan], 1);

  //      }
		//free(inputSamples);
		 


		// single channel (summarization of all signals)
        for (int i = 0; i < numberOfSamples; ++i)
        {
            float inputSample = 0;

            for (int chan = 0; chan < numInputChannels; ++chan)
                if (const float* inputChannel = inputChannelData[chan])
                    inputSample += inputChannel[i];  // find the sum of all the channels

            inputSample /= numInputChannels / gain; // boost the level to make it more easily visible.

            pushSample (&inputSample, 1);
        }

        // We need to clear the output buffers before returning, in case they're full of junk..
        for (int j = 0; j < numOutputChannels; ++j)
            if (float* outputChannel = outputChannelData[j])
                zeromem (outputChannel, (size_t) numberOfSamples * sizeof (float));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LiveScrollingAudioDisplay)
};
