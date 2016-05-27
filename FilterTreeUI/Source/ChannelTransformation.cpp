#include "DspFilters/MathSupplement.h"
#include <Common.h>
#include <FilterListener.h>
#include "ConfigItem.h"
#include "ProcessorHelpers.h"
#include "ChannelTransformation.h"
#include "Processor.h"

ChannelTransformation::ChannelTransformation(int numStepReponseSamples, const float* const stepResponseSamples, int numReponseSamples, const Dsp::complex_t * const responseSamples, double sampleRate) : m_numStepResponseSamples(numStepReponseSamples), m_stepResponseSamples(stepResponseSamples),
	m_numReponseSamples(numReponseSamples), m_responseSamples(responseSamples), m_sampleRate(sampleRate)
{

}
ChannelTransformation::~ChannelTransformation()
{
	delete[] m_responseSamples;
	delete[] m_stepResponseSamples;
}


double ChannelTransformation::getSampleRate() const
{
	return m_sampleRate;
}

Dsp::complex_t ChannelTransformation::response(double normalizedFrequency) const 
{
	int idx = (int)(m_numReponseSamples * normalizedFrequency * 2.0 + 0.5);
	return (idx >= 0 && idx < m_numReponseSamples) ? m_responseSamples[idx] : 0; //If there is sufficient munber of points calculated we don't need to interpolate.
}

const float* const ChannelTransformation::getStepResponse(int* numSamples)
{
	*numSamples = m_numStepResponseSamples;
	float* res = new float[m_numStepResponseSamples];
	memcpy(res, m_stepResponseSamples, m_numStepResponseSamples * sizeof(float));
	return res;
}

std::vector<Dsp::PoleZeroPair> ChannelTransformation::getPoleZeros() const
{
	std::vector<Dsp::PoleZeroPair> res; 
	return res; 
}
