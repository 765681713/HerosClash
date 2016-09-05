#include "GuanQiaScene.h"

Scene * GuanQiaScene::createScene(UserInfo * info, std::vector<Heroes *> heroes){
	auto scene = Scene::create();
	auto layer = GuanQiaScene::create();
	layer->mInfo = info;
	layer->setHeroes(heroes);
	scene->addChild(layer);
	return scene;
}

bool GuanQiaScene::init(){
	if (!Layer::init()){
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto guanQiaNode = CSLoader::createNode("GuanQia.csb");
	guanQiaNode->setPosition(Vec2::ZERO);
	guanQiaNode->setAnchorPoint(Vec2::ZERO);
	this->addChild(guanQiaNode);
	mPrepareLayout = static_cast<Layout *>(guanQiaNode->getChildByName("PrepareLayout"));
	mPrepareLayout->setVisible(false);
	prepareLayout = static_cast<Layout *>(mPrepareLayout->getChildByName("PrepareGame")->getChildByName("PrepareGameL"));
	myHero1 = static_cast<Layout *>(prepareLayout->getChildByName("MyHero1"));
	myHero2 = static_cast<Layout *>(prepareLayout->getChildByName("MyHero2"));
	myHero3 = static_cast<Layout *>(prepareLayout->getChildByName("MyHero3"));
	myHero4 = static_cast<Layout *>(prepareLayout->getChildByName("MyHero4"));
	myHero5 = static_cast<Layout *>(prepareLayout->getChildByName("MyHero5"));
	auto startGameBtn = static_cast<Button *>(prepareLayout->getChildByName("GameStartBtn"));
	startGameBtn->addClickEventListener(CC_CALLBACK_1(GuanQiaScene::startGame, this));
	auto backBtn = static_cast<Button *>(prepareLayout->getChildByName("Back_icon"));
	backBtn->addClickEventListener(CC_CALLBACK_1(GuanQiaScene::preBack, this));
	mPageView = static_cast<PageView *>(guanQiaNode->getChildByName("GuanQiaPageView"));
	//mPageView->setVisible(false);
	mGuanQia1 = static_cast<Layout *>(mPageView->getChildByName("GuanQia1"));
	mGuanQia2 = static_cast<Layout *>(mPageView->getChildByName("GuanQia2"));
	mTitle = static_cast<Text *>(guanQiaNode->getChildByName("GuanQianTitle"));
	Button * mGuanQiaBackBtn = static_cast<Button *>(guanQiaNode->getChildByName("BackBtn"));
	mGuanQiaBackBtn->addClickEventListener(CC_CALLBACK_1(GuanQiaScene::guanQiaBack, this));
	mGuanQiaBiaoZhi = static_cast<Layout *>(guanQiaNode->getChildByName("GuanQiaBiaoZhi"));
	decodeGuanQiaJson();
	addListener(mCurrentIndexPage);


	return true;
}

bool GuanQiaScene::onTouchBegan(Touch *pTouch, Event *pEvent){
	auto targt = pEvent->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(pTouch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		targt->runAction(ScaleTo::create(0.1f, 0.7f));
		return true;
	}
	return false;
}
void GuanQiaScene::onTouchEnded(Touch *pTouch, Event *pEvent){
	auto targt = pEvent->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(pTouch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		auto sequence = Sequence::create(ScaleTo::create(0.2f, 0.9f), ScaleTo::create(0.1f, 0.8f), CallFuncN::create(CC_CALLBACK_1(GuanQiaScene::downEnd, this)), NULL);
		targt->runAction(sequence);
	}
	else{
		targt->runAction(ScaleTo::create(0.1f, 0.8f));
	}

}

bool GuanQiaScene::onBoosGQTouchBegan(Touch *pTouch, Event *pEvent){
	auto targt = pEvent->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(pTouch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		targt->runAction(ScaleTo::create(0.1f, 0.4f));
		return true;
	}
	return false;

}
void GuanQiaScene::onBoosGQTouchEnded(Touch *pTouch, Event *pEvent){
	auto targt = pEvent->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(pTouch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		auto sequence = Sequence::create(ScaleTo::create(0.2f, 0.6f), ScaleTo::create(0.1f, 0.5f), CallFuncN::create(CC_CALLBACK_1(GuanQiaScene::downEnd, this)), NULL);
		targt->runAction(sequence);
	}
	else{
		targt->runAction(ScaleTo::create(0.1f, 0.5f));
	}
}

void GuanQiaScene::downEnd(Node *  node){
	int id = node->getTag();
	mGuanQiaItems = mGuanQias.at(mCurrentIndexPage)->getContent();

	for (auto item = mGuanQiaItems.begin(); item != mGuanQiaItems.end(); item++){
		if (id == (*item)->getId()){
			mCurrentItem = (*item);
			break;
		}
	}
	if (mCurrentItem != nullptr && mInfo != nullptr && mHeroes.size() > 0){
		mPrepareLayout->setVisible(true);
		myHero1->removeAllChildren();
		myHero1->setTag(-1);
		myHero2->removeAllChildren();
		myHero2->setTag(-2);
		myHero3->removeAllChildren();
		myHero3->setTag(-3);
		myHero4->removeAllChildren();
		myHero4->setTag(-4);
		myHero5->removeAllChildren();
		myHero5->setTag(-5);
		ListView * heroListView = static_cast<ListView *>(prepareLayout->getChildByName("GuanQiaHeroListView"));
		heroListView->removeAllChildren();
		heroListView->setScrollBarEnabled(false);
		int contantW = mHeroes.size()*(100 + 10);
		contantW = contantW > 750 ? contantW : 750;
		Size size;
		size.height = 110;
		size.width = contantW;
		heroListView->setInnerContainerSize(size);
		heroListView->setClippingEnabled(true);
		//heroListView->addEventListener((ListView::ccListViewCallback)CC_CALLBACK_2(GuanQiaScene::onItemClickListener, this));
		auto listListener = EventListenerTouchOneByOne::create();
		listListener->setSwallowTouches(true);
		listListener->onTouchBegan = CC_CALLBACK_2(GuanQiaScene::onItemClickListener,this);
		for (unsigned int i = 0; i < mHeroes.size(); i++){
			Layout * layout = Layout::create();
			layout->setContentSize(Size(100, 100));
			layout->setBackGroundImage("HeroListItem/hero_list_icon_bg.png");
			Sprite * icon = Sprite::create(mHeroes.at(i)->getIcon());
			icon->setContentSize(Size(50, 50));
			icon->setAnchorPoint(Vec2(0.5, 0.5));
			icon->setPosition(Vec2(50, 50));
			icon->setScale(1.3f);
			layout->addChild(icon);
			layout->setTag(i);
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listListener->clone(), layout);
			heroListView->pushBackCustomItem(layout);
		}
		mCurrentMonsters.clear();
		decodeMonsterJson();
		for (unsigned int i = 0; i < mCurrentMonsters.size(); i++){
			Layout * masterLayout;
			if (i == 0){
				masterLayout = static_cast<Layout *>(prepareLayout->getChildByName("Monster1"));
			}
			else if (i == 1){
				masterLayout = static_cast<Layout *>(prepareLayout->getChildByName("Monster2"));
			}
			else if (i == 2){
				masterLayout = static_cast<Layout *>(prepareLayout->getChildByName("Monster3"));
			}
			else{
				break;
			}
			Sprite * icon = Sprite::create(mCurrentMonsters.at(i)->getIcon());
			icon->setContentSize(Size(50, 50));
			icon->setScale(1.3f);
			icon->setAnchorPoint(Vec2(0.5, 0.5));
			icon->setPosition(Vec2(45, 45));
			masterLayout->addChild(icon);
		}
		mWuPins.clear();
		decodeWuPinJson();
		for (unsigned int i = 0; i < mWuPins.size(); i++){
			auto wuPinL = static_cast<Layout *>(prepareLayout->getChildByName("WuPin1"))->clone();
			Vec2 position = wuPinL->getPosition();
			position.x += 70 * i;
			Sprite * icon = Sprite::create(mWuPins.at(i)->getIcon());
			icon->setContentSize(Size(50, 50));
			icon->setScale(1.5f);
			icon->setAnchorPoint(Vec2(0.5, 0.5));
			icon->setPosition(Vec2(60, 60));
			wuPinL->setPosition(position);
			wuPinL->addChild(icon);
			prepareLayout->addChild(wuPinL);
		}
	}
}

