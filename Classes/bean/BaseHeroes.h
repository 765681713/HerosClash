#ifndef __BaseHeroes_H__
#define __BaseHeroes_H__

#include "cocos2d.h"

USING_NS_CC;

typedef enum {
	Hero,
	Boss
}HeroType;

typedef enum{
	ZhanShi,
	FaShi,
	GongJianShou,
	Other
}HeroAtkType;

class BaseHeroes : public Ref
{
public:
	CREATE_FUNC(BaseHeroes);
	virtual bool init();

	/*"id": 3001,
		"name" : "HeroZhanShi_1",
		"title" : "",
		"desc" : "",
		"hp" : 4,
		"pAtk" : 3,
		"sAtk" : 0,
		"pDef" : 2,
		"sDef" : 0,
		"round" : 2,
		"aAtk" : 2,
		"double" : 0.2,
		"skill" : 10001
		"type":0,
        "icon": "hero_zhanshi_1_icon.png",
		"atkEffect":"ZhanShiAtkEffect1",
		"skillEffect":""
		*/

	CC_SYNTHESIZE(int, id, Id);
	CC_SYNTHESIZE(std::string, name, Name);
	CC_SYNTHESIZE(std::string, title, Title);
	CC_SYNTHESIZE(std::string, desc, Desc);
	CC_SYNTHESIZE(int, hp, HP);
	CC_SYNTHESIZE(int, pAtk, PAtk);//物理攻击
	CC_SYNTHESIZE(int, sAtk, SAtk);//法术攻击
	CC_SYNTHESIZE(int, pDef, PDef);//物理防御
	CC_SYNTHESIZE(int, sDef, SDef);//法术防御
	CC_SYNTHESIZE(int, round, Round);//蓄力回合数
	CC_SYNTHESIZE(int, aAtk, AAtk);//每回合攻击增加 （物理和法术共加） 
	CC_SYNTHESIZE(double, doubleAtk, DoubleAtk);//双倍攻击的概率
	CC_SYNTHESIZE(int, skill, Skill);//一个技能（ID）
	CC_SYNTHESIZE(HeroType, type, Type);//是不是Boss
	CC_SYNTHESIZE(HeroAtkType, atkType, AtkType);//攻击类型
	CC_SYNTHESIZE(std::string, atkEffect, AtkEffect);//攻击效果
	CC_SYNTHESIZE(std::string, skillEffect, SkillEffect);//技能效果

};



#endif