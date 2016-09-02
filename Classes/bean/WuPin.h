#ifndef __WUPIN_H__
#define __WUPIN_H__

#include "cocos2d.h"

USING_NS_CC;

class WuPin : public Ref
{
public:
	CREATE_FUNC(WuPin);
	virtual bool init();

	/*"id": 4001,
		"name" : "item1",
		"desc" : "不",
		"title" : "",
		"icon" : "item1.png",
		"proterties" : {
		"pAtk": 1
	}*/

	CC_SYNTHESIZE(int, id, Id);
	CC_SYNTHESIZE(std::string, name, Name);
	CC_SYNTHESIZE(std::string, desc, Desc);
	CC_SYNTHESIZE(std::string, title, Title);
	CC_SYNTHESIZE(std::string, icon, Icon);
	CC_SYNTHESIZE(int, num, Num);
	std::map<std::string, int> proterties; //所有属性
	//CC_SYNTHESIZE(std::map<std::string, int> , proterties, Proterties);


};

#endif