#include "FilterChainItem.h"
int ChainItemBase::s_iniqueIdGenerator = 0;

void ChainItemBase::SetLineColor(Colour color)
{
	m_lineColor = color;
	auto parent = dynamic_cast<ChainItemBase*>(getParentItem());
	if (parent)
	{
		int numSubItems = parent->getNumSubItems();
		if (numSubItems > 1)
		{
			for (int i = 0; i < parent->getNumSubItems(); i++)
			{
				auto out = dynamic_cast<OutputItem*>(parent->getSubItem(i));
				if (!out) //If there anythig but output - paint it black
				{
					parent->SetLineColor(Colours::black);
					return;
				}
			}
		}
		parent->SetLineColor(color);
	}
}

void ChainItemBase::setHightlighted(bool value, bool setSubItems)
{
	m_isSelected = value;
	if (setSubItems)
	{
		for (int i = 0; i < this->getNumSubItems(); i++)
		{
			((ChainItemBase*) this->getSubItem(i))->setHightlighted(value, true);
		}
	}
	treeHasChanged();
}

void ChainItemBase::paintItem(Graphics& g, int width, int height) 
{
	if (m_isSelected)
	{
		g.setColour(Colours::red);
		g.drawRect(2, 0, width - 2, height, 1);
	}

	g.setColour(Colours::black);
	g.setFont(15.0f);

	g.drawText(GetLabel(),
		4, 0, width - 4, height,
		Justification::centredLeft, true);
}

void ChainItemBase::BuildSerializationIds(TreeViewItem *base, int *id)
{
	auto configItem = dynamic_cast<ConfigItemBase*>(base);
	if (!configItem) throw std::logic_error("TreeViewItem is not inherited from ConfigItemBase");
	int parentId = configItem->Id = *id;
	(*id)++;

	for (int i = 0; i < base->getNumSubItems(); i++)
	{
		auto si = base->getSubItem(i);
		auto configSItem = dynamic_cast<ConfigItemBase*>(si);
		if (!configSItem) throw std::logic_error("TreeViewItem child is not inherited from ConfigItemBase");
		configSItem->ParentId = parentId;

		BuildSerializationIds(si, id);
	}
}

var ChainItemBase::getDragSourceDescription() 
{
	return "Filter Chain Item";
}

bool ChainItemBase::isInterestedInDragSource(const DragAndDropTarget::SourceDetails& dragSourceDetails) 
{
	auto tree = dynamic_cast<TreeView*> (dragSourceDetails.sourceComponent.get());
	return (tree == this->getOwnerView() && CanBeParentFor(tree->getSelectedItem(0)));
}

void ChainItemBase::itemDropped(const DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex)
{
	auto tree = dynamic_cast<TreeView*> (dragSourceDetails.sourceComponent.get());
	if (tree == this->getOwnerView())
	{
		auto item = tree->getSelectedItem(0);
		if (CanBeParentFor(item))
		{
			auto originalParent = item->getParentItem();
			if (originalParent)
			{
				originalParent->removeSubItem(item->getIndexInParent(), false);
				addSubItem(item);
			}
		}
	}
}

bool ChainItemBase::CanBeParentFor(TreeViewItem *child)
{
	//Used for filter, input, and mixer
	auto cfg = dynamic_cast<ConfigItemBase*>(child);
	if (!cfg) return false;
	auto input = FindItem<InputItem>(this);
	if (input && input->IsVirtual)
	{
		if (input->getNumSubItems() > 0) return false;//Can't have more than one child for virtual input
		switch (cfg->ElementType)
		{
			//case Input:
			case Filter:
			case Block:
			//case Output:
			//case Mixer:
			//case VirtualOutput:
				return true;
			default: return false;
		}

	}

	switch (cfg->ElementType)
	{
		//case Input:
		case Filter:
		case Block:
		case Output:
			//case Mixer:
		case VirtualOutput:
			return true;
		default: return false;
	}
}

void ChainItem::drawRectangle(Graphics& g, float x, float y, float dX, bool isFilled)
{
	const float dY = dX * 0.57f;
	const float x1 = x - dX;
	const float y1 = y - dY;
	const float y2 = y + dY;

	Path path;
	path.startNewSubPath(x1, y1);
	path.lineTo(x1, y2);
	path.lineTo(x, y);
	path.lineTo(x1, y1);
	juce::PathStrokeType st(1.0f);
	if (isFilled)
	{
		g.fillPath(path);
	}
	//else
		{
			g.strokePath(path, st);
		}
}

void ChainItem::paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) 
{
	g.setColour(m_lineColor);
	g.drawLine(line);
}

void ChainItem::paintVerticalConnectingLine(Graphics& g, const Line<float>& line) 
{
	g.setColour(m_lineColor);
	g.drawLine(line);
}

