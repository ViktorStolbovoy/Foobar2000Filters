/*
  ==============================================================================

  This file was auto-generated!

  ==============================================================================
  */

#include <Common.h>
#include <SliderGroup.h>
#include <FilterControls.h>
#include "FilterChainItem.h"
#include "PanelInputChannels.h"
#include "PanelOutputChannels.h"
#include "PanelVOutputChannels.h"
#include "PanelFilterBlock.h"
#include "PanelFilter.h"
#include "PanelMixer.h"
#include <GainChart.h>
#include <PhaseChart.h>
#include <StepResponseChart.h>
#include <GroupDelayChart.h>
#include "MainComponent.h"
#include "ProcessorHelpers.h"
#include "Processor.h"


//==============================================================================
MainContentComponent::MainContentComponent() : TopLevelResizableLayout(this)
{
	const int w = 1024;
	const int h = 720;
	setSize(w, h);
	setMinimumSize(1024, 720);
	//m_listeners.add(this);
	int x = X0;
	int y = 4;
	{
		auto c = new TreeView();

		c->setBounds(x, y, 450, h - BUTTON_HEIGHT - 3 * GAP - PanelInputChannels::INPUT_PANEL_HEIGHT);
		c->setColour(TreeView::backgroundColourId, Colours::white);
		c->setColour(TreeView::linesColourId, Colours::navy);
		c->setDefaultOpenness(true);
		//c->setOpenCloseButtonsVisible(false);

		addToLayout(c,  Point<int>(0, 0), Point<int>(33, 100));
		addAndMakeVisible(c);

		m_treeView = c;

		y = c->getBounds().getBottom() + GAP;
	}

	
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH_D, BUTTON_HEIGHT);
		c->setButtonText("New Input");
		addToLayout(c, anchorBottomLeft);
		addAndMakeVisible(c);
		m_buttonAddInput = c;
		c->addListener(this);

		x = c->getBounds().getRight() + GAP;
	}

	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH_D, BUTTON_HEIGHT);
		c->setButtonText("New Filter Definition");
		addToLayout(c, anchorBottomLeft);
		addAndMakeVisible(c);
		m_buttonAddVInput = c;
		c->addListener(this);

		x = c->getBounds().getRight() + GAP;
	}

	
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH_D, BUTTON_HEIGHT);
		c->setButtonText("Add Filter");
		addToLayout(c, anchorBottomLeft);
		addAndMakeVisible(c);
		m_buttonAddFilter = c;
		c->addListener(this);

		x = c->getBounds().getRight() + GAP;
	}
	
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH_D, BUTTON_HEIGHT);
		c->setButtonText("Add Filter Reference");
		addToLayout(c, anchorBottomLeft);
		addAndMakeVisible(c);
		m_buttonAddBlock = c;
		c->addListener(this);

		x = c->getBounds().getRight() + GAP;
	}
		
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH_D, BUTTON_HEIGHT);
		c->setButtonText("Add Output");
		addToLayout(c, anchorBottomLeft);
		addAndMakeVisible(c);
		m_buttonAddOutput = c;
		c->addListener(this);

		x = c->getBounds().getRight() + GAP;
	}

	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH_D, BUTTON_HEIGHT);
		c->setButtonText("Add Mixer Bus");
		addToLayout(c, anchorBottomLeft);
		addAndMakeVisible(c);
		m_buttonAddVOutput = c;
		c->addListener(this);

		x = c->getBounds().getRight() + GAP;
	}

	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH_D, BUTTON_HEIGHT);
		c->setButtonText("Add Mixer");
		addToLayout(c, anchorBottomLeft);
		addAndMakeVisible(c);
		m_buttonAddMixer = c;
		c->addListener(this);

		x = c->getBounds().getRight() + 4 * GAP;
	}

	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH_D, BUTTON_HEIGHT);
		c->setButtonText("Delete");
		c->setColour(TextButton::textColourOffId, Colours::red);
		addToLayout(c, anchorBottomLeft);
		addAndMakeVisible(c);
		m_buttonDeleteSelected = c;
		c->addListener(this);

		y = c->getBounds().getBottom() + GAP;
	}

	x = X0;
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
		c->setButtonText("Set");
		addToLayout(c, anchorBottomLeft);
		addChildComponent(c);
		m_buttonSetParams = c;
		c->addListener(this);

		y = c->getBounds().getBottom() + GAP;
	}

	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
		c->setButtonText("Reset");
		addToLayout(c, anchorBottomLeft);
		addChildComponent(c);
		m_buttonResetParams = c;
		c->addListener(this);
	}

	y = m_buttonSetParams->getY();
	x = w - BUTTON_WIDTH - GAP;
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y , BUTTON_WIDTH, BUTTON_HEIGHT);
		c->setButtonText("Save");
		addToLayout(c, anchorBottomRight);
		addAndMakeVisible(c);
		m_buttonSave = c;
		c->addListener(this);

		y = c->getBounds().getBottom() + GAP;
	}

	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
		c->setButtonText("Load");
		addToLayout(c, anchorBottomRight);
		addAndMakeVisible(c);
		m_buttonLoad = c;
		c->addListener(this);

		y = c->getBounds().getBottom() + GAP;
	}
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
		c->setButtonText("OK");
		addToLayout(c, anchorBottomRight);
		addAndMakeVisible(c);
		m_buttonOk = c;
		c->addListener(this);

		y = c->getBounds().getBottom() + GAP;
	}
	{
		TextButton* c = new TextButton();
		c->setBounds(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
		c->setButtonText("Cancel");
		addToLayout(c, anchorBottomRight);
		addAndMakeVisible(c);
		m_buttonCancel = c;
		c->addListener(this);

		y = c->getBounds().getBottom() + GAP;
	}


	x = m_treeView->getRight() + GAP;
	///// CHARTS -------------------------------------
	const bool useLogScale = true;
	int hCharts = m_treeView->getHeight();
	int wCharts = w - x - GAP;

	{
		auto c = m_chartGain = new GainChart(m_listeners, useLogScale);
		c->setBounds(x, m_treeView->getY(), wCharts, hCharts / 2 - GAP);
		addToLayout(c, Point<int>(33, 0), Point<int>(100, 50));
		//addToLayout(c, Point<int>(0, 0), Point<int>(33, 100));
		addAndMakeVisible(c);
		y = c->getBottom() + GAP;
	}

	int smallChartHeight = (hCharts - GAP) / 4;
	{
		auto c = new PhaseChart(m_listeners, useLogScale);
		c->setBounds(x, y, wCharts, smallChartHeight);
		addToLayout(c, Point<int>(33, 50), Point<int>(100, 75));
		addAndMakeVisible(c);
		m_chartPhase = c;

		y = c->getBottom() + GAP;
	}

	int smallChartWitdth = (wCharts - GAP) / 2;
	{				   
		auto c = new GroupDelayChart(m_listeners, useLogScale);
		c->setBounds(x, y, smallChartWitdth, smallChartHeight);
		addToLayout(c, Point<int>(33, 75), Point<int>(66, 100));
		addAndMakeVisible(c);
		m_chartGroupDelay = c;

		x = c->getRight() + GAP;
	}

	{
		auto c = new StepResponseChart(m_listeners);
		c->setBounds(x, y, smallChartWitdth, smallChartHeight);
		addToLayout(c, Point<int>(66, 75), Point<int>(100, 100));
		addAndMakeVisible(c);
		m_chartStepResponse = c;
	}

	SelectedTreeViewItemChanged(nullptr);
	recalculateLayout();
	activateLayout();

	rootTVItem = new RootItem(this);
	m_treeView->setRootItem(rootTVItem);
	m_treeView->setRootItemVisible(false);
}

