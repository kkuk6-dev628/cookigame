#include "ThopplerTile.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

ThopplerTile::~ThopplerTile()
{
}

void ThopplerTile::initTexture()
{
	initWithCSB();
}

void TopplingObject::initWithCSB()
{
	rootNode = cocos2d::CSLoader::createNode("res/skeletal/SpineTopplerScoop.csb");
	//rootNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	rootNode->setContentSize(Size(CellSize, CellSize));
	rootNode->setScale(0.5f);
	rootNode->setPosition(Vec2(CellSize / 2.f + 2, CellSize / 4.f));
	addChild(rootNode);

	auto action = cocos2d::CSLoader::createTimeline("res/skeletal/SpineTopplerScoop.csb");
	action->gotoFrameAndPlay(0, true);
	rootNode->runAction(action);

}

void HopplingObject::initWithCSB()
{
	rootNode = cocos2d::CSLoader::createNode("res/skeletal/SpineHopplerSmores.csb");
	//rootNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	rootNode->setContentSize(Size(CellSize, CellSize));
	rootNode->setScale(0.5f);
	rootNode->setPosition(Vec2(CellSize / 2.f + 2, CellSize / 4.f));
	addChild(rootNode);

	auto action = cocos2d::CSLoader::createTimeline("res/skeletal/SpineHopplerSmores.csb");
	action->gotoFrameAndPlay(0, true);
	rootNode->runAction(action);

}
