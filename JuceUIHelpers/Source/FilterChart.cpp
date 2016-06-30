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
#include "FilterConfig.h"
#include "FilterChart.h"



FilterChart::FilterChart(FilterListeners& listeners)
	: m_listeners(listeners)
{
	setOpaque(true);

	m_cBack = Colour(0xfff8f8f8);
	m_cFrame = Colour(0xff808080);
	m_cAxis = Colour(0x80000000);
	m_cAxisMinor = Colour(0x20000000);
	m_cText = Colour(0xd0000000);

	m_listeners.add(this);
	m_filterData.clear();
}

FilterChart::~FilterChart()
{
	m_listeners.remove(this);
	if (m_path) delete[] m_path;
}

void FilterChart::paint(Graphics& g)
{
	const Rectangle<int> bounds = getLocalBounds();
	const Rectangle<int> r = bounds;

	g.setColour(m_cBack);
	g.fillRect(r.reduced(1, 1));

	paintContents(g);
	paintName(g);

	g.setColour(m_cFrame);
	g.drawRect(r, 1);
}

void FilterChart::resized()
{
	update();
}

void FilterChart::onFilterChanged(std::vector<FilterData> &newFilter)
{
	//if (m_isDefined) delete[] m_isDefined;
	//m_isDefined = new bool[newFilter.size()];
	
	if (m_path) delete[] m_path;
	m_path = new Path[newFilter.size()];

	m_filterData.clear();
	m_filterData.assign(newFilter.begin(), newFilter.end());
		
	update();
}

void FilterChart::onFilterParameters()
{
	update();
}

const String FilterChart::getName() const
{
	return String::empty;
}

void FilterChart::drawText(Graphics &g,
	const Point<int> ptOrigin,
	const String text,
	Justification just)
{
	const Font& font = g.getCurrentFont();
	const int w = font.getStringWidth(text);

	int x, y;

	if (just.getOnlyHorizontalFlags() & Justification::right)
		x = ptOrigin.getX() - w;
	else
		x = ptOrigin.getX();

	if (just.getOnlyVerticalFlags() & Justification::top)
		y = int(ptOrigin.getY() + font.getAscent() + 0.5);
	else
		y = ptOrigin.getY();

	g.drawSingleLineText(text, x, y);
}

void FilterChart::paintName(Graphics& g)
{
	const Rectangle<int> bounds = getLocalBounds();
	const String name = getName();

	g.setColour(m_cText);
	drawText(g, Point<int>(bounds.getRight() - 4, bounds.getY() + 4),
		name, Justification::topRight);
}

//------------------------------------------------------------------------------

FrequencyChart::FrequencyChart(FilterListeners& listeners, bool useLogScale)
	: FilterChart(listeners), m_useLogScale(useLogScale)
{
}

FrequencyChart::~FrequencyChart()
{
}

void FrequencyChart::paintOverChildren(Graphics& g)
{
	const Rectangle<int>& bounds = getLocalBounds();
	Rectangle <int> r = bounds.reduced(1, 1);

	int h = r.getHeight();
	float fMax = 22050;
	if (m_filterData.size() > 0)
	{
		fMax = (float)m_filterData[0].Filter->getSampleRate() / 2; 
	}

	if (m_useLogScale)
	{
		double logFMaxToFMin = log(fMax / fMin);
		g.setColour(Colour::fromRGBA(0, 0, 0, 32));
		for (int f = 10; f < 100; f += 10)
		{
			float x = (float)fRealToX((float)f, logFMaxToFMin);
			int xi = r.getX() + int(x * r.getWidth() + 0.5);
			g.fillRect(xi, r.getY(), 1, h);
			drawText(g, Point<int>(xi + 6, h - 20), String(f), Justification::topLeft);
		}

		for (int f = 100; f < 1000; f += 100)
		{
			float x = (float)fRealToX((float)f, logFMaxToFMin);
			int xi = r.getX() + int(x * r.getWidth() + 0.5);
			g.fillRect(xi, r.getY(), 1, h);
			drawText(g, Point<int>(xi + 6, h - 20), String(f), Justification::topLeft);
		}

		for (int f = 1000; f < fMax; f += 1000)
		{
			float x = (float)fRealToX((float)f, logFMaxToFMin);
			int xi = r.getX() + int(x * r.getWidth() + 0.5);
			g.setColour(Colour::fromRGBA((f == 20000) ? 255 : 0, 0, 0, 32));
			g.fillRect(xi, r.getY(), 1, h);
			drawText(g, Point<int>(xi + 6, h - 20), String(f / 1000) + String("k"), Justification::topLeft);
		}
	}
	else
	{
		int dI = (int)(r.getWidth() > 500 ? fMax / 22 : fMax / 11);
		dI = (dI / 1000) * 1000; //To get round thousands

		for (int i = 000; i <= fMax; i += dI)
		{
			g.setColour(Colour::fromRGBA((i == 20000) ? 255 : 0, 0, 0, 32));

			float x = i / fMax;
			float f = xToF(x, fMax);
			int xi = r.getX() + int(f * r.getWidth() + 0.5);
			g.fillRect(xi, r.getY(), 1, h);
			if (i > 0) drawText(g, Point<int>(xi + 6, h - 20), String(i / 1000) + "k", Justification::topLeft);
		}
	}
}

const double FrequencyChart::fMin = 10;

float FrequencyChart::xToF(float x, float fMax)
{
	if (m_useLogScale)
	{
		double f = fMin * pow(fMax / fMin, x);
		return (float) f / fMax;
	}
	else
	{
		return x;
	}
}

float FrequencyChart::fRealToX(float f, double logFMaxToFMin )
{
	if (m_useLogScale)
	{
		double k = f / fMin;
		return (float) (log(k) / logFMaxToFMin);
	}
	else
	{
		return f;
	}
}
