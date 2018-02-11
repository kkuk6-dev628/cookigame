#include "PopsicleObject.h"
#include "General/Utils.h"
#include "Models/BoardModels/Cell.h"


PopsicleObject::PopsicleObject()
{
}


PopsicleObject::~PopsicleObject()
{
}

void PopsicleObject::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);

	if (gridSize.empty()) return;

	auto tokens = Utils::splitString(gridSize, '_');
	auto g = Utils::StrToGridSize(tokens.front(), "x");
	colspan = g.Width;
	rowspan = g.Height;
	if(tokens.size() > 1)
	{
		popType = PopsicleType::_from_string(tokens.at(1).c_str());
	}
	if(tokens.size() > 2)
	{
		isMirror = (tokens.at(2) == "mirror");
	}
	initTexture();
}

void PopsicleObject::initTexture()
{
	std::string textureName;
	auto rotation = 0;
	switch(popType)
	{
	case PopsicleType::line:
		if(colspan > rowspan)
		{
			if(isMirror)
			{
				textureName = "PopsicleObject_line_W.png";
			}
			else
			{
				textureName = "PopsicleObject_line_E.png";
			}
		}
		else
		{
			if(isMirror)
			{
				textureName = "PopsicleObject_line_S.png";
			}
			else
			{
				textureName = "PopsicleObject_line_N.png";
			}
		}
		break;
	case PopsicleType::bomb:
		textureName = "PopsicleObject_bomb.png";
		break;
	case PopsicleType::rainbow:
		textureName = "PopsicleObject_rainbow.png";
		break;
	default:
		{
			if(colspan == rowspan)
			{
				textureName = "PopsicleObject_1.png";
			}
			else
			{
				if(colspan > rowspan)
				{
					textureName = "PopsicleObject_h.png";
				}
				else
				{
					textureName = "PopsicleObject_v.png";
				}
			}
		}
		break;
	}
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureName);
	if (spriteFrame == nullptr)
	{
		return;
	}
	if (textureSprite == nullptr)
	{
		textureSprite = Sprite::create();
		addChild(textureSprite);
	}
	textureSprite->setSpriteFrame(spriteFrame);
	textureSprite->setContentSize(Size(CellSize * colspan, CellSize * rowspan));
	textureSprite->setAnchorPoint(Vec2(0, 1));
	textureSprite->setPosition(0, CellSize);

}
