#include "DspFilters/MathSupplement.h"
#include <Common.h>
#include "ConfigItem.h"
#include "ProcessorHelpers.h"
#include <FilterListener.h>
#include "ChannelTransformation.h"
#include "Processor.h"
#include <Windows.h>


Processor::Processor(std::vector<ConfigItemBase *> &configItems) : m_channelCount(0), m_buffer(nullptr), m_inputMap(nullptr), 
					m_inputChannelCount(0), m_delay(nullptr), m_gain(nullptr), m_delayLen(nullptr), m_outputChannelCount(0), 
					m_outputMap(nullptr), m_delayIdx(nullptr), m_commands(nullptr), m_commandsLength(0)
{
	std::vector<MixerConfig *> mixers;
	std::vector<FilterBlockConfig *> filterBlockDefinitions;
	std::vector<ConfigItemBase *> other; //Filters, filter blocks, outputs, virtual outputs

	std::vector<CommandBase *> commands;

	for (size_t i = 0; i < configItems.size(); i++)
	{
		auto item = configItems[i];
		if (!item) continue;
		auto inp = dynamic_cast<InputConfigItem*>(item);
		if (inp)
		{
			if (inp->IsVirtual)
			{
				_RPT1(_CRT_WARN, "Found filter definition %i\r\n", inp->Id);
				filterBlockDefinitions.push_back(new FilterBlockConfig(inp->Id, inp->InputMask, configItems));
			}
			else
			{
				_RPT2(_CRT_WARN, "Found input %i placed in bus %i \r\n", inp->Id, m_busConfig.size());
				BusConfigItem *  configItem = new BusConfigItem(inp, m_channelCount);
				m_channelCount += ConfigItemBase::GetChannelCount(inp->InputMask);
				m_busConfig.push_back(configItem);
			}
		}
		else
		{
			auto mx = dynamic_cast<MixerConfigItem*>(item);
			if (mx)
			{
				mixers.push_back(new MixerConfig(mx));
			}
			else
			{
				other.push_back(item);
			}
		}
	}
	 

	std::vector<ConfigItemBase *> childFilters;
	std::vector<VirtualOutputConfigItem *> vOuts;
		
	for (uint8 busIdx = 0; busIdx < (uint8)m_busConfig.size(); busIdx++)
	{
		_RPT1(_CRT_WARN, "Processing %i bus\r\n", busIdx);

		//Items stored always from parent to child, so no extra loop is necessary 
		auto current = m_busConfig[busIdx];
		size_t otherStartIdx = 0;
	
		size_t numChildBusses = 1;
		while (numChildBusses > 0)
		{
			bool isFirstChild = true;
			uint8 channelIndex = current->GetChannelIndex();
			uint8 numChannels = current->GetNumChannels();
			uint32 currentId = current->CurrentConfigItemId;
			childFilters.clear();
			vOuts.clear();

			bool copyFirstFilter = false;
			_RPT1(_CRT_WARN, "	Looking for item %i children\r\n", currentId);
			for (size_t cfgIdx = otherStartIdx; cfgIdx < other.size(); cfgIdx++)
			{
				auto c = other[cfgIdx];

				if (c->ParentId == currentId)
				{
					if (isFirstChild)
					{
						isFirstChild = false;
						otherStartIdx = cfgIdx + 1; // Tehre is no children defore current item
					}
					auto vOut = dynamic_cast<VirtualOutputConfigItem *>(c);
					if (vOut)
					{
						_RPT1(_CRT_WARN, "		Processing virtual out %i \r\n", c->Id);
						AddToMixer(vOut, mixers, channelIndex, busIdx, numChannels, commands);
					}
					else
					{
						auto out = dynamic_cast<OutputConfigItem *>(c);
						if (out)
						{
							_RPT1(_CRT_WARN, "		Adding output info from %i \r\n", c->Id);
							current->AddOutputInfo(out);
							copyFirstFilter = true;
						}
						else
						{
							childFilters.push_back(c); //Filters or filter blocks
						}
					}
				}
			}

			//Process filters
			numChildBusses = (uint8)childFilters.size();
			_RPT2(_CRT_WARN, "		Processing rest of the children of %i (%i found)...\r\n", currentId, numChildBusses);
			if (numChildBusses > 0)
			{
				if ((numChildBusses == 1) && (!copyFirstFilter))
				{
					_RPT1(_CRT_WARN, "		Adding filter %i \r\n", childFilters[0]->Id);
					AddFilterCommand(busIdx, childFilters[0], filterBlockDefinitions, commands);
				}
				else
				{
					uint8 numNewBusses = (uint8) ((copyFirstFilter) ? numChildBusses : numChildBusses - 1);
					uint8 nextBusIdx = (uint8)m_busConfig.size();
					//copy command before filter command
					_RPT1(_CRT_WARN, "		Copying bus to %i \r\n", nextBusIdx);
					auto copy = new CopyCommand(channelIndex, busIdx, m_channelCount, nextBusIdx, numChannels, numNewBusses);
					commands.push_back(copy);

					size_t startIdx;
					if (!copyFirstFilter)
					{
						startIdx = 1;
						_RPT1(_CRT_WARN, "		Adding filter %i \r\n", childFilters[0]->Id);	   
						AddFilterCommand(busIdx, childFilters[0], filterBlockDefinitions, commands);
					}
					else
					{
						startIdx = 0;
					}

					for (size_t i = startIdx; i < numChildBusses; i++, nextBusIdx++)
					{
						//Additinal filters - new bus
						BusConfigItem *  configItem = new BusConfigItem(numChannels, m_channelCount);
						m_channelCount += numChannels;
						m_busConfig.push_back(configItem);
						size_t newBusIdx = m_busConfig.size() - 1;
						_RPT1(_CRT_WARN, "		Bus info initialized for %i \r\n", newBusIdx);
						_RPT2(_CRT_WARN, "		Adding filter %i to bus %i \r\n", childFilters[i]->Id, newBusIdx);
						AddFilterCommand(nextBusIdx, childFilters[i], filterBlockDefinitions, commands);
					}
				}
			}
		} 
	}
	
	for (size_t i = 0; i < filterBlockDefinitions.size(); i++)
	{
		delete filterBlockDefinitions[i];
	}

	for (size_t i = 0; i < mixers.size(); i++)
	{
		delete mixers[i];
	}

	m_commandsLength = commands.size();
	m_commands = new CommandBase*[m_commandsLength];
	for (uint32 i = 0; i < m_commandsLength; i++)
	{
		m_commands[i] = commands[i];
	}

}



