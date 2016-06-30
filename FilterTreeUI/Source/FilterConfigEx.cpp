#include <Common.h>
#include "FilterConfigEx.h"


FilterConfigEx::FilterConfigEx()
{
	FilterType = 2; // Butterworth
	FilterOrder = 4;
	Fc = 1200;
	RipleDb = 0.001f;
	StopDb = 60;
	Rolloff = 0.1f;
	BandwithOrQOrSlope = 100;
	Gain = 0;
	PassType = HighPass;
}

Dsp::Filter * FilterConfigEx::createFilter(double fs, int numChannels) const
{
	Dsp::Filter *f;

	switch (PassType)
	{
		case LowPass: return FilterConfigBase :: createFilter(fs, numChannels);
		case HighPass:
			switch (FilterType)
			{
				case 1: f = new Dsp::FilterDesign <Dsp::RBJ::Design::HighPass, Dsp::TransposedDirectFormII>(numChannels); break;
				case 2: f = new Dsp::FilterDesign <Dsp::Butterworth::Design::HighPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 3: f = new Dsp::FilterDesign <Dsp::ChebyshevI::Design::HighPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 4: f = new Dsp::FilterDesign <Dsp::ChebyshevII::Design::HighPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 5: f = new Dsp::FilterDesign <Dsp::Elliptic::Design::HighPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 6: f = new Dsp::FilterDesign <Dsp::Bessel::Design::HighPass<25>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 7: f = new Dsp::FilterDesign <Dsp::Legendre::Design::HighPass<25>, Dsp::TransposedDirectFormII>(numChannels); break;
				default: f = nullptr; break;
			}
			break;
		case BandPass:
			switch (FilterType)
			{
				case 1: f = new Dsp::FilterDesign <Dsp::RBJ::Design::BandPass1, Dsp::TransposedDirectFormII>(numChannels); break;
				case 2: f = new Dsp::FilterDesign <Dsp::Butterworth::Design::BandPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 3: f = new Dsp::FilterDesign <Dsp::ChebyshevI::Design::BandPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 4: f = new Dsp::FilterDesign <Dsp::ChebyshevII::Design::BandPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 5: f = new Dsp::FilterDesign <Dsp::Elliptic::Design::BandPass<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 6: f = new Dsp::FilterDesign <Dsp::Bessel::Design::BandPass<25>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 7: f = new Dsp::FilterDesign <Dsp::Legendre::Design::BandPass<25>, Dsp::TransposedDirectFormII>(numChannels); break;
				default: f = nullptr; break;
			}
			break;
		case BandStop:
			switch (FilterType)
			{
				case 1: f = new Dsp::FilterDesign <Dsp::RBJ::Design::BandStop, Dsp::TransposedDirectFormII>(numChannels); break;
				case 2: f = new Dsp::FilterDesign <Dsp::Butterworth::Design::BandStop<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 3: f = new Dsp::FilterDesign <Dsp::ChebyshevI::Design::BandStop<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 4: f = new Dsp::FilterDesign <Dsp::ChebyshevII::Design::BandStop<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 5: f = new Dsp::FilterDesign <Dsp::Elliptic::Design::BandStop<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 6: f = new Dsp::FilterDesign <Dsp::Bessel::Design::BandStop<25>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 7: f = new Dsp::FilterDesign <Dsp::Legendre::Design::BandStop<25>, Dsp::TransposedDirectFormII>(numChannels); break;
				default: f = nullptr; break;
			}
			break;
		case BandShelf:
			switch (FilterType)
			{
				case 1: f = new Dsp::FilterDesign <Dsp::RBJ::Design::BandShelf, Dsp::TransposedDirectFormII>(numChannels); break;
				case 2: f = new Dsp::FilterDesign <Dsp::Butterworth::Design::BandShelf<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 3: f = new Dsp::FilterDesign <Dsp::ChebyshevI::Design::BandShelf<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 4: f = new Dsp::FilterDesign <Dsp::ChebyshevII::Design::BandShelf<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 5: f = nullptr; break;
				case 6: f = nullptr; break;
				case 7: f = nullptr; break;
				default: f = nullptr; break;
			}
			break;
		case LowShelf:
			switch (FilterType)
			{
				case 1: f = new Dsp::FilterDesign <Dsp::RBJ::Design::LowShelf, Dsp::TransposedDirectFormII>(numChannels); break;
				case 2: f = new Dsp::FilterDesign <Dsp::Butterworth::Design::LowShelf<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 3: f = new Dsp::FilterDesign <Dsp::ChebyshevI::Design::LowShelf<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 4: f = new Dsp::FilterDesign <Dsp::ChebyshevII::Design::LowShelf<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 5: f = nullptr; break;
				case 6: f = new Dsp::FilterDesign <Dsp::Bessel::Design::LowShelf<25>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 7: f = nullptr; break;
				default: f = nullptr; break;
			}
			break;
		case HighShelf:
			switch (FilterType)
			{
				case 1: f = new Dsp::FilterDesign <Dsp::RBJ::Design::HighShelf, Dsp::TransposedDirectFormII>(numChannels); break;
				case 2: f = new Dsp::FilterDesign <Dsp::Butterworth::Design::HighShelf<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 3: f = new Dsp::FilterDesign <Dsp::ChebyshevI::Design::HighShelf<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 4: f = new Dsp::FilterDesign <Dsp::ChebyshevII::Design::HighShelf<50>, Dsp::TransposedDirectFormII>(numChannels); break;
				case 5: f = nullptr; break; //Elliptic
				case 6: f = nullptr; break; //Bessel
				case 7: f = nullptr; break; //Legendre
				default: f = nullptr; break;
			}
			break;
		default: f = nullptr; break; // throw std::logic_error("Attempt to ctreate unrecognozed filter pass type");
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
				case Dsp::ParamID::idBandwidth: params.value[i] = BandwithOrQOrSlope; break;

				case Dsp::ParamID::idBandwidthHz: params.value[i] = BandwithOrQOrSlope; break;
				case Dsp::ParamID::idSlope: params.value[i] = BandwithOrQOrSlope; break;

				case Dsp::ParamID::idGain: params.value[i] = Gain; break;
			}
		}

		f->setParams(params);
	}
	return f;
}

