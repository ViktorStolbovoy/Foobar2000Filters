#include "FilterChainItem.h"
#include "PanelInputChannels.h"
#include "PanelFilterBlock.h"

PanelFilterBlock::PanelFilterBlock(FilterBlockItem *block, std::vector<int> & virtualInputs) : m_block(block)
{
	this->setName("PanelFilterBlock");

	int x;
	{
		auto c = new Label();
		c->setText("Filter Definition: ", NotificationType::dontSendNotification);
		c->setBounds(X0, Y0, GetLabelWidth(c), BUTTON_HEIGHT);
		addAndMakeVisible(c);
		m_label = c;
		x = c->getRight() + GAP;
	}
	 
	{
		auto c = new ComboBox();
		for (uint32 i = 0; i < virtualInputs.size(); i++)
		{
			int vinp = virtualInputs[i];
			c->addItem(String(vinp), vinp);
		}
		c->setBounds(x, Y0, 70, BUTTON_HEIGHT);
		addAndMakeVisible(c);
		m_selector = c;
		m_panelWidth = c->getRight() + GAP;
	}

	ResetFromControl();
}


PanelFilterBlock::~PanelFilterBlock()
{
	this->removeChildComponent(m_label);
	delete m_label;
	this->removeChildComponent(m_selector);
	delete m_selector;
}
void PanelFilterBlock::SetControl()
{
	m_block->VirtualInput = m_selector->getSelectedId();
	m_block->treeHasChanged();
}

void PanelFilterBlock::ResetFromControl()
{
	m_selector->setSelectedId(m_block->VirtualInput);
}

