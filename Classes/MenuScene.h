#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "LoadingScene.h"

USING_NS_CC;

class MenuScene : public Layer
{
public:
	CREATE_FUNC(MenuScene);
	virtual bool init();

	static cocos2d::Scene* createScene();

	void menuCallBack(Ref * ref);
};

typedef enum {
	GameGuide,
	StartGame,
	GameSetting,
	AboutHelp
}MenuType;

#endif