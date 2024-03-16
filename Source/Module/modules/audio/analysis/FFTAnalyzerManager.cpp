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

	//script
	scriptObject.getDynamicObject()->setMethod("getFFTData", &FFTAnalyzerManager::getFFTDataFromScript);
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

		float tmpScopeData[scopeSize];
		for (int i = 0; i < scopeSize; ++i)                        // [3]
		{
			auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - i / (float)scopeSize) * 0.2f);
			auto fftDataIndex = jlimit(0, fftSize / 2, (int)(skewedProportionX * fftSize / 2));
			auto level = jmap(jlimit(mindB, maxdB, Decibels::gainToDecibels(fftData[fftDataIndex]) - Decibels::gainToDecibels((float)fftSize)), mindB, maxdB, 0.0f, 1.0f);
			tmpScopeData[i] = level;                                  // [4]
		}

		for (auto& i : items)
		{
			i->process(tmpScopeData, scopeSize);
		}

		{
			const ScopedLock lock(scopeDataMutex);
			memcpy(scopeData, tmpScopeData, sizeof(scopeData));
		}

		nextFFTBlockReady = false;
	}
}

void FFTAnalyzerManager::copyScopeData(float* scopeData, int maxSize) const
{
	const ScopedLock lock(scopeDataMutex);
	memcpy(scopeData, this->scopeData, std::min(maxSize * sizeof(*scopeData), sizeof(this->scopeData)));
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

var FFTAnalyzerManager::getFFTDataFromScript(const var::NativeFunctionArgs& a)
{
	FFTAnalyzerManager* manager = getObjectFromJS<FFTAnalyzerManager>(a);
	var result;
	if (a.numArguments > 0 && a.arguments[0].isArray())
	{
		result = a.arguments[0];
	}
	else
	{
		result = Array<var>{};
	}
	
	Array<var>& resultArray = *result.getArray();
	resultArray.clearQuick();

	float tmpScopeData[scopeSize];
	manager->copyScopeData(tmpScopeData);
	resultArray.addArray((float*)tmpScopeData, scopeSize);
	return result;
}
