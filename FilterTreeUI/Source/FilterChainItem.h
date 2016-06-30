#pragma once
#include <Common.h>
#include "FilterConfigEx.h"
#include "ConfigItem.h"

class ChainItemBase : public TreeViewItem	
{
public:
	ChainItemBase() : m_uniqueId(s_iniqueIdGenerator++), m_isSelected(false)
	{
		m_lineColor = Colours::black;
	}
	virtual ~ChainItemBase()
	{
		this->clearSubItems();
	}

	String getUniqueName() const override
	{
		return String(m_uniqueId);
	}

	bool mightContainSubItems() override
	{
		return false; //No buttons
	}

	void paintItem(Graphics& g, int width, int height) override;

	bool isInterestedInDragSource(const DragAndDropTarget::SourceDetails& dragSourceDetails) override;

	void setHightlighted(bool value, bool setSubItems);
			
	virtual String GetLabel() = 0;
	virtual void SetLineColor(Colour color);
	
	Colour GetLineColor()
	{
		return m_lineColor;
	}

	template <typename ItemType>
	static ItemType* FindItem(TreeViewItem* item)
	{
		TreeViewItem *parent = item;
		ItemType* castedItem = dynamic_cast<ItemType*>(parent);

		while (parent && (!castedItem))
		{
			parent = parent->getParentItem();
			castedItem = dynamic_cast<ItemType*>(parent);
		}

		return castedItem;
	}

	var getDragSourceDescription() override;

	void itemDropped(const DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex) override;
	
	static void BuildSerializationIds(TreeViewItem *base, int *id);

	virtual bool HasSpecialDropAction(ChainItemBase *parent) { return false; }
	virtual void SpecialDropAction(ChainItemBase *parent) { }

protected:
	Colour m_lineColor;
	virtual bool CanBeParentFor(TreeViewItem *child);
private:
	const int m_uniqueId;
	static int s_iniqueIdGenerator;
	
	bool m_isSelected;
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChainItemBase)
};

class TreeViewSelectionHandler
{
public:
	virtual void SelectedTreeViewItemChanged(ChainItemBase *item) = 0;
};


class ChainItem : public ChainItemBase
{
public:
	ChainItem(TreeViewSelectionHandler *parent) : ChainItemBase(), m_parent(parent)
	{
		setLinesDrawnForSubItems(true);
	}

	
	void drawRectangle(Graphics& g, float x, float y, float dX, bool isFilled);
	void paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) override;
	void paintVerticalConnectingLine(Graphics& g, const Line<float>& line) override;

protected:
	TreeViewSelectionHandler *m_parent;
private:
	void itemSelectionChanged(bool isNowSelected)
	{
		if (isNowSelected) m_parent->SelectedTreeViewItemChanged(this);
	}
};

class RootItem : public ChainItem, public RootConfigItem
{
public:
	RootItem(TreeViewSelectionHandler *parent) : ChainItem(parent)
	{
		
	}
protected:
	String GetLabel() override
	{
		return "Root";
	}
private: 
	bool CanBeParentFor(TreeViewItem *child) override;

};

class InputItem : public ChainItem, public InputConfigItem
{
public:
	InputItem(TreeViewSelectionHandler *parent) : ChainItem(parent), InputConfigItem() {}

	InputItem(InputConfigItem *src, TreeViewSelectionHandler *parent) : ChainItem(parent), InputConfigItem(src)
	{
	}
		
	void paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) override;

	void paintVerticalConnectingLine(Graphics& g, const Line<float>& line) override
	{
		//g.setColour(Colours::green);
		//g.drawLine(line);
	}

	virtual bool HasSpecialDropAction(ChainItemBase *parent) override;
	virtual void SpecialDropAction(ChainItemBase *parent) override;
protected:
	String GetLabel() override;
};


class OutputItem : public ChainItem, public OutputConfigItem 
{
public:
	OutputItem(TreeViewSelectionHandler *parent) : ChainItem(parent), OutputConfigItem() {}
	OutputItem(OutputConfigItem *src, TreeViewSelectionHandler *parent) : ChainItem(parent), OutputConfigItem(src) 
	{
		SetLineColor(src->GetOutputColor());
	}
	
	bool mightContainSubItems() override 
	{
		return false;
	}

	void paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) override;

	void SetLineColor(Colour color) override;
	
protected:
	size_t SerializeInternal(uint8** data, bool isWriting) override;
	String GetLabel() override;
private: 
	bool CanBeParentFor(TreeViewItem *child) override;
};


class VirtualOutputItem : public ChainItem, public VirtualOutputConfigItem
{
public:
	VirtualOutputItem(TreeViewSelectionHandler *parent) : ChainItem(parent), VirtualOutputConfigItem()
	{
	}

	VirtualOutputItem(VirtualOutputConfigItem *src, TreeViewSelectionHandler *parent) : ChainItem(parent), VirtualOutputConfigItem(src)
	{
	}
	
	bool mightContainSubItems() override
	{
		return false;
	}

	void paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) override;
	static std::vector<uint32> * GetAllUsedNumbers(TreeViewItem *root);
	void SetLineColor(Colour color) override;

protected:
	String GetLabel() override;
	size_t SerializeInternal(uint8** data, bool isWriting) override;
private:
	static void GetUsedNumbers(TreeViewItem *item, std::vector<uint32> & usedNumbers);
	bool CanBeParentFor(TreeViewItem *child) override;
};

class FilterItem : public ChainItem, public FilterConfigItem
{
public:
	FilterItem(TreeViewSelectionHandler *parent) : ChainItem(parent), FilterConfigItem() {}
	FilterItem(FilterConfigItem *src, TreeViewSelectionHandler *parent) : ChainItem(parent), FilterConfigItem(src)
	{
	}
protected:
	String GetLabel() override
	{
		return String("Filter:") + Config.getDescription();
	}
};


class FilterBlockItem : public ChainItem, public FilterBlockConfigItem
{
public:
	FilterBlockItem(TreeViewSelectionHandler *parent) : ChainItem(parent), FilterBlockConfigItem(){}
	FilterBlockItem(FilterBlockConfigItem *src, TreeViewSelectionHandler *parent) : ChainItem(parent), FilterBlockConfigItem(src)
	{
	}

protected:
	String GetLabel() override
	{
		return (VirtualInput) ? String("Filter Reference: ") + String(VirtualInput) : "Invalid Filter Reference";
	}
};



class MixerItem : public ChainItem, public MixerConfigItem
{
public:
	MixerItem(TreeViewSelectionHandler *parent) : ChainItem(parent), MixerConfigItem(){}
	MixerItem(MixerConfigItem *src, TreeViewSelectionHandler *parent) : ChainItem(parent), MixerConfigItem(src)
	{
	}

	void paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) override;

	void paintVerticalConnectingLine(Graphics& g, const Line<float>& line) override;
	
protected:
	String GetLabel() override;

};