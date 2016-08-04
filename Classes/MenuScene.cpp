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
	sceneNode = CSLoader::createNode("MainScene.csb");
	sceneNode->setAnchorPoint(Vec2::ZERO);
	sceneNode->setPosition(Vec2::ZERO);
	this->addChild(sceneNode);
	spriteListener = EventListenerTouchOneByOne::create();
	spriteListener->setSwallowTouches(true);
	spriteListener->onTouchBegan = CC_CALLBACK_2(MenuScene::spriteTouchBegan, this);
	spriteListener->onTouchMoved = CC_CALLBACK_2(MenuScene::spriteTouchMove, this);
	spriteListener->onTouchEnded = CC_CALLBACK_2(MenuScene::spriteTouchEnd, this);
	eventDispatcher = Director::getInstance()->getEventDispatcher();
	//MainLayout
	scrollView = static_cast<ui::ScrollView *>(sceneNode->getChildByName("MainLayout"));
	scrollView->setScrollBarEnabled(false);
	//Scroll
	//StartGame  YouXiang   YuanZheng  ShangDian  PaiHang  GongHui YingXiongWu ShiLian
	auto startGame = scrollView->getChildByName("StartGame");
	startGame->setTag(MenuType::ScrollOpenGame);
	eventDispatcher->addEventListenerWithSceneGraphPriority(spriteListener->clone(), startGame);
	auto youXiang = scrollView->getChildByName("YouXiang");
	youXiang->setTag(MenuType::ScrollYouXiang);
	eventDispatcher->addEventListenerWithSceneGraphPriority(spriteListener->clone(), youXiang);
	auto yuanZheng = scrollView->getChildByName("YuanZheng");
	yuanZheng->setTag(MenuType::ScrollYuanZheng);
	eventDispatcher->addEventListenerWithSceneGraphPriority(spriteListener->clone(), yuanZheng);
	auto shangDian = scrollView->getChildByName("ShangDian");
	shangDian->setTag(MenuType::ScrollShangDian);
	eventDispatcher->addEventListenerWithSceneGraphPriority(spriteListener->clone(), shangDian);
	auto paiHang = scrollView->getChildByName("PaiHang");
	paiHang->setTag(MenuType::ScrollPaiHang);
	eventDispatcher->addEventListenerWithSceneGraphPriority(spriteListener->clone(), paiHang);
	auto gongHui = scrollView->getChildByName("GongHui");
	gongHui->setTag(MenuType::ScrollGongHui);
	eventDispatcher->addEventListenerWithSceneGraphPriority(spriteListener->clone(), gongHui);
	auto yingXiongWu = scrollView->getChildByName("YingXiongWu");
	yingXiongWu->setTag(MenuType::ScrollYingXiongWu);
	eventDispatcher->addEventListenerWithSceneGraphPriority(spriteListener->clone(), yingXiongWu);
	auto shiLian = scrollView->getChildByName("ShiLian");
	shiLian->setTag(MenuType::ScrollShiLianu);
	eventDispatcher->addEventListenerWithSceneGraphPriority(spriteListener->clone(), shiLian);

	//LayoutInfo    
	Layout * infoLayout = static_cast<Layout * >(sceneNode->getChildByName("LayoutInfo"));
	infoLayout->addClickEventListener(CC_CALLBACK_1(MenuScene::onInfoLayoutClick, this));
	//InfoIcon InfoName  dengji_text
	//ImageView * infoIcon = static_cast<ImageView *> (infoLayout->getChildByName("InfoIcon"));
	//Text * infoName = static_cast<Text * >(infoLayout->getChildByName("InfoName"));
	//Text * infoLevel = static_cast<Text * >(infoLayout->getChildByName("dengji_text"));

	//���Ϸ���ť
	//gold_panel  add_gold  zuanshi_panel  add_zuanshi  tili_panel  add_tili
	//qiandao_panel  huodong_panel  chongzhi_panel
	Layout * topLayout = static_cast<Layout *>(sceneNode->getChildByName("gold_panel"));
	Button * addBtn = static_cast<Button *>(topLayout->getChildByName("add_gold"));
	addBtn->addClickEventListener(CC_CALLBACK_1(MenuScene::onAddGoldClick, this));
	topLayout = static_cast<Layout *>(sceneNode->getChildByName("zuanshi_panel"));
	addBtn = static_cast<Button *>(topLayout->getChildByName("add_zuanshi"));
	addBtn->addClickEventListener(CC_CALLBACK_1(MenuScene::onAddZuanShiClick, this));
	topLayout = static_cast<Layout *>(sceneNode->getChildByName("tili_panel"));
	addBtn = static_cast<Button *>(topLayout->getChildByName("add_tili"));
	addBtn->addClickEventListener(CC_CALLBACK_1(MenuScene::onAddTiLiClick, this));



	otherListener = EventListenerTouchOneByOne::create();
	otherListener->setSwallowTouches(true);
	otherListener->onTouchBegan = CC_CALLBACK_2(MenuScene::otherTouchBegan, this);
	otherListener->onTouchEnded = CC_CALLBACK_2(MenuScene::otherTouchEnd, this);
	auto qianDaoLayout = sceneNode->getChildByName("qiandao_panel");
	qianDaoLayout->setTag(MenuType::QianDao);
	eventDispatcher->addEventListenerWithSceneGraphPriority(otherListener->clone(), qianDaoLayout);
	auto huoDongLayout = sceneNode->getChildByName("huodong_panel");
	huoDongLayout->setTag(MenuType::HuoDong);
	eventDispatcher->addEventListenerWithSceneGraphPriority(otherListener->clone(), huoDongLayout);
	auto chongZhiLayout = sceneNode->getChildByName("chongzhi_panel_0");
	chongZhiLayout->setTag(MenuType::ChongZhi);
	eventDispatcher->addEventListenerWithSceneGraphPriority(otherListener->clone(), chongZhiLayout);

	//qita_panel   yingxiong_panel  beibao_panel  renwu_panel  qita
	auto otherNode = sceneNode->getChildByName("qita");
	Layout * otherLayout = static_cast<Layout *>(sceneNode->getChildByName("qita_panel"));
	otherLayout->setVisible(false);
	otherLayout->setScale(0);
	//Ӣ�� ���� ��Ʒ
	Layout * itemLayout = static_cast<Layout *>(otherLayout->getChildByName("yingxionglan_panel"));
	itemLayout->setTag(MenuType::YingXiongLan);
	itemLayout->addClickEventListener(CC_CALLBACK_1(MenuScene::onOtherBtnClick, this));
	itemLayout = static_cast<Layout *>(otherLayout->getChildByName("beibaolan_panel"));
	itemLayout->setTag(MenuType::BeiBao);
	itemLayout->addClickEventListener(CC_CALLBACK_1(MenuScene::onOtherBtnClick, this));
	itemLayout = static_cast<Layout *>(otherLayout->getChildByName("renwulan_panel"));
	itemLayout->setTag(MenuType::RenWu);
	itemLayout->addClickEventListener(CC_CALLBACK_1(MenuScene::onOtherBtnClick, this));
	//ת����ʾ ����ѡ��
	auto otherEvent = EventListenerTouchOneByOne::create();
	otherEvent->setSwallowTouches(true);
	otherEvent->onTouchBegan = [](Touch * touch, Event* ev){
		log("otherEvent->onTouchBegan");
		auto targt = ev->getCurrentTarget();
		Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
		Size size = targt->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(loacatinInNode)){
			return true;
		}
		return false;
	};
	otherEvent->onTouchEnded = [otherLayout](Touch * touch, Event* ev){
		auto targt = ev->getCurrentTarget();
		Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
		Size size = targt->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(loacatinInNode)){
			if (otherLayout->isVisible()){
				otherLayout->setVisible(false);
				targt->runAction(RotateBy::create(0.5f, 180));
				otherLayout->runAction(ScaleTo::create(0.5f,0));
			}
			else{
				otherLayout->setVisible(true);
				targt->runAction(RotateBy::create(0.5f, 180));
				otherLayout->runAction(ScaleTo::create(0.5f, 1.0f));
			}
		}
	};
	eventDispatcher->addEventListenerWithSceneGraphPriority(otherEvent, otherNode);

	//layer_1    setting_panel   InfoIcon  back_icon  InfoName  login_out_btn   my_info
	//yinyue_panel   yinxiao_panel   yinyue_btn  yinxiao_btn
	layer1 = static_cast<Layout *>(sceneNode->getChildByName("layer_1"));
	layer1->setVisible(false);
	settingLayout = static_cast<Layout *> (layer1->getChildByName("setting_panel"));
	//settingLayout->setAnchorPoint(Vec2(0.5, 0.5));
	settingLayout->setVisible(false);
	
	return true;
}