void Processor::AddFilterCommand(uint8 busIdx, ConfigItemBase * cfg, std::vector<FilterBlockConfig *>& filterBlockDefinitions, std::vector<CommandBase *> &commands)
{
	auto busConfig = m_busConfig[busIdx];
	uint8 channelIdx = busConfig->GetChannelIndex();
	uint8 numChannels = busConfig->GetNumChannels();
	auto filterCfg = dynamic_cast<FilterConfigItem *>(cfg);
	//uint32 originalId = busConfig->CurrentConfigItemId;
	if (filterCfg)
	{
		auto filterCommand = new FilterCommand(channelIdx, busIdx, numChannels, filterCfg->Config);
		if (filterCommand->IsValid())
		{
			commands.push_back(filterCommand);
			busConfig->CurrentConfigItemId = filterCfg->Id;
		}
	}
	else
	{
		auto filterBlock = dynamic_cast<FilterBlockConfigItem *>(cfg);
		if (filterBlock)
		{
			uint32 vInp = filterBlock->VirtualInput;
			size_t sz = filterBlockDefinitions.size();
			for (size_t i = 0; i < sz; i++)
			{
				auto fbd = filterBlockDefinitions[i];
				if (fbd->GetVInputId() == vInp)
				{
					fbd->GenCommands(commands, channelIdx, busIdx, numChannels);
					busConfig->CurrentConfigItemId = filterBlock->Id;
					break;
				}
			}
		}
	}
	
	//FilterBlock definition may not be assigned yet 
	//if (originalId == busConfig->CurrentConfigItemId) throw std::logic_error("AddCommand can't set filter command[s]");
}

