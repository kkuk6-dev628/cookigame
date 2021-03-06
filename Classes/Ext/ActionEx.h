#ifndef __ACTIONEX_H__
#define __ACTIONEX_H__

#include "cocos2d.h"

USING_NS_CC;

//ppp: 2017-1
// y = a ( x - b ) ^ 2 + c
class ParabolicSpline1To : public ActionInterval
{
public:
	static ParabolicSpline1To* create(float duration, const Point& startPoint, const Point& endPoint, float polarY);
    virtual ~ParabolicSpline1To();
    ParabolicSpline1To();
    
    bool initWithDuration(float duration, const Point& startPoint, const Point& endPoint, float polarY);

    // Overrides
	virtual ParabolicSpline1To *clone() const override;
    virtual ParabolicSpline1To* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;

protected:
    Point _startPoint, _endPoint;
	float _a, _b, _c;
	bool _isLine;
};

class ParabolicSpline2To : public ActionInterval
{
public:
	static ParabolicSpline2To* create(float duration, const Point& startPoint, const Point& endPoint, const Point& polarPoint);
    virtual ~ParabolicSpline2To();
    ParabolicSpline2To();
    
    bool initWithDuration(float duration, const Point& startPoint, const Point& endPoint, const Point& polarPoint);

    // Overrides
	virtual ParabolicSpline2To *clone() const override;
    virtual ParabolicSpline2To* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;

protected:
    Point _startPoint, _endPoint, _polarPoint;
	float _a1, _a2;
};

#endif //__ACTIONEX_H__