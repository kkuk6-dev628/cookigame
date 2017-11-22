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

	int top_level() const
	{
		return topLevel;
	}

	void top_level(int topLevel)
	{
		this->topLevel = topLevel;
	}

	__declspec(property(get = top_level, put = top_level)) int TopLevel;

#pragma endregion

};

