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
		if(colspan > rowspan)
		{
			if(gridPos.Col > 4)
			{
				rotation = 90;
			}
			else
			{
				rotation = 270;
			}
		}
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
	if(rotation > 0)
	{
		textureSprite->setAnchorPoint(Vec2(0.5, 0.5));
		textureSprite->setContentSize(Size(CellSize, CellSize * 2));
		textureSprite->setRotation(rotation);
		textureSprite->setPosition(CellSize, CellSize / 2);
	}
	else
	{
		textureSprite->setContentSize(Size(CellSize * colspan, CellSize * rowspan));
		textureSprite->setAnchorPoint(Vec2(0, 1));
		textureSprite->setPosition(0, CellSize);
	}
}

Vec2 PopsicleObject::getLineTarget(float boardWidth, float boardHeight) const
{
	if(colspan > rowspan)
	{
		if(isMirror)
		{
			return Vec2(0, pCell->getBoardPos().y);
		}
		else
		{
			return Vec2(boardWidth, pCell->getBoardPos().y);
		}
	}
	else
	{
		if(isMirror)
		{
			return Vec2(pCell->getBoardPos().x, 0);
		}
		else
		{
			return Vec2(pCell->getBoardPos().x, boardHeight);
		}
	}
}
