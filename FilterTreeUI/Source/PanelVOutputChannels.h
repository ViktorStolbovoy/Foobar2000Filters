#pragma once
class PanelVOutputChannels : public PanelItemControl
	
{
public:
	PanelVOutputChannels(VirtualOutputItem *output);
	~PanelVOutputChannels();
	
	void SetControl() override;
	void ResetFromControl() override;
private:
	VirtualOutputItem *m_output;

	Label *m_labelGain = nullptr;
	TextEditor *m_editGain = nullptr;

	TextButton *m_buttonColorSelect = nullptr;
	ToggleButton *m_buttonShowInGraphs = nullptr;
	Label *m_labelColorSelect = nullptr;
};