bool MenuScene::otherTouchBegan(Touch * touch, Event* ev){
	auto targt = ev->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		targt->runAction(ScaleTo::create(0.1f, 0.9f));
		return true;
	}
	return false;
}
void MenuScene::otherTouchEnd(Touch* touch, Event* ev){
	auto targt = ev->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		auto sequence = Sequence::create(ScaleTo::create(0.2f, 1.1f), ScaleTo::create(0.1f, 1.0f), CallFuncN::create(CC_CALLBACK_1(MenuScene::downEnd, this)), NULL);
		targt->runAction(sequence);
	}
	else{
		targt->runAction(ScaleTo::create(0.1f, 1.0f));
	}
}


bool MenuScene::spriteTouchBegan(Touch* touch, Event* ev){
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
	case MenuType::SettingRightBack:{
		auto sequence = Sequence::create(ScaleTo::create(0.4f, 0.0f), CallFunc::create([&](){
			layer1->setVisible(false);
			settingLayout->setVisible(false);
			auto node = this->getChildByTag(MenuType::SettingRightBack);
			eventDispatcher->removeEventListenersForTarget(node);
			this->removeChild(node);
			this->removeChild(settingLayout);
			this->removeChild(layer1);
		}), NULL);
		settingLayout->runAction(sequence);
		break;
	}
	case MenuType::ScrollYouXiang:{

		break;
	}
	case MenuType::ScrollShiLianu:{

		break;
	}
	case MenuType::ScrollYuanZheng:{

		break;
	}
	case MenuType::ScrollShangDian:{

		break;
	}
	case MenuType::ScrollPaiHang:{

		break;
	}
	case MenuType::ScrollGongHui:{

		break;
	}
	case MenuType::ScrollYingXiongWu:{

		break;
	}
	case MenuType::YingXiongLan:{

		break;
	}
	case MenuType::BeiBao:{

		break;
	}
	case MenuType::RenWu:{

		break;
	}
	case MenuType::QianDao:{

		break;
	}
	case MenuType::HuoDong:{

		break;
	}
	case MenuType::ChongZhi:{

		break;
	}
	}
}




