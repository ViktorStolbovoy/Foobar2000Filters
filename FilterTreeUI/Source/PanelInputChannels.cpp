#include "FilterChainItem.h"
#include "PanelInputChannels.h"

String PanelItemControl::ChannelNumberToAbbreviation(uint8 num)
{
	String s;
	switch (num)
	{
		case 1: s = " (L)"; break;
		case 2: s = " (R)"; break;
		case 3: s = " (C)"; break;;
		case 4: s = " (LFE)"; break;
		case 5: s = " (BL)"; break;
		case 6: s = " (BR)"; break;
		case 7: s = " (FCL)"; break;
		case 8: s = " (FCR)"; break;
		case 9: s = " (BC)"; break;
		case 10:s = " (SL)"; break;
		case 11:s = " (SR)"; break;
		case 12:s = " (TC)"; break;
		case 13:s = " (TFL)"; break;
		case 14:s = " (TFC)"; break;
		case 15:s = " (TFR)"; break;
		case 16:s = " (TBL)"; break;
		case 17:s = " (TBC)"; break;
		case 18:s = " (TBR)"; break;
		default: return String(num);
	};

	return String(num) + s;
}

int PanelItemControl::GetLabelWidth(Label * label)
{
	Font f = label->getFont();
	return f.getStringWidth(label->getText());
}

int PanelItemControl::GetWidth()
{
	return m_panelWidth;
}

void PanelItemControl::paint(Graphics& g)
{
	const Rectangle<int> bounds = getLocalBounds();

	g.setColour(Colour::fromRGBA(0, 0, 0, 64));
	g.fillRect(bounds.reduced(1, 1));

	g.setColour(Colour::fromRGBA(0, 0, 0, 128));
	g.drawRect(bounds, 1);
}

PanelInputChannels::PanelInputChannels(InputItem *input, int usedInputsMask) : m_input(input)
{
	this->setName("PanelInputChannels");
	m_usedInputMask = usedInputsMask & (~(input->InputMask)); //Allowing to set bits that already set bu this input control
	
	uint8 i = 0;
	int mask = 1;
	const int width = 80;
	const int numCols = MAX_NUM_CHANNELS / NUM_ROWS;
	for (int ix = 0; ix < numCols; ix++)
	{
		for (int iy = 0; iy < NUM_ROWS; iy++)
		{
			auto c = new ToggleButton(ChannelNumberToAbbreviation(i + 1));
			c->setBounds(X0 + (GAP + width) * ix, Y0 + (GAP + BUTTON_HEIGHT) * iy, width, BUTTON_HEIGHT);
			if (m_usedInputMask & mask) c->setEnabled(false);
			addAndMakeVisible(c);
			m_toggleButtons[i] = c;
			i++;
			mask <<= 1;
		}
	}
	
	m_panelWidth = X0 + (GAP + width) * numCols;
	ResetFromControl();
}


PanelInputChannels::~PanelInputChannels()
{
	this->removeAllChildren();
	for (int i = 0; i < MAX_NUM_CHANNELS; i++)
	{
		auto c = m_toggleButtons[i];
		this->removeChildComponent(c);
		delete c;
	}
}
void PanelInputChannels::SetControl()
{
	int mask = 1;
	int res = 0;
	for (int i = 0; i < MAX_NUM_CHANNELS; i++)
	{
		if (m_toggleButtons[i]->getToggleState()) res |= mask;
		mask <<= 1;
	}
	m_input->InputMask = res;
	m_input->treeHasChanged();
}

void PanelInputChannels::ResetFromControl()
{
	int mask = 1;
	for (int i = 0; i < MAX_NUM_CHANNELS; i++)
	{
		m_toggleButtons[i]->setToggleState((m_input->InputMask & mask) != 0 , NotificationType::dontSendNotification);
		mask <<= 1;
	}
}