void GuanQiaScene::startGame(Ref *  node){
	decodeAIJson();
	mCurrentHeroes.clear();
	if (myHero1->getTag() < 0 || myHero2->getTag() < 0 || myHero3->getTag() < 0){
		return;
	}
	mCurrentHeroes.push_back(mHeroes.at(myHero1->getTag()));
	mCurrentHeroes.push_back(mHeroes.at(myHero2->getTag()));
	mCurrentHeroes.push_back(mHeroes.at(myHero3->getTag()));
	/*if (myHero4->getTag() >= 0){
		mCurrentHeroes.push_back(mHeroes.at(myHero4->getTag()));
	}
	if (myHero5->getTag() >= 0){
		mCurrentHeroes.push_back(mHeroes.at(myHero5->getTag()));
	}*/
	////获取系统时间
	//struct timeval now;
	//gettimeofday(&now, NULL);
	////初始化随机种子
	////timeval是个结构体，里边有俩个变量，一个是以秒为单位的，一个是以微妙为单位的 
	//unsigned rand_seed = (unsigned)(now.tv_sec * 1000 + now.tv_usec / 1000);    //都转化为毫秒 
	//srand(rand_seed);
	auto gameScene = GameScene::createScene(mInfo, mCurrentHeroes, mCurrentMonsters, monsterAi);
	Director::getInstance()->pushScene(gameScene);
	mPrepareLayout->setVisible(false);

}

