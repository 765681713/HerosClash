#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "LoadingScene.h"
#include "cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio;
using namespace CocosDenshion;

class MenuScene : public Layer
{
public:
	CREATE_FUNC(MenuScene);
	virtual bool init();
	~MenuScene();

	static cocos2d::Scene* createScene();

	virtual void onEnterTransitionDidFinish();
	virtual void cleanup();

	bool spriteTouchBegan(Touch * touch, Event* ev);
	void spriteTouchMove(Touch* touch, Event* ev);
	void spriteTouchEnd(Touch* touch, Event* ev);

	void downEnd(Node *  node);

	virtual void onExit();

};

typedef enum {
	//滚动屏中的精灵
	ScrollOpenGame,
	//左上角的信息
	LayoutIcon,
	LayoutName,
	LayoutLevel
}MenuType;

#endif