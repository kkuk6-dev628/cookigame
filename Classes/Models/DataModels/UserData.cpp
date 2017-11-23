#include "UserData.h"

UserData* UserData::instance = nullptr;
UserData* UserData::getInstance()
{
	if (instance == nullptr)
	{
		instance = new UserData();
	}
	return instance;
}


UserData::UserData()
{
	userDefault = cocos2d::UserDefault::getInstance();
	topLevel = userDefault->getIntegerForKey("top_level", 1);
	topLevel = MAX(topLevel, 2);

}


UserData::~UserData()
{
}
