#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"
#include "MenuScene.h"

USING_NS_CC;
using namespace cocostudio;

class HelloWorld : public cocos2d::Layer
{
private:
	ui::LoadingBar * loadingBar;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void udpateProgress(float time);

	virtual void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
