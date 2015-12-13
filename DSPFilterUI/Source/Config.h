#ifndef DSP_CONFIG_H
#define DSP_CONFIG_H
class FSConfig
{
public:
	unsigned char FMultiplier;
	unsigned char FilterType;
	unsigned char FilterOrder;
	float Fc;
	float RipleDb;
	float StopDb;
	float Rolloff;
	void setFilterParam(Dsp::Filter * filter, int index, double value);
	Dsp::Filter * createFilter(double fs, int numChannels);
private:
	static void safeSetParam(Dsp::Filter *f, int id, double value);
};

class Config
{
public:
	Config();
	Dsp::Filter * createFilter(int sourceSampleRate, int numChannels);
	void setFilterType(int sourceSampleRate, int type, unsigned char fMultiplier);
	FSConfig * getConfigForSourceSampleRate(int sourceSampleRate);
	void cloneConfig(int sourceSampleRate, Config * newConfig);
	FSConfig FS44;
	FSConfig FS96;
};



#endif