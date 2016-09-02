#ifndef  _Constants_H_
#define  _Constants_H_

#include "cocos2d.h"

USING_NS_CC;

#define socketUrl "http://localhost:3000"
#define loginUrl String::createWithFormat("%s/login", socketUrl)->getCString()
#define userName "BingYanDeXuanLv"
#define userPwd "123456789"

#define ResultCodeSuccess 0

#define LoadInfoType_HeroList 0
#define LoadInfoType_WuPinList 1

#define EmitEvent_UserInfo "getUserInfo"
#define EmitEvent_HeroList "getHeroList"
#define EmitEvent_UpdateHero "setHeroInfo"
#define EmitEvent_UpdateWuPinNum "setWuPinNum"
#define EmitEvent_WuPinList "getWuPinList"


class  Constants 
{
public:
	//const static std::string socketUrl = "http://localhost:3000";
	//const std::string loginUrl = String::createWithFormat("%s/login", socketUrl)->getCString();
	//const std::string userName = "BingYanDeXuanLv";
	//const std::string userPwd = "123456789";
	//const int ResultCodeSuccess = 0;

};

#endif 

