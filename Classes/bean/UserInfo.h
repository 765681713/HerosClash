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

	CC_SYNTHESIZE(int, exp, EXP);
	CC_SYNTHESIZE(int, expned, EXPNed);
	//CC_SYNTHESIZE(std::vector<int>, heroesId, HeroesId);//所有英雄
	//CC_SYNTHESIZE(std::vector<int>, wuPinId, WuPinId);//所有物品
	CC_SYNTHESIZE(int, glodNum, GlodNum);
	CC_SYNTHESIZE(int, zuanShiNum, ZuanShiNum);
	CC_SYNTHESIZE(int, tiLiNum, TiLiNum);
	CC_SYNTHESIZE(int, tiLiNumCount, TiLiNumCount);

};

#endif