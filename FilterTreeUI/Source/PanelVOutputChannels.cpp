#include "FilterChainItem.h"
#include "PanelInputChannels.h"
#include "PanelVOutputChannels.h"
#include "ColorChangeButton.h"


PanelVOutputChannels::PanelVOutputChannels(VirtualOutputItem *output) : m_output(output)
{
	this->setName("PanelVOutputChannels");

	int x;
	int y;
	{
		auto c = new Label();
		c->setText("Gain: ", NotificationType::dontSendNotification);
		c->setBounds(X0, Y0, GetLabelWidth(c), BUTTON_HEIGHT);
		addAndMakeVisible(c);
		m_labelGain = c;
		x = c->getRight() + GAP;
	}
	 
	{
		auto c = new TextEditor();
		c->setBounds(x, Y0, 80, BUTTON_HEIGHT);
		addAndMakeVisible(c);
		m_editGain = c;

		x = c->getRight() + GAP;
		y = c->getBottom() + GAP;
	}

	int x1;
	{
		auto c = new ToggleButton();
		c->setButtonText("Show in Graphs");
		c->setBounds(X0, y, 160, BUTTON_HEIGHT);
		addAndMakeVisible(c);
		m_buttonShowInGraphs = c;
		x1 = c->getRight() + GAP;
		if (x1 > x) x = x1;
		y = c->getBottom() + GAP;
	}

	
	{
		auto c = new Label();
		c->setText("Color in Graphs: ", NotificationType::dontSendNotification);
		c->setBounds(X0, y, GetLabelWidth(c), BUTTON_HEIGHT);
		addAndMakeVisible(c);
		m_labelColorSelect = c;
		x1 = c->getRight() + GAP;
	}

	{
		auto c = new ColourChangeButton();
		c->setBounds(x1, y, BUTTON_HEIGHT, BUTTON_HEIGHT);
		addAndMakeVisible(c);
		m_buttonColorSelect = c;
		x1 = c->getRight() + GAP;
		if (x1 > x) x = x1;
	}

	m_panelWidth = x;
	ResetFromControl();
}





PanelVOutputChannels::~PanelVOutputChannels()
{
	this->removeAllChildren();

	this->removeChildComponent(m_editGain);
	delete m_editGain;
	this->removeChildComponent(m_labelGain);
	delete m_labelGain;

	delete m_buttonColorSelect;
	delete m_buttonShowInGraphs;
	delete m_labelColorSelect;
}

void PanelVOutputChannels::SetControl()
{
	m_output->Gain = m_editGain->getText().getFloatValue();
	m_output->SetLineColor(m_buttonColorSelect->findColour(TextButton::buttonColourId));
	m_output->ShowInGraphs = m_buttonShowInGraphs->getToggleState();
	m_output->treeHasChanged();
}

void PanelVOutputChannels::ResetFromControl()
{
	m_editGain->setText(String(m_output->Gain, 3), false);
	m_buttonColorSelect->setColour(TextButton::buttonColourId, m_output->GetLineColor());
	m_buttonShowInGraphs->setToggleState(m_output->ShowInGraphs, NotificationType::dontSendNotification);
}