void GuanQiaScene::guanQiaBack(Ref *  node){
	Director::getInstance()->popScene();
}
void GuanQiaScene::preBack(Ref *  node){
	mPrepareLayout->setVisible(false);
	for (auto monster = mCurrentMonsters.begin(); monster != mCurrentMonsters.end(); monster++){
		(*monster)->release();
	}
	for (auto wupin = mWuPins.begin(); wupin != mWuPins.end(); wupin++){
		(*wupin)->release();
	}
}

void GuanQiaScene::nodeFadeAction(Node * node, int fade){
	auto nodes = node->getChildren();
	for (auto node : nodes){
		node->runAction(FadeTo::create(0.1f, fade));
	}
	node->runAction(FadeTo::create(0.1f, fade));
}

bool GuanQiaScene::onItemClickListener(Touch * touch, Event * event){
	auto targt = event->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(touch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		int tag = targt->getTag();
		if (myHero1->getTag() == tag){
			nodeFadeAction(targt,255);
			myHero1->removeAllChildren();
			myHero1->setTag(-1);
			return true;
		}
		else if(myHero2->getTag() == tag){
			nodeFadeAction(targt, 255);
			myHero2->removeAllChildren();
			myHero2->setTag(-2);
			return true;
		}
		else if (myHero3->getTag() == tag){
			nodeFadeAction(targt, 255);
			myHero3->removeAllChildren();
			myHero3->setTag(-3);
			return true;
		}
		else if (myHero4->getTag() == tag){
			nodeFadeAction(targt, 255);
			myHero4->removeAllChildren();
			myHero4->setTag(-4);
			return true;
		}
		else if (myHero5->getTag() == tag){
			nodeFadeAction(targt, 255);
			myHero5->removeAllChildren();
			myHero5->setTag(-5);
			return true;
		}
		auto hero = mHeroes.at(tag);
		if (hero->getType() == HeroType::Boss){
			Sprite * icon = Sprite::create(hero->getIcon());
			icon->setContentSize(Size(50, 50));
			icon->setAnchorPoint(Vec2(0.5, 0.5));
			icon->setPosition(Vec2(50, 50));
			icon->setScale(1.2f);
			if (myHero4->getTag() < 0){
				nodeFadeAction(targt, 125);
				myHero4->setTag(tag);
				myHero4->addChild(icon);
				return true;
			}
			else if (myHero5->getTag() < 0){
				nodeFadeAction(targt, 125);
				myHero4->setTag(tag);
				myHero4->addChild(icon);
				return true;
			}
		}
		else{
			Sprite * icon = Sprite::create(hero->getIcon());
			icon->setContentSize(Size(50, 50));
			icon->setAnchorPoint(Vec2(0.5, 0.5));
			icon->setPosition(Vec2(50, 50));
			icon->setScale(1.0f);
			if (myHero1->getTag() < 0){
				nodeFadeAction(targt, 125);
				myHero1->setTag(tag);
				myHero1->addChild(icon);
				return true;
			}
			else if(myHero2->getTag() < 0){
				nodeFadeAction(targt, 125);
				myHero2->setTag(tag);
				myHero2->addChild(icon);
				return true;
			}
			else if(myHero3->getTag() < 0){
				nodeFadeAction(targt, 125);
				myHero3->setTag(tag);
				myHero3->addChild(icon);
				return true;
			}
		}
		return true;
	}
	return false;
}

