#pragma once

/// Rudimentary filter that blocks/zeroes signal
class NoPassFilter : public Dsp::Filter
{
public: 
	NoPassFilter(double sr, uint8 numChannels) : m_numChannels(numChannels), m_sampleRate(sr)
	{

	}
	virtual ~NoPassFilter() override {}

	virtual Dsp::Kind getKind() const override
	{
		return Dsp::Kind::kindOther;
	}

	virtual const std::string getName() const { return "No-pass"; }

	virtual int getNumParams() const override 
	{
		return 1;
	}
			
	virtual void reset() override {}
	
	virtual int getNumChannels() override
	{
		return m_numChannels;
	}

	virtual void process(int numSamples, float* const* arrayOfChannels) override
	{
		for (int i = 0; i < numSamples; i++)
		{
			for (int ch = 0; ch < m_numChannels; ch++)
			{
				*arrayOfChannels[ch] = 0;
			}
		}
	}

	virtual void process(int numSamples, double* const* arrayOfChannels) override
	{
		for (int i = 0; i < numSamples; i++)
		{
			for (int ch = 0; ch < m_numChannels; ch++)
			{
				*arrayOfChannels[ch] = 0;
			}
		}
	}
	virtual void processInterleavedSamples(int numPoints, float* samples) override
	{
		for (int i = 0; i < numPoints; i++)
		{
			for (int ch = 0; ch < m_numChannels; ch++)
			{
				samples[ch] = 0;
			}
		}
	}
	virtual void processInterleavedSamples(int numPoints, double* samples) override
	{
		for (int i = 0; i < numPoints; i++)
		{
			for (int ch = 0; ch < m_numChannels; ch++)
			{
				samples[ch] = 0;
			}
		}
	}

	virtual void processSingleSampleOneChannel(double *sample) override
	{
		*sample = 0;
	}

	virtual void processSingleSampleAllChannels(double *samples) override
	{
		for (int ch = 0; ch < m_numChannels; ch++)
		{
			samples[ch] = 0;
		}
	}

	virtual double getSampleRate() const override { return m_sampleRate; };
	virtual Dsp::complex_t response(double normalizedFrequency) const override
	{
		return 0;
	};

	virtual std::vector<Dsp::PoleZeroPair> getPoleZeros() const override
	{
		return std::vector<Dsp::PoleZeroPair>();
	}

	virtual Dsp::ParamInfo getParamInfo(int index) const override
	{
		return Dsp::ParamInfo();
	}
protected:
	virtual void doSetParams(const Dsp::Params& parameters) override {}

private:
	uint8 m_numChannels;
	double m_sampleRate;
};