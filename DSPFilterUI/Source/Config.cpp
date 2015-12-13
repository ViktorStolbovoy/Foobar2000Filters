#include "Common.h"
#include "Config.h"

Dsp::Filter * Config::createFilter(int sourceSampleRate, int numChannels)
{
	if (sourceSampleRate < 40000) return NULL;
	if (sourceSampleRate < 50000) return FS44.createFilter(sourceSampleRate * FS44.FMultiplier, numChannels);
	return FS96.createFilter(sourceSampleRate * FS96.FMultiplier, numChannels);
};

void Config::setFilterType(int sourceSampleRate, int type, unsigned char fMultiplier)
{
	if (sourceSampleRate < 40000) return;
	if (sourceSampleRate < 50000)
	{
		FS44.FilterType = type;
		FS44.FMultiplier = fMultiplier;
	}
	else 
	{
		FS96.FilterType = type;
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

FSConfig * Config::getConfigForSourceSampleRate(int sourceSampleRate)
{
	if (sourceSampleRate < 40000) return NULL;
	if (sourceSampleRate < 50000) return &FS44;
	return &FS96;
}

Dsp::Filter * FSConfig::createFilter(double fs, int numChannels)
{
	if (FMultiplier <= 1) return NULL;

	Dsp::Filter *f;
	switch (FilterType)
	{
		case 1: f = NULL; break;
		//case 3: f = new Dsp::FilterDesign <Dsp::ChebyshevI::Design::LowPass<50>, 1, Dsp::TransposedDirectFormII>(); break; //No ChebyshevI !!!
		case 4: f = new Dsp::FilterDesign <Dsp::ChebyshevII::Design::LowPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
		case 5: f = new Dsp::FilterDesign <Dsp::Elliptic::Design::LowPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
		case 6: f = new Dsp::FilterDesign <Dsp::Bessel::Design::LowPass<25>, Dsp::TransposedDirectFormII>(numChannels); break;
		case 7: f = new Dsp::FilterDesign <Dsp::Legendre::Design::LowPass<25>, Dsp::TransposedDirectFormII>(numChannels); break;
		default: f = new Dsp::FilterDesign <Dsp::Butterworth::Design::LowPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
	}

	if (f)
	{
		auto params = f->getParams();
		for(int i = 0; i < f->getNumParams(); i ++)
		{
			switch(f->getParamInfo(i).getId())
			{
				case Dsp::ParamID::idOrder: params.value[i] = (double)FilterOrder; break;
				case Dsp::ParamID::idSampleRate: params.value[i] = fs; break;
				case Dsp::ParamID::idFrequency: params.value[i] = Fc; break;

				case Dsp::ParamID::idRippleDb: params.value[i] = (double)RipleDb; break;
				case Dsp::ParamID::idStopDb: params.value[i] = StopDb; break;
				case Dsp::ParamID::idRolloff: params.value[i] = Rolloff; break;
			}
		}

		f->setParams(params);
	}

	return f;
};

void FSConfig::safeSetParam(Dsp::Filter *f, int id, double value)
{
	int idx = f->findParamId(id);
	if (idx >= 0) f->setParam(idx, value);
}

void FSConfig::setFilterParam(Dsp::Filter * filter, int index, double value)
{
	auto pinfo = filter->getParamInfo(index);
	switch(pinfo.getId())
	{
		case Dsp::ParamID::idSampleRate: break; //Do nothing 
		case Dsp::ParamID::idOrder: FilterOrder = (int) value; break;
		case Dsp::ParamID::idFrequency: Fc = value; break;
		case Dsp::ParamID::idRippleDb: RipleDb = value; break;
		case Dsp::ParamID::idStopDb: StopDb = value; break;

		case Dsp::ParamID::idRolloff: Rolloff = value; break;
		default: assert(false); break;
	}
	filter->setParam(index, value);
}

Config::Config()
{
	//Set defaults
	FS44.FMultiplier = 4;
	FS44.FilterType = 4; // ChebyshevII
	FS44.FilterOrder = 8;
	FS44.Fc = 21000;
	FS44.RipleDb = 0.001;
	FS44.StopDb = 60;
	FS44.Rolloff = 0.1;
	
	FS96.FMultiplier = 2;
	FS96.FilterType = 4; // ChebyshevII
	FS96.FilterOrder = 8;
	FS96.Fc = 48000;
	FS96.RipleDb = 0.001;
	FS96.StopDb = 60;
	FS96.Rolloff = 0.1;
};