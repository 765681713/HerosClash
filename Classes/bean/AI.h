#ifndef __AI_H__
#define __AI_H__

#include "cocos2d.h"
#include "BaseNPC.h"

USING_NS_CC;


class AI : public BaseNPC
{
public:
	CREATE_FUNC(AI);
	virtual bool init();

	CC_SYNTHESIZE(std::string, desc, Desc);

};

#endif