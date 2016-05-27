#pragma once
class Processor 
{
public:
	Processor(std::vector<ConfigItemBase *> &configItems);
	virtual ~Processor();
	uint32 FindOutputChannelsConfig(uint8 * count);
	void Setup(double fs, uint32 inputChannelMask);
	void SetInput(float *input);
	
	void GetOutput(float *output);
	
	void Reset(); //Reset delay buffers and filters

	//Testing 
	std::vector<FilterData> * CreateFilterData(int stepResponseGraphWidth, double sampleRate);
private:
	CommandBase ** m_commands;
	uint32 m_commandsLength;
	void AddFilterCommand(uint8 busIdx, ConfigItemBase * cfg, std::vector<FilterBlockConfig *>& filterBlockDefinitions, std::vector<CommandBase *> &commands);
	void AddToMixer(VirtualOutputConfigItem *vOut, std::vector<MixerConfig *> &mixers, uint8 srcChannelIndex, uint8 srcBusIdx, uint8 srcNumChannels, std::vector<CommandBase *> &commands);
	std::vector<BusConfigItem*> m_busConfig;
	inline void Process();

	double m_sampleRate;
	uint8 m_channelCount; //Total number of channels
	uint8 m_inputChannelCount; //Real number of input channels
	uint8 m_outputChannelCount; //Real number of input channels
	//Length of m_inputChannelCount 
	uint8 *m_inputMap;

	//Length of m_channelCount
	double *m_buffer;

	//Lenght of m_outputChannelCount
	double **m_delay;
	double *m_gain;
	uint32 *m_delayLen;
	uint32 *m_delayIdx;
	uint8 *m_outputMap;

	//Test
	void Processor::GetOutputResponsesColors(std::vector<Colour> & colors, std::vector<uint8>& m_colorToBuffer);
	void CalculateResponse(Dsp::complex_t  **responses, int responsePosition, double normalizedFrequency, std::vector<uint8>& colorToBuffer, Dsp::complex_t  *buffer);
	
	static const uint32 CNANNEL_NOT_ASSIGNED;
};

