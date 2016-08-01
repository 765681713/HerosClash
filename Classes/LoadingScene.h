#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "MenuScene.h"

USING_NS_CC;

class LoadingScene : public Layer
{
public:
	CREATE_FUNC(LoadingScene);
	virtual bool init();
	static cocos2d::Scene* createScene(int menuType);

	virtual void onEnterTransitionDidFinish();

	virtual void onExit();

	void onExitCallback(float time);
};

#endif