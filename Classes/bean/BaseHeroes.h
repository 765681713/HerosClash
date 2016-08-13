#ifndef __BaseHeroes_H__
#define __BaseHeroes_H__

#include "cocos2d.h"

USING_NS_CC;

typedef enum {
	Hero1,
	Hero2,
	Hero3,
	Boss1,
	Boss2

}HeroType;

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
		"skill" : 10001*/

	CC_SYNTHESIZE(int, id, Id);
	CC_SYNTHESIZE(std::string, name, Name);
	CC_SYNTHESIZE(std::string, title, Title);
	CC_SYNTHESIZE(std::string, desc, Desc);
	CC_SYNTHESIZE(int, hp, HP);
	CC_SYNTHESIZE(int, pAtk, PAtk);//��������
	CC_SYNTHESIZE(int, sAtk, SAtk);//��������
	CC_SYNTHESIZE(int, pDef, PDef);//��������
	CC_SYNTHESIZE(int, sDef, SDef);//��������
	CC_SYNTHESIZE(int, round, Round);//�����غ���
	CC_SYNTHESIZE(int, aAtk, AAtk);//ÿ�غϹ������� �������ͷ������ӣ� 
	CC_SYNTHESIZE(double, doubleAtk, DoubleAtk);//˫�������ĸ���
	CC_SYNTHESIZE(int, skill, Skill);//һ�����ܣ�ID��
	CC_SYNTHESIZE(HeroType, type, Type);//һ�����ܣ�ID��
};



#endif