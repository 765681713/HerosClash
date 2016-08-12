#ifndef __SKILL_H__
#define __SKILL_H__

#include "cocos2d.h"

USING_NS_CC;

typedef enum {
	ZhuDong,
	BeiDong,
	None
}SkillType;

class Skill : public Ref
{
public:
	CREATE_FUNC(Skill);
	virtual bool init();

	/*"id": 1001,
		"name" : "item1",
		"title" : "",
		"icon" : "item1.png",
		"desc" : "",
		"proterties" : {}*/

	CC_SYNTHESIZE(int, id, Id);
	CC_SYNTHESIZE(std::string, name, Name);
	CC_SYNTHESIZE(std::string, title, Title);
	CC_SYNTHESIZE(std::string, icon, Icon);
	CC_SYNTHESIZE(std::string, desc, Desc);
	CC_SYNTHESIZE(SkillType, type, Type);//技能类型
	//CC_SYNTHESIZE(int, proterties, Proterties);
};

#endif