bool FilterConfigEx::isFilterImplemented(uint8 filterType, FilterPassType passType)
{
	//As much as I hate to duplicate this...
	if (!filterType || filterType > 7) return false;
	switch (passType)
	{
		case HighShelf:
		case BandShelf: return filterType <= 4; //Not implemented for Elliptic, Bessel and Legendre
		case LowShelf: return filterType != 5 && filterType != 7; //Not implemented for Elliptic and Legendre
		default: return true;
	}
}

void FilterConfigEx::setFilterParam(Dsp::Filter * filter, int index, float value)
{
	auto pinfo = filter->getParamInfo(index);
	switch (pinfo.getId())
	{
		case Dsp::ParamID::idQ: Rolloff = value; break; //RBJ only
		case Dsp::ParamID::idBandwidth: 
		case Dsp::ParamID::idBandwidthHz: 
		case Dsp::ParamID::idSlope: BandwithOrQOrSlope = value; break;
		case Dsp::ParamID::idGain: Gain = value; break;
		default: FilterConfigBase::setFilterParam(filter, index, value); return;
	}
	filter->setParam(index, value);
}

String FilterConfigEx::getDescription()
{
	StringArray description;
	switch (PassType)
	{
		case LowPass: description.add("LP"); break;
		case HighPass: description.add("HP"); break;
		case BandPass: description.add("BP"); break;
		case BandStop: description.add("BSt"); break;
		case BandShelf:description.add("BSh"); break;
		case LowShelf: description.add("LSh"); break;
		case HighShelf: description.add("HSh"); break;
		default: description.add("?"); break;
	}

	description.add(" ");
	switch (FilterType)
	{
		case 1: description.add("RBJ"); break;
		case 2: description.add("Butterworth"); break;
		case 3: description.add("ChebyshevI"); break;
		case 4: description.add("ChebyshevII"); break;
		case 5: description.add("Elliptic"); break;
		case 6: description.add("Bessel"); break;
		case 7: description.add("Legendre"); break;
		default: description.add("?"); break;
	}

	if (FilterType != 1)
	{
		description.add("(");
		description.add(String(FilterOrder));
		description.add(")");
	}

	description.add(": Fc: ");
	description.add(String(Fc, 0));

	if (PassType == BandPass || PassType == BandShelf || PassType == BandStop)
	{
		description.add(", BW: ");
		description.add(String(BandwithOrQOrSlope, 0));
	}

	if (PassType == BandShelf || PassType == HighShelf || PassType == LowShelf)
	{
		description.add(", G: ");
		description.add(String(Gain, 1));
	}

	return description.joinIntoString("");
}