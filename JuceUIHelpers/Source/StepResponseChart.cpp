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
#include "StepResponseChart.h"

//------------------------------------------------------------------------------

StepResponseChart::StepResponseChart(FilterListeners& listeners)
	: FilterChart(listeners)
	, m_ymax(1)
{
}

const String StepResponseChart::getName() const
{
	return "Impulse Response";
}

int StepResponseChart::yToScreen(float y)
{
	AffineTransform t = calcTransform();
	Point<float> p(0, y);
	return int(p.transformedBy(t).getY());
}

void StepResponseChart::paintContents(Graphics& g)
{
	AffineTransform t = calcTransform();

	const Rectangle<int> bounds = getLocalBounds();
	const Rectangle<int> r = bounds;

	g.setColour(m_cAxis);
	g.fillRect(r.getX(), (r.getY() + r.getBottom()) / 2, r.getWidth(), 1);

	PathStrokeType st(1.0);
		
	// path
	for (unsigned int i = 0; i < m_filterData.size(); i++)
	{
		g.setColour(m_filterData[i].Color);
		g.strokePath(m_path[i], st, t);
	}
}

void StepResponseChart::paintOverChildren(Graphics& g)
{
	const Rectangle<int>& bounds = getLocalBounds();
	Rectangle <int> r = bounds.reduced(1, 1);
	if (r.getWidth() > m_numSamples * 2)
	{


		float dx = r.getWidth() / (float)m_numSamples;
		float x = (float)r.getX();
		for (int i = 1; i <= m_numSamples; i++)
		{
			g.setColour(Colour::fromRGBA(0, 0, 0, (i % 10 == 0) ? 32 : 16));
			g.fillRect((int)(x + 0.5), r.getY(), 1, r.getHeight());
			x += dx;
		}
	}
}

/*
 * compute the path.
 * the x coordinates will range from 0..1
 *
 */
void StepResponseChart::update()
{
	const Rectangle<int> bounds = getLocalBounds();
	const Rectangle<int> r = bounds.reduced(4, 4);

	m_ymax = .1f;

	for (unsigned int i = 0; i < m_filterData.size(); i++)
	{
		m_path[i].clear();
		const float* impulse = m_filterData[i].Filter->getStepResponse(&m_numSamples);
		//S: - I'd like to compare responses with the same scale
		// chop off an empty tail
		//int bigs = 0;
		//int n = numSamples-1;
		//for (int i = numSamples-1; i > 10; --i)
		//{
		//n--;
		//if (impulse[i] > 1e-3)
		//{
		//bigs++;
		//if (bigs > 10)
		//break;
		//}
		//}
		//numSamples = jmin (numSamples, int (1.2 * n));
		

		bool isDefined = true;
		int w = r.getWidth();
		for (int xi = 0; xi < w - 1; ++xi)
		{
			// resample
			float x = xi * m_numSamples / float(w);
			float t = x - floor(x);
			int intX((int)x);
			float y0;
			float y1;

			y0 = impulse[intX];
			y1 = impulse[intX + 1];

			float y = y0 + t * (y1 - y0);

			if (!Dsp::is_nan(y))
			{
				x /= m_numSamples;
				if (xi == 0)
					m_path[i].startNewSubPath(x, y);
				else
					m_path[i].lineTo(x, y);
				m_ymax = jmax(float(fabs(y)), float(m_ymax));
			}
			else
			{
				m_path[i].clear();
				isDefined = false;
				break;
			}
		}

		if (isDefined) m_path[i].startNewSubPath(0, 0);

		delete[] impulse;
	}	 

	repaint();
}

AffineTransform StepResponseChart::calcTransform()
{
	const Rectangle<int> bounds = getLocalBounds();
	const Rectangle<int> r = bounds.reduced(4, 4);

	AffineTransform t;

	// scale x from 0..1 to 0..getWidth(), flip vertical, scale
	t = AffineTransform::scale(float(r.getWidth()), -1.f / m_ymax);

	// scale y to fit bounds
	t = t.scaled(1.f, r.getHeight() / 2.1f);

	// translate
	t = t.translated(float(r.getX()), float(r.getCentreY()));

	return t;
}