bool RootItem::CanBeParentFor(TreeViewItem *child)
{
	auto cfg = dynamic_cast<ConfigItemBase*>(child);
	if (!cfg) return false;
	switch (cfg->ElementType)
	{
		case Input:
		//case Filter:
		//case Block:
		//case Output:
		case Mixer: 
		//case VirtualOutput:
			return true;
		default: return false;
	}
}

void InputItem::paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) 
{
	g.setColour(m_lineColor);
	float x = line.getStartX() + 4;
	float y = line.getStartY();
	drawRectangle(g, x, y, 8.0f, !IsVirtual);
	g.drawLine(x, y, line.getEndX(), y);
}

String InputItem::GetLabel()
{
	if (IsVirtual)
	{
		return String("Filter Definition: ") + String(InputMask);
	}
	else
	{
		StringArray s;

		for (int i = 0; i < 32; i++)
		{
			if (InputMask & (1 << i)) s.add(String(i + 1));
		}
		return String("Input: ") + s.joinIntoString(", ");
	}
}


void OutputItem::paintHorizontalConnectingLine(Graphics& g, const Line<float>& line)
{
	g.setColour(m_lineColor);
	float x = line.getEndX();
	float y = line.getEndY();
	drawRectangle(g, x + 3, y, 8.0f, true);
	g.drawLine(line);
}

void OutputItem::SetLineColor(Colour color) 
{
	if (ShowInGraphs) ChainItem::SetLineColor(ShowInGraphs ? color : Colours::black);
	m_color = color;
}

size_t OutputItem::SerializeInternal(uint8** data, bool isWriting) 
{
	size_t sz = OutputConfigItem::SerializeInternal(data, isWriting);
	SetLineColor(m_color);
	return sz;
}

String OutputItem::GetLabel() 
{

	StringArray s;
	for (int i = 0; i < OutputChannelsLenght; i++)
	{
		if (OutputChannels[i] > 0)
		{
			s.add(String(OutputChannels[i]));
		}
		else
		{
			s.add("N/C");
		}
	}
	return String(("Output: ") + s.joinIntoString(", ") + String::formatted(" (delay %1.6f ms)", DelayMS));
}

bool OutputItem::CanBeParentFor(TreeViewItem *child)
{
	return false;
}


void VirtualOutputItem::paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) 
{
	g.setColour(m_lineColor);
	float x = line.getEndX();
	float y = line.getEndY();
	drawRectangle(g, x + 3, y, 8.0f, false);
	g.drawLine(line);
}

std::vector<uint32> * VirtualOutputItem::GetAllUsedNumbers(TreeViewItem *root)
{
	std::vector<uint32> * used = new std::vector<uint32>();
	GetUsedNumbers(root, *used);
	std::sort(used->begin(), used->end());
	return used;
}

void VirtualOutputItem::SetLineColor(Colour color) 
{
	ChainItem::SetLineColor(color);
	m_color = color;
}


String VirtualOutputItem::GetLabel() 
{
	return String::formatted("Mixer Bus %i (gain %1.3f)", Number, Gain);
}

size_t VirtualOutputItem::SerializeInternal(uint8** data, bool isWriting) 
{
	size_t sz = VirtualOutputConfigItem::SerializeInternal(data, isWriting);
	SetLineColor(m_color);
	return sz;
}

void VirtualOutputItem::GetUsedNumbers(TreeViewItem *item, std::vector<uint32> & usedNumbers)
{
	int num = item->getNumSubItems();
	for (int i = 0; i < num; i++)
	{
		auto subItem = item->getSubItem(i);
		auto output = dynamic_cast<VirtualOutputItem*>(subItem);
		if (output)
		{
			usedNumbers.push_back(output->Number);
		}
		else
		{
			GetUsedNumbers(subItem, usedNumbers);
		}
	}
}
bool VirtualOutputItem::CanBeParentFor(TreeViewItem *child)
{
	return false;
}


void MixerItem::paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) 
{
	g.setColour(m_lineColor);
	float xStart = line.getStartX();
	float yMid = line.getStartY();
	float dX = 8.0f;
	float xLn1End = xStart - 4;
	float xLn1Start = xLn1End - 4;
	float xLn2Start = xLn1End + dX;
	float xLn2End = line.getEndX();
	g.drawLine(xLn1Start, yMid - 2, xLn1End, yMid - 2);
	g.drawLine(xLn1Start, yMid + 2, xLn1End, yMid + 2);
	drawRectangle(g, xLn2Start, yMid, dX, false);
	g.drawLine(xLn2Start, yMid, xLn2End, yMid);
}

void MixerItem::paintVerticalConnectingLine(Graphics& g, const Line<float>& line) 
{
}

String MixerItem::GetLabel() 
{
	StringArray s;
	for (uint32 i = 0; i < VirtualOutputs.size(); i++)
	{
		s.add(String(VirtualOutputs[i]));
	}
	return String("Mixer: ") + s.joinIntoString(", ");
}

