#pragma once
#include "cocos2d.h"

class UserData
{
public:
	static UserData* getInstance();
	~UserData();

private: 
	UserData();

private:
	cocos2d::UserDefault* userDefault;
	static UserData* instance;
	int topLevel;


#pragma region Get Set Method
public:

	int getTopLevel() const
	{
		return topLevel;
	}

	void setTopLevel(int topLevel)
	{
		this->topLevel = topLevel;
	}

#pragma endregion

};

