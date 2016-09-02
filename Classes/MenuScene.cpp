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
	//socketIO
	sioClient = SocketIO::connect(socketUrl, *this);
	sioClient->setTag(userName);
	sioClient->on(EmitEvent_UserInfo, CC_CALLBACK_2(MenuScene::getUserInfo, this));
	sioClient->on(EmitEvent_HeroList, CC_CALLBACK_2(MenuScene::getHeroList, this));
	sioClient->on(EmitEvent_WuPinList, CC_CALLBACK_2(MenuScene::getWuPinList, this));
	sioClient->on(EmitEvent_UpdateHero, CC_CALLBACK_2(MenuScene::updateHeroInfo, this));
	sioClient->on(EmitEvent_UpdateWuPinNum, CC_CALLBACK_2(MenuScene::updateWuPinNum, this));
	
	//最上方按钮
	//gold_panel  add_gold  zuanshi_panel  add_zuanshi  tili_panel  add_tili
	//qiandao_panel  huodong_panel  chongzhi_panel  zuanshi_text
	Layout * topLayout = static_cast<Layout *>(sceneNode->getChildByName("gold_panel"));
	Button * addBtn = static_cast<Button *>(topLayout->getChildByName("add_gold"));
	addBtn->addClickEventListener(CC_CALLBACK_1(MenuScene::onAddGoldClick, this));
	topLayout = static_cast<Layout *>(sceneNode->getChildByName("zuanshi_panel"));
	addBtn = static_cast<Button *>(topLayout->getChildByName("add_zuanshi"));
	addBtn->addClickEventListener(CC_CALLBACK_1(MenuScene::onAddZuanShiClick, this));
	topLayout = static_cast<Layout *>(sceneNode->getChildByName("tili_panel"));
	addBtn = static_cast<Button *>(topLayout->getChildByName("add_tili"));
	addBtn->addClickEventListener(CC_CALLBACK_1(MenuScene::onAddTiLiClick, this));

	//qiandao  huodong  监听
	/*otherListener = EventListenerTouchOneByOne::create();
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
	eventDispatcher->addEventListenerWithSceneGraphPriority(otherListener->clone(), chongZhiLayout);*/

	//qita_panel   yingxiong_panel  beibao_panel  renwu_panel  qita
	auto otherNode = sceneNode->getChildByName("qita");
	Layout * otherLayout = static_cast<Layout *>(sceneNode->getChildByName("qita_panel"));
	//otherLayout->setVisible(false);
	otherLayout->setScale(0);
	//弹出背包的监听
	//英雄 任务 物品
	Layout * itemLayout = static_cast<Layout *>(otherLayout->getChildByName("yingxionglan_panel"));
	itemLayout->setTag(MenuType::YingXiongLan);
	itemLayout->addClickEventListener(CC_CALLBACK_1(MenuScene::onOtherBtnClick, this));
	itemLayout = static_cast<Layout *>(otherLayout->getChildByName("beibaolan_panel"));
	itemLayout->setTag(MenuType::BeiBao);
	itemLayout->addClickEventListener(CC_CALLBACK_1(MenuScene::onOtherBtnClick, this));
	itemLayout = static_cast<Layout *>(otherLayout->getChildByName("renwulan_panel"));
	itemLayout->setTag(MenuType::RenWu);
	itemLayout->addClickEventListener(CC_CALLBACK_1(MenuScene::onOtherBtnClick, this));
	//转动显示 其他选项
	auto otherEvent = EventListenerTouchOneByOne::create();
	otherEvent->setSwallowTouches(true);
	otherEvent->onTouchBegan = [](Touch * touch, Event* ev){
		auto targt = ev->getCurrentTarget();
		Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
		Size size = targt->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(loacatinInNode)){
			return true;
		}
		return false;
	};
	otherEvent->onTouchEnded = [otherLayout, this](Touch * touch, Event* ev){
		auto targt = ev->getCurrentTarget();
		Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
		Size size = targt->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(loacatinInNode)){
			if (otherLayout->getScale() != 0){
				targt->runAction(RotateBy::create(0.3f, 180));
				otherLayout->runAction(Sequence::create(ScaleTo::create(0.3f, 0), NULL));
			}
			else{
				//otherLayout->setVisible(true);
				targt->runAction(RotateBy::create(0.3f, 180));
				otherLayout->runAction(ScaleTo::create(0.3f, 1.0f));
			}
		}
	};
	eventDispatcher->addEventListenerWithSceneGraphPriority(otherEvent, otherNode);

	//layer_1    setting_panel   InfoIcon  back_icon  InfoName  login_out_btn   my_info
	//yinyue_panel   yinxiao_panel   yinyue_btn  yinxiao_btn
	layer1 = static_cast<Layout *>(sceneNode->getChildByName("layer_1"));
	layer1->setVisible(false);
	settingLayout = static_cast<Layout *> (layer1->getChildByName("setting_panel"));
	settingLayout->setVisible(false);

	//英雄列表
	yingXiongLayout = static_cast<Layout *>(layer1->getChildByName("YingXiongPanel"));
	yingXiongLayout->setVisible(false);
	yingXiongLayout->setScale(0);
	//物品列表
	beiBaoLayout = static_cast<Layout *>(layer1->getChildByName("BeiBaoPanel"));
	beiBaoLayout->setVisible(false);
	beiBaoLayout->setScale(0);

	//返回事件监听
	backEvent = EventListenerTouchOneByOne::create();
	backEvent->setSwallowTouches(true);
	backEvent->retain();
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
	return true;
}

