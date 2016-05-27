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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "TheComponent.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
TheComponent::TheComponent()
{
	//[Constructor_pre] You can add your own custom stuff here..
	//[/Constructor_pre]

	addAndMakeVisible(treeView = new TreeView("new treeview"));
	treeView->setColour(TreeView::backgroundColourId, Colours::grey);

	addAndMakeVisible(buttonAddInput = new TextButton("AddInput"));
	buttonAddInput->setButtonText(TRANS("Add Input"));
	buttonAddInput->addListener(this);

	addAndMakeVisible(buttonAddFilter = new TextButton("AddFilter"));
	buttonAddFilter->setButtonText(TRANS("Add Filter"));
	buttonAddFilter->addListener(this);

	addAndMakeVisible(buttonDeleteBlock = new TextButton("DeleteBlock"));
	buttonDeleteBlock->setButtonText(TRANS("Delete"));
	buttonDeleteBlock->addListener(this);
	buttonDeleteBlock->setColour(TextButton::textColourOffId, Colour(0xffff0303));

	addAndMakeVisible(buttonAddOutput = new TextButton("AddOutput"));
	buttonAddOutput->setButtonText(TRANS("Add Output"));
	buttonAddOutput->addListener(this);

	addAndMakeVisible(component = new Component());
	addAndMakeVisible(component2 = new Component());
	addAndMakeVisible(imageButton = new ImageButton("new button"));
	imageButton->addListener(this);

	imageButton->setImages(false, true, true,
		Image(), 1.000f, Colour(0x00000000),
		Image(), 1.000f, Colour(0x00000000),
		Image(), 1.000f, Colour(0x00000000));
	addAndMakeVisible(component3 = new Component());
	component3->setName("new component");

	//[UserPreSize]
	//[/UserPreSize]

	setSize(600, 400);

	//[Constructor] You can add your own custom stuff here..
	//[/Constructor]
}

TheComponent::~TheComponent()
{
	//[Destructor_pre]. You can add your own custom destruction code here..
	//[/Destructor_pre]

	treeView = nullptr;
	buttonAddInput = nullptr;
	buttonAddFilter = nullptr;
	buttonDeleteBlock = nullptr;
	buttonAddOutput = nullptr;
	component = nullptr;
	component2 = nullptr;
	imageButton = nullptr;
	component3 = nullptr;

	//[Destructor]. You can add your own custom destruction code here..
	//[/Destructor]
}

//==============================================================================
void TheComponent::paint(Graphics& g)
{
	//[UserPrePaint] Add your own custom painting code here..
	//[/UserPrePaint]

	g.fillAll(Colours::white);

	//[UserPaint] Add your own custom painting code here..
	//[/UserPaint]
}

void TheComponent::resized()
{
	//[UserPreResize] Add your own custom resize code here..
	//[/UserPreResize]

	treeView->setBounds(0, 0, 672, 584);
	buttonAddInput->setBounds(19, 595, 150, 24);
	buttonAddFilter->setBounds(187, 595, 150, 24);
	buttonDeleteBlock->setBounds(520, 595, 150, 24);
	buttonAddOutput->setBounds(352, 595, 150, 24);
	component->setBounds(680, 8, 712, 312);
	component2->setBounds(680, 328, 712, 248);
	imageButton->setBounds(929, 656, 150, 24);
	component3->setBounds(0, 632, 1392, 96);
	//[UserResized] Add your own custom resize handling here..
	//[/UserResized]
}

void TheComponent::buttonClicked(Button* buttonThatWasClicked)
{
	//[UserbuttonClicked_Pre]
	//[/UserbuttonClicked_Pre]

	if (buttonThatWasClicked == buttonAddInput)
	{
		//[UserButtonCode_buttonAddInput] -- add your button handler code here..
		//[/UserButtonCode_buttonAddInput]
	}
	else if (buttonThatWasClicked == buttonAddFilter)
	{
		//[UserButtonCode_buttonAddFilter] -- add your button handler code here..
		//[/UserButtonCode_buttonAddFilter]
	}
	else if (buttonThatWasClicked == buttonDeleteBlock)
	{
		//[UserButtonCode_buttonDeleteBlock] -- add your button handler code here..
		//[/UserButtonCode_buttonDeleteBlock]
	}
	else if (buttonThatWasClicked == buttonAddOutput)
	{
		//[UserButtonCode_buttonAddOutput] -- add your button handler code here..
		//[/UserButtonCode_buttonAddOutput]
	}
	else if (buttonThatWasClicked == imageButton)
	{
		//[UserButtonCode_imageButton] -- add your button handler code here..
		//[/UserButtonCode_imageButton]
	}

	//[UserbuttonClicked_Post]
	//[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Introjucer information section --

	This is where the Introjucer stores the metadata that describe this GUI layout, so
	make changes in here at your peril!

	BEGIN_JUCER_METADATA

	<JUCER_COMPONENT documentType="Component" className="TheComponent" componentName=""
	parentClasses="public Component" constructorParams="" variableInitialisers=""
	snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
	fixedSize="0" initialWidth="600" initialHeight="400">
	<BACKGROUND backgroundColour="ffffffff"/>
	<TREEVIEW name="new treeview" id="a194c5bc0528a6d3" memberName="treeView"
	virtualName="" explicitFocusOrder="0" pos="0 0 672 584" backgroundColour="ff808080"
	rootVisible="1" openByDefault="0"/>
	<TEXTBUTTON name="AddInput" id="f074dbb6be5bd3a3" memberName="buttonAddInput"
	virtualName="" explicitFocusOrder="0" pos="19 595 150 24" buttonText="Add Input"
	connectedEdges="0" needsCallback="1" radioGroupId="0"/>
	<TEXTBUTTON name="AddFilter" id="dd04e03e7627170e" memberName="buttonAddFilter"
	virtualName="" explicitFocusOrder="0" pos="187 595 150 24" buttonText="Add Filter"
	connectedEdges="0" needsCallback="1" radioGroupId="0"/>
	<TEXTBUTTON name="DeleteBlock" id="15ec2f2c486357b5" memberName="buttonDeleteBlock"
	virtualName="" explicitFocusOrder="0" pos="520 595 150 24" textColOn="ffff0303"
	buttonText="Delete" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
	<TEXTBUTTON name="AddOutput" id="9d5b079f49f00b02" memberName="buttonAddOutput"
	virtualName="" explicitFocusOrder="0" pos="352 595 150 24" buttonText="Add Output"
	connectedEdges="0" needsCallback="1" radioGroupId="0"/>
	<JUCERCOMP name="" id="4c4b0384d94a2efa" memberName="component" virtualName=""
	explicitFocusOrder="0" pos="680 8 712 312" sourceFile="" constructorParams=""/>
	<JUCERCOMP name="" id="40294a03118644d8" memberName="component2" virtualName=""
	explicitFocusOrder="0" pos="680 328 712 248" sourceFile="" constructorParams=""/>
	<IMAGEBUTTON name="new button" id="92b6c34d2dd5f516" memberName="imageButton"
	virtualName="" explicitFocusOrder="0" pos="929 656 150 24" buttonText="new button"
	connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
	resourceNormal="" opacityNormal="1" colourNormal="0" resourceOver=""
	opacityOver="1" colourOver="0" resourceDown="" opacityDown="1"
	colourDown="0"/>
	<GENERICCOMPONENT name="new component" id="a8f2df1cfd9f39ba" memberName="component3"
	virtualName="" explicitFocusOrder="0" pos="0 632 1392 96" class="Component"
	params=""/>
	</JUCER_COMPONENT>

	END_JUCER_METADATA
	*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]