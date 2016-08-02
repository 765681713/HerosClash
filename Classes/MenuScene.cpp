#include "MenuScene.h"

Scene * MenuScene::createScene(){
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}


bool MenuScene::init(){
	if (!Layer::init()){
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Node * sceneNode = CSLoader::createNode("MainScene.csb");
	sceneNode->setAnchorPoint(Vec2::ZERO);
	sceneNode->setPosition(Vec2::ZERO);
	this->addChild(sceneNode);
	ui::ScrollView * scrollView = static_cast<ui::ScrollView *>(sceneNode->getChildByName("ScrollView_1"));
	scrollView->setScrollBarEnabled(false);
	auto node = scrollView->getChildByName("Sprite_1");
	node->setTag(MenuType::ScrollOpenGame);
	//auto node = scrollView->getChildByName("Sprite_1");
	//node->setTag(MenuType::ScrollOpenGame);
	auto spriteListener = EventListenerTouchOneByOne::create();
	spriteListener->setSwallowTouches(true);
	spriteListener->onTouchBegan = CC_CALLBACK_2(MenuScene::spriteTouchBegan, this);
	spriteListener->onTouchEnded = CC_CALLBACK_2(MenuScene::spriteTouchEnd, this);
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(spriteListener, node);
	return true;
}

bool MenuScene::spriteTouchBegan(Touch* touch, Event* ev){
	//auto targt = static_cast<Sprite *>(ev->getCurrentTarget());
	auto targt = ev->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		targt->runAction(ScaleTo::create(0.1f, 0.4f));
		return true;
	}
	return false;
}
void MenuScene::spriteTouchMove(Touch* touch, Event* ev){


}
void MenuScene::spriteTouchEnd(Touch* touch, Event* ev){
	//auto targt = static_cast<Sprite *>(ev->getCurrentTarget());
	auto targt = ev->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		auto sequence = Sequence::create(ScaleTo::create(0.2f, 0.6f), ScaleTo::create(0.1f, 0.5f), CallFuncN::create(CC_CALLBACK_1(MenuScene::downEnd, this)), NULL);
		targt->runAction(sequence);
	}
	else{
		targt->runAction(ScaleTo::create(0.1f, 0.5f));
	}
}

void MenuScene::downEnd(Node *  node){
	int type = node->getTag();
	switch (type){
	case MenuType::ScrollOpenGame:{
		Scene * ms = LoadingScene::createScene(MenuType::ScrollOpenGame);
		Director::getInstance()->pushScene(ms);
		break;
	}
	case MenuType::LayoutIcon:{

		break;
	}
	case MenuType::LayoutName:{

		break;
	}
	}
}

void MenuScene::onEnterTransitionDidFinish(){
	SimpleAudioEngine::getInstance()->playBackgroundMusic("main_scene.mp3", true);

}

void MenuScene::cleanup(){
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("main_scene.mp3");

}


void MenuScene::onExit(){
	Layer::onExit();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("main_scene.mp3");
}

MenuScene::~MenuScene(){
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->getScheduler()->unscheduleAll();
	Director::getInstance()->getTextureCache()->removeAllTextures();
	SpriteFrameCache::getInstance()->removeSpriteFrames();
	//SimpleAudioEngine::getInstance()->
	//AnimationCache::getInstance()->
}


