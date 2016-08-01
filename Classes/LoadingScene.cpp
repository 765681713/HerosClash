#include "LoadingScene.h"

Scene * LoadingScene::createScene(int menuType){

	auto scene = Scene::create();
	auto layer = LoadingScene::create();
	layer->setTag(menuType);
	scene->addChild(layer);
	return scene;
}


bool LoadingScene::init(){
	if (!Layer::init()){
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Sprite * sprite = Sprite::createWithSpriteFrameName("loading_bg2");
	sprite->setPosition(Vec2::ZERO);
	sprite->setAnchorPoint(Vec2::ZERO);
	this->addChild(sprite);
	Animation * animation = Animation::create();
	for (int i = 1; i <= 12; i++){
		__String* str = __String::createWithFormat("loading%04d.png", i);
		log(" init : str = %s" , str->getCString());
		SpriteFrame * sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(str->getCString());
		animation->addSpriteFrame(sf);
	}
	animation->setDelayPerUnit(0.3f);
	animation->setRestoreOriginalFrame(true);
	Animate * ani = Animate::create(animation);
	Sprite * loadSprite = Sprite::createWithSpriteFrameName("loading0001.png");
	loadSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5));
	this->addChild(loadSprite);
	loadSprite->runAction(RepeatForever::create(ani));
	return true;
}

void LoadingScene::onEnterTransitionDidFinish(){
	Texture2D * texture = Director::getInstance()->getTextureCache()->addImage("scene_bg_1.png");
	Rect rect = Rect::ZERO;
	rect.size = texture->getContentSize();
	SpriteFrame * frame = SpriteFrame::createWithTexture(texture, rect);
	SpriteFrameCache::getInstance()->addSpriteFrame(frame,"scene_bg_1");

	this->schedule(schedule_selector(LoadingScene::onExitCallback), 1, 1, 3);
}

void LoadingScene::onExit(){
	Layer::onExit();
	this->unschedule(schedule_selector(LoadingScene::onExitCallback));
}

void LoadingScene::onExitCallback(float time){
	MenuType type = static_cast<MenuType>(this->getTag());
	if (type == MenuType::GameGuide){
		log("LoadingScene   GameGuide");
		
	}
	else if(type == MenuType::StartGame){
		log("LoadingScene   StartGame");

		
	}
	
}