//void GuanQiaScene::onItemClickListener(Ref * pSender, ListView::EventType type){
//	switch (type) {
//	case ListView::EventType::ON_SELECTED_ITEM_START:{
//		ListView* listView = static_cast<ListView *>(pSender);
//		log("%d", listView->getCurSelectedIndex());
//
//		break;
//	
//	}
//		
//	case ListView::EventType::ON_SELECTED_ITEM_END:{
//		ListView* listView = static_cast<ListView *>(pSender);
//		log("%d", listView->getCurSelectedIndex());
//		break;
//	}
//	default:
//		break;
//	}
//	log(" onItemClickListener ");
//}


void GuanQiaScene::decodeMonsterJson(){
	std::vector<int> monsters = mCurrentItem->getMonster();
	Data data = FileUtils::getInstance()->getDataFromFile("Monster.json");
	std::string content = std::string((const char *)data.getBytes(), 0, data.getSize());
	rapidjson::Document document;
	document.Parse<0>(content.c_str());
	if (!document.HasParseError() && monsters.size() > 0){
		for (std::vector<int>::iterator mon = monsters.begin(); mon != monsters.end(); mon++){
			int id = *mon;
			std::string idStr = String::createWithFormat("%d", id)->getCString();
			if (document.HasMember(idStr.c_str())){
				const rapidjson::Value & monsterValue = document[idStr.c_str()];
				Monster * monster = Monster::create();
				monster->retain();
				const rapidjson::Value & id = monsterValue["id"];
				const rapidjson::Value & title = monsterValue["title"];
				const rapidjson::Value & name = monsterValue["name"];
				const rapidjson::Value & desc = monsterValue["desc"];
				const rapidjson::Value & hp = monsterValue["hp"];
				const rapidjson::Value & pAtk = monsterValue["pAtk"];
				const rapidjson::Value & sAtk = monsterValue["sAtk"];
				const rapidjson::Value & pDef = monsterValue["pDef"];
				const rapidjson::Value & sDef = monsterValue["sDef"];
				const rapidjson::Value & round = monsterValue["round"];
				const rapidjson::Value & aAtk = monsterValue["aAtk"];
				const rapidjson::Value & doubleAtk = monsterValue["double"];
				const rapidjson::Value & skill = monsterValue["skill"];
				const rapidjson::Value & icon = monsterValue["icon"];
				if (monsterValue.HasMember("isBoss")){
					bool isBoss = monsterValue["isBoss"].GetBool();
					monster->setType(isBoss ? HeroType::Boss : HeroType::Hero);
				}
				else{
					monster->setType(HeroType::Hero);
				}
				if (monsterValue.HasMember("type")){
					int atkType = monsterValue["type"].GetInt();
					monster->setAtkType((HeroAtkType)atkType);
				}
				else{
					monster->setAtkType(HeroAtkType::Other);
				}
				monster->setId(id.GetInt());
				monster->setTitle(title.GetString());
				monster->setName(name.GetString());
				monster->setDesc(desc.GetString());
				monster->setHP(hp.GetInt());
				monster->setPAtk(pAtk.GetInt());
				monster->setSAtk(sAtk.GetInt());
				monster->setPDef(pDef.GetInt());
				monster->setSDef(sDef.GetInt());
				monster->setRound(round.GetInt());
				monster->setAAtk(aAtk.GetInt());
				monster->setDoubleAtk(doubleAtk.GetDouble());
				monster->setSkill(skill.GetInt());
				monster->setIcon(icon.GetString());
				monster->setLevel(1);
				monster->setAtkEffect(monsterValue["atkEffect"].GetString());
				monster->setSkillEffect(monsterValue["skillEffect"].GetString());
				mCurrentMonsters.push_back(monster);
			}
		}
	}
}

