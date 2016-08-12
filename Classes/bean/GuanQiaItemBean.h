#ifndef __GUANQIAITEM_BEAN_H__
#define __GUANQIAITEM_BEAN_H__

#include "cocos2d.h"
USING_NS_CC;
class GuanQiaItemBean : public Ref
{
public:
	CREATE_FUNC(GuanQiaItemBean);
	virtual bool init();

		/*"id":111,
		"name" : "guanqia_1_1_1",
		"glod" : 250,
		"exp" : 56,
		"title":"",
		"desc":"",
		"isfinish" : false,
		"goods" : [
			1001, 1002
		],
		"isboss" : false,
		"monster" : [
			"HeroZhanShi_1", "HeroFaShi_1", "HeroGongJianShou_1"

		]*/


	CC_SYNTHESIZE(int, id, Id);
	CC_SYNTHESIZE(std::string, name, Name);
	CC_SYNTHESIZE(std::string, title, Title);
	CC_SYNTHESIZE(std::string, desc, Desc);
	CC_SYNTHESIZE(int, glod, Glod);
	CC_SYNTHESIZE(int, exp, Exp);
	CC_SYNTHESIZE(bool, isfinish, Isfinish);
	CC_SYNTHESIZE(std::vector<int>, goods, Goods);
	CC_SYNTHESIZE(bool, isboss, Isboss);
	CC_SYNTHESIZE(int, ai, AI);
	CC_SYNTHESIZE(std::vector<int>, monster, Monster);
};

#endif