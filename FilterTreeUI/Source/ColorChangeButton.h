#pragma once
class ColourChangeButton : public TextButton,
	public ChangeListener
{
public:
	ColourChangeButton() : TextButton("")
	{
		setSize(24, 24);
		changeWidthToFitText();
	}

	void clicked() override
	{
		ColourSelector* colourSelector = new ColourSelector(ColourSelector::showColourAtTop | ColourSelector::showSliders | ColourSelector::showColourspace);
		colourSelector->setName("background");
		
		colourSelector->setCurrentColour(findColour(TextButton::buttonColourId));
		colourSelector->addChangeListener(this);
		colourSelector->setColour(ColourSelector::backgroundColourId, Colours::transparentBlack);
		colourSelector->setSize(300, 400);

		CallOutBox::launchAsynchronously(colourSelector, getScreenBounds(), nullptr);
	}

	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		if (ColourSelector* cs = dynamic_cast <ColourSelector*> (source))
		{
			setColour(TextButton::buttonColourId, cs->getCurrentColour());
		}
	}
};