void GuanQiaScene::decodeGuanQiaJson(){
	Data data = FileUtils::getInstance()->getDataFromFile("GuanQia.json");
	std::string content = std::string((const char *)data.getBytes(), 0, data.getSize());
	rapidjson::Document document;
	//解析关卡信息
	document.Parse<0>(content.c_str());
	if (!document.HasParseError()){
		//存储多章关卡
		mGuanQias = std::vector<GuanQiaBean *>();
		const rapidjson::Value & guanQias = document["GuanQia"];
		if (guanQias.IsArray()){
			for (unsigned int i = 0; i < guanQias.Size(); i++){
				const rapidjson::Value & item = guanQias[i];
				const rapidjson::Value & name = item["name"];
				//每章关卡
				mGuanQia = GuanQiaBean::create();
				mGuanQia->retain();
				mGuanQia->setName(name.GetString());
				const rapidjson::Value & content = item["content"];
				if (content.IsArray()){
					//每一小关
					mGuanQiaItems = std::vector < GuanQiaItemBean * >();
					for (unsigned int i = 0; i < content.Size(); i++){
						mGuanQiaItem = GuanQiaItemBean::create();
						mGuanQiaItem->retain();
						const rapidjson::Value & itemGQ = content[i];
						const rapidjson::Value & id = itemGQ["id"];
						mGuanQiaItem->setId(id.GetInt());
						const rapidjson::Value & nameGQ = itemGQ["name"];
						mGuanQiaItem->setName(nameGQ.GetString());
						const rapidjson::Value & title = itemGQ["title"];
						mGuanQiaItem->setTitle(title.GetString());
						const rapidjson::Value & desc = itemGQ["desc"];
						mGuanQiaItem->setDesc(desc.GetString());
						const rapidjson::Value & glod = itemGQ["glod"];
						mGuanQiaItem->setGlod(glod.GetInt());
						const rapidjson::Value & exp = itemGQ["exp"];
						mGuanQiaItem->setExp(exp.GetInt());
						if (itemGQ.HasMember("ai")){
							const rapidjson::Value & ai = itemGQ["ai"];
							mGuanQiaItem->setAI(ai.GetInt());
						}
						const rapidjson::Value & isfinish = itemGQ["isfinish"];
						mGuanQiaItem->setIsfinish(isfinish.GetBool());
						const rapidjson::Value & goods = itemGQ["goods"];
						//每一小关可能获取的物品
						if (goods.IsArray()){
							std::vector<int> gds;
							for (auto it = goods.Begin(); it != goods.End(); it++){
								const rapidjson::Value & good = *it;
								gds.push_back(good.GetInt());
							}
							mGuanQiaItem->setGoods(gds);
						}
						const rapidjson::Value & isboss = itemGQ["isboss"];
						mGuanQiaItem->setIsboss(isboss.GetBool());
						if (itemGQ.HasMember("monster")){
							//每一小关的怪物种类
							const rapidjson::Value & monster = itemGQ["monster"];
							if (monster.IsArray()){
								std::vector<int> monsters;
								for (auto it = monster.Begin(); it != monster.End(); it++){
									const rapidjson::Value & mon = *it;
									monsters.push_back(mon.GetInt());
								}
								mGuanQiaItem->setMonster(monsters);
							}
						}
						mGuanQiaItems.push_back(mGuanQiaItem);
					}
					mGuanQia->setContent(mGuanQiaItems);
					mGuanQias.push_back(mGuanQia);
				}
			}
		}
	}
}

