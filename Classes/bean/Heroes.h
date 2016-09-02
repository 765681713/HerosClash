#ifndef __Heroes_H__
#define __Heroes_H__

#include "cocos2d.h"
#include "BaseHeroes.h"

USING_NS_CC;

class Heroes : public BaseHeroes
{
public:
	CREATE_FUNC(Heroes);
	virtual bool init();

	CC_SYNTHESIZE(std::string,_fk,_FK);
	CC_SYNTHESIZE(std::string, icon, Icon);
	CC_SYNTHESIZE(int, level, Level);
	CC_SYNTHESIZE(int, exp, EXP);
	CC_SYNTHESIZE(int, masExp, MaxEXP);
	CC_SYNTHESIZE(int, hTag, HTag);
};

#endif