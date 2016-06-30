/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vincent Falco

Official project location:
http://code.google.com/p/dspfilterscpp/

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vincent Falco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*******************************************************************************/

#include "Common.h"
#include "SliderGroup.h"

SliderGroup::Model::~Model ()
{
}

SliderGroup::SliderGroup(Model* model, const int width)
: ResizableLayout (this)
, m_model (model)
, m_label (new Label)
, m_slider (new Slider)
//VS:, m_slider(new CustomSlider)
, m_text (new TextEditor)
, m_labelUnit(new Label)
{
  //vs:const int w = 30;
  

  m_label->setFont (14);

  BorderSize<int> bs(0);
    
  m_label->setBorderSize (bs);
  m_label->setText(m_model->getName(), NotificationType::sendNotificationSync);
  m_label->setJustificationType (Justification::centred);
  m_label->setBounds(0, 2, width, int(ceil(m_label->getFont().getHeight())));
  addToLayout (m_label, anchorTopLeft, anchorTopRight);
  addAndMakeVisible (m_label);

  const int h = 40;

  m_slider->setRange (0, 1);
  m_slider->setVelocityBasedMode (false);
  //VS:m_slider->setSliderStyle (Slider::RotaryVerticalDrag); - it is beautifull but VERY hard to use
  m_slider->setSliderStyle(Slider::LinearHorizontal); 
  m_slider->setTextBoxStyle (Slider::NoTextBox, false, 0, 0); //It'll show 0..1 values
  //m_slider->setRotaryParameters (float_Pi * 1.2f, float_Pi * 2.8f, false);
  m_slider->setBounds(0, m_label->getBounds().getBottom() + 2, width, h);
  m_slider->addListener (this);
  m_slider->setValue(m_model->getControlValue(), NotificationType::dontSendNotification);
  addToLayout (m_slider, anchorTopLeft, anchorBottomRight);
  addAndMakeVisible (m_slider);

  const int y = m_slider->getBounds().getBottom() - 8;
  const int hEdit = int(ceil(m_label->getFont().getHeight()));
  m_text->setFont (15);
  //m_text->setBorderSize (bs);
  m_text->setInputRestrictions (10, "0123456789.");
  m_text->setBounds(width / 2 - 30, y, 60, hEdit);
  m_text->addListener(this);
  addToLayout (m_text, anchorBottomLeft, anchorBottomRight);
  addAndMakeVisible (m_text);
  
  m_labelUnit->setFont(14);
  m_labelUnit->setText(m_model->getUnits(), NotificationType::sendNotificationSync);
  m_labelUnit->setJustificationType(Justification::topLeft);
  m_labelUnit->setBounds(width / 2 + 40, y, 50, hEdit);
  
  addToLayout(m_labelUnit, anchorBottomLeft, anchorBottomRight);
  addAndMakeVisible(m_labelUnit);

  updateText();
  setSize(width, m_text->getBounds().getBottom() + 4);

  activateLayout ();
}

SliderGroup::~SliderGroup ()
{
  m_slider->removeListener (this);
}

void SliderGroup::sliderValueChanged (Slider* ctrl)
{
  m_model->setControlValue (ctrl->getValue ());
  updateText();
}

void SliderGroup::textEditorReturnKeyPressed(TextEditor& editor)
{
	double val = editor.getText().getDoubleValue();
	m_model->setNativeValue(val);
	m_slider->setValue(m_model->getControlValue(), NotificationType::dontSendNotification);
}

/** Called when the user presses the escape key. */
void SliderGroup::textEditorEscapeKeyPressed(TextEditor& editor)
{
	sliderValueChanged(m_slider);
}

/** Called when the text editor loses focus. */
void SliderGroup::textEditorFocusLost(TextEditor& editor)
{
	textEditorReturnKeyPressed(editor);
}


void SliderGroup::updateText ()
{
	m_text->setText(m_model->getNativeValueAsText(), NotificationType::dontSendNotification);
}
