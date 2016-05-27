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

#ifndef DSPDEMO_MAINPANEL_H
#define DSPDEMO_MAINPANEL_H

#include "Common.h"
#include "ResizableLayout.h"
#include "FilterListener.h"
#include "Config.h"

class MainPanel :
	public Component,
	public ButtonListener,
	public SliderListener,
	public ComboBoxListener,
	public TopLevelResizableLayout,
	public FilterListener,
	public ChangeListener
{
public:
	MainPanel(Config * config);
	~MainPanel();

	void createCharts(const juce::Rectangle<int>& r);
	void paint(Graphics& g);
	void setAudio(int audioId);
	void buttonClicked(Button *ctrl);
	void sliderValueChanged(Slider* ctrl);
	void comboBoxChanged(ComboBox* ctrl);
	void changeListenerCallback(ChangeBroadcaster* source);
	void onFilterParameters();

private:
	void createFilters();
	void setConvertTo();
	double getTargetSampleRate();
	int getSourceSampleRate();
	void resetCurrentFilterLabel();
	void resetParameters(FilterConfig * cf);

private:
	ListenerList<FilterListener> m_listeners;
	ComboBox* m_menuFamily;
	ComboBox* m_convertTo;
	TextButton* m_setButton;
	TextButton* m_okButton;
	TextButton* m_cancelButton;
	Label* m_labelCurrentFilter;
	TabbedButtonBar * m_sourceSettings;
	FilterControls * m_filterControls;
	Config m_configCurrent;
	Config *m_configMainRef;
	ScopedPointer<Dsp::Filter> m_filter[2];
};

#endif

