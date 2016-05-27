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
//#include "binaries.h"
#include "BrickWallChart.h"
#include "CustomSlider.h"
#include "FilterControls.h"
#include "GainChart.h"
#include "GroupDelayChart.h"
#include "MainPanel.h"
#include "PhaseChart.h"
#include "PoleZeroChart.h"
#include "StepResponseChart.h"
#define SIMPLE_LAYOUT 1

MainPanel::MainPanel(Config * config)
	: TopLevelResizableLayout(this)
{
	m_configMainRef = config;
	m_configCurrent = *config;

	setOpaque(true);

	const int w = 1024;
	const int h = 512 + 102;
	const int gap = 4;
	const int x0 = 4;

	m_listeners.add(this);

	int x;
	int y = 4;

	x = x0;
	{
		TabbedButtonBar * c = new TabbedButtonBar(TabbedButtonBar::Orientation::TabsAtTop);
		c->addTab("44.1 Handling", Colours::lightgrey, 1);
		c->addTab("96 Handling", Colours::lightgrey, 2);
		c->setBounds(x, y, 300, 24);
		c->addChangeListener(this);
		addToLayout(c, anchorTopLeft);
		addAndMakeVisible(c);
		m_sourceSettings = c;
	}

	x = w - 170;
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, 80, 24);
		c->setButtonText("OK");
		addToLayout(c, anchorTopRight);
		addAndMakeVisible(c);
		m_okButton = c;
		c->addListener(this);
	}
	x = this->getChildComponent(this->getNumChildComponents() - 1)->getBounds().getRight() + gap;
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, 80, 24);
		c->setButtonText("Cancel");
		addToLayout(c, anchorTopRight);
		addAndMakeVisible(c);
		m_cancelButton = c;
		c->addListener(this);
	}
			
	y = this->getChildComponent(this->getNumChildComponents() - 1)->getBounds().getBottom() + gap;
	x = x0;

	{
		Label * c = new Label();
		c->setBounds(x, y, 80, 24);
		c->setText("Conversion:", NotificationType::dontSendNotification);
		c->setJustificationType(Justification::centredRight);
		addToLayout(c, anchorTopLeft);
		addAndMakeVisible(c);
	}

	x = this->getChildComponent(this->getNumChildComponents() - 1)->getBounds().getRight() + gap;

	{
		ComboBox* c = new ComboBox;
		c->setBounds(x, y, 160, 24);
		addToLayout(c, anchorTopLeft);
		addAndMakeVisible(c);
		//ctrl->addItem ("RBJ Biquad",   1);
		m_convertTo = c;
		c->addListener(this);
	}

	x = this->getChildComponent(this->getNumChildComponents() - 1)->getBounds().getRight() + gap;

	{
		Label * c = new Label();
		c->setBounds(x, y, 60, 24);
		c->setText("Filter:", NotificationType::dontSendNotification);
		c->setJustificationType(Justification::centredRight);
		addToLayout(c, anchorTopLeft);
		addAndMakeVisible(c);
	}

	x = this->getChildComponent(this->getNumChildComponents() - 1)->getBounds().getRight() + gap;

	{
		ComboBox* c = new ComboBox;
		c->setBounds(x, y, 160, 24);
		addToLayout(c, anchorTopLeft);
		addAndMakeVisible(c);
		c->addItem ("No filter", 8);
		c->addItem("Butterworth", 2);
		//ctrl->addItem("Chebyshev I", 3);
		c->addItem("Chebyshev II", 4);
		c->addItem("Elliptic", 5);
		c->addItem("Bessel", 6);
		c->addItem("Legendre", 7);
		m_menuFamily = c;
		c->addListener(this);
	}

	x = this->getChildComponent(this->getNumChildComponents() - 1)->getBounds().getRight() + gap;
	{
		Label * c = new Label();
		c->setBounds(x, y, 230, 24);
		c->setText("Current: none", NotificationType::dontSendNotification);
		c->setJustificationType(Justification::centredRight);
		addToLayout(c, anchorTopLeft);
		m_labelCurrentFilter = c;
		addAndMakeVisible(c);
	}

	x = this->getChildComponent(this->getNumChildComponents() - 1)->getBounds().getRight() + gap;
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, 80, 24);
		c->setButtonText("Set");
		addToLayout(c, anchorTopLeft);
		addAndMakeVisible(c);
		
		m_setButton = c;
		c->addListener(this);
	}

	

	y = this->getChildComponent(this->getNumChildComponents() - 1)->getBounds().getBottom() + gap;
	x = x0;

	x = w - gap;

	const int hfc = 80;

	{
		FilterControls* c = new FilterControls(m_listeners);
		c->setBounds(x0, y, x - x0, hfc);
		addToLayout(c, anchorTopLeft, anchorTopRight);
		addAndMakeVisible(c);
		m_filterControls = c;
		m_listeners.add(c);
	}


	y = this->getChildComponent(this->getNumChildComponents() - 1)->getBounds().getBottom() + gap;
	x = x0;

	const Rectangle<int> r(x, y, w - (x + gap), h - (y + gap));
	createCharts(r);

	setSize(w, h);
	setMinimumSize(256, 256);
	recalculateLayout();
	activateLayout();
	setConvertTo();
	
}

