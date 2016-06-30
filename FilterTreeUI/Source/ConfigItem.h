#pragma once
#include <Common.h>
#include "FilterConfigEx.h"
#include "BinarySerializable.h"

const int MAX_NUM_CHANNELS = 32;

enum BuildingElementType
{
	None = 0,
	Input = 1,
	Filter = 2,
	Block = 3,
	Output = 4,
	Mixer = 5,
	VirtualOutput = 6
};


class ConfigItemBase : public BinarySerializable
{
public:
	ConfigItemBase(BuildingElementType elementType) : ElementType(elementType)
	{
	}

	ConfigItemBase(ConfigItemBase *src) : ElementType(src->ElementType), Id(src->Id), ParentId(src->ParentId)
	{

	}

	virtual ~ConfigItemBase()
	{
	}
	
	const BuildingElementType ElementType;


	uint32 Id = 0;
	uint32 ParentId = 0;

	static uint8 GetChannelCount(uint32 mask);
	static uint8 * ChannelMaskToArray(uint32 mask, uint8 *count);
	static uint32 ArrayToChannelMask(const uint8 * arr, uint8 count);
	static void LoadConfig(uint8* data, size_t len, std::vector<ConfigItemBase*> &config);

protected:
	size_t SerializeInternal(uint8** data, bool isWriting) override;
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConfigItemBase)
};


class RootConfigItem : public ConfigItemBase
{
public:
	RootConfigItem() : ConfigItemBase(None)
	{
		
	}

	size_t SerializeInternal(uint8** data, bool isWriting) override
	{
		return 0;
	}
	
};

class InputConfigItem : public ConfigItemBase
{
public:
	InputConfigItem() : ConfigItemBase(Input)
	{
	}

	InputConfigItem(InputConfigItem *src) : ConfigItemBase(src), IsVirtual(src->IsVirtual), InputMask(src->InputMask)
	{
	}


	bool IsVirtual;
	uint32 InputMask;
	
	size_t SerializeInternal(uint8** data, bool isWriting) override
	{
		return	ConfigItemBase::SerializeInternal(data, isWriting)
			+ Serialize(&IsVirtual, data, isWriting) 
			+ Serialize(&InputMask, data, isWriting);
	}

	
private:
	static const size_t CONFIG_SIZE = 1 + sizeof(uint32);
};


class OutputConfigItem : public ConfigItemBase
{
public:
	OutputConfigItem() : ConfigItemBase(Output)
	{
		//DelayMS = 0;
		Gain = 1;
		OutputChannelsLenght = 0;
		ShowInGraphs = true;
		memset(DelaysMS, 0, MAX_NUM_CHANNELS * sizeof(float));
	}

	OutputConfigItem(OutputConfigItem *src) : ConfigItemBase(src), Gain(src->Gain), OutputChannelsLenght(src->OutputChannelsLenght), ShowInGraphs(src->ShowInGraphs), m_color(src->GetOutputColor())
	{
		memcpy(OutputChannels, src->OutputChannels, MAX_NUM_CHANNELS * sizeof(uint8));
		memcpy(DelaysMS, src->DelaysMS, MAX_NUM_CHANNELS * sizeof(float));
	}
	

	uint8 OutputChannelsLenght = 0;
	uint8 OutputChannels[MAX_NUM_CHANNELS];
	float DelaysMS[MAX_NUM_CHANNELS];
	float Gain;
	bool ShowInGraphs;	 //It is just for UI, but I want to save it anyway
	Colour GetOutputColor() const
	{
		return m_color;
	}

protected:
	Colour m_color;

	size_t SerializeInternal(uint8** data, bool isWriting) override;
};


class VirtualOutputConfigItem : public ConfigItemBase
{
public:
	VirtualOutputConfigItem() : ConfigItemBase(VirtualOutput)
	{
		Gain = 1;
		ShowInGraphs = false;
		Number = 0;
	}

	VirtualOutputConfigItem(VirtualOutputConfigItem *src) : ConfigItemBase(src), Gain(src->Gain), ShowInGraphs(src->ShowInGraphs), Number(src->Number), m_color(src->GetOutputColor()) {}

	uint32 Number;
	float Gain;
	bool ShowInGraphs;

	Colour GetOutputColor() const
	{
		return m_color;
	}

protected:
	size_t SerializeInternal(uint8** data, bool isWriting) override;
	Colour m_color;
};

class FilterConfigItem : public ConfigItemBase
{
public:
	FilterConfigItem() : ConfigItemBase(Filter)
	{
	}

	FilterConfigItem(FilterConfigItem *src) : ConfigItemBase(src), Config(src->Config)
	{
	}

	FilterConfigEx Config;
protected:
	size_t SerializeInternal(uint8** data, bool isWriting) override;
};


class FilterBlockConfigItem : public ConfigItemBase
{
public:
	FilterBlockConfigItem() : ConfigItemBase(Block)
	{
	}

	FilterBlockConfigItem(FilterBlockConfigItem *src) : ConfigItemBase(src), VirtualInput(src->VirtualInput)
	{
	}

	uint32 VirtualInput = 0;
protected:
	size_t SerializeInternal(uint8** data, bool isWriting) override
	{
		return	ConfigItemBase::SerializeInternal(data, isWriting)
			+ Serialize(&VirtualInput, data, isWriting);
	}
};


class MixerConfigItem : public ConfigItemBase
{
public:
	MixerConfigItem() : ConfigItemBase(Mixer)
	{
	}

	MixerConfigItem(MixerConfigItem *src) : ConfigItemBase(src), VirtualOutputs(src->VirtualOutputs)
	{
	}

	std::vector<uint32> VirtualOutputs;

	size_t SerializeInternal(uint8** data, bool isWriting) override
	{
		return	ConfigItemBase::SerializeInternal(data, isWriting)
			+ Serialize<uint32>(&VirtualOutputs, data, isWriting);
	}
};