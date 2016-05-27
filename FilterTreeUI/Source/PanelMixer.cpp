#include "FilterChainItem.h"
#include "PanelInputChannels.h"
#include "PanelMixer.h"


PanelMixer::PanelMixer(MixerItem *mixer, std::vector<uint32> * availableVOuts) : m_mixer(mixer), m_availableVOuts(availableVOuts)
{
	this->setName("PanelMixer");
	m_numToggleButtons = m_availableVOuts->size();
	
	uint8 i = 0;
	int mask = 1;
	const int width = 80;
	int ix = 0;
	m_toggleButtons = new ToggleButton*[m_numToggleButtons];
	for (; i < m_numToggleButtons; ix++)
	{
		for (int iy = 0; iy < NUM_ROWS && i < m_numToggleButtons; iy++)
		{
			auto c = new ToggleButton(String((*availableVOuts)[i]));
			c->setBounds(X0 + (GAP + width) * ix, Y0 + (GAP + BUTTON_HEIGHT) * iy, width, BUTTON_HEIGHT);
			addAndMakeVisible(c);
			m_toggleButtons[i] = c;
			i++;
			mask <<= 1;
		}
	}
	
	m_panelWidth = X0 + (GAP + width) * ix;
	ResetFromControl();
}


PanelMixer::~PanelMixer()
{
	this->removeAllChildren();
	for (int i = 0; i < m_numToggleButtons; i++)
	{
		auto c = m_toggleButtons[i];
		this->removeChildComponent(c);
		delete c;
	}
	delete m_availableVOuts;
}
void PanelMixer::SetControl()
{
	int mask = 1;
	int res = 0;
	for (int i = 0; i < m_numToggleButtons; i++)
	{
		if (m_toggleButtons[i]->getToggleState()) res |= mask;
		mask <<= 1;
	}
	m_mixer->VirtualOutputs.clear();
	for (int i = 0; i < m_numToggleButtons; i++)
	{
		if (m_toggleButtons[i]->getToggleState()) m_mixer->VirtualOutputs.push_back((*m_availableVOuts)[i]);
	}
	m_mixer->treeHasChanged();
}

void PanelMixer::ResetFromControl()
{
	for (int i = 0; i < m_numToggleButtons; i++)
	{
		m_toggleButtons[i]->setToggleState(false, NotificationType::dontSendNotification);
		for (uint32 j = 0; j < m_mixer->VirtualOutputs.size(); j++)
		{
			if ((*m_availableVOuts)[i] == m_mixer->VirtualOutputs[j])
			{
				m_toggleButtons[i]->setToggleState(true, NotificationType::dontSendNotification);
				break;
			}
		}
	}
}



