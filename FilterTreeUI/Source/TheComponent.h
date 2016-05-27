/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.0.2

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
  */

#ifndef __JUCE_HEADER_B3C2321283F715E8__
#define __JUCE_HEADER_B3C2321283F715E8__

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
//[/Headers]

//==============================================================================
/**
																	//[Comments]
																	An auto-generated component, created by the Introjucer.

																	Describe your class and how it works here!
																	//[/Comments]
																	*/
class TheComponent : public Component,
	public ButtonListener
{
public:
	//==============================================================================
	TheComponent();
	~TheComponent();

	//==============================================================================
	//[UserMethods]     -- You can add your own custom methods in this section.
	//[/UserMethods]

	void paint(Graphics& g);
	void resized();
	void buttonClicked(Button* buttonThatWasClicked);

private:
	//[UserVariables]   -- You can add your own custom variables in this section.
	//[/UserVariables]

	//==============================================================================
	ScopedPointer<TreeView> treeView;
	ScopedPointer<TextButton> buttonAddInput;
	ScopedPointer<TextButton> buttonAddFilter;
	ScopedPointer<TextButton> buttonDeleteBlock;
	ScopedPointer<TextButton> buttonAddOutput;
	ScopedPointer<Component> component;
	ScopedPointer<Component> component2;
	ScopedPointer<ImageButton> imageButton;
	ScopedPointer<Component> component3;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TheComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B3C2321283F715E8__
