#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	//Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//ʹ��cocostudio ����csb�ļ�
	Node * sceneNode = CSLoader::createNode("loadingScene.csb");
	sceneNode->setAnchorPoint(Vec2::ZERO);
	sceneNode->setPosition(Vec2::ZERO);
	this->addChild(sceneNode);
	//���loadingbar
	loadingBar = static_cast <ui::LoadingBar *>(sceneNode->getChildByName("LoadingBar_1"));
	this->schedule(schedule_selector(HelloWorld::udpateProgress), 0.5);
	//��ò˵���������
	//Texture2D * texture = Director::getInstance()->getTextureCache()->addImage("menu_bg.png");
	//Rect rect = Rect::ZERO;
	//rect.size = texture->getContentSize();
	//�õ�����֡   ͨ������ʹ�С
	//SpriteFrame * frame = SpriteFrame::createWithTexture(texture, rect);
	//��ӵ�����
	//SpriteFrameCache::getInstance()->addSpriteFrame(frame, "menu_bg");


	auto loading = Director::getInstance()->getTextureCache()->addImage("loading_bg2.png");
	Rect r = Rect::ZERO;
	r.size = loading->getContentSize();
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::createWithTexture(loading, r), "loading_bg2");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("loading_bg.plist");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("main_scene.mp3");

	return true;
}

void HelloWorld::udpateProgress(float time){
	if (loadingBar != nullptr){
		int loading = loadingBar->getPercent();
		loading += 20;
		if (loading >= 100){
			loadingBar->setPercent(100);
			Scene * ms = MenuScene::createScene();
			Director::getInstance()->replaceScene(ms);
		}
		else{
			loadingBar->setPercent(loading);
		}
	}
}

void HelloWorld::onExit(){
	Layer::onExit();
	this->unschedule(schedule_selector(HelloWorld::udpateProgress));

}