MainPanel::~MainPanel()
{
	deleteAllChildren();
}

/*

Resp   Phase  Poles

+---------+   Delay
|  Brick  |
+---------+   Step

*/
void MainPanel::createCharts(const Rectangle<int>& r)
{
	const int gap = 4;
	const int w = (r.getWidth() - (2 * gap)) / 3;
	const int h = (r.getHeight() - (2 * gap)) / 3;
	const int w2 = w * 2 + gap; //r.getWidth()  - (w + gap);

#ifdef SIMPLE_LAYOUT //VS
	{
		GainChart* c = new GainChart(m_listeners, false);
		c->setBounds(r.getX(), r.getY(), w2, h * 3 + gap + gap);
		addToLayout(c, Point<int>(0, 0), Point<int>(66, 100));
		addAndMakeVisible(c);
	}

	{
		PhaseChart* c = new PhaseChart(m_listeners, false);

		c->setBounds(r.getX() + w + gap + w + gap, r.getY(), w, h);
		addToLayout(c, Point<int>(66, 0), Point<int>(100, 33));
		addAndMakeVisible(c);
	}
#else 
	{
		GainChart* c = new GainChart(m_listeners);
		c->setBounds (r.getX(), r.getY(), w, h);
		addToLayout (c, Point<int>(0, 0), Point<int>(33, 33));
		addAndMakeVisible(c);
	}

	{
		PhaseChart* c = new PhaseChart(m_listeners);
		c->setBounds (r.getX() + w + gap, r.getY(), w, h);
		addToLayout (c, Point<int>(33, 0), Point<int>(66, 33));
		addAndMakeVisible(c);
	}

	{
		PoleZeroChart* c = new PoleZeroChart (m_listeners);
		c->setBounds (r.getX() + w + gap + w + gap, r.getY(), w, h);
		addToLayout (c, Point<int>(66, 0), Point<int>(100, 33));
		addAndMakeVisible (c);
	}

	{
		BrickWallChart* c = new BrickWallChart (m_listeners);
		c->setBounds (r.getX(), r.getY() + h + gap, w2, h2);
		addToLayout (c, Point<int>(0, 33), Point<int>(66, 100));
		addAndMakeVisible (c);
	}

#endif // VS


  {
	  GroupDelayChart* c = new GroupDelayChart(m_listeners, false);
	  c->setBounds(r.getX() + w + gap + w + gap, r.getY() + h + gap, w, h);
	  addToLayout(c, Point<int>(66, 33), Point<int>(100, 66));
	  addAndMakeVisible(c);
  }

  {
	  StepResponseChart* c = new StepResponseChart(m_listeners);
	  c->setBounds(r.getX() + w + gap + w + gap, r.getY() + h + gap + h + gap, w, h);
	  addToLayout(c, Point<int>(66, 66), Point<int>(100, 100));
	  addAndMakeVisible(c);
  }
}