void Processor::AddToMixer(VirtualOutputConfigItem *vOut, std::vector<MixerConfig *> &mixers, uint8 srcChannelIndex, uint8 srcBusIdx, uint8 srcNumChannels, std::vector<CommandBase *> &commands)
{
	uint32 vOutNum = vOut->Number;
	size_t mixersSz = mixers.size();
	for (size_t i = 0; i < mixersSz; i++)
	{
		auto mx = mixers[i];
		if (mx->IsConnectedToVOut(vOutNum))
		{
			if (mx->BusIdx == 0) //0 bus is always input, so it is mew mixer
			{
				mx->BusIdx = (uint8)m_busConfig.size();
				commands.push_back(new MixerNewCommand(srcChannelIndex, srcBusIdx, mx->BusIdx, m_channelCount, vOut->Gain, srcNumChannels));
				_RPT2(_CRT_WARN, "		New mixer command %i added to bus %i \r\n", mx->GetId(), mx->BusIdx);

				BusConfigItem *  configItem = new BusConfigItem(1, m_channelCount);
				configItem->CurrentConfigItemId = mx->GetId();
				m_channelCount++;
				m_busConfig.push_back(configItem);
			}
			else
			{
				commands.push_back(new MixerCommand(srcChannelIndex, srcBusIdx, mx->BusIdx, m_busConfig[mx->BusIdx]->GetChannelIndex(), vOut->Gain, srcNumChannels));
				_RPT2(_CRT_WARN, "		Mixer %i connected to bus %i\r\n", mx->GetId(), mx->BusIdx);
			}
		}
	}
}

Processor::~Processor()
{
	for (size_t i = 0; i < m_commandsLength; i++)
	{
		delete m_commands[i];
	}

	for (size_t i = 0; i < m_busConfig.size(); i++)
	{
		delete m_busConfig[i];
	}

	delete[] m_buffer;
}
const uint32 Processor::CNANNEL_NOT_ASSIGNED = 255;

void Processor::Setup(double fs, uint32 inputChannelMask)
{
	delete[] m_buffer;
	m_buffer = new double[m_channelCount];
	m_sampleRate = fs;

	for (size_t i = 0; i < m_commandsLength; i++)
	{
		m_commands[i]->Setup(fs);
	}

	delete[] m_inputMap;
	
	auto inpChannels = ConfigItemBase::ChannelMaskToArray(inputChannelMask, &m_inputChannelCount);
	m_inputMap = new uint8[m_inputChannelCount];
	for (uint8 inpChIdx = 0; inpChIdx < m_inputChannelCount; inpChIdx++)
	{
		//We need to ignore input mapping, just use order in the input array. For example, mono tracks have channel 3 (mask: 4) 
		m_inputMap[inpChIdx] = CNANNEL_NOT_ASSIGNED; 
		uint8 inpCh = inpChannels[inpChIdx];
		size_t busConfigSz = m_busConfig.size();
		for (size_t busIdx = 0; busIdx < busConfigSz; busIdx++)
		{
			uint32 busMask = m_busConfig[busIdx]->GetInputChannelsMask();
			if (busMask)
			{
				
				uint8 busChCount;
				auto busChannels = ConfigItemBase::ChannelMaskToArray(busMask, &busChCount);
				for (uint8 busChIdx = 0; busChIdx < busChCount; busChIdx++)
				{
					if (busChannels[busChIdx] == inpCh)
					{
						m_inputMap[inpChIdx] = m_busConfig[busIdx]->GetChannelIndex() + busChIdx;
						break;
					}
				}
				delete[] busChannels;
				if (m_inputMap[inpChIdx] != CNANNEL_NOT_ASSIGNED) break;
			}
		}
	}

	delete[] inpChannels;
}


