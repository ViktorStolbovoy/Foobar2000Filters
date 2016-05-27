#pragma once

class PanelItemControl : public Component
{
public:
	virtual void SetControl() = 0;
	virtual void ResetFromControl() = 0;
	virtual int GetWidth();
	static const int INPUT_PANEL_HEIGHT = (24 + 4) * 4 + 4;
	static String ChannelNumberToAbbreviation(uint8 num);
	static int GetLabelWidth(Label * label);
protected:
	void paint(Graphics& g) override;
	int m_panelWidth;
	static const int GAP = 4;
	static const int BUTTON_HEIGHT = 24;
	static const int BUTTON_WIDTH = 40;
	static const int NUM_ROWS = 4;
	static const int Y0 = 4;
	static const int X0 = 4;
};


class PanelInputChannels : public PanelItemControl
	
{
public:
	PanelInputChannels(InputItem *input, int usedInputsMask);
	~PanelInputChannels();

	void SetControl() override;
	void ResetFromControl() override;
private:
	int m_usedInputMask;
	InputItem *m_input;
	ToggleButton *m_toggleButtons[MAX_NUM_CHANNELS];
	
};