MainContentComponent::~MainContentComponent()
{
	m_treeView->deleteRootItem();
	deleteAllChildren();
}


void MainContentComponent::SelectedTreeViewItemChanged(ChainItemBase *item)
{
	//m_buttonAddInput->setEnabled(true);
	auto ci = dynamic_cast<ConfigItemBase*>(item);
	BuildingElementType selectedType = (ci) ? ci->ElementType : None;
	switch (selectedType)
	{
		case Input:
		case Filter:
		case Block:
		case Mixer:
		{
			auto input = ChainItemBase::FindItem<InputItem>(item);
			bool canAdd = (input == nullptr /*starts with a mixer with unassigned inputs*/) || (!input->IsVirtual) || (item->getNumSubItems() == 0);
			m_buttonAddBlock->setEnabled(canAdd);
			m_buttonAddFilter->setEnabled(canAdd);
			m_buttonDeleteSelected->setEnabled(canAdd);
			bool canAddOut = !input || !input->IsVirtual;
			m_buttonAddOutput->setEnabled(canAddOut);
			m_buttonAddVOutput->setEnabled(canAddOut);
			break;
		}
		case VirtualOutput:
		case Output:
		{
			m_buttonAddBlock->setEnabled(false);
			m_buttonAddFilter->setEnabled(false);
			m_buttonAddOutput->setEnabled(false);
			m_buttonDeleteSelected->setEnabled(true);
			m_buttonAddVOutput->setEnabled(false);
			break;
		}
		default:
		{
			m_buttonAddBlock->setEnabled(false);
			m_buttonAddFilter->setEnabled(false);
			m_buttonAddOutput->setEnabled(false);
			m_buttonDeleteSelected->setEnabled(false);
			m_buttonAddVOutput->setEnabled(false);
			break;
		}
	}

	if (m_itemSetupPannel)
	{
		this->removeChildComponent(m_itemSetupPannel);
		//this->removeFromLayout(m_itemSetupPannel);
		delete m_itemSetupPannel; 
	 	m_itemSetupPannel = nullptr;
	}
	switch (selectedType)
	{
		case Input:
		{	//No settings for filter definition
			if (!((InputItem*)item)->IsVirtual) m_itemSetupPannel = new PanelInputChannels((InputItem*)item, GetUsedInputsMask()); break;
		}
		case Filter:
		{
			m_itemSetupPannel = new PanelFilter((FilterItem *)item); break;
		}
		case Block:
		{
			auto vInps = GetVirtualInputs();
			m_itemSetupPannel = new PanelFilterBlock((FilterBlockItem *)item, *vInps); 
			delete vInps;
			break;
		}
		case Output:
		{
			auto out = (OutputItem*)item;
			bool isAlreadyGraphing = IsGraphingParallelOutput(out);
			m_itemSetupPannel = new PanelOutputChannels(out, GetUsedOutputsMask(), !isAlreadyGraphing); break;
			break;
		}
		case VirtualOutput:
		{
			m_itemSetupPannel = new PanelVOutputChannels((VirtualOutputItem*)item); break;
			break;
		}
		case Mixer:
		{
			m_itemSetupPannel = new PanelMixer((MixerItem*)item, VirtualOutputItem::GetAllUsedNumbers(m_treeView->getRootItem())); break;
			break;
		}
	}

	if (m_itemSetupPannel)
	{
		int y = m_buttonDeleteSelected->getBounds().getBottom() + GAP;
		int panelWidth = m_itemSetupPannel->GetWidth();
		m_itemSetupPannel->setBounds(X0, y, panelWidth, PanelItemControl::INPUT_PANEL_HEIGHT);
		addToLayout(m_itemSetupPannel, anchorBottomLeft);

		addAndMakeVisible(m_itemSetupPannel);

		int x = m_itemSetupPannel->getBounds().getRight() + GAP;
		m_buttonSetParams->setBounds(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
		y += BUTTON_HEIGHT + GAP;
		m_buttonResetParams->setBounds(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
		m_buttonSetParams->setVisible(true);
		m_buttonResetParams->setVisible(true);
		
		updateLayout();
	}
	else
	{
		m_buttonResetParams->setVisible(false);
		m_buttonSetParams->setVisible(false);
	}
	RedrawGraphs();
}

uint32 MainContentComponent::GetUsedInputsMask()
{
	auto ri = (RootItem *) m_treeView->getRootItem();
	jassert(ri);

	uint32 mask = 0;
	for (int i = 0; i < ri->getNumSubItems(); i++)
	{
		auto ii = dynamic_cast<InputItem*>(ri->getSubItem(i));
		if (ii && !(ii->IsVirtual))	
		{
			mask |= ii->InputMask;
		}
	}

	return mask;
}

uint32 MainContentComponent::GetUsedOutputsMask()
{
	auto ri = m_treeView->getRootItem();
	jassert(ri);
	return GetUsedOutputsMask(ri);
}

bool MainContentComponent::IsGraphingParallelOutput(const OutputItem * out)
{
	auto p = out->getParentItem();
	jassert(p);
	for (int i = 0; i < p->getNumSubItems(); i++)
	{
		auto child = dynamic_cast<OutputItem *>( p->getSubItem(i));
		if (child && child != out)
		{
			if (child->ShowInGraphs) return true;
		}
	}
	return false;
}


uint32 MainContentComponent::GetUsedOutputsMask(TreeViewItem *item)
{
	uint32 mask = 0;
	int num = item->getNumSubItems();
	for (int i = 0; i < num; i++)
	{
		auto subItem = item->getSubItem(i);
		auto output = dynamic_cast<OutputItem*>(subItem);
		if (output)
		{
			mask |= ConfigItemBase::ArrayToChannelMask(output->OutputChannels, output->OutputChannelsLenght);
		}
		else
		{
			mask |= GetUsedOutputsMask(subItem);
		}
	}
	return mask;
}

Colour MainContentComponent::GetOutputColour(TreeViewItem *parent)
{
	static Colour graphColours[MAX_NUM_CHANNELS] =
	{
		Colours::red,			Colours::green,			Colours::blue,			Colours::orange,		Colours::cyan,		Colours::brown,		Colours::navy,			Colours::darkred, 
		Colours::darkmagenta,	Colours::magenta,		Colours::lime,			Colours::yellowgreen,	Colours::pink,		Colours::firebrick,	Colours::cornflowerblue,Colours::olive, 
		Colours::azure,			Colours::coral,			Colours::darkgreen,		Colours::darkblue,		Colours::darkorange,Colours::darkcyan,	Colours::darkkhaki,		Colours::deepskyblue,
		Colours::lightgreen,	Colours::darkolivegreen,Colours::mediumaquamarine,Colours::honeydew,	Colours::lightblue,	Colours::lightpink,	Colours::orchid,		Colours::tan
	};

	for (int i = 0; i < parent->getNumSubItems(); i++)
	{
		auto anotherOut = dynamic_cast<OutputItem*> (parent->getSubItem(i));
		if (anotherOut)
		{
			return anotherOut->GetLineColor();
		}
	}

	Colour usedColors[MAX_NUM_CHANNELS];
	int numUsedColors = 0;
	auto ri = m_treeView->getRootItem();
	GetUsedColors(ri, usedColors, &numUsedColors);
	for (int i = 0; i < MAX_NUM_CHANNELS; i++)
	{
		int j = 0;
		for (; j < numUsedColors; j++)
		{
			if (usedColors[j] == graphColours[i]) break;
		}
		if (j == numUsedColors) return graphColours[i];
	}
	jassertfalse;
	return Colours::beige;
}

void MainContentComponent::GetUsedColors(TreeViewItem *item, Colour *usedColors, int * numUsedColors)
{
	int num = item->getNumSubItems();
	for (int i = 0; i < num; i++)
	{
		auto subItem = item->getSubItem(i);
		auto output = dynamic_cast<OutputItem*>(subItem);
		if (output)
		{
			usedColors[*numUsedColors] = output->GetLineColor();
			(*numUsedColors)++;
		}
		else
		{
			GetUsedColors(subItem, usedColors, numUsedColors);
		}
	}
}


std::vector<int> * MainContentComponent::GetVirtualInputs()
{
	auto res = new std::vector<int>();

	auto root = m_treeView->getRootItem();
	assert(root);
	for (int i = 0; i < root->getNumSubItems(); i++)
	{
		auto input = dynamic_cast<InputItem*>(root->getSubItem(i));
		if (input && input->IsVirtual)
		{
			res->push_back(input->InputMask);
		}
	}

	std::sort(res->begin(), res->end());

	return res;
}

int MainContentComponent::GetNextVitualInput()
{

	auto ri = (RootItem *)m_treeView->getRootItem();
	jassert(ri);

	uint32 nextId = 1;
	bool isFound = false;
	while (!isFound)
	{
		isFound = true;
		for (int i = 0; i < ri->getNumSubItems(); i++)
		{
			auto ii = dynamic_cast<InputItem*>(ri->getSubItem(i));
			if (ii && ii->IsVirtual)
			{
				if (ii->InputMask == nextId)
				{
					nextId++;
					isFound = false;
					break;
				}
			}
		}
	}

	return nextId;
}

uint32 MainContentComponent::GetNextVirtualOutput()
{
	auto ri = (RootItem *)m_treeView->getRootItem();
	jassert(ri);
	auto used = VirtualOutputItem::GetAllUsedNumbers(ri);
	for (uint32 i = 1; i < INT_MAX; i++)
	{
		bool isFound = false;
		for each (uint32 n in *used)
		{
			if (n == i)
			{
				isFound = true;
				break;
			}
		}
		if (!isFound)
		{
			delete used;
			return i;
		}
	}

	delete used;
	jassertfalse;
	return 0;
}



void MainContentComponent::buttonClicked(Button *ctrl)
{
	if (ctrl == m_buttonSave)
	{
		SaveGraph();
	}
	else if (ctrl == m_buttonLoad)
	{
		LoadGraph();
	}
	else if (ctrl == m_buttonAddInput || ctrl == m_buttonAddVInput || ctrl == m_buttonAddMixer)
	{
		ChainItem * item;
		if (ctrl == m_buttonAddMixer)
		{
			item = new MixerItem(this);
		}
		else
		{
			InputItem *ii;
			if (ctrl == m_buttonAddVInput)
			{
				ii = new InputItem(this);
				ii->IsVirtual = true;
				ii->InputMask = GetNextVitualInput();

			}
			else
			{
				ii = new InputItem(this); 
				ii->IsVirtual = false;
				ii->InputMask = 0;
			}
			item = ii;
		}
		auto root = m_treeView->getRootItem();
		jassert(root);
		root->addSubItem(item);
		item->setSelected(true, true);
	}
	else if (ctrl == m_buttonDeleteSelected)
	{
		auto tvi = dynamic_cast<ChainItemBase*>(m_treeView->getSelectedItem(0));
		if (tvi)
		{
			tvi->setHightlighted(true, true);
			if (AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
				"Deleting Components",
				String("All components starting from the ") + tvi->GetLabel() + (" will be deleted recursively. Proceed?")))
			{
				auto parent = tvi->getParentItem();
				jassert(parent);
				parent->removeSubItem(tvi->getIndexInParent(), true);
				SelectedTreeViewItemChanged(nullptr);
			}
			else
			{
				tvi->setHightlighted(false, true);
			}
		}
	}
	else if (ctrl == m_buttonSetParams)
	{
		if (m_itemSetupPannel) m_itemSetupPannel->SetControl();
		RedrawGraphs();
	}
	else if (ctrl == m_buttonResetParams)
	{
		if (m_itemSetupPannel) m_itemSetupPannel->ResetFromControl();
	}
	else if (ctrl == m_buttonOk)
	{
		if (auto dw = findParentComponentOfClass<DialogWindow>()) dw->exitModalState(1);
	}
	else if (ctrl == m_buttonCancel)
	{
		if (auto dw = findParentComponentOfClass<DialogWindow>()) dw->exitModalState(-1);
	}
	else 
	{
		auto tvi = dynamic_cast<ChainItemBase*>(m_treeView->getSelectedItem(0));
		BuildingElementType elType = tvi == nullptr ? None : ((ConfigItemBase*)tvi)->ElementType;
		if (elType != None && elType != Output && elType != VirtualOutput)
		{
			ChainItem * item;
			if (ctrl == m_buttonAddOutput)
			{
				Colour c = GetOutputColour(tvi);//Need to call this one before new output added
				item = new OutputItem(this);
				tvi->addSubItem(item);
				item->SetLineColor(c);
				item->treeHasChanged();
			}
			else 
			{
				if (ctrl == m_buttonAddBlock)
				{
					item = new FilterBlockItem(this);
			
				}
				else if (ctrl == m_buttonAddVOutput)
				{
					auto newItem = new VirtualOutputItem(this);
					newItem->Number = GetNextVirtualOutput();
					item = newItem;
				}
				else //if(ctrl == m_buttonAddFilter)
				{
					item = new FilterItem(this);
				}
				tvi->addSubItem(item);
			}
			
			tvi->setOpen(true);
			item->setSelected(true, true);
		}
	}
	
}


void MainContentComponent::SaveGraph()
{
	const String extension = ".flcfg";
	WildcardFileFilter wildcardFilter(String("*") + extension, String::empty, "Filter config files");
	FileBrowserComponent browser(FileBrowserComponent::FileChooserFlags::canSelectFiles | FileBrowserComponent::FileChooserFlags::saveMode | FileBrowserComponent::FileChooserFlags::warnAboutOverwriting, // | FileBrowserComponent::FileChooserFlags::useTreeView,
		File::nonexistent,
		&wildcardFilter,
		nullptr);
	FileChooserDialogBox dialogBox("Select filter configuration",
		"Please choose file to save current filter configuration",
		browser,
		true,
		Colours::lightgrey);
	if (dialogBox.show())
	{
		File selectedFile = browser.getSelectedFile(0);
		String ext = selectedFile.getFileExtension();

		File selectedFileWithCorrectExtension(selectedFile.getParentDirectory().getFullPathName() + String("\\") + selectedFile.getFileNameWithoutExtension() + extension);

		if (selectedFile.hasWriteAccess())
		{
			if (selectedFileWithCorrectExtension.exists()) selectedFileWithCorrectExtension.deleteFile();

			auto stream = selectedFileWithCorrectExtension.createOutputStream();
			if (!stream->openedOk())
			{
				AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "File write error", "Can't open wrtie stream");
				delete stream;
				return;
			}
			stream->setPosition(0);

			auto root = m_treeView->getRootItem();
			jassert(root);
			int id = 1;
			ChainItemBase::BuildSerializationIds(root, &id);
			for (int i = 0; i < root->getNumSubItems(); i++)
			{
				SaveTreeViewItem(stream, root->getSubItem(i));
			}

			stream->truncate();
			stream->flush();

			delete stream;
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "File write error", "File is read-only");
		}
		
	}
}

