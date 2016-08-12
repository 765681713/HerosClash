#ifndef __BASE_NPC_H__
#define __BASE_NPC_H__

#include "cocos2d.h"

USING_NS_CC;


class BaseNPC : public Ref
{
public:
	CREATE_FUNC(BaseNPC);
	virtual bool init();

	/*"5001":{
		"id":5001,
			"level" : 1,
			"icon" : "",
			"name" : "冰焰的旋律",
			"hp" : 25,
			"def" : 2,
			"mp" : 25,
			"heroesCount" : 15,
			"act" : 3
	}*/
	CC_SYNTHESIZE(int, id, Id);
	CC_SYNTHESIZE(int, level, Level);
	CC_SYNTHESIZE(std::string, icon, Icon);
	CC_SYNTHESIZE(std::string, name, Name);
	CC_SYNTHESIZE(int, hp, HP);
	CC_SYNTHESIZE(int, def, DEF);
	CC_SYNTHESIZE(int, mp, MP);
	CC_SYNTHESIZE(int, heroesCount, HeroesCount);//英雄数量
	CC_SYNTHESIZE(int, act, ACT);//行动次数
};

#endif