void GuanQiaScene::decodeWuPinJson(){
	std::vector<int> wupins = mCurrentItem->getGoods();
	Data data = FileUtils::getInstance()->getDataFromFile("WuPin.json");
	std::string content = std::string((const char *)data.getBytes(), 0, data.getSize());
	rapidjson::Document document;
	document.Parse<0>(content.c_str());
	if (!document.HasParseError()){
		for (std::vector<int>::iterator wupin = wupins.begin(); wupin != wupins.end(); wupin++){
			int id = *wupin;
			std::string idStr = String::createWithFormat("%d", id)->getCString();
			if (document.HasMember(idStr.c_str())){
				const rapidjson::Value & wupinValue = document[idStr.c_str()];
				WuPin * wupin = WuPin::create();
				wupin->retain();
				const rapidjson::Value & id = wupinValue["id"];
				const rapidjson::Value & title = wupinValue["title"];
				const rapidjson::Value & name = wupinValue["name"];
				const rapidjson::Value & desc = wupinValue["desc"];
				const rapidjson::Value & icon = wupinValue["icon"];
				const rapidjson::Value & proterties = wupinValue["proterties"];
				wupin->setId(id.GetInt());
				wupin->setTitle(title.GetString());
				wupin->setName(name.GetString());
				wupin->setDesc(desc.GetString());
				wupin->setIcon(icon.GetString());
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

void GuanQiaScene::decodeAIJson(){
	int aiId = mCurrentItem->getAI();
	std::string idStr = String::createWithFormat("%d", aiId)->getCString();
	Data data = FileUtils::getInstance()->getDataFromFile("AI.json");
	std::string content = std::string((const char *)data.getBytes(), 0, data.getSize());
	rapidjson::Document document;
	document.Parse<0>(content.c_str());
	if (!document.HasParseError() && document.HasMember(idStr.c_str())){
		const rapidjson::Value & aiV = document[idStr.c_str()];
		monsterAi = AI::create();
		monsterAi->retain();
		//monsterAi->setId(aiV["id"].GetString());
		monsterAi->setLevel(aiV["level"].GetInt());
		monsterAi->setIcon(aiV["icon"].GetString());
		monsterAi->setName(aiV["name"].GetString());
		monsterAi->setHP(aiV["hp"].GetInt());
		monsterAi->setDEF(aiV["def"].GetInt());
		monsterAi->setMP(aiV["mp"].GetInt());
		monsterAi->setHeroesCount(aiV["heroesCount"].GetInt());
		monsterAi->setACT(aiV["act"].GetInt());
	}
}

void GuanQiaScene::onExit(){
	Layer::onExit();
	
}

void GuanQiaScene::addListener(int index){
	mGuanQiaCount = mGuanQiaCount - index * 18;
	xiaoGuanListener = EventListenerTouchOneByOne::create();
	xiaoGuanListener->setSwallowTouches(true);
	xiaoGuanListener->onTouchBegan = CC_CALLBACK_2(GuanQiaScene::onTouchBegan, this);
	xiaoGuanListener->onTouchEnded = CC_CALLBACK_2(GuanQiaScene::onTouchEnded, this);
	bossListener = EventListenerTouchOneByOne::create();
	bossListener->setSwallowTouches(true);
	bossListener->onTouchBegan = CC_CALLBACK_2(GuanQiaScene::onBoosGQTouchBegan, this);
	bossListener->onTouchEnded = CC_CALLBACK_2(GuanQiaScene::onBoosGQTouchEnded, this);
	EventDispatcher * event = Director::getInstance()->getEventDispatcher();
	mGuanQiaItems = mGuanQias.at(index)->getContent();
	int count = 1;
	for (auto item = mGuanQiaItems.begin(); item != mGuanQiaItems.end(); item++){
		if (mGuanQiaCount >= count){
			auto itemGuanQiaNode = mGuanQia1->getChildByName((*item)->getName());
			itemGuanQiaNode->setTag((*item)->getId());
			itemGuanQiaNode->setColor(Color3B(255, 255, 255));
			if ((*item)->getIsboss()){
				event->addEventListenerWithSceneGraphPriority(bossListener->clone(), itemGuanQiaNode);
			}
			else{
				event->addEventListenerWithSceneGraphPriority(xiaoGuanListener->clone(), itemGuanQiaNode);
			}
			count++;
		}
		else{
			break;
		}
	}
}

void GuanQiaScene::removeListener(){
	auto event = Director::getInstance()->getEventDispatcher();
	event->removeEventListener(xiaoGuanListener);
	event->removeEventListener(bossListener);
	xiaoGuanListener = nullptr;
	bossListener = nullptr;
}

GuanQiaScene::~GuanQiaScene(){
	removeListener();
	for (auto node = mGuanQias.begin(); node != mGuanQias.end(); node++){
		auto contents = (*node)->getContent();
		for (auto item = contents.begin(); item != contents.end(); item++){
			(*item)->release();
			(*item) = nullptr;
		}
		(*node)->release();
		(*node) = nullptr;
	}
	//战斗开始需要用到  不用release
	/*for (auto monster = mCurrentMonsters.begin(); monster != mCurrentMonsters.end(); monster++){
	(*monster)->release();
	}
	for (auto wupin = mWuPins.begin(); wupin != mWuPins.end(); wupin++){
	(*wupin)->release();
	}*/
}