#ifndef __Monster_H__
#define __Monster_H__

#include "cocos2d.h"
#include "BaseHeroes.h"

USING_NS_CC;

class Monster : public BaseHeroes
{
public:
	CREATE_FUNC(Monster);
	virtual bool init();

	CC_SYNTHESIZE(std::string, icon, Icon);
	CC_SYNTHESIZE(int, level, Level);
	//CC_SYNTHESIZE(int, id, Id);
};

#endif