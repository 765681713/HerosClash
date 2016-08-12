#ifndef __USER_INFO_H__
#define __USER_INFO_H__

#include "cocos2d.h"
#include "BaseNPC.h"

USING_NS_CC;

class UserInfo : public BaseNPC
{
public:
	CREATE_FUNC(UserInfo);
	virtual bool init();

	
	CC_SYNTHESIZE(std::string, exp, EXP);
	CC_SYNTHESIZE(std::vector<int>, heroesId, HeroesId);//所有英雄
	CC_SYNTHESIZE(std::vector<int>, wuPinId, WuPinId);//所有物品

};

#endif