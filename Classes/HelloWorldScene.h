#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"
#include "MenuScene.h"
#include "network\HttpClient.h"
#include "json\writer.h"
#include "json\stringbuffer.h"
#include "Constants.h"

USING_NS_CC;
using namespace cocostudio;
using namespace network;

class HelloWorld : public cocos2d::Layer
{
private:
	ui::LoadingBar * loadingBar;
	bool isLogin = false;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onEnterTransitionDidFinish();
    
	void udpateProgress(float time);
	void login(HttpRequest * req,HttpResponse * res);

	virtual void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
