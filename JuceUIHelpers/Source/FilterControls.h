/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
By Vincent Falco

Official project location:
http://code.google.com/p/dspfilterscpp/

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vincent Falco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*******************************************************************************/

#ifndef DSPDEMO_FILTERCONTROLS_H
#define DSPDEMO_FILTERCONTROLS_H

#include "Common.h"
#include "FilterConfig.h"
#include "FilterListener.h"
#include "ResizableLayout.h"
#include "SliderGroup.h"


/*
 * Creates filter Gui controls dynamically, using
 * the introspection features of Dsp::Filter.
 *
 */
class FilterControls
	: public Component
	, public FilterListener
{
public:
	FilterControls(ListenerList<FilterListener>& listeners);
	~FilterControls();

	void paint(Graphics& g);

	void onFilterChanged(std::vector<FilterData> &newFilter);
	void sourceFSChanged(FilterConfigBase * m_config);

protected:
	void clear();

private:
	struct Item
	{
		Item()
		{
			group = 0;
		}

		SliderGroup* group;
	};

	ListenerList<FilterListener>& m_listeners;
	Array<Item> m_items;
	Dsp::Filter* m_filter;
	FilterConfigBase* m_config = NULL;
};

#pragma warning( push )
#pragma warning( disable : 4512 )

class FilterParamSliderGroupModel : public SliderGroup::Model
{
public:
	FilterParamSliderGroupModel(ListenerList<FilterListener>& listeners,
		Dsp::Filter* filter,
		int paramIndex,
		FilterConfigBase* config)
		: m_listeners(listeners)
		, m_filter(filter)
		, m_paramIndex(paramIndex)
		, m_paramInfo(filter->getParamInfo(paramIndex))
		, m_config(config)
	{
	}

	~FilterParamSliderGroupModel()
	{
	}

	const String getName() const
	{
		return m_paramInfo.getLabel();
	}

	double getControlValue() const
	{
		return m_paramInfo.toControlValue(m_filter->getParam(m_paramIndex));
	}

	void setControlValue(double controlValue)
	{
		m_config->setFilterParam(m_filter, m_paramIndex, (float)m_paramInfo.toNativeValue(controlValue));
		m_listeners.call(&FilterListener::onFilterParameters);
	}

	const String getNativeValueAsText() const
	{
		const Dsp::ParamInfo paramInfo = m_filter->getParamInfo(m_paramIndex);
		return String(paramInfo.toString(m_filter->getParam(m_paramIndex)).c_str());
	}

	void setNativeValue(double nativeValue)
	{
		m_config->setFilterParam(m_filter, m_paramIndex, (float)nativeValue);
		m_listeners.call(&FilterListener::onFilterParameters);
	}

	String getUnits() override
	{
		switch (m_paramInfo.getId())
		{
			case Dsp::ParamID::idFrequency:
			case Dsp::ParamID::idSampleRate:
			case Dsp::ParamID::idBandwidthHz: return "Hz";

			case Dsp::ParamID::idGain:
			case Dsp::ParamID::idRippleDb:
			case Dsp::ParamID::idStopDb: return "dB";

			default: return "";
		}
	}

protected:
	int m_paramIndex;
	const Dsp::ParamInfo m_paramInfo;

	ListenerList<FilterListener>& m_listeners;
	Dsp::Filter* m_filter;
	FilterConfigBase* m_config;
};
#pragma warning( pop )
#endif
