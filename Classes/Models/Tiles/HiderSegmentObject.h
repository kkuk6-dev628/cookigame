#pragma once
#include "FixTiles.h"

class HiderGroup;

class HiderSegmentObject : public FixTiles
{
public:
	HiderSegmentObject();
	~HiderSegmentObject();

	CREATE_FUNC(HiderSegmentObject)

	HiderGroup* getGroup() const { return group; }
	void setGroup(HiderGroup* hiderGroup) { group = hiderGroup; }
	
	HiderSegmentObject* getNextSegment() const { return nextSegment; }
	void setNextSegment(HiderSegmentObject* nextSg) { nextSegment = nextSg; }

	HiderSegmentObject* getPrevSegment() const { return prevSegment; }
	void setPrevSegment(HiderSegmentObject* prevSg) { prevSegment = prevSg; }

	bool isHead() const { return hiderSegPosition == +HiderSegmentPosition::head; }
	bool isTail() const { return hiderSegPosition == +HiderSegmentPosition::tail; }

	void initWithJson(rapidjson::Value& json) override;

	void initTexture() override;
	void initTexture(std::string textureName) override;
	bool crush(bool showEffect) override;
	void showShakeAnimation();
	void showEatAnimation();

private:
	HiderGroup* group = nullptr;
	HiderSegmentObject* nextSegment = nullptr;
	HiderSegmentObject* prevSegment = nullptr;
	bool isCurve = false;
	Size originalTextureSize;
	AdjacentDirs segmentDirection = AdjacentDirs::E;
	spine::SkeletonAnimation* headSpine = nullptr;
};

