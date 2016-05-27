#pragma once
class PanelMixer : public PanelItemControl
	
{
public:
	PanelMixer(MixerItem *mixer, std::vector<uint32> * availableVOuts);
	~PanelMixer();

	void SetControl() override;
	void ResetFromControl() override;
private:
	MixerItem *m_mixer;
	ToggleButton **m_toggleButtons;
	std::vector<uint32> * m_availableVOuts;
	int m_numToggleButtons;
	
};

