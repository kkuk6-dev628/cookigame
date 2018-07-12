#include "HiderSegmentObject.h"
#include "Models/BoardModels/Cell.h"
#include "Controllers/ActionController.h"


HiderSegmentObject::HiderSegmentObject()
{
}


HiderSegmentObject::~HiderSegmentObject()
{
}

void HiderSegmentObject::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);
	if(hiderSegPosition == +HiderSegmentPosition::head)
	{
		receiveNearbyAffect = true;
	}
}

void HiderSegmentObject::initTexture()
{
	if (pCell == nullptr) return;

	std::string textureName;
	auto rotation = 0.f;
	auto currPos = pCell->gridPos;
	if(prevSegment != nullptr)
	{
		auto prevPos = prevSegment->getCell()->gridPos;
		auto prevDir = Utils::getDirection(prevPos, currPos);
		if(nextSegment != nullptr)
		{
			auto nextPos = nextSegment->getCell()->gridPos;
			auto nextDir = Utils::getDirection(currPos, nextPos);
			segmentDirection = nextDir;

			if(prevDir == nextDir)
			{
				textureName = "HiderSegmentObject_body.png";
				isCurve = false;
				switch(prevDir)
				{
				case AdjacentDirs::E:
					rotation = 0.f;
					break;
				case AdjacentDirs::W:
					rotation = 180.f;
					break;
				case AdjacentDirs::N:
					rotation = 90.f;
					break;
				case AdjacentDirs::S:
					rotation = 270.f;
					break;
				}
			}
			else
			{
				isCurve = true;
				textureName = "HiderSegmentObject_curve.png";
				switch (prevDir)
				{
				case AdjacentDirs::N:
					if(nextDir == +AdjacentDirs::E)
					{
						rotation = 90.f;
					}
					else
					{
						rotation = 180.f;
					}
					break;
				case AdjacentDirs::E:
					if (nextDir == +AdjacentDirs::N)
					{
						rotation = 270.f;
					}
					else
					{
						rotation = 180.f;
					}
					break;
				case AdjacentDirs::S:
					if (nextDir == +AdjacentDirs::E)
					{
						rotation = 0.f;
					}
					else
					{
						rotation = 270.f;
					}
					break;
				case AdjacentDirs::W:
					if (nextDir == +AdjacentDirs::N)
					{
						rotation = 0.f;
					}
					else
					{
						rotation = 90.f;
					}
					break;
				}
			}
		}
		else
		{
			segmentDirection = prevDir;
			textureName = "HiderSegmentObject_tail.png";
			switch(prevDir)
			{
			case AdjacentDirs::N:
				rotation = 270;
				break;
			case AdjacentDirs::E:
				rotation = 0;
				break;
			case AdjacentDirs::S:
				rotation = 90;
				break;
			case AdjacentDirs::W:
				rotation = 180;
				break;
			}
		}
	}
	else
	{
		textureName = "";
		segmentDirection = Utils::getDirection(currPos, nextSegment->gridPos);
		switch (segmentDirection)
		{
		case AdjacentDirs::N:
			rotation = 180;
			break;
		case AdjacentDirs::E:
			rotation = 270;
			break;
		case AdjacentDirs::S:
			rotation = 0;
			break;
		case AdjacentDirs::W:
			rotation = 90;
			break;
		}
	}

	initTexture(textureName);
	setRotation(rotation);
}

void HiderSegmentObject::initTexture(std::string textureName)
{
	if(textureName.empty())
	{
		if (hiderSegPosition == +HiderSegmentPosition::head)
		{
			if(headSpine == nullptr)
			{
				headSpine = spine::SkeletonAnimation::createWithJsonFile("spineAnimations/HiderSegmentObject.json", "spineAnimations/HiderSegmentObject.atlas");
				headSpine->setAnimation(0, "Idle", true);
				headSpine->setAnchorPoint(Vec2(0.5f, 0.5f));
				headSpine->setPosition(Vec2(CellSize / 2, CellSize / 2 - 6));
				addChild(headSpine);
			}
		}
	}
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureName);
	if (spriteFrame == nullptr)
	{
		return;
	}
	if (textureSprite == nullptr)
	{
		textureSprite = Sprite::createWithSpriteFrame(spriteFrame);
		addChild(textureSprite);
		originalTextureSize = textureSprite->getContentSize();
	}
	else
	{
		textureSprite->setSpriteFrame(spriteFrame);
	}
	textureSprite->setAnchorPoint(Vec2(0.5, 0.5));
	originalTextureSize = spriteFrame->getOriginalSizeInPixels();
	if(isCurve)
	{
		textureSprite->setPosition(CellSize / 2 + 4, CellSize / 2 + 4);
		textureSprite->setContentSize(Size(CellSize - 6, CellSize - 6));
	}
	else
	{
		auto height = originalTextureSize.height * CellSize / originalTextureSize.width;
		textureSprite->setContentSize(Size(CellSize, height));
		textureSprite->setPosition(CellSize / 2, CellSize / 2);
	}
}

bool HiderSegmentObject::crush(bool showEffect)
{
	return true;
}

void HiderSegmentObject::showShakeAction()
{
	CKAction ckAction;
	auto actionController = ActionController::getInstance();
	ckAction.node = this;
	ckAction.action = actionController->createHiderSegmentShakeAction(this, segmentDirection == +AdjacentDirs::E || segmentDirection == +AdjacentDirs::W);
	actionController->pushAction(ckAction, false);
}

void HiderSegmentObject::showEatAnimation()
{
	if (!isHead()) return;

	headSpine->setAnimation(0, "animation", false);
	headSpine->setCompleteListener([=](int trackIndex, int loopCount)
	{
		//headSpine->clearTracks();
		headSpine->setAnimation(0, "Idle", true);
	});
}
