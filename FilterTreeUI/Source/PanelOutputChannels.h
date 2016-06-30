#pragma once
class PanelOutputChannels : public PanelItemControl, public ComboBoxListener
	
{
public:
	PanelOutputChannels(OutputItem *input, uint32 usedOutputMask, bool canGraph);
	~PanelOutputChannels();
	
	void SetControl() override;
	void ResetFromControl() override;
private:

	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	
	OutputItem *m_output;
	Label **m_labels = nullptr;
	ComboBox **m_selectors = nullptr;
	TextEditor **m_editsDelay = nullptr;
	uint8 m_numChannels = 0;

	const bool m_canGraph;

	TextButton *m_buttonColorSelect = nullptr;
	ToggleButton *m_buttonShowInGraphs = nullptr;
	Label *m_labelColorSelect = nullptr;
	static const int CHANNEL_NOT_SELECTED = 255;
	void AddChannelSelector(String chName, int *x, int x1, int *y, int labelWidth, int validChannelsLenght, uint8 *validChannels, int index);
};

