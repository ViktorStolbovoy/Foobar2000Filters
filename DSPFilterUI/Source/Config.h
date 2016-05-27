#ifndef DSP_CONFIG_H
#define DSP_CONFIG_H

class FilterConfig : public FilterConfigBase
{
	public: 
		unsigned char FMultiplier;
		Dsp::Filter * createFilter(double fs, int numChannels) const override;
};

  
class Config
{
public:
	Config();
	Dsp::Filter * createFilter(int sourceSampleRate, int numChannels);
	void setFilterType(int sourceSampleRate, int type, unsigned char fMultiplier);
	FilterConfig * getConfigForSourceSampleRate(int sourceSampleRate);
	void cloneConfig(int sourceSampleRate, Config * newConfig);
	FilterConfig FS44;
	FilterConfig FS96;
};



#endif