#include "Common.h"
#include "FilterConfig.h"
#include "Config.h"


Dsp::Filter * FilterConfig::createFilter(double fs, int numChannels) const
{
	if (FMultiplier <= 1) return nullptr;
	return FilterConfigBase::createFilter(fs, numChannels);
}

FilterConfig * Config::getConfigForSourceSampleRate(int sourceSampleRate)
{
	if (sourceSampleRate < 40000) return nullptr;
	if (sourceSampleRate < 50000) return &FS44;
	if (sourceSampleRate > 170000) return nullptr;
	return &FS96;
}

Dsp::Filter * Config::createFilter(int sourceSampleRate, int numChannels)
{
	if (sourceSampleRate < 40000) return nullptr;
	if (sourceSampleRate < 50000) return FS44.createFilter(sourceSampleRate * FS44.FMultiplier, numChannels);
	return FS96.createFilter(sourceSampleRate * FS96.FMultiplier, numChannels);
};

void Config::setFilterType(int sourceSampleRate, int type, unsigned char fMultiplier)
{
	if (sourceSampleRate < 40000) return;
	if (sourceSampleRate < 50000)
	{
		FS44.FilterType = (uint8) type;
		FS44.FMultiplier = fMultiplier;
	}
	else 
	{
		FS96.FilterType = (uint8) type;
		FS96.FMultiplier = fMultiplier;
	}
}


void Config::cloneConfig(int sourceSampleRate, Config * newConfig)
{
	if (sourceSampleRate < 40000) return;
	if (sourceSampleRate < 50000)
	{
		FS44 = newConfig->FS44;
	}
	else
	{
		FS96 = newConfig->FS96;
	}
}



Config::Config()
{
	//Set defaults
	FS44.FMultiplier = 4;
	FS44.FilterType = 4; // ChebyshevII
	FS44.FilterOrder = 8;
	FS44.Fc = 21000;
	FS44.RipleDb = 0.001f;
	FS44.StopDb = 60;
	FS44.Rolloff = 0.1f;
	
	FS96.FMultiplier = 2;
	FS96.FilterType = 4; // ChebyshevII
	FS96.FilterOrder = 8;
	FS96.Fc = 48000;
	FS96.RipleDb = 0.001f;
	FS96.StopDb = 60;
	FS96.Rolloff = 0.1f;
};