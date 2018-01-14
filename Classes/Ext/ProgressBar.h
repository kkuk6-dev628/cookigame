#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__

#include "cocos2d.h"

/*
 ProgressBar
*/

class ProgressBar : public cocos2d::Sprite
{
public:
	ProgressBar(const char* backImage, const char* foreImage);
	virtual ~ProgressBar();

	void setPercentage(float value) { _progress->setPercentage(value); }

	cocos2d::ProgressTimer* getProgress() { return _progress; }

protected:
	cocos2d::ProgressTimer* _progress;
};

#endif //__PROGRESSBAR_H__