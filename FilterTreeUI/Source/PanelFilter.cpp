#include <SliderGroup.h>
#include <FilterControls.h>
#include "FilterChainItem.h"
#include "FilterConfigEx.h"
#include "PanelInputChannels.h"
#include "PanelFilter.h"


FilterPanelControls::FilterPanelControls(FilterConfigEx * config) : m_config(config)
{
	m_filter = m_config->createFilter(44100, 1); //Fs and ch# don't matter here 
	m_paramCount = m_filter ? m_filter->getNumParams() - 1 : 0; //one less because need to skip Fs 

	const int x = 4;
	const int y = 4;
	const int gap = 8;
	const int controlWidth = 150;
	const int controlHeight = 72;

	m_sliders = new SliderGroup*[m_paramCount];

	for (int i = 0; i < m_paramCount; i++)
	{
		const auto info = m_filter->getParamInfo(i + 1);
		//SliderGroup handles lifetime of FilterParamSliderGroupModel internally
		auto c = new SliderGroup(new FilterParamSliderGroupModel(m_listeners, m_filter, i + 1, m_config), controlWidth);
		c->setBounds(x + (controlWidth + gap) * i, y, controlWidth, controlHeight);
		addAndMakeVisible(c);
		m_sliders[i] = c;
	}
}

FilterPanelControls::~FilterPanelControls()
{
		

	for (int i = 0; i < m_paramCount; i++)
	{
		auto c = m_sliders[i];
		this->removeChildComponent(c);
		delete c;
	}

	delete [] m_sliders;

	if (m_filter) delete m_filter;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


PanelFilter::PanelFilter(FilterItem *filterItem) : m_filterItem(filterItem)
{
	this->setName("PanelFilter");
	m_filterControls = nullptr;
	int x = X0;
	int y = Y0;

	{
		Label * c = new Label();
		c->setText("Filter:", NotificationType::dontSendNotification);
		c->setBounds(x, y, GetLabelWidth(c), BUTTON_HEIGHT);
		c->setJustificationType(Justification::centredRight);
		addAndMakeVisible(c);
		m_labelFilter = c;

		x = c->getRight() + GAP;
	}

	{
		ComboBox* c = new ComboBox;
		c->setBounds(x, y, 160, BUTTON_HEIGHT);
		c->addItem("Low Pass", (int)(FilterPassType::LowPass));
		c->addItem("High Pass", (int)FilterPassType::HighPass);
		c->addItem("Band Pass", (int)FilterPassType::BandPass);
		c->addItem("Band Stop", (int)FilterPassType::BandStop);
		c->addItem("Low Shelf", (int)FilterPassType::LowShelf);
		c->addItem("Hight Shelf", (int)FilterPassType::HighShelf);
		c->addItem("Band Shelf", (int)FilterPassType::BandShelf);
		addAndMakeVisible(c);
		m_filterPassType = c;
		c->addListener(this);

		x = c->getRight() + GAP;
	}

	{
		ComboBox* c = new ComboBox;
		c->setBounds(x, y, 160, BUTTON_HEIGHT);
		addAndMakeVisible(c);
		c->addItem("RBJ", 1);
		c->addItem("Butterworth", 2);
		c->addItem("Chebyshev I", 3);
		c->addItem("Chebyshev II", 4);
		c->addItem("Elliptic", 5);
		c->addItem("Bessel", 6);
		c->addItem("Legendre", 7);
		c->addListener(this);
		m_filterFamily = c;
	}

	m_panelWidth = 800;
	ResetFromControl();
}


PanelFilter::~PanelFilter()
{
	this->removeChildComponent(m_labelFilter);
	delete m_labelFilter;
	this->removeChildComponent(m_filterPassType);
	delete m_filterPassType;
	this->removeChildComponent(m_filterFamily);
	delete m_filterFamily;

	if (m_filterControls)
	{
		this->removeChildComponent(m_filterControls);
		delete m_filterControls;;
	}
}

void PanelFilter::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	auto fpt = (FilterPassType)m_filterPassType->getSelectedId();
	uint8 selFF = (uint8)m_filterFamily->getSelectedId();
	if (comboBoxThatHasChanged == m_filterPassType)
	{
		if (!FilterConfigEx::isFilterImplemented(selFF, fpt))
		{
			m_filterFamily->setSelectedId(0, NotificationType::dontSendNotification);
			selFF = 0;
		}
		for (uint8 filterFamily = 1; filterFamily <= 7; filterFamily++)
		{
			m_filterFamily->setItemEnabled(filterFamily, FilterConfigEx::isFilterImplemented(filterFamily, fpt));
		}
	}
	if (m_filterControls)
	{
		this->removeChildComponent(m_filterControls);
		delete m_filterControls;;
	}
	m_currentConfig.PassType = fpt;
	m_currentConfig.FilterType = selFF;
	m_filterControls = new FilterPanelControls(&m_currentConfig);
	m_filterControls->setBounds(X0, m_filterFamily->getBottom() + GAP, m_panelWidth, 72);
	addAndMakeVisible(m_filterControls);
}

void PanelFilter::SetControl()
{
	m_filterItem->Config = m_currentConfig;
	m_filterItem->treeHasChanged();
}

void PanelFilter::ResetFromControl()
{
	m_currentConfig = m_filterItem->Config;
	m_filterPassType->setSelectedId(m_currentConfig.PassType);
	m_filterFamily->setSelectedId(m_currentConfig.FilterType);

	comboBoxChanged(m_filterPassType);
}