//签到 huodong chongzhi 
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

//scorll 上的精灵监听
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
//精灵单击根据类型回调
void MenuScene::downEnd(Node *  node){
	int type = node->getTag();
	switch (type){
	case MenuType::ScrollOpenGame:{
		Scene * ms = GuanQiaScene::createScene(userInfo, mHeroes);
		Director::getInstance()->pushScene(ms);
		break;
	}
	case MenuType::SettingRightBack:{
		auto sequence = Sequence::create(ScaleTo::create(0.3f, 0.0f), CallFunc::create([&](){
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
		auto youXiangNode = CSLoader::createNode("YouXiangLayer.csb");
		auto youXiangLayout = static_cast<Layout *>(youXiangNode->getChildByName("YouXiangLBG")->getChildByName("YouXiangL"));
		//auto youXiangItemLayout = static_cast<Layout *>(CSLoader::createNode("YouXiangItem.csb")->getChildByName("YingXiongL"));
		auto youXiangList = static_cast<ListView *>(youXiangLayout->getChildByName("YouXiangList"));
		auto yXBack = static_cast<Button *>(youXiangLayout->getChildByName("YouXiangBackBtn"));
		yXBack->addClickEventListener([this,youXiangNode](Ref * ref){
			this->removeChild(youXiangNode);
		});
		Size listSize = youXiangList->getContentSize();
		int contantH = (5 + 120) * 3;
		contantH = contantH > listSize.height ? contantH : listSize.height;
		Size size;
		size.height = contantH;
		size.width = listSize.width;
		youXiangList->setInnerContainerSize(size);
		youXiangList->setClippingEnabled(true);
		youXiangList->setScrollBarEnabled(false);
		for (int i = 0; i < 3; i++){
			auto itemNode = CSLoader::createNode("YouXiangItem.csb");
			auto itemLayout = static_cast<Layout *>(itemNode->getChildByName("YingXiongL"));
			//auto icon = static_cast<CheckBox *>(itemLayout->getChildByName("YouXiangIcon"));
			//auto title = static_cast<Text *>(itemLayout->getChildByName("YouXiangTitle"));
			//auto sendName = static_cast<Text *>(itemLayout->getChildByName("YouXiangSendName"));
			//auto time = static_cast<Text *>(itemLayout->getChildByName("YouXiangTime"));
			Layout * layout = Layout::create();
			itemNode->setPosition(Vec2(15,-10));
			layout->setContentSize(itemNode->getContentSize());
			layout->addChild(itemNode);
			
			youXiangList->pushBackCustomItem(layout);
		}
		this->addChild(youXiangNode);
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
		layer1->setVisible(true);
		//弹出英雄列表
		CheckBox * allCheckBox = static_cast<CheckBox *>(yingXiongLayout->getChildByName("YingXiongAll"));
		auto gongCheckBox = static_cast<CheckBox *>(yingXiongLayout->getChildByName("YingXiongGong"));
		auto fangCheckBox = static_cast<CheckBox *>(yingXiongLayout->getChildByName("YingXiongFang"));
		allCheckBox->setSelected(true);
		allCheckBox->addClickEventListener([allCheckBox, gongCheckBox, fangCheckBox](Ref * ref){
			allCheckBox->setSelected(true);
			gongCheckBox->setSelected(false);
			fangCheckBox->setSelected(false);

		});
		gongCheckBox->setSelected(false);
		gongCheckBox->addClickEventListener([allCheckBox, gongCheckBox, fangCheckBox](Ref * ref){
			gongCheckBox->setSelected(true);
			allCheckBox->setSelected(false);
			fangCheckBox->setSelected(false);

		});
		fangCheckBox->setSelected(false);
		fangCheckBox->addClickEventListener([allCheckBox, gongCheckBox, fangCheckBox](Ref * ref){
			fangCheckBox->setSelected(true);
			allCheckBox->setSelected(false);
			gongCheckBox->setSelected(false);

		});
		auto leftBack = yingXiongLayout->getChildByName("back_icon");
		leftBack->setTag(MenuType::YXLeftBack);
		eventDispatcher->addEventListenerWithSceneGraphPriority(backEvent->clone(), leftBack);
		yingXiongLayout->setVisible(true);
		yingXiongLayout->setScale(0);
		yingXiongLayout->runAction(Sequence::create(ScaleTo::create(0.2f, 1.0f), CallFunc::create(CC_CALLBACK_0(MenuScene::onYingXiongLanClick, this)), NULL));
		break;
	}
	case MenuType::BeiBao:{
		//物品列表
		layer1->setVisible(true);
		CheckBox * allCheckBox = static_cast<CheckBox *>(beiBaoLayout->getChildByName("BeiBaoAll"));
		auto zbCheckBox = static_cast<CheckBox *>(beiBaoLayout->getChildByName("BeiBaoZhuangBei"));
		auto xhpCheckBox = static_cast<CheckBox *>(beiBaoLayout->getChildByName("BeiBaoXiaoHaoPin"));
		allCheckBox->setSelected(true);
		allCheckBox->addClickEventListener([allCheckBox, zbCheckBox, xhpCheckBox](Ref * ref){
			allCheckBox->setSelected(true);
			zbCheckBox->setSelected(false);
			xhpCheckBox->setSelected(false);

		});
		zbCheckBox->setSelected(false);
		zbCheckBox->addClickEventListener([allCheckBox, zbCheckBox, xhpCheckBox](Ref * ref){
			zbCheckBox->setSelected(true);
			allCheckBox->setSelected(false);
			xhpCheckBox->setSelected(false);

		});
		xhpCheckBox->setSelected(false);
		xhpCheckBox->addClickEventListener([allCheckBox, zbCheckBox, xhpCheckBox](Ref * ref){
			xhpCheckBox->setSelected(true);
			allCheckBox->setSelected(false);
			zbCheckBox->setSelected(false);

		});
		auto leftBack = beiBaoLayout->getChildByName("back_icon");
		leftBack->setTag(MenuType::BBLeftBack);
		eventDispatcher->addEventListenerWithSceneGraphPriority(backEvent->clone(), leftBack);
		beiBaoLayout->setVisible(true);
		beiBaoLayout->runAction(Sequence::create(ScaleTo::create(0.2f, 1.0f), CallFunc::create(CC_CALLBACK_0(MenuScene::onBeiBaoLanClick, this)), NULL));

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
	case MenuType::YXLeftBack:{
		auto sequence = Sequence::create(ScaleTo::create(0.3f, 0.0f), CallFunc::create([&](){
			layer1->setVisible(false);
			yingXiongLayout->setVisible(false);
			auto leftBack = this->getChildByTag(MenuType::YXLeftBack);
			eventDispatcher->removeEventListenersForTarget(leftBack);
			this->removeChild(leftBack);
			this->removeChild(yingXiongLayout);
			this->removeChild(layer1);
		}), NULL);
		yingXiongLayout->runAction(sequence);
		break;
	}
	case MenuType::BBLeftBack:{
		auto sequence = Sequence::create(ScaleTo::create(0.3f, 0.0f), CallFunc::create([&](){
			layer1->setVisible(false);
			beiBaoLayout->setVisible(false);
			auto leftBack = this->getChildByTag(MenuType::BBLeftBack);
			eventDispatcher->removeEventListenersForTarget(leftBack);
			this->removeChild(leftBack);
			this->removeChild(beiBaoLayout);
			this->removeChild(layer1);
		}), NULL);
		beiBaoLayout->runAction(sequence);
		break;
	}

	}
}
//弹出个人信息
void MenuScene::onInfoLayoutClick(Ref * ref){
	layer1->setVisible(true);
	settingLayout->setVisible(true);
	settingLayout->setScale(0);
	auto sequence = Sequence::create(ScaleTo::create(0.3f, 1.0f), NULL);
	settingLayout->runAction(sequence);
	//人物详细信息 返回按钮
	auto node = settingLayout->getChildByName("back_info_icon");
	node->setTag(MenuType::SettingRightBack);
	eventDispatcher->addEventListenerWithSceneGraphPriority(backEvent->clone(), node);
	CheckBox * yinYueBtn = static_cast<CheckBox *>(settingLayout->getChildByName("yinyue_panel")->getChildByName("yinyue_cb"));
	UserDefault * user = UserDefault::getInstance();
	if (user->getBoolForKey(MUSIC_KEY, true)){
		yinYueBtn->setSelected(true);
	}
	else{
		yinYueBtn->setSelected(false);
	}
	yinYueBtn->addClickEventListener([yinYueBtn, user](Ref * ref){
		if (user->getBoolForKey(MUSIC_KEY, true)){
			user->setBoolForKey(MUSIC_KEY, false);
			SimpleAudioEngine::getInstance()->stopBackgroundMusic("main_scene.mp3");
			yinYueBtn->setSelected(false);
		}
		else{
			user->setBoolForKey(MUSIC_KEY, true);
			SimpleAudioEngine::getInstance()->playBackgroundMusic("main_scene.mp3", true);
			yinYueBtn->setSelected(true);
		}
	});

	CheckBox * yinXiaoBtn = static_cast<CheckBox *>(settingLayout->getChildByName("yinxiao_panel")->getChildByName("yinxiao_cb"));
	if (user->getBoolForKey(SOUND_KEY, true)){
		yinXiaoBtn->setSelected(true);
	}
	else{
		yinXiaoBtn->setSelected(false);
	}
	yinXiaoBtn->addClickEventListener([yinXiaoBtn, user](Ref * ref){
		if (user->getBoolForKey(SOUND_KEY, true)){
			user->setBoolForKey(SOUND_KEY, false);
			yinXiaoBtn->setSelected(false);
		}
		else{
			user->setBoolForKey(SOUND_KEY, true);
			yinXiaoBtn->setSelected(true);
		}
	});

	Button * loginOut = static_cast<Button *>(settingLayout->getChildByName("login_out_btn"));
	loginOut->addClickEventListener([](Ref * ref){
		Director::getInstance()->end();
	});
	if (userInfo != nullptr){
		ImageView * infoIcon = static_cast<ImageView *>(settingLayout->getChildByName("InfoIcon"));
		Text * name = static_cast<Text *>(settingLayout->getChildByName("InfoName"));
		Text * info = static_cast<Text *>(settingLayout->getChildByName("my_info"));
		infoIcon->loadTexture(userInfo->getIcon(), TextureResType::LOCAL);
		name->setString(userInfo->getName());
		std::string infoStr;
		infoStr = String::createWithFormat("Level: %d \nExp: %d/%d \nHP: %d \nDEF: %d \nHeroCount: %d \nActCount: %d ",
			userInfo->getLevel(), userInfo->getEXP(), userInfo->getEXPNed(), userInfo->getHP(), userInfo->getDEF(), userInfo->getHeroesCount(), userInfo->getACT())->getCString();
		info->setString(infoStr);
	}

}

void MenuScene::onAddGoldClick(Ref * ref){


}
void MenuScene::onAddZuanShiClick(Ref * ref){


}
void MenuScene::onAddTiLiClick(Ref * ref){


}

void MenuScene::onOtherBtnClick(Ref * ref){
	//英雄，物品任务 点击
	Layout * layout = static_cast<Layout *>(ref);
	auto sequence = Sequence::create(ScaleTo::create(0.1f, 0.9f), ScaleTo::create(0.2f, 1.1f), ScaleTo::create(0.1f, 1.0f), CallFuncN::create(CC_CALLBACK_1(MenuScene::downEnd, this)), NULL);
	layout->runAction(sequence);
}

//英雄列表
void MenuScene::onYingXiongLanClick(){
	Layout * heroLayout = static_cast<Layout *>(yingXiongLayout->getChildByName("HeroScrollViewL"));
	yingXiongList = static_cast<ScrollView *>(heroLayout->getChildByName("HeroScrollView"));
	int heroSize = mHeroes.size();
	int contantH = (20 + 170) * ((int)(heroSize / 2) + 1);
	contantH = contantH > 480 ? contantH : 480;
	Size size;
	size.height = contantH;
	size.width = 700;
	yingXiongList->setInnerContainerSize(size);
	yingXiongList->setClippingEnabled(true);
	yingXiongList->setScrollBarEnabled(false);
	//设置 标志 类型 tag
	for (int i = 0; i < heroSize; i++){
		auto node = CSLoader::createNode("HeroListItemLayer.csb");
		Layout * itemLayout = static_cast<Layout *>(node->getChildByName("HeroListItemL"));
		Text * heroLevel = static_cast<Text *>(itemLayout->getChildByName("HeroLevel"));
		auto h = mHeroes.at(i);
		std::string level = String::createWithFormat("LV%d", h->getLevel())->getCString();
		std::string title = mHeroes.at(i)->getTitle();
		heroLevel->setString(level);
		Text * heroName = static_cast<Text *>(itemLayout->getChildByName("HeroName"));
		heroName->setString(title);
		switch (h->getHTag()){
		case 0:
			itemLayout->getChildByName("HeroTag1")->setVisible(true);
			itemLayout->getChildByName("HeroTag2")->setVisible(false);
			itemLayout->getChildByName("HeroTag3")->setVisible(false);
			break;
		case 1:
			itemLayout->getChildByName("HeroTag1")->setVisible(false);
			itemLayout->getChildByName("HeroTag2")->setVisible(true);
			itemLayout->getChildByName("HeroTag3")->setVisible(false);
			break;
		case 2:
			itemLayout->getChildByName("HeroTag1")->setVisible(false);
			itemLayout->getChildByName("HeroTag2")->setVisible(false);
			itemLayout->getChildByName("HeroTag3")->setVisible(true);
			break;
		default:
			itemLayout->getChildByName("HeroTag1")->setVisible(false);
			itemLayout->getChildByName("HeroTag2")->setVisible(false);
			itemLayout->getChildByName("HeroTag3")->setVisible(false);
			break;
		}

		Layout * iconLayout = static_cast<Layout *>(itemLayout->getChildByName("HeroIconL"));
		ImageView * icon = static_cast<ImageView *>(iconLayout->getChildByName("HeroIcon"));
		icon->loadTexture(mHeroes.at(i)->getIcon());
		node->setScale(1.7f);
		node->setPosition(Vec2(i % 2 * (340 + 20), contantH - 170 - (int)(i / 2) * (20 + 170)));
		yingXiongList->addChild(node);
	}

}
//背包
void MenuScene::onBeiBaoLanClick(){
	Layout * wupinLayout = static_cast<Layout *>(beiBaoLayout->getChildByName("WuPinScrollViewL"));
	wuPinList = static_cast<ScrollView *>(wupinLayout->getChildByName("WuPinScrollView"));
	int wupinSize = mWuPins.size();
	int rowSize = 5;
	int contantH = (80 + 20) * ((int)(wupinSize / rowSize) + 1);
	contantH = contantH > 480 ? contantH : 480;
	
	wuPinList->setInnerContainerSize(Size(520, contantH));
	wuPinList->setClippingEnabled(true);
	wuPinList->setScrollBarEnabled(false);
	for (int i = 0; i < wupinSize; i++){
		WuPin * item = mWuPins.at(i);
		Layout * itemLayout = Layout::create();
		itemLayout->setBackGroundImage("wupin_icon_bg.png");
		itemLayout->setContentSize(Size(80, 80));
		itemLayout->setAnchorPoint(Vec2(0, 0));
		itemLayout->setLayoutType(Layout::Type::ABSOLUTE);
		itemLayout->setBackGroundColorType(Layout::BackGroundColorType::NONE);
		ImageView * wupinIcon = ImageView::create();
		wupinIcon->setContentSize(Size(60, 60));
		wupinIcon->setAnchorPoint(Vec2(0.5, 0.5));
		wupinIcon->setPosition(Vec2(40,40));
		wupinIcon->loadTexture(item->getIcon());
		Text * text = Text::create();
		text->setFontSize(22);
		text->setString(String::createWithFormat("%d",item->getNum())->getCString());
		text->setAnchorPoint(Vec2(1,0));
		text->setPosition(Vec2(70, 5));
		itemLayout->addChild(wupinIcon);
		itemLayout->addChild(text);
		itemLayout->setPosition(Vec2(i % 5 * (80 + 20), contantH - 80 - (80 + 20) * (int)(i / 5)));
		wuPinList->addChild(itemLayout);
	}

}

//连网
void MenuScene::onConnect(SIOClient* client){
	log(" MenuScene:: onConnect");
	

}
void MenuScene::onMessage(SIOClient* client, const std::string& data){
	//log(" MenuScene:: onMessage data = %s",data.c_str());
}
void MenuScene::onClose(SIOClient* client){
	log(" MenuScene:: onClose");
	isLogin = false;
}
void MenuScene::onError(SIOClient* client, const std::string& data){
	log(" MenuScene:: onError data = %s", data.c_str());
	isLogin = false;
}
//回调
void MenuScene::getUserInfo(SIOClient* client, const std::string& data){
	userInfo = UserInfo::create();
	userInfo->retain();
	rapidjson::Document document;
	document.Parse<0>(data.c_str());
	if (!document.HasParseError()){
		int resultCode = document["resultCode"].GetInt();
		if (resultCode == ResultCodeSuccess){
			isLogin = true;
			const rapidjson::Value & data = document["resultData"];
			userInfo->setId(data["_fk"].GetString());
			userInfo->setLevel(data["level"].GetInt());
			userInfo->setTiLiNum(data["tiLiNum"].GetInt());
			userInfo->setTiLiNumCount(data["tiLiNumCount"].GetInt());
			userInfo->setZuanShiNum(data["zuanShiNum"].GetInt());
			userInfo->setGlodNum(data["glodNum"].GetInt());
			userInfo->setIcon(data["icon"].GetString());
			userInfo->setName(data["name"].GetString());
			userInfo->setEXP(data["expNow"].GetInt());
			userInfo->setEXPNed(data["expNed"].GetInt());
			userInfo->setHP(data["hp"].GetInt());
			userInfo->setDEF(data["def"].GetInt());
			userInfo->setMP(data["mp"].GetInt());
			userInfo->setHeroesCount(data["heroesCount"].GetInt());
			userInfo->setACT(data["act"].GetInt());
		}
	}
	//LayoutInfo    
	Layout * infoLayout = static_cast<Layout *>(sceneNode->getChildByName("LayoutInfo"));
	infoLayout->addClickEventListener(CC_CALLBACK_1(MenuScene::onInfoLayoutClick, this));
	if (userInfo != nullptr){
		ImageView * infoIcon = static_cast<ImageView *> (infoLayout->getChildByName("InfoIcon"));
		Text * infoName = static_cast<Text *>(infoLayout->getChildByName("InfoName"));
		Text * infoLevel = static_cast<Text *>(infoLayout->getChildByName("dengji_text"));
		infoName->setString(userInfo->getName());
		infoLevel->setString(String::createWithFormat("%d", userInfo->getLevel())->getCString());
		infoIcon->loadTexture(userInfo->getIcon(), TextureResType::LOCAL);
		Text * goldText = static_cast<Text *>(sceneNode->getChildByName("gold_panel")->getChildByName("gole_text"));
		goldText->setString(String::createWithFormat("%d",userInfo->getGlodNum())->getCString());
		Text * zhuanShiText = static_cast<Text *>(sceneNode->getChildByName("zuanshi_panel")->getChildByName("zuanshi_text"));
		zhuanShiText->setString(String::createWithFormat("%d", userInfo->getZuanShiNum())->getCString());
		Text * tiLiText = static_cast<Text *>(sceneNode->getChildByName("tili_panel")->getChildByName("tili_text"));
		tiLiText->setString(String::createWithFormat("%d/%d", userInfo->getTiLiNum(), userInfo->getTiLiNumCount())->getCString());
		//加载英雄列表
		__String * content = __String::createWithFormat("{\"_id\":\"%s\"}",userInfo->getId().c_str());
		sioClient->emit(EmitEvent_HeroList, content->getCString());
		//加载物品列表
		content = __String::createWithFormat("{\"_id\":\"%s\"}", userInfo->getId().c_str());
		sioClient->emit(EmitEvent_WuPinList, content->getCString());
	}

}

void MenuScene::getHeroList(SIOClient* client, const std::string& data){
	rapidjson::Document document;
	document.Parse<0>(data.c_str());
	if (!document.HasParseError()){
		int resultCode = document["resultCode"].GetInt();
		if (resultCode == ResultCodeSuccess){
			mHeroes.clear();
			const rapidjson::Value & data = document["resultData"];
			for (auto item = data.Begin(); item != data.End();item++){
				const rapidjson::Value & heroInfo = *item;
				Heroes * hero = Heroes::create();
				hero->retain();
				if (heroInfo.HasMember("isBoss")){
					bool isBoss = heroInfo["isBoss"].GetBool();
					hero->setType(isBoss ? HeroType::Boss : HeroType::Hero);
				}
				else{
					hero->setType(HeroType::Hero);
				}
				if (heroInfo.HasMember("type")){
					int atkType = heroInfo["type"].GetInt();
					hero->setAtkType((HeroAtkType)atkType);
				}
				else{
					hero->setAtkType(HeroAtkType::Other);
				}
				hero->setId(heroInfo["id"].GetInt());
				hero->set_FK(heroInfo["_fk"].GetString());
				hero->setTitle(heroInfo["title"].GetString());
				hero->setName(heroInfo["name"].GetString());
				hero->setDesc(heroInfo["desc"].GetString());
				hero->setHP(heroInfo["hp"].GetInt());
				hero->setPAtk(heroInfo["pAtk"].GetInt());
				hero->setSAtk(heroInfo["sAtk"].GetInt());
				hero->setPDef(heroInfo["pDef"].GetInt());
				hero->setSDef(heroInfo["sDef"].GetInt());
				hero->setRound(heroInfo["round"].GetInt());
				hero->setAAtk(heroInfo["aAtk"].GetInt());
				hero->setDoubleAtk(heroInfo["double"].GetDouble());
				hero->setSkill(heroInfo["skill"].GetInt());
				hero->setIcon(heroInfo["icon"].GetString());
				hero->setLevel(heroInfo["level"].GetInt());
				hero->setEXP(heroInfo["exp"].GetInt());
				hero->setHTag(heroInfo["tag"].GetInt());
				hero->setMaxEXP(heroInfo["maxExp"].GetInt());
				if (heroInfo.HasMember("atkEffect")){
					hero->setAtkEffect(heroInfo["atkEffect"].GetString());
				}
				if (heroInfo.HasMember("skillEffect")){
					hero->setSkillEffect(heroInfo["skillEffect"].GetString());
				}
				mHeroes.push_back(hero);
			}
		}
	}
}

void MenuScene::getWuPinList(SIOClient* client, const std::string& data){
	rapidjson::Document document;
	document.Parse<0>(data.c_str());
	if (!document.HasParseError()){
		int resultCode = document["resultCode"].GetInt();
		if (resultCode == ResultCodeSuccess){
			mWuPins.clear();
			const rapidjson::Value & data = document["resultData"];
			for (auto item = data.Begin(); item != data.End(); item++){
				const rapidjson::Value & wuPinInfo = *item;
				WuPin * wupin = WuPin::create();
				wupin->retain();
				wupin->setId(wuPinInfo["id"].GetInt());
				wupin->setTitle(wuPinInfo["title"].GetString());
				wupin->setName(wuPinInfo["name"].GetString());
				wupin->setDesc(wuPinInfo["desc"].GetString());
				wupin->setIcon(wuPinInfo["icon"].GetString());
				wupin->setNum(wuPinInfo["num"].GetInt());
				const rapidjson::Value & proterties = wuPinInfo["proterties"];
				if (proterties.HasMember("hp")){
					const rapidjson::Value & hp = proterties["hp"];
					wupin->proterties["hp"] = hp.GetInt();
				}
				if (proterties.HasMember("sAtk")){
					const rapidjson::Value & sAtk = proterties["sAtk"];
					wupin->proterties["sAtk"] = sAtk.GetInt();
				}
				if (proterties.HasMember("pDef")){
					const rapidjson::Value & pDef = proterties["pDef"];
					wupin->proterties["pDef"] = pDef.GetInt();
				}
				if (proterties.HasMember("sDef")){
					const rapidjson::Value & sDef = proterties["sDef"];
					wupin->proterties["sDef"] = sDef.GetInt();
				}
				if (proterties.HasMember("round")){
					const rapidjson::Value & round = proterties["round"];
					wupin->proterties["round"] = round.GetInt();
				}
				if (proterties.HasMember("aAtk")){
					const rapidjson::Value & aAtk = proterties["aAtk"];
					wupin->proterties["aAtk"] = aAtk.GetInt();
				}
				if (proterties.HasMember("double")){
					const rapidjson::Value & doubleAtk = proterties["double"];
					wupin->proterties["double"] = doubleAtk.GetInt();
				}
				mWuPins.push_back(wupin);
			}
		}
	}
}

void MenuScene::updateHeroInfo(SIOClient* client, const std::string& data){

}

void MenuScene::updateWuPinNum(SIOClient* client, const std::string& data){

}

void MenuScene::onEnterTransitionDidFinish(){
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY, true)){
		SimpleAudioEngine::getInstance()->playBackgroundMusic("main_scene.mp3", true);
	}
	runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=](){
		__String * content = __String::createWithFormat("{\"username\":\"%s\" , \"userpwd\" : \"%s\"}",
			userName, userPwd);
		sioClient->emit(EmitEvent_UserInfo, content->getCString());
	}), DelayTime::create(1), CallFunc::create([=](){
		//添加一个英雄
		//__String * content = __String::createWithFormat("{\"_id\":\"%s\" , \"heroId\":2001}", userInfo->getId().c_str());
		//sioClient->emit(EmitEvent_UpdateHero, content->getCString());

		//content = __String::createWithFormat("{\"_id\":\"%s\" , \"heroId\":2002}", userInfo->getId().c_str());
		//sioClient->emit(EmitEvent_UpdateHero, content->getCString());

		//content = __String::createWithFormat("{\"_id\":\"%s\" , \"heroId\":2003}", userInfo->getId().c_str());
		//sioClient->emit(EmitEvent_UpdateHero, content->getCString());

		//content = __String::createWithFormat("{\"_id\":\"%s\" , \"heroId\":6001}", userInfo->getId().c_str());
		//sioClient->emit(EmitEvent_UpdateHero, content->getCString());

		////添加一个物品
		//content = __String::createWithFormat("{\"_id\":\"%s\", \"wuPinId\":4001}", userInfo->getId().c_str());
		//sioClient->emit(EmitEvent_UpdateWuPinNum, content->getCString());

	}), NULL));
}

void MenuScene::cleanup(){
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY, true)){
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("main_scene.mp3");
	}
}

void MenuScene::onExit(){
	Layer::onExit();
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY, true)){
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("main_scene.mp3");
	}
}

MenuScene::~MenuScene(){
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->getScheduler()->unscheduleAll();
	Director::getInstance()->getTextureCache()->removeAllTextures();
	SpriteFrameCache::getInstance()->removeSpriteFrames();
	//SimpleAudioEngine::getInstance()->
	//AnimationCache::getInstance()->
	if (userInfo != nullptr){
		userInfo->release();
		for (auto hero = mHeroes.begin(); hero != mHeroes.end(); hero++){
			(*hero)->release();
			(*hero) = nullptr;
		}
		for (auto wupin = mWuPins.begin(); wupin != mWuPins.end(); wupin++){
			(*wupin)->release();
			(*wupin) = nullptr;
		}
		userInfo = nullptr;
	}
}

std::string MenuScene::getCurrentTime(){
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int year = tm->tm_year + 1900;//年
	int month = tm->tm_mon + 1;//月
	int day = tm->tm_mday;//日
	__String * mTime = __String::createWithFormat("%d - %d - %d", year, month, day);
	return mTime->getCString();
}
