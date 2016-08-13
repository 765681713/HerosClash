#ifndef __HeroObj_H__
#define __HeroObj_H__

#include "cocos2d.h"
#include "BaseHeroes.h"
#include "cocostudio\CocoStudio.h"

USING_NS_CC;

class HeroObj : public Ref
{
public:
	CREATE_FUNC(HeroObj);
	virtual bool init();

	CC_SYNTHESIZE(int, id, ID);
	CC_SYNTHESIZE(int, indexX, IndexX);
	CC_SYNTHESIZE(int, indexY, IndexY);
	CC_SYNTHESIZE(int, positionX, positionX);
	CC_SYNTHESIZE(int, positionY, positionY);
	CC_SYNTHESIZE(bool, isUse, IsUse);

	CC_SYNTHESIZE(BaseHeroes *, hero, Hero);
	CC_SYNTHESIZE(Node *, mNode, MNode);
	//CC_SYNTHESIZE(ActionTimeline *, action, HAction);
	
};

#endif