void MainContentComponent::LoadConfig(std::vector<ConfigItemBase*> &config)
{
	std::vector<ChainItem*> items;
	for (size_t i = 0; i < config.size(); i ++)
	{
		auto c = config[i];
		ChainItem *item;
		switch (c->ElementType)
		{
			case Input: item = new InputItem((InputConfigItem*)c, this); break;
			case Filter: item = new FilterItem((FilterConfigItem*)c, this); break;
			case Block: item = new FilterBlockItem((FilterBlockConfigItem*) c, this); break;

			case Output: item = new OutputItem((OutputConfigItem *)c, this); break;
			case Mixer: item = new MixerItem((MixerConfigItem*)c, this); break;
			case VirtualOutput: item = new VirtualOutputItem((VirtualOutputConfigItem*) c, this); break;
			default: throw std::logic_error((String("Invalid element type: ") + String(c->ElementType)).toStdString());
		}
		items.push_back(item);
	}

	RestoreItems(items);
	RedrawGraphs();
}

void MainContentComponent::ExportConfig(std::vector<ConfigItemBase*> &config)
{
	auto root = m_treeView->getRootItem();
	jassert(root);
	int id = 1;
	ChainItemBase::BuildSerializationIds(root, &id);
	for (int i = 0; i < root->getNumSubItems(); i++)
	{
		AddConfigSubTree(config, root->getSubItem(i));
	}
}

