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

#include "Common.h"
#include "CustomSlider.h"
#include "FilterControls.h"



#pragma warning( push )
#pragma warning( disable : 4512 )

class FilterFrequencySliderGroupModel : public FilterParamSliderGroupModel
{
public:
	FilterFrequencySliderGroupModel(ListenerList<FilterListener>& listeners, Dsp::Filter* filter, int paramIndex, FilterConfigBase* config)
		: FilterParamSliderGroupModel(listeners, filter, paramIndex, config), m_halfSampleRate(getSampleRate(filter) / 2.0f)
	{
		m_fMin = ((m_filter->getName()).compare("Bessel Low Pass")) ? 18000.0f : 1000.0f;
		
	}

	~FilterFrequencySliderGroupModel()
	{
	}

	

	float m_fMin;
	double getControlValue() const
	{
		//Control range is 18kHz - > FS/2
		float val = (float)m_filter->getParam(m_paramIndex);
		if (val < m_fMin || val > m_halfSampleRate)
		{
			val = 20000;
			m_config->setFilterParam(m_filter, m_paramIndex, val);
		}

		//Transforming to 0..1 range
		double nativeRange = m_halfSampleRate - m_fMin;

		return (val - m_fMin) / nativeRange;
	}

	void setControlValue(double controlValue)
	{
		float nativeRange = m_halfSampleRate - m_fMin;
		float val = ((float)controlValue * nativeRange) + m_fMin;

		m_config->setFilterParam(m_filter, m_paramIndex, val);
		m_listeners.call(&FilterListener::onFilterParameters);
	}

private:
	const float m_halfSampleRate;
	static float getSampleRate(Dsp::Filter* filter)
	{
		for (int i = 0; i < filter->getNumParams(); i++)
		{
			if ((filter->getParamInfo(i)).getId() == Dsp::ParamID::idSampleRate)
			{
				return (float)filter->getParam(i);
			}

		}
		return 0;
	}
};

#pragma warning( pop )

//------------------------------------------------------------------------------

FilterControls::FilterControls(ListenerList<FilterListener>& listeners)
	: m_listeners(listeners)
{
}

FilterControls::~FilterControls()
{
	clear();
}

void FilterControls::paint(Graphics& g)
{
	const Rectangle<int> bounds = getLocalBounds();

	g.setColour(Colour::fromRGBA(0, 0, 0, 64));
	g.fillRect(bounds.reduced(1, 1));

	g.setColour(Colour::fromRGBA(0, 0, 0, 128));
	g.drawRect(bounds, 1);
}


void FilterControls::sourceFSChanged(FilterConfigBase * config)
{
	m_config = config;
}

// Use introspection to build the list of controls
void FilterControls::onFilterChanged(std::vector<FilterData> &newFilter)
{
	
	if (newFilter.size() > 0)
	{
		m_filter = dynamic_cast<Dsp::Filter*>(newFilter[0].Filter);
	}
	else
	{
		m_filter = nullptr;
	}
	
	clear();

	if (m_filter)
	{
		const Rectangle<int> bounds = getLocalBounds();

		//VS: const int w = 70;
		const int w = 310;
		const int ygap = 0;
		const int h = bounds.getHeight() - 2 * ygap;
		const int y = bounds.getY() + ygap;

		int x = bounds.getX() + 2;
	
		for (int i = 0; i < m_filter->getNumParams(); ++i)
		{
			const Dsp::ParamInfo info = m_filter->getParamInfo(i);
			if (info.getId() != Dsp::idSampleRate)
			{
				Item item;
				if (info.getId() == Dsp::ParamID::idFrequency)
				{
					item.group = new SliderGroup(new FilterFrequencySliderGroupModel(m_listeners, m_filter, i, m_config));
				}
				else
				{
					item.group = new SliderGroup(new FilterParamSliderGroupModel(m_listeners, m_filter, i, m_config));
				}

				item.group->setBounds(x, y, w, h);
				addAndMakeVisible(item.group);
				m_items.add(item);
				x += w;
			}
		}
	}
}

void FilterControls::clear()
{
	for (int i = 0; i < m_items.size(); ++i)
	{
		delete m_items[i].group;
	}

	m_items.clear();
}