uint32 Processor::FindOutputChannelsConfig(uint8 * count)
{
	
	if (m_delay)
	{
		for (uint8 i = 0; i < m_outputChannelCount; i++)
		{
			delete[] m_delay[i];
		}
	}
	delete [] m_delay;
	
	
	delete [] m_gain;
	delete [] m_outputMap;
	delete [] m_delayLen;
	delete [] m_delayIdx;

	m_outputChannelCount = 0;
	uint32 outMask = 0;

	const int MAX_NUM_CH = 32;
	size_t channelToBus[MAX_NUM_CH] = { CNANNEL_NOT_ASSIGNED };

	size_t busConfigSz = m_busConfig.size();
	for (size_t busIdx = 0; busIdx < busConfigSz; busIdx++)
	{
		auto outCh = m_busConfig[busIdx]->GetOutputChannels();
		for (size_t outChIdx = 0; outChIdx < outCh.size(); outChIdx++)
		{
			auto out = outCh[outChIdx];
			outMask |= ConfigItemBase::ArrayToChannelMask(out->OutputChannels, out->OutputChannelsLenght);
			for (uint8 chIdx = 0; chIdx < out->OutputChannelsLenght; chIdx++)
			{
				uint8 chNum0b = out->OutputChannels[chIdx] - 1; //Channels are 1-based
				if (chNum0b < MAX_NUM_CH)
				{
					channelToBus[chNum0b] = busIdx;
				}
			}
		}
	}

	auto allOutpChs = ConfigItemBase::ChannelMaskToArray(outMask, &m_outputChannelCount);
	m_delay = new double*[m_outputChannelCount];
	m_gain = new double[m_outputChannelCount];
	m_delayLen = new uint32[m_outputChannelCount];
	m_delayIdx = new uint32[m_outputChannelCount];
	m_outputMap = new uint8[m_outputChannelCount];

	for (uint8 outChIdx = 0; outChIdx < m_outputChannelCount; outChIdx++)
	{
		uint8 outCh = allOutpChs[outChIdx];
		m_outputMap[outChIdx] = CNANNEL_NOT_ASSIGNED;
		size_t busIdx = channelToBus[outCh - 1]; //Channels are 1-based
		if (busIdx >= 0 && busIdx < busConfigSz)
		{
			auto bus = m_busConfig[busIdx];
			uint8 busNumCh = bus->GetNumChannels();
			auto outChConfigs = bus->GetOutputChannels();
			size_t outChConfigsSz = outChConfigs.size();
			for (size_t outChConfigIdx = 0; outChConfigIdx < outChConfigsSz; outChConfigIdx++)
			{
				auto outConfig = outChConfigs[outChConfigIdx];
				for (uint8 i = 0; i < outConfig->OutputChannelsLenght; i++)
				{
					if (outConfig->OutputChannels[i] == outCh)
					{
						m_gain[outChIdx] = outConfig->Gain;
						if (i < busNumCh)
						{
							m_outputMap[outChIdx] = bus->GetChannelIndex() + i;
						}
						else
						{
							m_outputMap[outChIdx] = bus->GetChannelIndex(); // First channel will be diplicated if there are less actial channels
						}

						size_t dl = (size_t) ((m_sampleRate * outConfig->DelaysMS[i] / 1000.0) + 0.5);
						m_delayLen[outChIdx] = dl;
						if (dl == 0)
						{
							m_delay[outChIdx] = nullptr;
						}
						else
						{
							m_delay[outChIdx] = new double[dl];
						}
						break;
					}
				}
				if (m_outputMap[outChIdx] != CNANNEL_NOT_ASSIGNED) break;
			}
		}
	}

	delete [] allOutpChs;
	*count = m_outputChannelCount;
	Reset();
	return outMask;
}

void Processor::Reset()
{
	if (m_delayIdx) memset(m_delayIdx, 0, m_outputChannelCount * sizeof(uint32));
	if (m_buffer) memset(m_buffer, 0, m_channelCount * sizeof(double));
	for (uint8 i = 0; i < m_outputChannelCount; i++)
	{
		double * d = m_delay[i];
		if (d)
		{
			memset(d, 0, m_delayLen[i] * sizeof(double));
		}
	}

	for (size_t i = 0; i < m_commandsLength; i++)
	{
		m_commands[i]->Reset();
	}
}

void Processor::SetInput(float *input)
{
	if (m_inputChannelCount == 0) throw std::logic_error("Setup was not called properly");

	for (uint8 i = 0; i < m_inputChannelCount; i++)
	{
		uint8 bufChIdx = m_inputMap[i];
		if (bufChIdx != CNANNEL_NOT_ASSIGNED)
		{
			m_buffer[bufChIdx] = input[i];
		}
	}
}

void Processor::GetOutput(float *output)
{
	if (m_outputChannelCount == 0) throw std::logic_error("FindOutputChannelsConfig was not called properly");

	Process();

	for (uint8 outChIdx = 0; outChIdx < m_outputChannelCount; outChIdx++)
	{
		uint8 bufChIdx = m_outputMap[outChIdx];
		if (bufChIdx != CNANNEL_NOT_ASSIGNED)
		{
			double sample = m_buffer[bufChIdx];
			//Delay(outChIdx, &sample);
			double * d = m_delay[outChIdx];
			if (d)
			{
				size_t delayArrayIdx = m_delayIdx[outChIdx];
				double * sd = d + delayArrayIdx;
				double stored = *sd; //Store delayed value
				*sd = sample;     //Setting current value
				sample = stored;
				delayArrayIdx++; //Advancing pointer
				if (delayArrayIdx >= m_delayLen[outChIdx]) delayArrayIdx = 0;
				m_delayIdx[outChIdx] = delayArrayIdx;
			}

			sample *= m_gain[outChIdx];
			output[outChIdx] = (float) sample;
		}
		else
		{
			assert(false);
			output[outChIdx] = 0; //This should never happen as m_outputChannelCount should correspond the mapping
		}
	} 
}