void MainContentComponent::AddConfigSubTree(std::vector<ConfigItemBase*> &config, TreeViewItem *tvi)
{
	auto item = dynamic_cast<ConfigItemBase*>(tvi);
	if (item)
	{
		config.push_back(item);
	}
		
	for (int i = 0; i < tvi->getNumSubItems(); i++)
	{
		AddConfigSubTree(config, tvi->getSubItem(i));
	}
}

void MainContentComponent::LoadGraph()
{
	WildcardFileFilter wildcardFilter("*.flcfg", String::empty, "Filter config files");
	FileBrowserComponent browser(FileBrowserComponent::FileChooserFlags::canSelectFiles | FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::warnAboutOverwriting | FileBrowserComponent::FileChooserFlags::useTreeView,
		File::nonexistent,
		&wildcardFilter,
		nullptr);
	FileChooserDialogBox dialogBox("Select filter configuration",
		"Please choose existing filter configuration to load",
		browser,
		false,
		Colours::lightgrey);
	if (dialogBox.show())
	{
		File selectedFile = browser.getSelectedFile(0);
		auto stream = selectedFile.createInputStream();
		LoadTreeViewItem(stream);
		delete stream;
		RedrawGraphs();
	}
}

void MainContentComponent::SaveTreeViewItem(FileOutputStream *stream, TreeViewItem *tvi)
{
	auto ci = dynamic_cast<ConfigItemBase*>(tvi);
	if (ci)
	{
		size_t sz;
		uint8 * data = ci->ToBytes(&sz);
		stream->write(data, sz);
		delete[] data;
	}
	for (int i = 0; i < tvi->getNumSubItems(); i++)
	{
		SaveTreeViewItem(stream, tvi->getSubItem(i));
	}
}


