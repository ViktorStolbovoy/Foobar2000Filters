#pragma once

class ChannelTransformation : public Dsp::TransformBase
{
public:
	ChannelTransformation(int numStepReponseSamples, const float* const stepResponseSamples, int numReponseSamples, const Dsp::complex_t * const responseSamples, double sampleRate);
	virtual ~ChannelTransformation();
	double getSampleRate() const override;
	Dsp::complex_t response(double normalizedFrequency) const override;
	virtual const float* const getStepResponse(int* numSamples) override;
	std::vector<Dsp::PoleZeroPair> getPoleZeros() const;
private: 
	ChannelTransformation & operator = (ChannelTransformation & other) {}
	const int m_numStepResponseSamples;
	const float* const m_stepResponseSamples;
	const int m_numReponseSamples;
	const Dsp::complex_t * const m_responseSamples;
	const double m_sampleRate;
};