inline void Processor::Process()
{
	for (size_t i = 0; i < m_commandsLength; i++)
	{
		m_commands[i]->Apply(m_buffer);
	}
}

const int MAX_NUM_CH = 32;
//Testing 
std::vector<FilterData>* Processor::CreateFilterData(int numFrequencyResponseSamples, double sampleRate)
{
	std::vector<FilterData>* res = new std::vector<FilterData>();
	std::vector<Colour> colors;
	std::vector<uint8> colorToBuffer;
	const int NUM_STEP_SAMPLES = 1024;
	
	

	GetOutputResponsesColors(colors, colorToBuffer);

	size_t numChannels = colors.size();
	auto stepResponses = new float*[numChannels];
	auto responses = new Dsp::complex_t*[numChannels];
	auto buffer = new Dsp::complex_t[m_busConfig.size()];
	float samples[MAX_NUM_CH];

	for (size_t i = 0; i < numChannels; i++)
	{
		stepResponses[i] = new float[NUM_STEP_SAMPLES];
		responses[i] = new Dsp::complex_t[numFrequencyResponseSamples];
	}
	
	Reset();
	for (int i = 0; i < numFrequencyResponseSamples; i++)
	{
		CalculateResponse(responses, i, i / (2.0 * numFrequencyResponseSamples), colorToBuffer, buffer);
	}

	Reset();
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < MAX_NUM_CH; j++) samples[j] = 0.0f;
		SetInput(samples);
		Process();
		for (size_t c = 0; c < numChannels; c++)
		{
			uint8 bufIdx = colorToBuffer[c];
			uint8 channel = m_busConfig[bufIdx]->GetChannelIndex();
			stepResponses[c][i] = (float)m_buffer[channel];
		}
	}

	for (int i = 10; i < NUM_STEP_SAMPLES; i++)
	{
		for (int j = 0; j < MAX_NUM_CH; j++) samples[j] = 1;
		SetInput(samples);
		Process();
		for (size_t c = 0; c < numChannels; c++)
		{
			stepResponses[c][i] = (float)m_buffer[m_busConfig[colorToBuffer[c]]->GetChannelIndex()];
		}
	}

	for (size_t c = 0; c < numChannels; c++)
	{

		FilterData fd(new ChannelTransformation(NUM_STEP_SAMPLES, stepResponses[c], numFrequencyResponseSamples, responses[c], sampleRate), colors[c]);
		res->push_back(fd);
	}
	

	delete[] buffer;
	return res;
}

void Processor::GetOutputResponsesColors(std::vector<Colour> & colors, std::vector<uint8>& colorToBuffer)
{
	for (size_t bufIdx = 0; bufIdx < m_busConfig.size(); bufIdx++)
	{ 
		auto outs = m_busConfig[bufIdx]->GetOutputChannels();
		for (size_t outIdx = 0; outIdx < outs.size(); outIdx++)
		{
			//Find first color as it doesnt maker sense display output twice
			auto out = outs[outIdx];
			if (out->ShowInGraphs)
			{
				colors.push_back(out->GetOutputColor());
				colorToBuffer.push_back((const uint8)bufIdx);
				break;
			}
		}
	}
}

void Processor::CalculateResponse(Dsp::complex_t  **responses, int responsePosition, double normalizedFrequency, std::vector<uint8>& colorToBuffer, Dsp::complex_t  *buffer)
{
	for (size_t i = 0; i < m_busConfig.size(); i++)
	{
		buffer[i] = 1;
	}

	for (size_t i = 0; i < m_commandsLength; i++)
	{
		m_commands[i]->CalculateResponse(buffer, normalizedFrequency);
	}

	for (size_t i = 0; i < colorToBuffer.size(); i++)
	{
		responses[i][responsePosition] = buffer[colorToBuffer[i]];
	}
}