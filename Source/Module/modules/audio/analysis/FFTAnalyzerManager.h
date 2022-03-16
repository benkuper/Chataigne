/*
  ==============================================================================

    FFTAnalyzerManager.h
    Created: 14 May 2019 4:08:37pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class FFTAnalyzerManager :
	public BaseManager<FFTAnalyzer>
{
public:
	FFTAnalyzerManager();
	~FFTAnalyzerManager();


	FloatParameter* minDB;
	FloatParameter* maxDB;


	//FFT
	enum
	{
		fftOrder = 11,            // [1]
		fftSize = 1 << fftOrder, // [2]
		scopeSize = 256            // [3]
	};

	dsp::FFT forwardFFT;                  // [4]
	dsp::WindowingFunction<float> window; // [5]
	float fifo[fftSize];                 // [6]
	float fftData[2 * fftSize];          // [7]
	int fifoIndex = 0;                    // [8]
	bool nextFFTBlockReady = false;       // [9]
	float scopeData[scopeSize];          // [10]

	void process(const float* samples, int numSamples);
	void pushNextSampleIntoFifo(float sample);

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
};