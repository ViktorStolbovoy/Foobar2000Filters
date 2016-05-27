#include "ConfigItem.h"
#include "FilterChainItem.h"
#include "PanelInputChannels.h"
#include "PanelOutputChannels.h"
#include "ColorChangeButton.h"


PanelOutputChannels::PanelOutputChannels(OutputItem *output, uint32 usedOutputMask, bool canGpaph) : m_output(output), m_canGraph(canGpaph), m_buttonShowInGraphs(nullptr), m_labelColorSelect(nullptr), m_buttonColorSelect(nullptr)
{
	this->setName("PanelOutputChannels");
	InputItem * input = ChainItemBase::FindItem<InputItem>(output);
	MixerItem * mixer = nullptr;

	uint8 * inputChannels = nullptr;

	if (input)
	{
		inputChannels = ConfigItemBase::ChannelMaskToArray(input->InputMask, &m_numChannels);
	}
	else
	{
		mixer = ChainItemBase::FindItem<MixerItem>(output);
		m_numChannels = (mixer)? 1 : 0;
	}

	m_labels = new Label*[m_numChannels];
	m_selectors = new ComboBox*[m_numChannels];

	uint32 thisMask = ConfigItemBase::ArrayToChannelMask(output->OutputChannels, output->OutputChannelsLenght);
	thisMask |= ~usedOutputMask;

	uint8 validChannelsLenght = 0;
	auto validChannels = ConfigItemBase::ChannelMaskToArray(thisMask, &validChannelsLenght);

	int x;
	int y;
	{
		auto c = new Label();
		c->setText("Delay, ms: ", NotificationType::dontSendNotification);
		c->setBounds(X0, Y0, GetLabelWidth(c), BUTTON_HEIGHT);
		addAndMakeVisible(c);
		m_labelDelay = c;
		x = c->getRight() + GAP;
	}
	 
	{
		auto c = new TextEditor();
		c->setBounds(x, Y0, 80, BUTTON_HEIGHT);
		addAndMakeVisible(c);
		m_editDelay = c;

		x = c->getRight() + GAP;
		y = c->getBottom() + GAP;
	}

	int x1;
	if (m_canGraph)
	{
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
	}

	const int labelWidth = m_labelDelay->getFont().getStringWidth("99 (WWW): ");
	if (mixer && m_numChannels == 1)
	{
		//Special case for chain starting with mixer
		AddChannelSelector("Mixed: ", &x, x, &y, labelWidth, validChannelsLenght, validChannels, 0);
	}
	else
	{
		
		for (int i = 0, ix = 0; i < m_numChannels; ix++)
		{
			int y = Y0;
			x1 = x;

			for (int iy = 0; iy < NUM_ROWS && i < m_numChannels; iy++, i++)
			{
				uint8 chNum = inputChannels[i];
				AddChannelSelector(ChannelNumberToAbbreviation(chNum) + ": ", &x, x1, &y, labelWidth, validChannelsLenght, validChannels, i);
				
			}
		}
	}
	m_panelWidth = x;
	delete [] validChannels;
	ResetFromControl();
	if (inputChannels) delete[] inputChannels;
}


void PanelOutputChannels::AddChannelSelector(String chName, int *x, int x1, int *y, int labelWidth, int validChannelsLenght, uint8 *validChannels, int index)
{
	int x2;
	{
		auto c = new Label();
		c->setText(chName, NotificationType::dontSendNotification);
		c->setBounds(x1, *y, labelWidth, BUTTON_HEIGHT);
		c->setJustificationType(Justification::right);
		addAndMakeVisible(c);
		m_labels[index] = c;

		x2 = c->getRight() + GAP;
	}
	{
		auto c = new ComboBox();
		c->addItem("N/C", CHANNEL_NOT_SELECTED);
		for (int j = 0; j < validChannelsLenght; j++)
		{
			c->addItem(ChannelNumberToAbbreviation(validChannels[j]), validChannels[j]);
		}

		c->setBounds(x2, *y, labelWidth + 10, BUTTON_HEIGHT);
		c->addListener(this);
		addAndMakeVisible(c);
		m_selectors[index] = c;

		*y = c->getBottom() + GAP;
		*x = c->getRight() + GAP;
	}
}


PanelOutputChannels::~PanelOutputChannels()
{
	this->removeAllChildren();
	
	for (int i = 0; i < m_numChannels; i++)
	{
		Component* c = m_labels[i];
		this->removeChildComponent(c);
		delete c;

		c = m_selectors[i];
		this->removeChildComponent(c);
		delete c;
	}

	delete[] m_selectors;
	delete[] m_labels;

	this->removeChildComponent(m_editDelay);
	delete m_editDelay;
	this->removeChildComponent(m_labelDelay);
	delete m_labelDelay;

	delete m_buttonColorSelect;
	delete m_buttonShowInGraphs;
	delete m_labelColorSelect;
}

void PanelOutputChannels::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	//Enable everything 
	for (int i = 1; i <= m_numChannels; i++)
	{
		int id = m_selectors[0]->getItemId(i);
		for (int j = 0; j < m_numChannels; j++)
		{
			m_selectors[j]->setItemEnabled(id, true);
		}
	}

	for (int i = 0; i < m_numChannels; i++)
	{
		int selectedId = m_selectors[i]->getSelectedId();

		if (selectedId != CHANNEL_NOT_SELECTED)
		{
			for (int j = 0; j < m_numChannels; j++)
			{
				if (i != j)
				{
					m_selectors[j]->setItemEnabled(selectedId, false);
				}
			}
		}
	}
}

void PanelOutputChannels::SetControl()
{
	m_output->DelayMS = m_editDelay->getText().getFloatValue();
	m_output->OutputChannelsLenght = m_numChannels;

	for (int i = 0; i < m_numChannels; i++)
	{
		int id = m_selectors[i]->getSelectedId();
		if (id == CHANNEL_NOT_SELECTED) id = 0;
		m_output->OutputChannels[i] = (uint8) id;
	}
	
	if (m_canGraph)
	{
		m_output->SetLineColor(m_buttonColorSelect->findColour(TextButton::buttonColourId));
		m_output->ShowInGraphs = m_buttonShowInGraphs->getToggleState();
	}
	else
	{
		m_output->ShowInGraphs = false;
	}
	m_output->treeHasChanged();
}

void PanelOutputChannels::ResetFromControl()
{
	m_editDelay->setText(String(m_output->DelayMS, 6), false);

	for (int i = 0; i < m_numChannels; i++)
	{
		int id = m_output->OutputChannels[i];
		if (id == 0) id = CHANNEL_NOT_SELECTED;
		m_selectors[i]->setSelectedId(id);
	}
	if (m_canGraph)
	{
		m_buttonColorSelect->setColour(TextButton::buttonColourId, m_output->GetLineColor());
		m_buttonShowInGraphs->setToggleState(m_output->ShowInGraphs, NotificationType::dontSendNotification);
	}
	comboBoxChanged(nullptr);
}
