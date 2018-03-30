#include "ColorPieObject.h"
#include "cocostudio/ActionTimeline/CSLoader.h"


ColorPieObject::~ColorPieObject()
{
}

void ColorPieObject::showCrushEffect()
{
	
}

bool ColorPieObject::crush(bool showEffect)
{

	if (slots->find(nearbyColor) == slots->end()) return false;
	if(!setVisibleColorNode(nearbyColor))
	{
		return false;
	}
	//auto slotNode = slots->at(nearbyColor);
	//if (slotNode->isVisible() == false) return false;

	//if (slotNode != nullptr) slotNode->setVisible(false);

	pieNumber--;

	if (pieNumber > 0)
	{
		return false;
	}
	else
	{
		setVisible(false);
		return true;
	}
}

void ColorPieObject::initTexture()
{
	auto csbFileName = StringUtils::format("res/%s.csb", type.c_str());
	rootNode = cocos2d::CSLoader::createNode(csbFileName);
	rootNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	rootNode->setPosition(CellSize / 2, CellSize / 2);
	addChild(rootNode);
	slots = new std::map<TileColors, Node*>;
	char i = 0;
	for(auto slotColor : *slotColors)
	{
		i++;
		auto slotNode1 = getSlotNodeWithColor(StringUtils::format("%s_%d", type.c_str(), i), slotColor);
		if (slotNode1 != nullptr)
		{
			slots->insert(std::pair<TileColors, Node*>(slotColor, slotNode1));
		}
	}
}

bool ColorPieObject::setVisibleColorNode(TileColors color)
{
	auto rootChildren = rootNode->getChildren();
	for(auto rootChild : rootChildren)
	{
		auto colorNodeName = StringUtils::format("%s_%s", type.c_str(), color._to_string());
		auto colorNode = rootChild->getChildByName(colorNodeName);
		if(colorNode != nullptr && colorNode->isVisible() && rootChild->isVisible())
		{
			rootChild->setVisible(false);
			return true;
		}
	}
	return false;
}


Node* ColorPieObject::getSlotNodeWithColor(std::string nodeName, TileColors slotColor)
{
	auto slotNode = rootNode->getChildByName(nodeName);
	auto colorNodeName = StringUtils::format("%s_%s", type.c_str(), slotColor._to_string());
	auto children = slotNode->getChildren();
	for(auto colorNode : children)
	{
		if(colorNode->getName() == colorNodeName)
		{
			colorNode->setVisible(true);
		}
		else
		{
			colorNode->setVisible(false);
		}
	}
	return slotNode;
}

