#pragma once
#include <Common.h>
#include "FilterConfigEx.h"
#include "BinarySerializable.h"
#include "ConfigItem.h"


uint8 ConfigItemBase::GetChannelCount(uint32 mask)
{
	uint8 cnt = 0;
	for (uint32 i = 1; i > 0; i <<= 1)
	{
		if (mask & i) cnt++;
	}
	return cnt;
}
uint8 * ConfigItemBase::ChannelMaskToArray(uint32 mask, uint8 *count)
{
	*count = GetChannelCount(mask);
	uint8 * res = new uint8[*count];
	int idx = 0;
	for (uint8 i = 0; i < MAX_NUM_CHANNELS; i++)
	{
		if (mask & (1 << i))
		{
			res[idx] = i + 1;
			idx++;
		}
	}
	return res;
}

uint32 ConfigItemBase::ArrayToChannelMask(const uint8 * arr, uint8 count)
{
	int mask = 0;
	for (uint8 i = 0; i < count; i++)
	{
		mask |= (1 << (arr[i] - 1));
	}
	return mask;
}

void ConfigItemBase::LoadConfig(uint8* data, size_t len, std::vector<ConfigItemBase*> &config)
{
	uint8 * pastLast = data + len;

	while (data < pastLast)
	{
		ConfigItemBase *item;
		BuildingElementType elT = (BuildingElementType)*data;
		data++;
		switch (elT)
		{
			case Input: item = new InputConfigItem(); break;
			case Filter: item = new FilterConfigItem(); break;
			case Block: item = new FilterBlockConfigItem(); break;

			case Output: item = new OutputConfigItem(); break;
			case Mixer: item = new MixerConfigItem(); break;
			case VirtualOutput: item = new VirtualOutputConfigItem(); break;
			default: throw std::logic_error((String("Invalid element type: ") + String(elT)).toStdString());
		}

		item->InitFromBytes(&data, pastLast);
		config.push_back(item);
	}
}


size_t ConfigItemBase::SerializeInternal(uint8** data, bool isWriting) 
{
	size_t sz = 0;
	if (isWriting)
	{
		uint8 et = (uint8)ElementType;
		sz = Serialize(&et, data, true);
	}
	sz += Serialize(&Id, data, isWriting);
	sz += Serialize(&ParentId, data, isWriting);
	return sz;
}


size_t OutputConfigItem::SerializeInternal(uint8** data, bool isWriting) 
{
	size_t sz = ConfigItemBase::SerializeInternal(data, isWriting);
	
	sz += Serialize(&Gain, data, isWriting);
	sz += Serialize(&ShowInGraphs, data, isWriting);
	uint32 color = m_color.getARGB();
	sz += Serialize(&color, data, isWriting);
	m_color = Colour(color);
	uint8 * chs = &OutputChannels[0];
	sz += Serialize<uint8>(&chs, &OutputChannelsLenght, data, isWriting);
	float * dls = &DelaysMS[0];
	sz += Serialize<float>(&dls, &OutputChannelsLenght, data, isWriting);
	if (!isWriting)
	{
		if (OutputChannelsLenght > MAX_NUM_CHANNELS) OutputChannelsLenght = MAX_NUM_CHANNELS;
		memcpy(OutputChannels, chs, OutputChannelsLenght);
	}

	return sz;
}

size_t VirtualOutputConfigItem::SerializeInternal(uint8** data, bool isWriting) 
{
	size_t sz = ConfigItemBase::SerializeInternal(data, isWriting);
	sz += Serialize(&Number, data, isWriting);
	sz += +Serialize(&Gain, data, isWriting);
	sz += +Serialize(&ShowInGraphs, data, isWriting);
	uint32 color = m_color.getARGB();
	sz += Serialize(&color, data, isWriting);
	m_color = Colour(color);
	return sz;
}


size_t FilterConfigItem::SerializeInternal(uint8** data, bool isWriting) 
{
	size_t sz = ConfigItemBase::SerializeInternal(data, isWriting);
	sz += Serialize(&Config.FilterOrder, data, isWriting);
	sz += Serialize(&Config.FilterType, data, isWriting);
	sz += Serialize(&Config.Fc, data, isWriting);
	uint8 pt = (uint8)Config.PassType;
	sz += Serialize(&pt, data, isWriting);
	Config.PassType = (FilterPassType) pt;
	sz += Serialize(&Config.BandwithOrQOrSlope, data, isWriting);
	sz += Serialize(&Config.Gain, data, isWriting);
	sz += Serialize(&Config.RipleDb, data, isWriting);
	sz += Serialize(&Config.Rolloff, data, isWriting);
	sz += Serialize(&Config.StopDb, data, isWriting);
	return sz;
}

