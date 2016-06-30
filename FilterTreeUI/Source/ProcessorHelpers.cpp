#include "DspFilters/MathSupplement.h"
#include <Common.h>
#include "ConfigItem.h"
#include "ProcessorHelpers.h"


FilterCommand::FilterCommand(uint8 channelIdx, uint8 busIdx, uint8 numChannels, FilterConfigEx fc) : CommandBase(channelIdx, busIdx, numChannels), m_filterConfig(fc), m_filter(nullptr){}
FilterCommand::~FilterCommand()
{
	delete m_filter;
}

void FilterCommand::Setup(double fs) 
{
	m_filter = m_filterConfig.createFilter(fs, m_numChannels);
}
void FilterCommand::Apply(double *data) 
{
	double * chs = data + m_channelIdx;
	m_filter->processInterleavedSamples(1, chs);
}

void FilterCommand::CalculateResponse(Dsp::complex_t  *responses, double normalizedFrequency) 
{
	responses[m_busIdx] *= m_filter ? m_filter->response(normalizedFrequency) : 0;
}

void FilterCommand::Reset()
{
	if (m_filter) m_filter->reset();
}

MixerCommand::MixerCommand(uint8 channelIdx, uint8 busIdx, uint8 outputBus, uint8 outputChannel, double gain, uint8 numChannels) : CommandBase(channelIdx, busIdx, numChannels), m_channelGain(gain), m_outputBus(outputBus), m_outputChannel(outputChannel) {}
MixerCommand::~MixerCommand() {}

void MixerCommand::Apply(double *data)
{
	double * inp = data + m_channelIdx;
	double * out = data + m_outputChannel;

	for (uint8 i = 0; i < m_numChannels; i++)
	{
		*out += inp[i] * m_channelGain;
	}

}

void MixerCommand::CalculateResponse(Dsp::complex_t  *responses, double normalizedFrequency)
{
	responses[m_outputBus] += responses[m_busIdx] * m_channelGain;
}

void MixerCommand::Setup(double fs) {}

MixerNewCommand::MixerNewCommand(uint8 channelIdx, uint8 busIdx, uint8 outputBus, uint8 outputChannel, double gain, uint8 numChannels) : MixerCommand(channelIdx, busIdx, outputBus, outputChannel, gain, numChannels) {}
MixerNewCommand::~MixerNewCommand() {}

void MixerNewCommand::Apply(double *data)
{
	//Mixers mix everything to a single channel 
	double * inp = data + m_channelIdx;
	double * out = data + m_outputChannel;

	*out = (*inp) * m_channelGain;
	for (uint8 i = 1; i < m_numChannels; i++)
	{
		*out += inp[i] * m_channelGain;
	}
}

void MixerNewCommand::CalculateResponse(Dsp::complex_t  *responses, double normalizedFrequency) 
{
	responses[m_outputBus] = responses[m_busIdx] * m_channelGain;
}

CopyCommand::CopyCommand(uint8 channelIdx, uint8 busIdx, uint8 firstOutputChannel, uint8 firstOutputBus, uint8 numChannels, uint8 numOutputBusses) : CommandBase(channelIdx, busIdx, numChannels), m_firstOutputChannel(firstOutputChannel),
		m_numOutputBusses(numOutputBusses), m_firstOutputBus(firstOutputBus){}
void CopyCommand::Apply(double *data) 
{
	double * targetBase = data + m_firstOutputChannel;
	double * targetStop = targetBase + m_numChannels * m_numOutputBusses;

	double * srcBase = data + m_channelIdx;

	for (uint8 chIdx = 0; chIdx < m_numChannels; chIdx++)
	{
		double sample = srcBase[chIdx];
		for (double*target = targetBase + chIdx; target < targetStop; target += m_numChannels)
		{
			*target = sample;
		}
	}
}

void CopyCommand::CalculateResponse(Dsp::complex_t  *responses, double normalizedFrequency) 
{
	Dsp::complex_t s = responses[m_busIdx];
	for (uint8 i = m_firstOutputBus; i < m_firstOutputBus + m_numOutputBusses; i++)
	{
		responses[i] = s;
	}
}

void CopyCommand::Setup(double fs) {}

MixerConfig::MixerConfig(MixerConfigItem* config) : BusIdx(0), m_config(config){}
uint8 BusIdx;
bool MixerConfig::IsConnectedToVOut(uint32 vOutId)
{
	for (size_t i = 0; i < m_config->VirtualOutputs.size(); i++)
	{
		if (vOutId == m_config->VirtualOutputs[i]) return true;
	}
	return false;
}


BusConfigItem::BusConfigItem(const InputConfigItem* input, uint8 channleIndex) : m_inputMask(input->InputMask), m_numChannels(ConfigItemBase::GetChannelCount(input->InputMask)), CurrentConfigItemId(input->Id), m_channelIndex(channleIndex) {}

BusConfigItem::BusConfigItem(uint8 numChannels, uint8 channleIndex) : m_numChannels(numChannels), m_inputMask(0), m_channelIndex(channleIndex) {}

BusConfigItem::BusConfigItem(const BusConfigItem & src) : m_numChannels(src.m_numChannels), m_inputMask(src.m_inputMask), m_channelIndex(src.m_channelIndex)
{
	if (src.m_outputChannels.size() > 0)
	{
		for (size_t i = 0; i < src.m_outputChannels.size(); i++)
		{
			m_outputChannels.push_back(src.m_outputChannels[i]);
		}
	}
}

BusConfigItem & BusConfigItem::operator =(const BusConfigItem & src)
{
	if (this == &src) return *this;
	m_numChannels = src.m_numChannels;
	m_inputMask = src.m_inputMask;
	for (size_t i = 0; i < m_outputChannels.size(); i++)
	{
		delete m_outputChannels[i];
	}
	m_outputChannels.clear();

	if (src.m_outputChannels.size() > 0)
	{
		for (size_t i = 0; i < src.m_outputChannels.size(); i++)
		{
			m_outputChannels.push_back(src.m_outputChannels[i]);
		}
	}
	
	m_channelIndex = src.m_channelIndex;
	return *this;
}


FilterBlockConfig::FilterBlockConfig(uint32 elementId, uint32 vInputId, const std::vector<ConfigItemBase *>& cfgs) : m_vInputId(vInputId)
{
	uint32 currentId = elementId;
	//Items are always stored from parent to child
	for (size_t i = 0; i < cfgs.size(); i++)
	{
		auto c = dynamic_cast<FilterConfigItem*>(cfgs[i]);
		if (c && c->ParentId == currentId)
		{
			currentId = c->Id;
			m_filters.push_back(c);
		}
	}
}


void FilterBlockConfig::GenCommands(std::vector<CommandBase *> & commands, uint8 channelIndex, uint8 busIndex, uint8 numChannels)
{
	for (size_t i = 0; i < m_filters.size(); i++)
	{
		auto f = new FilterCommand(channelIndex, busIndex, numChannels, m_filters[i]->Config);
		commands.push_back(f);
	}
}
	
