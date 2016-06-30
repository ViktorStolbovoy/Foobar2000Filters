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
#include "BrickWallChart.h"

#define MARGIN 1.1f

//------------------------------------------------------------------------------

BrickWallChart::BrickWallChart(FilterListeners& listeners, bool useLogScale)
	: FrequencyChart(listeners, useLogScale)
	, m_scale_y(1)
{
}

const String BrickWallChart::getName() const
{
	return "Magnitude";
}

int BrickWallChart::yToScreen(float y)
{
	AffineTransform t = calcTransform();
	Point<float> p(0, y);
	return int(p.transformedBy(t).getY());
}

void BrickWallChart::paintContents(Graphics& g)
{
	AffineTransform t = calcTransform();

	{
		int y = yToScreen(0);

		g.setColour(m_cAxis);
		g.fillRect(getLocalBounds().getX() + 1, y,
			getLocalBounds().getWidth() - 2, 1);

		g.setColour(m_cText);
		drawText(g, Point<int>(6, y - 2), "0");
	}

	{
		int y = yToScreen(1);

		g.setColour(m_cAxis);
		g.fillRect(getLocalBounds().getX() + 1, y,
			getLocalBounds().getWidth() - 2, 1);

		g.setColour(m_cText);
		drawText(g, Point<int>(6, y + 2), "1", Justification::topLeft);
	}

	PathStrokeType st(1.0);

	// path
	for (unsigned int i = 0; i < m_filterData.size(); i++)
	{
		g.setColour(m_filterData[i].Color);
		g.strokePath(m_path[i], st, t);
	}
}

void BrickWallChart::update()
{
	const Rectangle<int> bounds = getLocalBounds();
	const Rectangle<int> r = bounds.reduced(4, 4);

	for (unsigned int i = 0; i < m_filterData.size(); i++)
	{
		bool isDefilned = true;
		m_path[i].clear();

		float fMax = (float) m_filterData[i].Filter->getSampleRate() / 2;
		for (int xi = 0; xi < r.getWidth(); ++xi)
		{
			float x = xi / float(r.getWidth()); // [0..1)
			float f = xToF(x, fMax);

			Dsp::complex_t c = m_filterData[i].Filter->response(f / 2.f);
			float y = float(std::abs(c));

			if (!Dsp::is_nan(y))
			{
				if (xi == 0)
					m_path[i].startNewSubPath(x, y);
				else
					m_path[i].lineTo(x, y);
			}
			else
			{
				m_path[i].clear();
				isDefilned = false;
				break;
			}
		}

		if (isDefilned)
		{
			m_path[i].startNewSubPath(0, 0);
		}
	}

	float yh = m_path[0].getBounds().getHeight();
	m_scale_y = (yh > MARGIN) ? (1 / yh) : (1 / MARGIN);

	repaint();
}

AffineTransform BrickWallChart::calcTransform()
{
	const Rectangle<int> bounds = getLocalBounds();
	const Rectangle<int> r = bounds.reduced(4, 4);

	AffineTransform t;

	// scale x from 0..1 to 0..getWidth(), and flip vertical
	t = AffineTransform::scale(float(r.getWidth()), -1.f);

	// scale y from gain to 0..1 bounds in r
	t = t.scaled(1.f, m_scale_y);

	// scale y from 0..1 to getHeight()
	t = t.scaled(1.f, float(r.getHeight()));

	// translate
	t = t.translated(float(r.getX()), float(r.getBottom()));

	return t;
}