void MainContentComponent::LoadTreeViewItem(FileInputStream *stream)
{
	int len = (int)stream->getTotalLength(); //Shouldn't be too long
	uint8 * data = new uint8[len];
	len = stream->read(data, len);
	
	uint8 * curData = data;
	uint8 * pastLast = data + len;

	std::vector<ChainItem*> items;
	while (curData < pastLast)
	{
		ChainItem *item;
		BuildingElementType elT = (BuildingElementType)*curData;
		curData++;
		switch (elT)
		{
			case Input: item = new InputItem(this); break;
			case Filter: item = new FilterItem(this); break;
			case Block: item = new FilterBlockItem(this); break;

			case Output: item = new OutputItem(this); break;
			case Mixer: item = new MixerItem(this); break;
			case VirtualOutput: item = new VirtualOutputItem(this); break;
			default: throw std::logic_error((String("Invalid element type: ") + String(elT)).toStdString());
		}
		auto bsi = dynamic_cast<BinarySerializable*>(item);
		jassert(bsi);
		bsi->InitFromBytes(&curData, pastLast);
		items.push_back(item);
	}

	RestoreItems(items);
}

void MainContentComponent::RestoreItems(std::vector<ChainItem*> &items)
{
	//reset tree
	m_treeView->deleteRootItem();
	auto root = new RootItem(this);
	m_treeView->setRootItem(root);
	m_treeView->setRootItemVisible(false);
	jassert(root);

	root->Id = 1; //always 1
	root->ParentId = 0; //always 0

	RestoreSubItems(root, items);
	root->treeHasChanged();
}

