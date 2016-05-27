#pragma once
class FilterPanelControls : public Component
{
public:
	FilterPanelControls(FilterConfigEx * config);
	~FilterPanelControls();
private:
	FilterConfigEx * m_config;
	SliderGroup **m_sliders;
	int m_paramCount;
	Dsp::Filter *m_filter;
	ListenerList<FilterListener> m_listeners;
};

class PanelFilter : public PanelItemControl, public ComboBoxListener
	
{
public:
	PanelFilter(FilterItem *filter);
	~PanelFilter();
	
	void SetControl() override;
	void ResetFromControl() override;
private:
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

	FilterItem *m_filterItem;
	FilterConfigEx m_currentConfig;
	ComboBox *m_filterFamily;
	ComboBox *m_filterPassType;
	Label * m_labelFilter;

	FilterPanelControls *m_filterControls;
};

