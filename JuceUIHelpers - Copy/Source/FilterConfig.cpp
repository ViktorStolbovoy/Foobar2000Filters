#include "Common.h"
#include "FilterConfig.h"

Dsp::Filter * FilterConfigBase::createFilter(double fs, int numChannels)
{
	Dsp::Filter *f;
	switch (FilterType)
	{
		case 1: f = new Dsp::FilterDesign <Dsp::RBJ::Design::LowPass, Dsp::TransposedDirectFormII>(numChannels); break;
		case 2: f = new Dsp::FilterDesign <Dsp::Butterworth::Design::LowPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
		case 3: f = new Dsp::FilterDesign <Dsp::ChebyshevI::Design::LowPass<50>, Dsp::TransposedDirectFormII>(numChannels); break; 
		case 4: f = new Dsp::FilterDesign <Dsp::ChebyshevII::Design::LowPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
		case 5: f = new Dsp::FilterDesign <Dsp::Elliptic::Design::LowPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
		case 6: f = new Dsp::FilterDesign <Dsp::Bessel::Design::LowPass<25>, Dsp::TransposedDirectFormII>(numChannels); break;
		case 7: f = new Dsp::FilterDesign <Dsp::Legendre::Design::LowPass<25>, Dsp::TransposedDirectFormII>(numChannels); break;
		default: f = nullptr;
	}

	if (f)
	{
		auto params = f->getParams();
		for (int i = 0; i < f->getNumParams(); i++)
		{
			switch (f->getParamInfo(i).getId())
			{
				case Dsp::ParamID::idOrder: params.value[i] = (double)FilterOrder; break;
				case Dsp::ParamID::idSampleRate: params.value[i] = fs; break;
				case Dsp::ParamID::idFrequency: params.value[i] = Fc; break;

				case Dsp::ParamID::idRippleDb: params.value[i] = (double)RipleDb; break;
				case Dsp::ParamID::idStopDb: params.value[i] = StopDb; break;
				case Dsp::ParamID::idRolloff: params.value[i] = Rolloff; break; //Elliptic only
				case Dsp::ParamID::idQ: params.value[i] = Rolloff; break; //RBJ only
			}
		}

		f->setParams(params);
	}

	return f;
};

void FilterConfigBase::safeSetParam(Dsp::Filter *f, int id, double value)
{
	int idx = f->findParamId(id);
	if (idx >= 0) f->setParam(idx, value);
}

void FilterConfigBase::setFilterParam(Dsp::Filter * filter, int index, float value)
{
	auto pinfo = filter->getParamInfo(index);
	switch (pinfo.getId())
	{
		case Dsp::ParamID::idSampleRate: break; //Do nothing 
		case Dsp::ParamID::idOrder: FilterOrder = (unsigned char)value; break;
		case Dsp::ParamID::idFrequency: Fc = value; break;
		case Dsp::ParamID::idRippleDb: RipleDb = value; break;
		case Dsp::ParamID::idStopDb: StopDb = value; break;

		case Dsp::ParamID::idRolloff: Rolloff = value; break; //Elliptic only
		case Dsp::ParamID::idQ: Rolloff = value; break; //RBJ only
		default: assert(false); break;
	}
	filter->setParam(index, value);
}