void MainContentComponent::RestoreSubItems(TreeViewItem *parent, std::vector<ChainItem*> &items)
{
	auto cparent = dynamic_cast<ConfigItemBase*>(parent);
	if (!cparent) throw std::logic_error("Invalid parent element");
	uint32 parentId = cparent->Id;

	for (uint32 i = 0; i < items.size(); i++)
	{
		auto item = items[i];
		auto citem = dynamic_cast<ConfigItemBase*>(item);

		if (citem && citem->ParentId == parentId)
		{
			parent->addSubItem(item);
			items[i] = nullptr;
		}
	}

	for (int i = 0; i < parent->getNumSubItems(); i++)
	{
		RestoreSubItems(parent->getSubItem(i), items);
	}
}

void MainContentComponent::RedrawGraphs()
{
	double const fs = 44100;
	std::vector<ConfigItemBase* > config;
	//We call this from resize() method too - can be unitialized
	if (!m_treeView) return;
	auto ri = m_treeView->getRootItem();
	if (!ri) return;
	int id = 1;
	ChainItemBase::BuildSerializationIds(ri, &id);
	FlattenTreeView(config, ri);
	Processor p(config);

	auto oldFilterData = m_filterData;

	p.Setup(fs, 0xFFFFFFFF);
	m_filterData = nullptr;
	m_filterData = p.CreateFilterData((int)fs, fs);
	
	Dsp::Filter * f = nullptr;
	auto fc = dynamic_cast<FilterConfigItem*>(m_treeView->getSelectedItem(0));
	if (fc)
	{
		f = fc->Config.createFilter(fs, 1);
		if (f)
		{
			FilterData fd(f, Colours::black);
			m_filterData->push_back(fd);
		}
	} 

	m_listeners.call(&FilterListener::onFilterChanged, *m_filterData);

	if (oldFilterData)
	{
		for (size_t i = 0; i < oldFilterData->size(); i++)
		{
			delete oldFilterData->at(i).Filter;
		}

		delete oldFilterData;
	}
}


void MainContentComponent::FlattenTreeView(std::vector<ConfigItemBase* > &config, TreeViewItem *tvi)
{
	auto ci = dynamic_cast<ConfigItemBase*>(tvi);
	if (ci)
	{
		config.push_back(ci);
	}
	for (int i = 0; i < tvi->getNumSubItems(); i++)
	{
		FlattenTreeView(config, tvi->getSubItem(i));
	}
}

void MainContentComponent::resized() 
{
	RedrawGraphs();
}


/*void MainContentComponent::paint (Graphics& g)
{
g.fillAll (Colour (0xff001F36));

g.setFont (Font (16.0f));
g.setColour (Colours::white);
g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}*/


