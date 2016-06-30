#pragma once
class FilterConfigBase
{
public:
	unsigned char FilterType;
	unsigned char FilterOrder;
	float Fc;
	float RipleDb;
	float StopDb;
	float Rolloff;
	virtual void setFilterParam(Dsp::Filter * filter, int index, float value);
	virtual Dsp::Filter * createFilter(double fs, int numChannels);
private:
	static void safeSetParam(Dsp::Filter *f, int id, double value);
};

