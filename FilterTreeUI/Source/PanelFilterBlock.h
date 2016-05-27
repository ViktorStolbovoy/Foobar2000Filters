#pragma once
class PanelFilterBlock : public PanelItemControl
	
{
public:
	PanelFilterBlock(FilterBlockItem *input, std::vector<int> & virtualInputs);
	~PanelFilterBlock();
	
	void SetControl() override;
	void ResetFromControl() override;
private:
	FilterBlockItem *m_block;
	ComboBox *m_selector;
	Label * m_label;
};

