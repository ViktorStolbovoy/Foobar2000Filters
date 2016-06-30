#pragma once
class CommandBase
{
public:
	CommandBase(uint8 channelIdx, uint8 busIdx, uint8 numChannels) : m_channelIdx(channelIdx), m_busIdx(busIdx), m_numChannels(numChannels) {}
	virtual ~CommandBase() {}
	virtual void Apply(double *buses) = 0;
	virtual void CalculateResponse(Dsp::complex_t  *responses, double normalizedFrequency) = 0;
	virtual void Setup(double Fs) = 0;
	virtual void Reset() {}
protected:
	uint8 m_channelIdx;   //Offset in the double array
	uint8 m_busIdx;	   //Bus config index 
	uint8 m_numChannels;
};

class FilterCommand : public CommandBase
{
public:
	FilterCommand(uint8 channelIdx, uint8 busIdx, uint8 numChannels, FilterConfigEx fc);
	~FilterCommand();
	void Setup(double fs) override;
	void Apply(double *data) override;
	void CalculateResponse(Dsp::complex_t  *responses, double normalizedFrequency) override;
	void Reset() override;
	bool IsValid() { return m_filter != nullptr; }
private:
	FilterConfigEx m_filterConfig;
	Dsp::Filter * m_filter;
};

class MixerCommand : public CommandBase
{
public:
	MixerCommand(uint8 channelIdx, uint8 busIdx, uint8 outputBus, uint8 outputChannel, double gain, uint8 numChannels);
	~MixerCommand() override;

	void Apply(double *data) override;
	void CalculateResponse(Dsp::complex_t  *responses, double normalizedFrequency) override;

	void Setup(double fs) override;

protected:
	double m_channelGain;
	uint8 m_outputBus;
	uint8 m_outputChannel;
};
class MixerNewCommand : public MixerCommand
{
public:
	MixerNewCommand(uint8 channelIdx, uint8 busIdx, uint8 outputBus, uint8 outputChannel, double gain, uint8 numChannels);
	~MixerNewCommand() override;

	void Apply(double *data) override;
	
	void CalculateResponse(Dsp::complex_t  *responses, double normalizedFrequency) override;
};

class CopyCommand : public CommandBase
{
public:
	CopyCommand(uint8 channelIdx, uint8 busIdx, uint8 firstOutputChannel, uint8 firstOutputBus, uint8 numChannels, uint8 numOutputBusses);
	void Apply(double *data) override;
	void CalculateResponse(Dsp::complex_t  *responses, double normalizedFrequency) override;
	void Setup(double fs) override;

private:
	const uint8 m_firstOutputChannel;
	const uint8 m_firstOutputBus;
	const uint8 m_numOutputBusses;

private:
	CopyCommand& operator=(const CopyCommand&);
};

class MixerConfig final
{
public:
	MixerConfig(MixerConfigItem* config);
	uint8 BusIdx;
	bool IsConnectedToVOut(uint32 vOutId);
	uint32 GetId() { return m_config->Id; };

private:
	MixerConfigItem* m_config;
};



class BusConfigItem final
{
public:
	BusConfigItem(const InputConfigItem* input, uint8 channleIndex);
	BusConfigItem(uint8 numChannels, uint8 channleIndex);
	BusConfigItem(const BusConfigItem & src);
	BusConfigItem & operator =(const BusConfigItem & src);
	~BusConfigItem() { }
	void AddOutputInfo(const OutputConfigItem* out) { m_outputChannels.push_back(out); CurrentConfigItemId = out->Id; }
	uint32 CurrentConfigItemId;
	uint8 GetNumChannels() { return m_numChannels; }
	uint32 GetInputChannelsMask() { return m_inputMask; }
	const std::vector<const OutputConfigItem*>& GetOutputChannels() { return m_outputChannels; }
	uint8 GetChannelIndex() { return m_channelIndex; }
private:
	uint8 m_channelIndex;
	uint8 m_numChannels;
	uint32 m_inputMask;
	std::vector<const OutputConfigItem*> m_outputChannels; //Multiple outputs can be combined, each #channels
};

class FilterBlockConfig final
{
public:
	FilterBlockConfig(uint32 elementId, uint32 vInputId, const std::vector<ConfigItemBase *>& cfgs);
	uint32 GetVInputId() { return m_vInputId; }
	void GenCommands(std::vector<CommandBase *> & commands, uint8 channelIndex, uint8 busIndex, uint8 numChannels);
private: 
	uint32 m_vInputId;
	std::vector<FilterConfigItem*> m_filters; 
};