void MainPanel::paint(Graphics& g)
{
	g.fillAll(Colour(0xffc1d0ff));
}

void MainPanel::setConvertTo()
{
	auto cf = m_configCurrent.getConfigForSourceSampleRate(getSourceSampleRate());
	m_filterControls->sourceFSChanged(cf);
	m_convertTo->clear();
	if (m_sourceSettings->getCurrentTabIndex())
	{
		//96 tab
		m_convertTo->addItem("Original", 1);
		m_convertTo->addItem("192kHz", 2);
		
	}
	else 
	{
		//44.1 tab
		m_convertTo->addItem("88.2kHz", 2);
		m_convertTo->addItem("176.4kHz", 4);
	}
	resetParameters(cf)	;
}

void MainPanel::resetParameters(FilterConfig * cf)
{
	m_menuFamily->setSelectedId(cf->FilterType, NotificationType::dontSendNotification);
	m_convertTo->setSelectedId(cf->FMultiplier, NotificationType::dontSendNotification);
	createFilters();

}


double MainPanel::getTargetSampleRate()
{
	if (m_sourceSettings->getCurrentTabIndex())
	{
		switch (m_convertTo->getSelectedId())
		{
			case 2: return 192000;
			default: return 0;
		}
	}
	else 
	{
		switch (m_convertTo->getSelectedId())
		{
			case 2: return 88200;
			case 4: return 176400;
			default: return 0;
		}
	}
}

int MainPanel::getSourceSampleRate()
{
	return  (m_sourceSettings->getCurrentTabIndex()) ? 96000 : 44100;
}

void MainPanel::createFilters()
{
	int sourceFS = getSourceSampleRate();
	m_configCurrent.setFilterType(sourceFS, m_menuFamily->getSelectedId(), (unsigned char) m_convertTo->getSelectedId());
	m_filter[0] = m_configCurrent.createFilter(getSourceSampleRate(), 1);
	
	//The same filter as before possible changing the target sample rate
	m_filter[1] = m_configMainRef->createFilter(getSourceSampleRate(), 1);

	String name = m_menuFamily->getText();

	auto config = m_configMainRef->getConfigForSourceSampleRate(sourceFS);
	if (config && config->FMultiplier > 1)
	{
		String msg("Current: ");
		m_labelCurrentFilter->setText(msg << name << "(" << ((int)config->FilterOrder) << ") at " << ((int)config->Fc), NotificationType::dontSendNotification);
	}
	else
	{
		resetCurrentFilterLabel();
	}

	std::vector<FilterData> fdv;
	if (m_filter[0])
	{
		FilterData fd;
		fd.Filter = m_filter[0];
		fd.Color = Colours::blue;
		fdv.push_back(fd);
	}

	if (m_filter[1])
	{
		FilterData fd;
		fd.Filter = m_filter[1];
		fd.Color = Colours::green;
		fdv.push_back(fd);
	}

	m_listeners.call(&FilterListener::onFilterChanged, fdv);
}


void MainPanel::buttonClicked(Button* ctrl)
{
	if(ctrl == m_setButton)
	{
		int sourceFS = getSourceSampleRate();
		m_configMainRef->cloneConfig(sourceFS, &m_configCurrent);
		
		createFilters();
	}
	else if (ctrl == m_okButton)
	{
		if (auto dw = findParentComponentOfClass<DialogWindow>()) dw->exitModalState(1);
	}
	else if (ctrl == m_cancelButton)
	{
		if (auto dw = findParentComponentOfClass<DialogWindow>()) dw->exitModalState(-1);
	}

}


void MainPanel::comboBoxChanged(ComboBox* ctrl)
{
	createFilters();
}

void MainPanel::resetCurrentFilterLabel()
{
	m_labelCurrentFilter->setText("Curent: original", NotificationType::dontSendNotification);
}

void MainPanel::changeListenerCallback(ChangeBroadcaster* source)
{
	if(source == m_sourceSettings)
	{
		setConvertTo();
	}
}

void MainPanel::sliderValueChanged(Slider* ctrl)
{
}

void MainPanel::onFilterParameters()
{

}

//------------------------------------------------------------------------------
