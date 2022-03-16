/*
  ==============================================================================

    FFTAnalyzerManager.cpp
    Created: 14 May 2019 4:08:37pm
    Author:  bkupe

  ==============================================================================
*/

FFTAnalyzerManager::FFTAnalyzerManager() :
	BaseManager("FFT Analysis"),
	forwardFFT(fftOrder),
	window(fftSize, dsp::WindowingFunction<float>::hann)
{
	setCanBeDisabled(true);
	enabled->setValue(false);
	editorIsCollapsed = true;


	minDB = addFloatParameter("Min DB", "", -100, -100, 20);
	maxDB = addFloatParameter("Max DB", "", 0, -100, 20);

	selectItemWhenCreated = false;
}

FFTAnalyzerManager::~FFTAnalyzerManager()
{
}



void FFTAnalyzerManager::process(const float* samples, int numSamples)
{
	if (!enabled->boolValue()) return;

	for (auto i = 0; i < numSamples; ++i) pushNextSampleIntoFifo(samples[i]);
	window.multiplyWithWindowingTable(fftData, fftSize);      // [1]


	if (nextFFTBlockReady)
	{
		forwardFFT.performFrequencyOnlyForwardTransform(fftData);

		auto mindB = minDB->floatValue();
		auto maxdB = jmax<float>(maxDB->floatValue(), mindB);

		for (int i = 0; i < scopeSize; ++i)                        // [3]
		{
			auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - i / (float)scopeSize) * 0.2f);
			auto fftDataIndex = jlimit(0, fftSize / 2, (int)(skewedProportionX * fftSize / 2));
			auto level = jmap(jlimit(mindB, maxdB, Decibels::gainToDecibels(fftData[fftDataIndex]) - Decibels::gainToDecibels((float)fftSize)), mindB, maxdB, 0.0f, 1.0f);
			scopeData[i] = level;                                  // [4]
		}

		for (auto& i : items) i->process(scopeData, scopeSize);

		nextFFTBlockReady = false;
	}
}

void FFTAnalyzerManager::pushNextSampleIntoFifo(float sample)
{
	if (fifoIndex == fftSize)    // [11]
	{
		if (!nextFFTBlockReady) // [12]
		{
			zeromem(fftData, sizeof(fftData));
			memcpy(fftData, fifo, sizeof(fifo));
			nextFFTBlockReady = true;
		}
		fifoIndex = 0;
	}
	fifo[fifoIndex++] = sample;  // [12]
}

InspectableEditor* FFTAnalyzerManager::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new FFTAnalyzerManagerEditor(this, isRoot);
}