/*
  ==============================================================================

  This file was auto-generated!

  ==============================================================================
  */

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <FilterControls.h>
#include <FilterListener.h>
#include <ResizableLayout.h>
#include "FilterChainItem.h"
#include "PanelInputChannels.h"
#include <GainChart.h>
#include <GroupDelayChart.h>
#include <PhaseChart.h>
#include <StepResponseChart.h>


//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
	*/
class MainContentComponent : 
	public Component, 
	public TopLevelResizableLayout,
	public ButtonListener,
	public TreeViewSelectionHandler,
	public DragAndDropContainer
{
public:
	//==============================================================================
	MainContentComponent();
	~MainContentComponent();

	void SelectedTreeViewItemChanged(ChainItemBase *item);
	// void paint (Graphics&);
	void LoadConfig(std::vector<ConfigItemBase*> &config);
	void ExportConfig(std::vector<ConfigItemBase*> &config);
	void resized() override;

private:
	void buttonClicked(Button *ctrl);
	
	static const int X0 = 4;
	static const int GAP = 4;
	static const int BUTTON_HEIGHT = 24;
	static const int BUTTON_WIDTH_D = 120;
	static const int BUTTON_WIDTH = 80;
private:
	TreeView *m_treeView = nullptr;

	TextButton *m_buttonAddInput	= nullptr;
	TextButton *m_buttonAddVInput	= nullptr;
	TextButton *m_buttonAddFilter	= nullptr;
	TextButton *m_buttonAddBlock	= nullptr;
	TextButton *m_buttonDeleteSelected = nullptr;
	TextButton *m_buttonAddOutput	= nullptr;
	TextButton *m_buttonAddVOutput = nullptr;
	TextButton *m_buttonAddMixer	= nullptr;
	TextButton *m_buttonResetParams	= nullptr;
	TextButton *m_buttonSetParams	= nullptr;

	TextButton *m_buttonLoad = nullptr;
	TextButton *m_buttonSave = nullptr;
	TextButton *m_buttonOk = nullptr;
	TextButton *m_buttonCancel = nullptr;

	FilterControls *m_filterControls= nullptr;
	ListenerList<FilterListener> m_listeners;
	TreeViewItem *rootTVItem		= nullptr;

	GainChart *m_chartGain;
	PhaseChart *m_chartPhase;
	GroupDelayChart *m_chartGroupDelay;
	StepResponseChart *m_chartStepResponse;

	uint32 GetUsedInputsMask();
	uint32 GetUsedOutputsMask();
	static uint32 GetUsedOutputsMask(TreeViewItem *item);
	std::vector<int> * GetVirtualInputs();
	void GetUsedVirtualOutputs(TreeViewItem *item, std::vector<uint32> & usedNumbers);
	uint32 GetNextVirtualOutput();
	Colour  GetOutputColour(TreeViewItem *parent);
	bool IsGraphingParallelOutput(const OutputItem * out);
	void GetUsedColors(TreeViewItem *item, Colour *usedColors, int * numUsedColors);

	void SaveGraph();

	void SaveTreeViewItem(FileOutputStream *stream, TreeViewItem *tvi);

	void LoadGraph();
	void LoadTreeViewItem(FileInputStream *stream);
	void RestoreItems(std::vector<ChainItem*> &items);
	void RestoreSubItems(TreeViewItem *parent, std::vector<ChainItem*> &items);

	int GetNextVitualInput();

	void RedrawGraphs();
	void FlattenTreeView(std::vector<ConfigItemBase* > &config, TreeViewItem *tvi);
	void AddConfigSubTree(std::vector<ConfigItemBase*> &config, TreeViewItem *tvi);
	std::vector<FilterData>* m_filterData = nullptr;
	PanelItemControl * m_itemSetupPannel = nullptr;

	
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};

#endif  // MAINCOMPONENT_H_INCLUDED
