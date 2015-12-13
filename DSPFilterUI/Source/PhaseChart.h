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

#ifndef DSPDEMO_PHASECHART_H
#define DSPDEMO_PHASECHART_H

#include "FilterChart.h"

/*
 * Displays the phase response of a Dsp::Filter in degrees
 *
 */
class PhaseChart : public FrequencyChart
{
public:
  enum
  {
    // this is so there is some room on the top and bottom
    maxPhase = 120
  };

  PhaseChart (FilterListeners& listeners);

  const String getName () const;

  int yToScreen (float y);

  void paintContents (Graphics& g);

private:
  void update ();
  bool drawPhaseLine (Graphics& g, int degrees, bool drawLabel = true);
  AffineTransform calcTransform ();

private:
  Path m_path[NUM_FILTERS_ON_CHART];
  
};

#endif
