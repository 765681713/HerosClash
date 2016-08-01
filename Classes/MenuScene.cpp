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
	//SpriteFrame * frame = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->get);
	Sprite * sprite = Sprite::createWithSpriteFrameName("menu_bg");
	//Size size = Size::ZERO;
	//size.width = visibleSize.width;
	//size.height = visibleSize.height;
	//sprite->setContentSize(size);
	sprite->setPosition(Vec2::ZERO);
	sprite->setAnchorPoint(Vec2::ZERO);
	this->addChild(sprite);
	MenuItem * gameGuide = MenuItemImage::create("game_guide_off.png", "game_guide_on.png", CC_CALLBACK_1(MenuScene::menuCallBack, this));
	gameGuide->setTag(MenuType::GameGuide);
	MenuItem * startGame = MenuItemImage::create("start_game_off.png", "start_game_on.png", CC_CALLBACK_1(MenuScene::menuCallBack, this));
	startGame->setTag(MenuType::StartGame);
	MenuItem * gameSetting = MenuItemImage::create("game_setting_off.png", "game_setting_on.png", CC_CALLBACK_1(MenuScene::menuCallBack, this));
	gameSetting->setTag(MenuType::GameSetting);
	MenuItem * aboutHelp = MenuItemImage::create("about_help_off.png", "about_help_on.png", CC_CALLBACK_1(MenuScene::menuCallBack, this));
	aboutHelp->setTag(MenuType::AboutHelp);

	auto menu = Menu::create(gameGuide, startGame, gameSetting, aboutHelp, NULL);
	menu->alignItemsVerticallyWithPadding(12);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(visibleSize.width - gameGuide->getContentSize().width / 2 - 40, visibleSize.height / 2 + 40);
	this->addChild(menu);
	return true;
}

void MenuScene::menuCallBack(Ref * ref){
	switch (((MenuItem*)ref)->getTag()){

	case MenuType::GameGuide:{
		log(" menuCallBack GameGuide ");
		Scene * sc = LoadingScene::createScene(MenuType::GameGuide);
		Director::getInstance()->replaceScene(sc);
		break;
	}
	case MenuType::StartGame:{
		log(" menuCallBack StartGame ");
		Scene * sc = LoadingScene::createScene(MenuType::StartGame);
		Director::getInstance()->replaceScene(sc);
		break;
	}
	case MenuType::GameSetting:{
		log(" menuCallBack GameSetting ");
		break;
	}
	case MenuType::AboutHelp:{
		log(" menuCallBack AboutHelp ");
		break;
	}

	}
}