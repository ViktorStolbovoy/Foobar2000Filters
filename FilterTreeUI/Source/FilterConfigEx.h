#pragma once
#include <FilterConfig.h>
enum FilterPassType : uint8 { LowPass = 1, HighPass = 2, BandPass = 3, BandStop = 4, LowShelf = 5, HighShelf = 6, BandShelf = 7 };
class FilterConfigEx : public FilterConfigBase
{
public:
	
	FilterConfigEx();
	FilterPassType PassType;	 
	float BandwithOrQOrSlope;
	float Gain;
	Dsp::Filter * createFilter(double fs, int numChannels) const override;

	void setFilterParam(Dsp::Filter * filter, int index, float value) override;
	static bool isFilterImplemented(uint8 filterType, FilterPassType passType);

	String getDescription();
};