void MenuScene::onInfoLayoutClick(Ref * ref){
	layer1->setVisible(true);
	settingLayout->setVisible(true);
	settingLayout->setScale(0);
	auto sequence = Sequence::create(ScaleTo::create(0.4f, 1.0f), NULL);
	settingLayout->runAction(sequence);
	//������ϸ��Ϣ ���ذ�ť
	auto node = settingLayout->getChildByName("back_info_icon");
	node->setTag(MenuType::SettingRightBack);
	auto backEvent = EventListenerTouchOneByOne::create();
	backEvent->setSwallowTouches(true);
	backEvent->onTouchBegan = [](Touch * touch, Event* ev){
		auto targt = ev->getCurrentTarget();
		Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
		Size size = targt->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(loacatinInNode)){
			targt->runAction(ScaleTo::create(0.1f, 0.7f));
			return true;
		}
		return false;
	};
	backEvent->onTouchEnded = [this](Touch * touch, Event* ev){
		auto targt = ev->getCurrentTarget();
		Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
		Size size = targt->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(loacatinInNode)){
			auto sequence = Sequence::create(ScaleTo::create(0.2f, 0.9f), ScaleTo::create(0.1f, 0.8f), CallFuncN::create(CC_CALLBACK_1(MenuScene::downEnd, this)), NULL);
			targt->runAction(sequence);
		}
	};
	eventDispatcher->addEventListenerWithSceneGraphPriority(backEvent, node);

}

void MenuScene::onAddGoldClick(Ref * ref){


}
void MenuScene::onAddZuanShiClick(Ref * ref){

	
}
void MenuScene::onAddTiLiClick(Ref * ref){

	
}

void MenuScene::onOtherBtnClick(Ref * ref){
	//Ӣ�ۣ���Ʒ���� ���
	Layout * layout = static_cast<Layout *>(ref);
	auto sequence = Sequence::create(ScaleTo::create(0.1f, 0.9f),ScaleTo::create(0.2f, 1.1f), ScaleTo::create(0.1f, 1.0f), CallFuncN::create(CC_CALLBACK_1(MenuScene::downEnd, this)), NULL);
	layout->runAction(sequence);
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


