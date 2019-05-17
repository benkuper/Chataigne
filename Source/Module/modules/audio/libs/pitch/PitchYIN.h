#include "JuceHeader.h"
#include "PitchDetector.h"

class PitchYIN :
	public PitchDetector
{

public:

    PitchYIN (int sampleRate, unsigned int bufferSize) : yin (1, bufferSize), bufferSize (bufferSize), tolerence (0.15), sampleRate(sampleRate),
    deltaWasNegative (false)
    {
    }
    
    PitchYIN (unsigned int bufferSize) : yin (1, bufferSize), bufferSize (bufferSize), tolerence (0.15), sampleRate(44100),
    deltaWasNegative (false)
    {
    }
    
    void setSampleRate(unsigned int newSampleRate)
    {
        sampleRate = newSampleRate;
    }

	unsigned int getSampleRate()
	{
		return sampleRate;
	}

	unsigned int getBufferSize() override
	{
		return bufferSize;
	}

	void setBufferSize(int value) override
	{
		bufferSize = value;
	}


    /** Output the difference function */
    void difference(AudioSampleBuffer& input) 
    {
        float tmp;
        float *yinData = yin.getWritePointer(0);
        const float *inputData = input.getReadPointer(0);

        FloatVectorOperations::fill(yinData, 0.0, yin.getNumSamples());

        for (int tau = 1; tau < yin.getNumSamples(); tau++) 
        {
            for (int j = 0; j < yin.getNumSamples(); j++) 
            {
                tmp = inputData[j] - inputData[j + tau];
                yinData[tau] += (tmp * tmp);
            }
        }
    }

    /** cumulative mean normalized difference function */
    void cumulativeMean ()
    {
        float *yinData = yin.getWritePointer(0);
        float tmp = 0.;
        yinData[0] = 1.;
        //AUBIO_DBG("%f\t",yinData[0]);
        for (int tau = 1; tau < yin.getNumSamples(); tau++)
        {
            tmp += yinData[tau];
            yinData[tau] *= tau / tmp;
            //AUBIO_DBG("%f\t",yinData[tau]);
        }
        //AUBIO_DBG("\n");
    }

    int getPitch ()
    {
        int tau = 0;
        float *yinData = yin.getWritePointer(0);
        do {
            if (yinData[tau] < 0.1) 
            {
                while (yinData[tau + 1] < yinData[tau]) 
                {
                    tau++;
                }
                return tau;
            }
            tau++;
        } while (tau < yin.getNumSamples());
        //AUBIO_DBG("No pitch found");
        return 0;
        
    }
    
    
    /** Full YIN algorithm */

	float getPitch(const float* inputData) override
	{
		return calculatePitch(inputData);
	}

    float calculatePitch (const float* inputData) noexcept
    {
        int period;
        float delta = 0.0, runningSum = 0.0;
        float *yinData = yin.getWritePointer(0);
        //deltaWasNegative = false;

        //DBG ("calculatePitch");

        yinData[0] = 1.0;
        for (int tau = 1; tau < yin.getNumSamples(); tau++)
        {
            yinData[tau] = 0.0;
            for (int j = 0; j < yin.getNumSamples(); j++)
            {
                delta = inputData[j] - inputData[j + tau];
                yinData[tau] += (delta * delta);
                //if (delta < 0) deltaWasNegative = true;
            }
            runningSum += yinData[tau];
            if (runningSum != 0)
            {
                yinData[tau] *= tau / runningSum;
            }
            else
            {
                yinData[tau] = 1.0;
            }
            period = tau - 3;
            if (tau > 4 && (yinData[period] < tolerence) &&
                    (yinData[period] < yinData[period + 1]))
            {
                DBG ("return early");
                return quadraticPeakPosition (yin.getReadPointer(0), period);
            }
        }
        return quadraticPeakPosition (yin.getReadPointer(0), minElement (yin.getReadPointer(0)));
    }

    float getPitchInHz (const float* inputData) noexcept
    {
        float pitch = 0.0;
        //slideBlock (input);
        pitch = calculatePitch (inputData);
       // log->writeToLog("pitch: " + String(pitch));
        
        if (pitch > 0)
        {
            pitch = sampleRate / (pitch + 0.0);
            //log->writeToLog ("pitchInHz: " + String(pitch));
        }
        else
        {
            pitch = 0.0;
        }
        currentPitch = pitch;
        //if (deltaWasNegative) DBG("delta negative");
        
        return pitch;
    }
    
    void setTolerence(float newTolerence)
    {
        tolerence = newTolerence;
    }

private:
    AudioSampleBuffer yin; //, buf;
    //float* yinData;
    unsigned int bufferSize;
    unsigned int tolerence; //, confidence;
    unsigned int sampleRate;
    bool deltaWasNegative;
    float currentPitch;
    Logger *log;

//    /** adapter to stack ibuf new samples at the end of buf, and trim `buf` to `bufsize` */
//    void slideBlock (AudioSampleBuffer& ibuf)
//    {
//        float *bufData = buf.getWritePointer(0);
//        const float *ibufData = ibuf.getReadPointer(0);
//
//        unsigned int j = 0, overlapSize = 0;
//        overlapSize = buf.getNumSamples() - ibuf.getNumSamples();
//        for (j = 0; j < overlapSize; j++) 
//        {
//            bufData[j] = bufData[j + ibuf.getNumSamples()];
//        }
//        for (j = 0; j < ibuf.getNumSamples(); j++) 
//        {
//            bufData[j + overlapSize] = ibufData[j];
//        }
//    }

    // Below functions should go in a seperate utilities class

    float quadraticPeakPosition (const float *data, unsigned int pos) noexcept
    {
        float s0, s1, s2;
        unsigned int x0, x2;
        if (pos == 0 || pos == bufferSize - 1) return pos;
        x0 = (pos < 1) ? pos : pos - 1;
        x2 = (pos + 1 < bufferSize) ? pos + 1 : pos;
        if (x0 == pos) return (data[pos] <= data[x2]) ? pos : x2;
        if (x2 == pos) return (data[pos] <= data[x0]) ? pos : x0;
        s0 = data[x0];
        s1 = data[pos];
        s2 = data[x2];
        return pos + 0.5 * (s0 - s2 ) / (s0 - 2.* s1 + s2);
    }

    unsigned int minElement (const float *data) noexcept
    {
    #ifndef JUCE_USE_VDSP_FRAMEWORK
        unsigned int j, pos = 0;
        float tmp = data[0];
        for (j = 0; j < bufferSize; j++)
        {
            pos = (tmp < data[j]) ? pos : j;
            tmp = (tmp < data[j]) ? tmp : data[j];
        }
    #else
        float tmp = 0.0;
        unsigned int pos = 0;
        #if !DOUBLE_SAMPLES
        vDSP_minvi(data, 1, &tmp, (vDSP_Length *)&pos, bufferSize);
        #else
        vDSP_minviD(data, 1, &tmp, (vDSP_Length *)&pos, bufferSize);
        #endif
    #endif
        return pos;
    }    
};
