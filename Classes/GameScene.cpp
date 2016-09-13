#include "GameScene.h"

Scene * GameScene::createScene(UserInfo * info, std::vector<Heroes *> heroes, std::vector<Monster *> monsters, AI * ai){
	auto scene = Scene::create();
	auto layer = new GameScene();
	layer->mUserInfo = info;
	layer->baseHeroes = heroes;
	layer->baseMonsters = monsters;
	layer->monstersAI = ai;
	if (layer && layer->init()){
		layer->autorelease();
		scene->addChild(layer);
	}
	else{
		CC_SAFE_DELETE(layer);
	}
	return scene;
}

bool GameScene::init(){
	if (!Layer::init()){
		return false;
	}
	int bgIndex = (int)(CCRANDOM_0_1() * 3);
	mSceneSize = Director::getInstance()->getVisibleSize();
	gameSceneNode = CSLoader::createNode("GameScene.csb");
	gameSceneNode->setAnchorPoint(Vec2::ZERO);
	gameSceneNode->setPosition(Vec2::ZERO);
	this->addChild(gameSceneNode);
	gameSceneLayout = static_cast<Layout *>(gameSceneNode->getChildByName("GameSceneL"));
	std::string bgName = String::createWithFormat("scene_bg_%d.png",bgIndex)->getCString();
	gameSceneLayout->setBackGroundImage(bgName);
	//ImageView * jiNeng1;
	leftWall = static_cast<Sprite *>(gameSceneLayout->getChildByName("game_left_wall"));
	rightWall = static_cast<Sprite *>(gameSceneLayout->getChildByName("game_reight_wall"));
	Layout * mInfo = static_cast<Layout *>(gameSceneNode->getChildByName("JueSeInfoL"));
	Sprite * jueSeIcon = static_cast<Sprite *>(mInfo->getChildByName("JueSeIcon"));
	jueSeIcon->initWithFile(mUserInfo->getIcon().c_str());
	jueSeIcon->setAnchorPoint(Vec2(0,0));
	mHPLoadingBar = static_cast<LoadingBar *>(mInfo->getChildByName("HPLoadingBar"));
	mMPLoadingBar = static_cast<LoadingBar *>(mInfo->getChildByName("MPLoadingBar"));
	mHPAtlas = static_cast<TextAtlas *>(mInfo->getChildByName("HPAtlas"));
	mMPAtlas = static_cast<TextAtlas *>(mInfo->getChildByName("MPAtlas"));
	mLevel = static_cast<Text *>(mInfo->getChildByName("JueSeLevel"));
	mLevel->setString(String::createWithFormat("LV%d",mUserInfo->getLevel())->getCString());
	Layout * monsterInfo = static_cast<Layout *>(gameSceneNode->getChildByName("MonsterInfoL"));
	Sprite * monsterIcon = static_cast<Sprite *>(monsterInfo->getChildByName("MIcon"));
	monsterIcon->initWithFile(monstersAI->getIcon().c_str());
	monsterIcon->setAnchorPoint(Vec2(1, 0));
	monsterHPLoadingBar = static_cast<LoadingBar *>(monsterInfo->getChildByName("HPLoadingBar"));
	monsterMPLoadingBar = static_cast<LoadingBar *>(monsterInfo->getChildByName("MPLoadingBar"));
	monsterHPAtlas = static_cast<TextAtlas *>(monsterInfo->getChildByName("HPAtlas"));
	monsterMPAtlas = static_cast<TextAtlas *>(monsterInfo->getChildByName("MPAtlas"));
	monsterLevel = static_cast<Text *>(monsterInfo->getChildByName("MLevel"));
	monsterLevel->setString(String::createWithFormat("LV%d", monstersAI->getLevel())->getCString());

	roundCountAtlas = static_cast<TextAtlas *>(gameSceneNode->getChildByName("RoundL")->getChildByName("RoundCountAtlas"));
	secTimeAtlas = static_cast<TextAtlas *>(gameSceneNode->getChildByName("SecTimeAtlas"));
	secTimeAtlas->setVisible(false);
	mActCountAtlas = static_cast<TextAtlas *>(gameSceneNode->getChildByName("JActL")->getChildByName("ActCountAtlas"));
	monsterActCountAtlas = static_cast<TextAtlas *>(gameSceneNode->getChildByName("MActL")->getChildByName("ActCountAtlas"));
	gameSceneNode->getChildByName("JActL")->setVisible(false);
	gameSceneNode->getChildByName("MActL")->setVisible(false);
	Layout * heroCountL = static_cast<Layout *>(gameSceneNode->getChildByName("MHeroCountL"));
	heroCountL->addClickEventListener(CC_CALLBACK_1(GameScene::heroCountLCallBack,this));
	mHeroAllCount = static_cast<TextAtlas *>(heroCountL->getChildByName("HeroCountAtlas"));
	monsterAllCount = static_cast<TextAtlas *>(gameSceneNode->getChildByName("MonsterCountL")->getChildByName("monsterCountAtlas"));
	//事件监听
	eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto gameSceneListener = EventListenerTouchOneByOne::create();
	gameSceneListener->setSwallowTouches(true);
	gameSceneListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	gameSceneListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	gameSceneListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	eventDispatcher->addEventListenerWithSceneGraphPriority(gameSceneListener, gameSceneLayout);
	//pause btn
	Sprite * pauseS = Sprite::create("game_scene_pause_off.png");
	pauseS->setAnchorPoint(Vec2(0.5,0.5));
	pauseS->setPosition(Vec2(mSceneSize.width / 2, mSceneSize.height / 9 * 8 - 10));
	gameSceneNode->addChild(pauseS);
	eventDispatcher->addEventListenerWithSceneGraphPriority(gameSceneListener->clone(), pauseS);
	//
	isAiRound = true;
	schedule(schedule_selector(GameScene::timeSchedule), 1.0f);
	//加载资源
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("HeroMuBei.plist", "HeroMuBei.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("EffectBg.plist", "EffectBg.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("roundChangeAni.plist","roundChangeAni.png");
	for (auto heros = baseHeroes.begin(); heros != baseHeroes.end(); heros++){
		std::string heroesFileName = String::createWithFormat("%s.plist", (*heros)->getName().c_str())->getCString();
		std::string atkEffectFileName = String::createWithFormat("%s.plist", (*heros)->getAtkEffect().c_str())->getCString();
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(heroesFileName.c_str());
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(atkEffectFileName.c_str());
	}
	for (auto monster = baseMonsters.begin(); monster != baseMonsters.end(); monster++){
		std::string heroesFileName = String::createWithFormat("%s.plist", (*monster)->getName().c_str())->getCString();
		std::string atkEffectFileName = String::createWithFormat("%s.plist", (*monster)->getAtkEffect().c_str())->getCString();
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(heroesFileName.c_str());
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(atkEffectFileName.c_str());
	}

	for (int i = 0; i < MAX_ROW; i++){
		for (int j = 0; j < MAX_COLUMN; j++){
			mHeroes[i*MAX_ROW + j] = nullptr;
		}
	}
	for (int i = 0; i < MAX_ROW; i++){
		for (int j = 0; j < MAX_COLUMN; j++){
			monsterHeros[i*MAX_ROW + j] = nullptr;
		}
	}
	//roundChange动画
	Animation * ani = Animation::create();
	for (int i = 0; i < 10; i++){
		std::string filePath = String::createWithFormat("ef_001_p_gl_%d.png",(336 + i))->getCString();
		ani->addSpriteFrame(SpriteFrame::create(filePath, Rect(0, 0, 128, 128)));
	}
	//ani->setDelayPerUnit(2.0f / 10.0f);   
	ani->setRestoreOriginalFrame(true);
	AnimationCache::getInstance()->addAnimation(ani, "randChangeEffect");
	mCurrentHP = mUserInfo->getHP();
	mCurrentMP = 5;
	mDEF = mUserInfo->getDEF();
	secCurrentTime = RoundTime;
	mActCount = mUserInfo->getACT();
	monsterCurrentHP = monstersAI->getHP();
	monsterCurrentMP = 5;
	monsterDEF = monstersAI->getDEF();
	monsterActCount = monstersAI->getACT();
	updateHeroCount();
	updateHPMP();
	updateHeroAct();
	return true;
}

void GameScene::onEnter(){
	Layer::onEnter();
	isMWinGame = false;
	isMonsterWinGame = false;
	this->scheduleUpdate();

}

void GameScene::onEnterTransitionDidFinish(){
	DelayTime * dTime = DelayTime::create(0);
	Sequence * action = Sequence::create(dTime, CallFunc::create(CC_CALLBACK_0(GameScene::initGame, this)), NULL);
	this->runAction(action);
	srand(time(NULL));

}

void GameScene::actRoundChage(){
	if (isRoundChangeing){
		return;
	}
	isRoundChangeing = true;
	secTimeAtlas->setVisible(false);
	LayerColor * colorLayer = LayerColor::create(Color4B(10, 10, 10, 155), mSceneSize.width, mSceneSize.height);
	colorLayer->setPosition(Vec2(0, mSceneSize.height ));
	colorLayer->setAnchorPoint(Vec2(0, 0));
	std::string title;
	if (!isAiRound){
		title = "The Enemy Action";
	}
	else{
		title = "My Our Action";
	}
	Sprite * sprite = Sprite::createWithSpriteFrameName("ef_001_p_gl_336.png");
	Animation * animation = AnimationCache::getInstance()->getAnimation("randChangeEffect");
	auto ani = Animate::create(animation);
	sprite->runAction(ani);
	
	sprite->setPosition(mSceneSize.width / 2, mSceneSize.height * 3 / 4);
	colorLayer->addChild(sprite);
	Label * titleL = Label::create();
	titleL->setSystemFontSize(66);
	titleL->setTextColor(Color4B(255, 0, 170, 255));
	titleL->setAnchorPoint(Vec2(0.5, 0.5));
	titleL->setPosition(mSceneSize.width / 2, mSceneSize.height / 2);
	titleL->setString(title);
	colorLayer->addChild(titleL);
	gameSceneNode->addChild(colorLayer, 100);
	colorLayer->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, -mSceneSize.height)), DelayTime::create(2),
		MoveBy::create(0.5f, Vec2(0, -mSceneSize.height)), CallFunc::create([=](){
		isRoundChangeing = false;
		updateUi();
		gameSceneNode->removeChild(colorLayer);
	}), NULL));
}

void GameScene::updateUi(){
	secCurrentTime = RoundTime;
	roundCount++;
	roundCountAtlas->setString(String::createWithFormat("%d", roundCount)->getCString());
	isAiRound = !isAiRound;
	mActCount = mUserInfo->getACT();
	monsterActCount = monstersAI->getACT();
	updateActTime();
	updateHeroCount();
	updateHPMP();
	updateHeroAct();
	secTimeAtlas->setVisible(true);
	if (isInit){
		runHeroPosition(isAiRound ? monsterHeros:mHeroes);
	}
	if (isAiRound){
		this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create([this](){
			this->schedule(schedule_selector(GameScene::aiSchedule), 1.0f);
		}), NULL));
	}
	else{
		unschedule(schedule_selector(GameScene::aiSchedule));
	}
}

void GameScene::updateActTime(){
	if (isAiRound){
		gameSceneNode->getChildByName("JActL")->setVisible(false);
		gameSceneNode->getChildByName("MActL")->setVisible(true);
	}
	else{
		gameSceneNode->getChildByName("MActL")->setVisible(false);
		gameSceneNode->getChildByName("JActL")->setVisible(true);
	}
	mActCountAtlas->setString(String::createWithFormat("%d", mActCount)->getCString());
	monsterActCountAtlas->setString(String::createWithFormat("%d", monsterActCount)->getCString());
}

void GameScene::updateHeroCount(){
	int mCount = mUserInfo->getHeroesCount() - existHeroCount;
	mHeroAllCount->setString(String::createWithFormat("%d", mCount > 0 ? mCount : 0)->getCString());
	int monsterCount = monstersAI->getHeroesCount() - existMonsterCount;
	monsterAllCount->setString(String::createWithFormat("%d", monsterCount > 0 ? monsterCount : 0)->getCString());
}

void  GameScene::updateHPMP(){
	if (mCurrentHP <= 0){
		mCurrentHP = 0;
		if (isMonsterWinGame){
			return;
		}
		isMonsterWinGame = true;
		mHPAtlas->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=](){
			unschedule(schedule_selector(GameScene::aiSchedule));
			unscheduleUpdate();
			gameWin(false);
		}), NULL));
	}
	if (monsterCurrentHP <= 0){
		monsterCurrentHP = 0;
		if (isMWinGame){
			return;
		}
		isMWinGame = true;
		monsterHPAtlas->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=](){
			unschedule(schedule_selector(GameScene::aiSchedule));
			unscheduleUpdate();
			gameWin(true);
		}), NULL));
	}
	float mHp = (float)mCurrentHP / mUserInfo->getHP();
	float mMp = (float)mCurrentMP / mUserInfo->getMP();
	float mosHp = (float)monsterCurrentHP / monstersAI->getHP();
	float mosMp = (float)monsterCurrentMP / monstersAI->getMP();
	mHPLoadingBar->setPercent(mHp * 100);
	mMPLoadingBar->setPercent(mMp * 100);
	mHPAtlas->setString(String::createWithFormat("%d/%d", mCurrentHP, mUserInfo->getHP())->getCString());
	mMPAtlas->setString(String::createWithFormat("%d/%d", mCurrentMP, mUserInfo->getMP())->getCString());
	monsterHPLoadingBar->setPercent(mosHp * 100);
	monsterMPLoadingBar->setPercent(mosMp * 100);
	monsterHPAtlas->setString(String::createWithFormat("%d/%d", monsterCurrentHP, monstersAI->getHP())->getCString());
	monsterMPAtlas->setString(String::createWithFormat("%d/%d", monsterCurrentMP, monstersAI->getMP())->getCString());
}

void GameScene::gameWin(bool isMy){
	LayerColor * winLayer = LayerColor::create(Color4B(67, 35, 12, 255), mSceneSize.width, mSceneSize.height);
	winLayer->setPosition(Vec2(0,0));
	winLayer->setAnchorPoint(Vec2(0.5, 0.5));
	Label * titleL = Label::create();
	titleL->setSystemFontSize(66);
	titleL->setTextColor(Color4B(255, 0, 170, 255));
	titleL->setAnchorPoint(Vec2(0.5, 0.5));
	titleL->setPosition(mSceneSize.width / 2, mSceneSize.height / 2);
	titleL->setString(isMy ? "Game Win": "Game Lost");
	winLayer->addChild(titleL);
	winLayer->setScale(0);
	this->addChild(winLayer, 100);
	winLayer->runAction(Sequence::create(ScaleTo::create(1.0f,1.0f),CallFunc::create([=](){
		
	}), NULL));
}

void GameScene::updateHeroAct(){
	if (isAiRound){
		for (auto heroes = prepareMonster.begin(); heroes != prepareMonster.end(); heroes++){
			auto hs = (*heroes);
			for (auto hero : hs){
				hero->updateRound();
			}
			if (hs.at(0)->getHeroACT() <= 0){
				isActionRuning = true;
				for (unsigned int i = 0; i < hs.size(); i++){
					auto heroObj = hs.at(i);
					heroObj->atkEnemy([=](){
						heroObj->attack([=](Node * node){
							//打墙
							if (i == 0){
								int hp = 0;
								//打墙
								if (heroObj->getIsMonster()){
									hp = heroObj->getHeroATK() - mDEF;
									mCurrentHP = mCurrentHP - hp;
									showCupBlood(hp, 0 + heroW, node->getPositionY(), heroObj->getIndexY());
								}
								else{
									hp = heroObj->getHeroATK() - monsterDEF;
									monsterCurrentHP = monsterCurrentHP - hp;
									showCupBlood(hp, mSceneSize.width - heroW, node->getPositionY(), heroObj->getIndexY());
								}
								updateHPMP();
								isActionRuning = false;
							}
							heroObj->setHeroHP(0);
							heroObj->setHeroATK(0);
						}, nullptr);
					});
				}
			}
		}
	}
	else{
		for (auto heroes = prepareHeroes.begin(); heroes != prepareHeroes.end();heroes++){
			auto hs = (*heroes);
			for (auto hero : hs){
				hero->updateRound();
			}
			if (hs.at(0)->getHeroACT() <= 0){
				isActionRuning = true;
				for (unsigned int i = 0; i < hs.size(); i++){
					auto heroObj = hs.at(i);
					heroObj->atkEnemy([=](){
						heroObj->attack([=](Node * node){
							//打墙
							if (i == 0){
								int hp = heroObj->getHeroATK() - monsterDEF;
								monsterCurrentHP = monsterCurrentHP - hp;
								showCupBlood(hp, mSceneSize.width - heroW, node->getPositionY(), heroObj->getIndexY());
								updateHPMP();
							}
							heroObj->setHeroHP(0);
							heroObj->setHeroATK(0);
						}, nullptr);
					});
				}
			}
		}
	}
}

void GameScene::heroEntry(){
	for (; existHeroCount < mUserInfo->getHeroesCount();){
		int indexY = (int)(CCRANDOM_0_1() * MAX_ROW);
		if (indexY == MAX_ROW){
			indexY = MAX_ROW - 1;
		}
		int indexType = (int)(CCRANDOM_0_1() * baseHeroes.size());
		if (indexType == baseHeroes.size()){
			indexType = baseHeroes.size()-1;
		}
		Heroes * heroes = baseHeroes.at(indexType);
		if (heroes->getType() == HeroType::Hero){
			for (int j = 0; j < MAX_COLUMN; j++){
				int index = j * MAX_ROW + indexY;
				if (mHeroes[index] == nullptr){
					//判断是否符合规则
					if (!initNotRepeatForHero(j, indexY, heroes->getId())){
						break;
					}
					HeroObj * hObj = HeroObj::create();
					hObj->retain();
					hObj->setIsMonster(false);
					hObj->setIndexX(j);
					hObj->setIndexY(indexY);
					hObj->setHero(heroes);
					std::string nodeName = String::createWithFormat("%s.csb", heroes->getName().c_str())->getCString();
					auto hero = CSLoader::createNode(nodeName.c_str());
					hero->setPosition(-60, hObj->getpositionY());
					hero->setScaleX(-1);
					hObj->addNode(gameSceneLayout, hero);
					//出场动画
					ActionTimeline * action = CSLoader::createTimeline(nodeName.c_str());
					hero->runAction(action);
					hObj->setAction(action);
					hObj->runAction(HeroActionType::Run);
					hero->runAction(Sequence::create(DelayTime::create(0.2f * j), MoveTo::create(0.2f * (MAX_COLUMN + 1 - j), Vec2(hObj->getpositionX(), hObj->getpositionY())), 
						CallFuncN::create([=](Node * node){
						hObj->runAction(HeroActionType::Stand);
					}), NULL));
					mHeroes[index] = hObj;
					existHeroCount++;
					break;
				}
			}
		}
		else if (heroes->getType() == HeroType::Boss){
			//for (int j = 0; j < MAX_COLUMN - 1; j++){
			//	int index = j * MAX_ROW + indexY;
			//	if (indexY == 0){//上边界
			//		if (mHeroes[index] == nullptr && mHeroes[index + 1] == nullptr
			//			&& mHeroes[index + MAX_ROW] == nullptr && mHeroes[index + MAX_ROW + 1] == nullptr){//占四个地方
			//			addBoss(index, j, indexY, hStartY - indexY * heroH - heroH / 2, heroes);
			//			existHeroCount++;
			//			break;
			//		}
			//	}
			//	else if (indexY == MAX_ROW - 1){//下边界
			//		if (mHeroes[index] == nullptr && mHeroes[index - 1] == nullptr
			//			&& mHeroes[index + MAX_ROW] == nullptr && mHeroes[index + MAX_ROW - 1] == nullptr){//占四个地方
			//			addBoss(index - 1, j, indexY - 1, hStartY - indexY * heroH + heroH / 2, heroes);
			//			existHeroCount++;
			//			break;
			//		}
			//	}
			//	else{//中间
			//		if (mHeroes[index] == nullptr && mHeroes[index + 1] == nullptr
			//			&& mHeroes[index + MAX_ROW] == nullptr && mHeroes[index + MAX_ROW + 1] == nullptr){//占四个地方
			//			addBoss(index, j, indexY, hStartY - indexY * heroH - heroH / 2, heroes);
			//			existHeroCount++;
			//			break;
			//		}
			//		else if (mHeroes[index] == nullptr && mHeroes[index - 1] == nullptr
			//			&& mHeroes[index + MAX_ROW] == nullptr && mHeroes[index + MAX_ROW - 1] == nullptr){//占四个地方
			//			//tag , (0,0) , px py , hero
			//			addBoss(index - 1, j, indexY - 1,  hStartY - indexY * heroH + heroH / 2, heroes);
			//			existHeroCount++;
			//			break;
			//		}
			//	}
			//}
		}
	}
}

void GameScene::monsterEntry(){
	for (; existMonsterCount < monstersAI->getHeroesCount();){
		int indexY = (int)(CCRANDOM_0_1() * MAX_ROW);
		if (indexY == MAX_ROW){
			indexY = MAX_ROW - 1;
		}
		int indexType = (int)(CCRANDOM_0_1() * baseMonsters.size());
		if (indexType == baseMonsters.size()){
			indexType = baseMonsters.size() - 1;
		}
		Monster * monster = baseMonsters.at(indexType);
		if (monster->getType() == HeroType::Hero){
			for (int j = 0; j < MAX_COLUMN; j++){
				int index = j * MAX_ROW + indexY;
				if (monsterHeros[index] == nullptr){
					if (!initNotRepeatForMonster(j, indexY, monster->getId())){
						break;
					}
					HeroObj * hObj = HeroObj::create();
					hObj->retain();
					hObj->setIsMonster(true);
					hObj->setIndexX(j);
					hObj->setIndexY(indexY);
					hObj->setHero(monster);
					std::string nodeName = String::createWithFormat("%s.csb", monster->getName().c_str())->getCString();
					auto hero = CSLoader::createNode(nodeName.c_str());
					hero->retain();
					hero->setPosition(mSceneSize.width + 60, hObj->getpositionY());
					hObj->addNode(gameSceneLayout, hero);
					ActionTimeline * action = CSLoader::createTimeline(nodeName.c_str());
					hero->runAction(action);
					hObj->setAction(action);
					hObj->runAction(HeroActionType::Run);
					hero->runAction(Sequence::create(DelayTime::create(0.3f * j), MoveTo::create(0.2f * (MAX_COLUMN + 1 - j), Vec2(hObj->getpositionX(), hObj->getpositionY())), CallFuncN::create([hObj](Node * node){
						hObj->runAction(HeroActionType::Stand);
					}), NULL));
					monsterHeros[index] = hObj;
					existMonsterCount++;
					break;
				}
			}
		}
		else if (monster->getType() == HeroType::Boss){

		}
	}
}

void GameScene::initGame(){
	isInit = true;
	heroEntry();
	monsterEntry();
	isInit = false;
	updateHeroCount();
	actRoundChage();

}

bool GameScene::initNotRepeatForHero(int indexX, int indexY, int id){
	if (isInit){
		int lastX = indexX - 1;
		int lastLastX = indexX - 2;
		int lastY = indexY - 1;
		int lastLastY = indexY - 2;
		int nextY = indexY + 1;
		int nextNextY = indexY + 2;

		if (indexX >= 2){
			if (mHeroes[lastLastX * MAX_ROW + indexY] != nullptr && mHeroes[lastX * MAX_ROW + indexY] != nullptr){
				if (id == mHeroes[lastLastX * MAX_ROW + indexY]->getHero()->getId() &&
					id == mHeroes[lastX * MAX_ROW + indexY]->getHero()->getId()){
					return false;
				}
			}
		}

		if (indexY == 0){
			if (mHeroes[indexX * MAX_ROW + nextNextY] == nullptr || mHeroes[indexX * MAX_ROW + nextY] == nullptr){
				return true;
			}
			if (id == mHeroes[indexX * MAX_ROW + nextY]->getHero()->getId() &&
				id == mHeroes[indexX * MAX_ROW + nextNextY]->getHero()->getId()){
				return false;
			}
		}
		else if (indexY == 1){
			if (mHeroes[indexX * MAX_ROW + nextY] != nullptr && mHeroes[indexX * MAX_ROW + lastY] != nullptr){
				if (id == mHeroes[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == mHeroes[indexX * MAX_ROW + lastY]->getHero()->getId()){
					return false;
				}
			}
			if (mHeroes[indexX * MAX_ROW + nextNextY] == nullptr || mHeroes[indexX * MAX_ROW + nextY] == nullptr){
				return true;
			}
			if (id == mHeroes[indexX * MAX_ROW + nextY]->getHero()->getId() &&
				id == mHeroes[indexX * MAX_ROW + nextNextY]->getHero()->getId()){
				return false;
			}
		}
		else if (indexY == MAX_ROW - 1){
			if (mHeroes[indexX * MAX_ROW + lastLastY] == nullptr || mHeroes[indexX * MAX_ROW + lastY] == nullptr){
				return true;
			}
			if (id == mHeroes[indexX * MAX_ROW + lastY]->getHero()->getId() &&
				id == mHeroes[indexX * MAX_ROW + lastLastY]->getHero()->getId()){
				return false;
			}
		}
		else if (indexY == MAX_ROW - 2){
			if (mHeroes[indexX * MAX_ROW + nextY] != nullptr && mHeroes[indexX * MAX_ROW + lastY] != nullptr){
				if (id == mHeroes[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == mHeroes[indexX * MAX_ROW + lastY]->getHero()->getId()){
					return false;
				}
			}

			if (mHeroes[indexX * MAX_ROW + lastLastY] != nullptr && mHeroes[indexX * MAX_ROW + lastY] != nullptr){
				if (id == mHeroes[indexX * MAX_ROW + lastY]->getHero()->getId() &&
					id == mHeroes[indexX * MAX_ROW + lastLastY]->getHero()->getId()){
					return false;
				}
			}
		}
		else{
			if (mHeroes[indexX * MAX_ROW + nextY] != nullptr && mHeroes[indexX * MAX_ROW + lastY] != nullptr){
				if (id == mHeroes[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == mHeroes[indexX * MAX_ROW + lastY]->getHero()->getId()){
					return false;
				}
			}
			if (mHeroes[indexX * MAX_ROW + lastLastY] != nullptr && mHeroes[indexX * MAX_ROW + lastY] != nullptr){
				if (id == mHeroes[indexX * MAX_ROW + lastY]->getHero()->getId() &&
					id == mHeroes[indexX * MAX_ROW + lastLastY]->getHero()->getId()){
					return false;
				}
			}
			if (mHeroes[indexX * MAX_ROW + nextNextY] != nullptr &&  mHeroes[indexX * MAX_ROW + nextY] != nullptr){
				if (id == mHeroes[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == mHeroes[indexX * MAX_ROW + nextNextY]->getHero()->getId()){
					return false;
				}
			}
		}
		return true;
	}
	else{
		return true;
	}
}

bool GameScene::initNotRepeatForMonster(int indexX, int indexY, int id){
	if (isInit){
		int lastX = indexX - 1;
		int lastLastX = indexX - 2;
		int lastY = indexY - 1;
		int lastLastY = indexY - 2;
		int nextY = indexY + 1;
		int nextNextY = indexY + 2;

		if (indexX >= 2){
			if (monsterHeros[lastLastX * MAX_ROW + indexY] == nullptr || monsterHeros[lastX * MAX_ROW + indexY] == nullptr){
				return true;
			}
			if (id == monsterHeros[lastLastX * MAX_ROW + indexY]->getHero()->getId() &&
				id == monsterHeros[lastX * MAX_ROW + indexY]->getHero()->getId()){
				return false;
			}
		}

		if (indexY == 0){
			if (monsterHeros[indexX * MAX_ROW + nextNextY] == nullptr || monsterHeros[indexX * MAX_ROW + nextY] == nullptr){
				return true;
			}
			if (id == monsterHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
				id == monsterHeros[indexX * MAX_ROW + nextNextY]->getHero()->getId()){
				return false;
			}
		}
		else if (indexY == 1){
			if (monsterHeros[indexX * MAX_ROW + nextY] != nullptr && monsterHeros[indexX * MAX_ROW + lastY] != nullptr){
				if (id == monsterHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == monsterHeros[indexX * MAX_ROW + lastY]->getHero()->getId()){
					return false;
				}
			}

			if (monsterHeros[indexX * MAX_ROW + nextNextY] == nullptr || monsterHeros[indexX * MAX_ROW + nextY] == nullptr){
				return true;
			}
			if (id == monsterHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
				id == monsterHeros[indexX * MAX_ROW + nextNextY]->getHero()->getId()){
				return false;
			}
		}
		else if (indexY == MAX_ROW - 1){
			if (monsterHeros[indexX * MAX_ROW + lastLastY] == nullptr || monsterHeros[indexX * MAX_ROW + lastY] == nullptr){
				return true;
			}
			if (id == monsterHeros[indexX * MAX_ROW + lastY]->getHero()->getId() &&
				id == monsterHeros[indexX * MAX_ROW + lastLastY]->getHero()->getId()){
				return false;
			}
		}
		else if (indexY == MAX_ROW - 2){
			if (monsterHeros[indexX * MAX_ROW + nextY] != nullptr && monsterHeros[indexX * MAX_ROW + lastY] != nullptr){
				if (id == monsterHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == monsterHeros[indexX * MAX_ROW + lastY]->getHero()->getId()){
					return false;
				}
			}
			if (monsterHeros[indexX * MAX_ROW + lastLastY] == nullptr || monsterHeros[indexX * MAX_ROW + lastY] == nullptr){
				return true;
			}
			if (id == monsterHeros[indexX * MAX_ROW + lastY]->getHero()->getId() &&
				id == monsterHeros[indexX * MAX_ROW + lastLastY]->getHero()->getId()){
				return false;
			}
		}
		else{
			if (monsterHeros[indexX * MAX_ROW + nextY] != nullptr && monsterHeros[indexX * MAX_ROW + lastY] != nullptr){
				if (id == monsterHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == monsterHeros[indexX * MAX_ROW + lastY]->getHero()->getId()){
					return false;
				}
			}
			if (monsterHeros[indexX * MAX_ROW + lastLastY] != nullptr && monsterHeros[indexX * MAX_ROW + lastY] != nullptr){
				if (id == monsterHeros[indexX * MAX_ROW + lastY]->getHero()->getId() &&
					id == monsterHeros[indexX * MAX_ROW + lastLastY]->getHero()->getId()){
					return false;
				}
			}

			if (monsterHeros[indexX * MAX_ROW + nextNextY] != nullptr && monsterHeros[indexX * MAX_ROW + nextY] != nullptr){
				if (id == monsterHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == monsterHeros[indexX * MAX_ROW + nextNextY]->getHero()->getId()){
					return false;
				}
			}

		}

		return true;
	}
	else{
		return true;
	}
}

void GameScene::addBoss(int index, int indexX, int indexY, int posionY, BaseHeroes* heroes){
	//HeroObj * hObj = HeroObj::create();
	//hObj->retain();
	//hObj->setIsMonster(false);
	//hObj->setIndexX(indexX);
	//hObj->setIndexY(indexY);
	//hObj->setpositionY(posionY);
	//hObj->setpositionX(hObj->getpositionX() - heroW / 4);
	//hObj->setHero(heroes);
	//std::string name = heroes->getName();
	//std::string nodeName = String::createWithFormat("%s.csb", name.c_str())->getCString();
	//auto hero = CSLoader::createNode(nodeName.c_str());
	//hero->setAnchorPoint(Vec2(0.5,0.5));
	//hero->setPosition(-120, hObj->getpositionY());
	//hero->setScaleX(-1.0f);
	////hero->setScaleY(1.5f);
	//hObj->addNode(gameSceneLayout, hero);
	////出场动画
	//ActionTimeline * action = CSLoader::createTimeline(nodeName.c_str());
	//hero->runAction(action);
	//hObj->setAction(action);
	//hObj->runAction(HeroActionType::Run);
	//hero->runAction(Sequence::create(DelayTime::create(0.2f * indexX), MoveTo::create(0.2f * (MAX_COLUMN + 1 - indexX), Vec2(hObj->getpositionX(), hObj->getpositionY())), CallFuncN::create([hObj](Node * node){
	//	hObj->runAction(HeroActionType::Stand);
	//}), NULL));
	//mHeros[index] = hObj;
	//mHeros[index + 1] = hObj;
	//mHeros[index + MAX_ROW] = hObj;
	//mHeros[index + MAX_ROW + 1] = hObj;
}

bool GameScene::onTouchBegan(Touch * pTouch, Event * pEvent){
	auto targt = pEvent->getCurrentTarget();
	Vec2 loacatinInNode = targt->convertToNodeSpace(pTouch->getLocation());
	Size size = targt->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(loacatinInNode)){
		Director::getInstance()->pause();
		auto node = CSLoader::createNode("GamePauseLayout.csb");
		node->setAnchorPoint(Vec2::ZERO);
		node->setPosition(Vec2::ZERO);
		this->addChild(node, 20, 20);
		auto pauseLayout = node->getChildByName("GamePauseLayout");
		auto backBtn = static_cast<Button *>(pauseLayout->getChildByName("GameBtnBack"));
		auto startBtn = static_cast<Button *>(pauseLayout->getChildByName("GameBtnStart"));
		auto yinXiaoC = static_cast<CheckBox *>(pauseLayout->getChildByName("GameYinXiaoC"));
		auto yinYueC = static_cast<CheckBox *>(pauseLayout->getChildByName("GameYinYueC"));
		backBtn->addClickEventListener([this](Ref * ref){
			this->removeAllChildren();
			Director::getInstance()->resume();
			Director::getInstance()->popScene();

		});
		startBtn->addClickEventListener([this](Ref * ref){
			Director::getInstance()->resume();
			this->removeChildByTag(20);
		});
		yinXiaoC->addClickEventListener([](Ref * ref){

		});
		yinYueC->addClickEventListener([](Ref * ref){

		});
		return true;
	}
	if (isActionRuning || isAiRound || mActCount <=0 ||isMWinGame || isMonsterWinGame){
		return false;
	}
	Vec2 touch = pTouch->getLocation();
	if (touch.x < hStartX + heroW / 2 && touch.y < hStartY + heroH){
		touchDownY = touch.y;
		if (mCurrentHero == nullptr){
			schedule(schedule_selector(GameScene::onLongTouchDown), 0.5, 0, 0);
			longTouchDownX = touch.x;
			longTouchDownY = touch.y;
		}
		return true;
	}
	return false;
}
void GameScene::onTouchMoved(Touch * pTouch, Event * pEvent){}
void GameScene::onTouchEnded(Touch * pTouch, Event * pEvent){
	unschedule(schedule_selector(GameScene::onLongTouchDown));
	if (isLongPress){
		isLongPress = false;
		return;
	}
	Vec2 touch = pTouch->getLocation();
	if (touch.x < hStartX + heroW / 2 && touch.y < hStartY + heroH && abs(touchDownY - touch.y) < heroH){
		int index = MAX_ROW + 1 - touch.y / heroH;
		if (index < 0 || index >= MAX_ROW){
			return;
		}
		if (mCurrentHero == nullptr){
			int indexHero = -1;
			for (int i = 0; i < MAX_COLUMN; i++){
				if (mHeroes[i * MAX_COLUMN + index] != nullptr){
					indexHero = i * MAX_COLUMN + index;
				}
				else{
					break;
				}
			}
			if (indexHero == -1){
				return;
			}
			if (mHeroes[indexHero]->getActionType() == HeroActionType::Stand && mHeroes[indexHero]->getHero()->getType() == HeroType::Hero){
				mCurrentHero = mHeroes[indexHero];
				mCurrentHero->getMCurrentNode()->runAction(FadeTo::create(0.2f, 125));
				return;
			}
			/*if (mHeros[indexHero]->getActionType() == HeroActionType::Stand && mHeros[indexHero]->getHero()->getType() == HeroType::Boss){
				if (indexHero + MAX_ROW > AllHeroCount){
					mCurrentHero = mHeros[indexHero];
					mCurrentHero->getMCurrentNode()->runAction(FadeTo::create(0.2f, 125));
					return;
				}
				else{
					if (indexHero >= MAX_ROW){
						if (mHeros[indexHero] == mHeros[indexHero - 1] && mHeros[indexHero] == mHeros[indexHero - MAX_ROW] &&
							mHeros[indexHero] == mHeros[indexHero - MAX_ROW - 1]){
							if (mHeros[indexHero + MAX_ROW] == nullptr && mHeros[indexHero + MAX_ROW - 1] == nullptr){
								mCurrentHero = mHeros[indexHero];
								mCurrentHero->getMCurrentNode()->runAction(FadeTo::create(0.2f, 125));
							}
							return;
						}
						if (mHeros[indexHero] == mHeros[indexHero + 1] && mHeros[indexHero] == mHeros[indexHero - MAX_ROW] &&
							mHeros[indexHero] == mHeros[indexHero - MAX_ROW + 1]){
							if (mHeros[indexHero + MAX_ROW] == nullptr && mHeros[indexHero + MAX_ROW + 1] == nullptr){
								mCurrentHero = mHeros[indexHero];
								mCurrentHero->getMCurrentNode()->runAction(FadeTo::create(0.2f, 125));
							}
							return;
						}
					}
					if (indexHero + 2 * MAX_ROW < AllHeroCount){
						if (mHeros[indexHero] == mHeros[indexHero - 1] && mHeros[indexHero] == mHeros[indexHero + MAX_ROW] &&
							mHeros[indexHero] == mHeros[indexHero + MAX_ROW - 1]){
							if (mHeros[indexHero + 2 * MAX_ROW] == nullptr && mHeros[indexHero + 2 * MAX_ROW - 1] == nullptr){
								mCurrentHero = mHeros[indexHero];
								mCurrentHero->getMCurrentNode()->runAction(FadeTo::create(0.2f, 125));
							}
							return;
						}
						if (mHeros[indexHero] == mHeros[indexHero + 1] && mHeros[indexHero] == mHeros[indexHero + MAX_ROW] &&
							mHeros[indexHero] == mHeros[indexHero + MAX_ROW + 1]){
							if (mHeros[indexHero + 2 * MAX_ROW] == nullptr && mHeros[indexHero + 2 * MAX_ROW + 1] == nullptr){
								mCurrentHero = mHeros[indexHero];
								mCurrentHero->getMCurrentNode()->runAction(FadeTo::create(0.2f, 125));
							}
							return;
						}
					}
				}
			}*/
			mCurrentHero = nullptr;
		}
		else{
			if (mCurrentHero->getIndexY() == index){
				mCurrentHero->getMCurrentNode()->runAction(FadeTo::create(0.2f, 255));
				mCurrentHero = nullptr;
				return;
			}
			int indexXHero = -1;
			for (int i = 0; i < MAX_COLUMN; i++){
				if (mHeroes[i * MAX_COLUMN + index] == nullptr){
					indexXHero = i;
					break;
				}
			}
			if (indexXHero == -1){
				mCurrentHero->getMCurrentNode()->runAction(FadeTo::create(0.2f, 255));
				mCurrentHero = nullptr;
				return;
			}
			int posX = hStartX - indexXHero * heroW;
			int posY = hStartY - index * heroH;
			isActionRuning = true;
			mCurrentHero->getMCurrentNode()->runAction(Sequence::create(MoveTo::create(0.1f *(MAX_COLUMN - mCurrentHero->getIndexX()), Vec2(-60, mCurrentHero->getpositionY())),
				MoveTo::create(0, Vec2(-60, posY)), MoveTo::create(0.1f * (MAX_COLUMN - indexXHero), Vec2(posX, posY)), FadeTo::create(0.2f, 255),
				CallFunc::create([=](){
				mHeroes[mCurrentHero->getIndexX() * MAX_COLUMN + mCurrentHero->getIndexY()] = nullptr;
				mCurrentHero->setIndexX(indexXHero);
				mCurrentHero->setIndexY(index);
				mCurrentHero->updateNode();
				mHeroes[indexXHero * MAX_COLUMN + index] = mCurrentHero;
			}), CallFunc::create([=](){
				//判断 准备和防御
				mHeroPrepareAndDef(true,mHeroes);
				mCurrentHero = nullptr;
				isActionRuning = false;
				
			}), NULL));
		}
	}
}

void GameScene::onLongTouchDown(float delay){
	if (isAiRound || mActCount <= 0 || isMWinGame || isMonsterWinGame){
		return;
	}
	isLongPress = true;
	int posX = abs(longTouchDownX - (hStartX + heroW / 2));
	int posY = abs(longTouchDownY - (hStartY + heroH / 2));
	int indexX = posX / heroW;
	int indexY = posY / heroH;
	if (indexX >= 0 && indexX < MAX_COLUMN && indexY >= 0 && indexY < MAX_ROW){
		HeroObj* hero = mHeroes[indexX * MAX_ROW + indexY];
		if (hero != nullptr && (hero->getActionType() == HeroActionType::Stand || hero->getActionType() == HeroActionType::Def)){
			//ActionTimeline * action = static_cast<ActionTimeline *>(hero->getMNode()->getActionByTag(hero->getID()));
			//action->play("hit",false);
			//action->setLastFrameCallFunc([=](){
			gameSceneLayout->removeChild(mHeroes[indexX * MAX_ROW + indexY]->getMCurrentNode(), true);
			mHeroes[indexX * MAX_ROW + indexY] = nullptr;
			existHeroCount--;
			updateHeroCount();
			for (int i = indexX + 1; i < MAX_COLUMN; i++){
				if (mHeroes[i * MAX_ROW + indexY] != nullptr){
					HeroObj * mHero = mHeroes[i * MAX_ROW + indexY];
					mHero->setIndexX(i - 1);
					mHero->updateNode();
					mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
					mHeroes[(i - 1) * MAX_ROW + indexY] = mHero;
					mHeroes[i * MAX_ROW + indexY] = nullptr;
				}
				else{
					break;
				}
			}
			//判断 准备和防御
			mHeroPrepareAndDef(true,mHeroes);
			//});
			hero = nullptr;
		}
	}
}

void GameScene::mHeroPrepareAndDef(bool isClick, HeroObj * mHeros[]){//判断是否人点击 runHeroPosition()  //计算行动次数
	std::vector<std::vector<HeroObj *>> allHHeroes;
	std::vector<std::vector<HeroObj *>> allVHeroes;

	int id;
	for (int i = 0; i < MAX_ROW; i++){//行
		std::vector<HeroObj *> hHeroes;
		for (int j = 0; j < MAX_COLUMN; j++){//列
			HeroObj * hero = mHeros[j * MAX_ROW + i];
			if (hero != nullptr && hero->getActionType() == HeroActionType::Stand && hero->getHero()->getType() != HeroType::Boss){
				if (hHeroes.empty()){
					id = hero->getHero()->getId();
					hHeroes.push_back(hero);
					continue;
				}
				if (id == hero->getHero()->getId()){
					hHeroes.push_back(hero);
				}
				else{
					if (hHeroes.size() >= 3){
						allHHeroes.push_back(hHeroes);
					}
					hHeroes.clear();
					id = hero->getHero()->getId();
					hHeroes.push_back(hero);
				}
			}
			else{
				if (hHeroes.size() >= 3){
					allHHeroes.push_back(hHeroes);
				}
				hHeroes.clear();
			}
		}
		if (hHeroes.size() >= 3){
			allHHeroes.push_back(hHeroes);
		}
		hHeroes.clear();
	}
	for (int i = 0; i < MAX_COLUMN; i++){//列
		std::vector<HeroObj *> vHeroes;
		for (int j = 0; j < MAX_ROW; j++){//行
			HeroObj * hero = mHeros[i * MAX_ROW + j];
			if (hero != nullptr && hero->getActionType() == HeroActionType::Stand && hero->getHero()->getType() != HeroType::Boss){
				if (vHeroes.empty()){
					id = hero->getHero()->getId();
					//log(" hero->getHero()->getId() = %d ", hero->getHero()->getId());
					//log(" hero->getId() = %d ", hero->getId());
					vHeroes.push_back(hero);
					continue;
				}
				//log(" hero->getHero()->getId() = %d ", hero->getHero()->getId());
				//log(" hero->getId() = %d ", hero->getId());
				if (id == hero->getHero()->getId()){
					vHeroes.push_back(hero);
				}
				else{
					if (vHeroes.size() >= 3){
						allVHeroes.push_back(vHeroes);
					}
					vHeroes.clear();
					id = hero->getHero()->getId();
					vHeroes.push_back(hero);
				}
			}
			else{
				if (vHeroes.size() >= 3){
					allVHeroes.push_back(vHeroes);
				}
				vHeroes.clear();
			}
		}
		if (vHeroes.size() >= 3){
			allVHeroes.push_back(vHeroes);
		}
		vHeroes.clear();
	}
	if (!allHHeroes.empty() && !allVHeroes.empty()){
		//筛选重复的hero
		std::vector<HeroObj *> repeatHero;
		for (auto hHeroes : allHHeroes){
			for (auto hHero : hHeroes){
				for (auto vHeroes : allVHeroes){
					for (auto vHero : vHeroes){
						if (hHero->getId() == vHero->getId()){
							repeatHero.push_back(hHero);
							break;
						}
					}
				}
			}
		}
		log(" repeatHero = %d ", repeatHero.size());
		if (repeatHero.empty()){
			mHeroDefAction(allVHeroes, mHeros);
			mHeroPrepareAction(allHHeroes, mHeros);
		}
		else{
			//有重复的就多填一个   总数量减一
			for (auto hero : repeatHero){
				for (int i = hero->getIndexX() + 1; i < MAX_COLUMN; i++){
					int index = i*MAX_COLUMN + hero->getIndexY();
					if (mHeros[index] == nullptr){
						HeroObj * h = hero->cloneThis(i);
						for (auto& vHeroes : allVHeroes){//带上&  代表我要修改列表中的值  等同于 begin end
							for (auto vHero = vHeroes.begin(); vHero != vHeroes.end(); vHero++){
								if (hero->getId() == (*vHero)->getId()){
									vHeroes.erase(vHero);
									vHeroes.push_back(h);
									mHeros[index] = h;
									//删除原数组中的  添加一个后面的
									break;
								}
							}
						}
						break;
					}
				}
			}
			mHeroPrepareAction(allHHeroes, mHeros);
			mHeroDefAction(allVHeroes, mHeros);
			//次数加一
			if (isAiRound){
				monsterActCount++;
			}
			else{
				mActCount++;
			}
			updateActTime();
			////超出界面  处理  从后面删一个 再填
			//for (int i = MAX_COLUMN -1; i >= 0; i--){
			//	int index = i*MAX_COLUMN + hero->getIndexY();
			//	HeroObj * h = mHeros[index];
			//	if (h->getId() != hero->getId() && h->getActionType() == HeroActionType::Prepare){
			//		h->getMCurrentNode()->removeFromParentAndCleanup(true);
			//		h = nullptr;
			//		mHeros[index] = nullptr;
			//		HeroObj * heorItem = hero->cloneThis(i);
			//		for (auto vHeroes : allVHeroes){
			//			for (auto vHero = vHeroes.begin(); vHero != vHeroes.end(); vHero++){
			//				if (hero->getId() == (*vHero)->getId()){
			//					vHeroes.erase(vHero);
			//					vHeroes.push_back(heorItem);
			//					break;
			//				}
			//			}
			//		}
			//		break;
			//	}
			//}
		}

	//}
	}
	else if (!allHHeroes.empty()){
		//log(" allHHeroes ", allHHeroes.size());
		mHeroPrepareAction(allHHeroes, mHeros);
	}
	else if (!allVHeroes.empty()){
		//log(" allVHeroes ", allVHeroes.size());
		mHeroDefAction(allVHeroes, mHeros);
	}
	if (isClick){//是人单击就减少一次
		//回合过后不减
		//次数减一
		if (isAiRound){
			monsterActCount--;
		}
		else{
			mActCount--;
		}
		updateActTime();
		if (monsterActCount <= 0 || mActCount <= 0){
			actRoundChage();
		}
	}
}

void GameScene::swapHeroPosition(HeroObj * from, HeroObj * to, HeroObj * mHeros[]){
	int indexX;
	if (to != nullptr && to->getActionType() == HeroActionType::Def){
		return;
	}
	else if (to != nullptr){
		indexX = from->getIndexX();
		from->setIndexX(to->getIndexX());
		from->updateNode();
		mHeros[from->getId()] = from;
		to->setIndexX(indexX);
		to->updateNode();
		mHeros[to->getId()] = to;
		if (from->getIsMonster()){
			from->getMCurrentNode()->runAction(MoveBy::create(0.1f, Vec2(-heroW, 0)));
			to->getMCurrentNode()->runAction(MoveBy::create(0.1f, Vec2(heroW, 0)));
		}
		else{
			from->getMCurrentNode()->runAction(MoveBy::create(0.1f, Vec2(heroW, 0)));
			to->getMCurrentNode()->runAction(MoveBy::create(0.1f, Vec2(-heroW, 0)));
		}
	}
	else{
		mHeros[from->getId()] = nullptr;
		from->setIndexX(from->getIndexX() - 1);
		from->updateNode();
		mHeros[from->getId()] = from;
		if (from->getIsMonster()){
			from->getMCurrentNode()->runAction(MoveBy::create(0.1f, Vec2(-heroW, 0)));
		}
		else{
			from->getMCurrentNode()->runAction(MoveBy::create(0.1f, Vec2(heroW, 0)));
		}
	}
	//移动完后
	stopActionByTag(SwapActionTag);
	Action * action = Sequence::create(DelayTime::create(0.5), CallFunc::create([=](){
		mHeroPrepareAndDef(false,mHeros);
	}), NULL);
	action->setTag(SwapActionTag);
	runAction(action);
}

void GameScene::mHeroPrepareAction(std::vector<std::vector<HeroObj *>> allHHeroes, HeroObj * mHeros[]){
	for (auto hHeroes = allHHeroes.begin(); hHeroes != allHHeroes.end(); hHeroes++){
		std::vector<HeroObj *>  heroes = std::vector<HeroObj *>();
		for (unsigned int i = 0; i < (*hHeroes).size(); i++){
			heroes.push_back((*hHeroes).at(i));
			if ((i + 1) % 3 == 0){//3个为一组
				for (int j = 0; j < 3; j++){
					//准备动画
					heroes[j]->prepare(j == 1);
					if (heroes[j]->getIsMonster()){
						heroes[j]->setTargetPosX((j + 1) * heroW);
					}
					else{
						heroes[j]->setTargetPosX(mSceneSize.width - (j + 1) * heroW);
					}
					heroes[j]->setTargetPosY(heroes[j]->getpositionY());
				}
				//准备好后 移动
				for (int k = heroes[0]->getIndexX() - 1; k >= 0; k--){
					HeroObj * obj = mHeros[k * MAX_ROW + heroes[0]->getIndexY()];
					if (obj != nullptr && obj->getActionType() == HeroActionType::Prepare  //pre 并且相同  攻击没有加倍  对象没有从准备中消失
						&& obj->getHero()->getId() == heroes[0]->getHero()->getId()){
						for (int m = 0; m < 3; m++){
							HeroObj * heroItem = heroes[m];
							heroItem->getMCurrentNode()->runAction(Sequence::create(MoveTo::create(0.3f,
								Vec2(heroItem->getpositionX() + heroW, heroItem->getpositionY())),
								CallFuncN::create([=](Node * node){
								gameSceneLayout->removeChild(node);
								mHeros[heroItem->getId()] = nullptr;
								//最后一个移动 判断后面有没有hero
								if (m == 2){
									for (int j = heroItem->getIndexX() + 1; j < MAX_COLUMN; j++){
										HeroObj * heroLast = mHeros[j * MAX_ROW + heroItem->getIndexY()];
										if (heroLast != nullptr){
											heroLast->setIndexX(j - 3);
											heroLast->updateNode();
											heroLast->getMCurrentNode()->runAction(MoveTo::create(0.3f, Vec2(heroLast->getpositionX(), heroLast->getpositionY())));
										}
										else{
											break;
										}
									}
								}
							}), NULL));
						}
						for (auto hes = heroes.begin(); hes != heroes.end(); hes++){
							(*hHeroes).erase(hes);
						}
						heroes.clear(); //删除
						for (int l = 1; i <= 3; i++){//攻击加倍
							int hId = heroes[0]->getId() - i * MAX_ROW;
							HeroObj * hObj = mHeros[hId];
							hObj->setHeroATK(hObj->getHeroATK() * 2);
							hObj->setHeroHP(hObj->getHeroHP() * 2);
							hObj->setHeroAdd(hObj->getHeroAdd() * 2);
							hObj->updateATK();
						}
						break;
					}
					else if (obj != nullptr && obj->getActionType() == HeroActionType::Prepare){ //pre
						break;
					}
					else {
						for (int n = 0; n < 3; n++){
							swapHeroPosition(heroes[n], obj,mHeros);
						}
					}
				}
				if (heroes.empty()){
					continue;
				}
				if ((*hHeroes).at(i)->getIsMonster()){
					prepareMonster.push_back(heroes);
				}
				else{
					prepareHeroes.push_back(heroes);//添加到已经准备的英雄组中
				}
			}
		}
	}

}

void GameScene::mHeroDefAction(std::vector<std::vector<HeroObj *>> allVHeroes, HeroObj * mHeros[]){
	for (auto vHeroes = allVHeroes.begin(); vHeroes != allVHeroes.end(); vHeroes++){
		for (auto hero : (*vHeroes)){
			hero->def();
		}
		for (auto hero : (*vHeroes)){
			//防御后移动
			if (hero->getIndexX() == 0){
				break;
			}
			else{
				for (int i = hero->getIndexX(); i > 0; i--){
					swapHeroPosition(mHeros[i * MAX_ROW + hero->getIndexY()], mHeros[(i - 1) * MAX_ROW + hero->getIndexY()],mHeros);
				}
			}
		}
	}
}

//void GameScene::monsterPrepareAndDef(bool isClick, HeroObj * mHeros[]){//判断是否人点击 runHeroPosition()  //计算行动次数
//	std::vector<std::vector<HeroObj *>> allHMonsters;
//	std::vector<std::vector<HeroObj *>> allVMonsters;
//	int id;
//	for (int i = 0; i < MAX_ROW; i++){//行
//		std::vector<HeroObj *> hHeroes;
//		for (int j = 0; j < MAX_COLUMN; j++){//列
//			HeroObj * hero = monsterHeros[j * MAX_ROW + i];
//			if (hero != nullptr && hero->getActionType() == HeroActionType::Stand && hero->getHero()->getType() != HeroType::Boss){
//				if (hHeroes.empty()){
//					id = hero->getHero()->getId();
//					hHeroes.push_back(hero);
//					continue;
//				}
//				if (id == hero->getHero()->getId()){
//					hHeroes.push_back(hero);
//				}
//				else{
//					if (hHeroes.size() >= 3){
//						allHMonsters.push_back(hHeroes);
//					}
//					hHeroes.clear();
//					id = hero->getHero()->getId();
//					hHeroes.push_back(hero);
//				}
//			}
//			else{
//				if (hHeroes.size() >= 3){
//					allHMonsters.push_back(hHeroes);
//				}
//				hHeroes.clear();
//			}
//		}
//		if (hHeroes.size() >= 3){
//			allHMonsters.push_back(hHeroes);
//		}
//		hHeroes.clear();
//	}
//	for (int i = 0; i < MAX_COLUMN; i++){//列
//		std::vector<HeroObj *> vHeroes;
//		for (int j = 0; j < MAX_ROW; j++){//行
//			HeroObj * hero = monsterHeros[i * MAX_ROW + j];
//			if (hero != nullptr && hero->getActionType() == HeroActionType::Stand && hero->getHero()->getType() != HeroType::Boss){
//				if (vHeroes.empty()){
//					id = hero->getHero()->getId();
//					vHeroes.push_back(hero);
//					continue;
//				}
//				if (id == hero->getHero()->getId()){
//					vHeroes.push_back(hero);
//				}
//				else{
//					if (vHeroes.size() >= 3){
//						allVMonsters.push_back(vHeroes);
//					}
//					vHeroes.clear();
//					id = hero->getHero()->getId();
//					vHeroes.push_back(hero);
//				}
//			}
//			else{
//				if (vHeroes.size() >= 3){
//					allVMonsters.push_back(vHeroes);
//				}
//				vHeroes.clear();
//			}
//		}
//		if (vHeroes.size() >= 3){
//			allVMonsters.push_back(vHeroes);
//		}
//		vHeroes.clear();
//	}
//	if (!allHMonsters.empty() && !allVMonsters.empty()){
//		//筛选重复的hero
//		std::vector<HeroObj *> repeatHero;
//		for (auto hHeroes : allHMonsters){
//			for (auto hHero : hHeroes){
//				for (auto vHeroes : allVMonsters){
//					for (auto vHero : vHeroes){
//						if (hHero->getId() == vHero->getId()){
//							repeatHero.push_back(hHero);
//							break;
//						}
//					}
//				}
//			}
//		}
//		if (repeatHero.empty()){
//			monsterDefAction(allVMonsters);
//			monsterPrepareAction(allHMonsters);
//		}
//		else{
//			//有重复的就多填一个   总数量减一
//			for (auto hero : repeatHero){
//				for (int i = hero->getIndexX() + 1; i < MAX_COLUMN; i++){
//					int index = i*MAX_COLUMN + hero->getIndexY();
//					if (monsterHeros[index] == nullptr){
//						HeroObj * h = hero->cloneThis(i);
//						for (auto& vHeroes : allVMonsters){//带上&  代表我要修改列表中的值  等同于 begin end
//							for (auto vHero = vHeroes.begin(); vHero != vHeroes.end(); vHero++){
//								if (hero->getId() == (*vHero)->getId()){
//									vHeroes.erase(vHero);
//									vHeroes.push_back(h);
//									monsterHeros[index] = h;
//									//删除原数组中的  添加一个后面的
//									break;
//								}
//							}
//						}
//						break;
//					}
//				}
//			}
//			monsterPrepareAction(allHMonsters);
//			monsterDefAction(allVMonsters);
//			//次数加一
//			if (isAiRound){
//				monsterActCount++;
//			}
//			else{
//				mActCount++;
//			}
//			updateActTime();
//
//		}
//
//	}
//	else if (!allHMonsters.empty()){
//		monsterPrepareAction(allHMonsters);
//	}
//	else if (!allVMonsters.empty()){
//		monsterDefAction(allVMonsters);
//	}
//	if (isClick){//是人单击就减少一次
//		//回合过后不减
//		//次数减一
//		monsterActCount--;
//		updateActTime();
//		if (monsterActCount <= 0 ){
//			actRoundChage();
//		}
//	}
//}
//void GameScene::monsterPrepareAction(std::vector<std::vector<HeroObj *>> allHHeroes){
//
//}
//void GameScene::monsterDefAction(std::vector<std::vector<HeroObj *>> allVHeroes){
//
//}
//void GameScene::swapMonsterPosition(HeroObj * from, HeroObj * to){
//
//}
//void GameScene::runMonsterPosition(){
//	
//}

void GameScene::aiSchedule(float delay){
	if (monsterActCount <= 0){
		return;
	}
	bool isWalk = false;
	if (isAiRound && !isActionRuning){
		HeroObj * mSelectObj;
		//HeroObj * mMidObj;
		//横向 一步 消失
		for (int i = 0; i < MAX_ROW; i++){
			//
			int standCount = 0;
			for (int j = 0; j < MAX_COLUMN; j++){
				int index = j * MAX_ROW + i;
				if (monsterHeros[index] == nullptr){
					break;
				}
				if (monsterHeros[index]->getActionType() == HeroActionType::Stand){
					standCount++;
				}
				else{
					standCount = 0;
				}
				//mSelectObj
				if (standCount >= 4){
					mSelectObj = monsterHeros[(j - 3)* MAX_ROW + i];
					int lastIndex = (j - 2)* MAX_ROW + i;
					int lastLastIndex = (j - 1)* MAX_ROW + i;
					int lastLastLastIndex = j * MAX_ROW + i;
					int mCurrrntId = mSelectObj->getHero()->getId();
					if (mCurrrntId == monsterHeros[lastIndex]->getHero()->getId()
						&& mCurrrntId == monsterHeros[lastLastLastIndex]->getHero()->getId()){
						// 1 1 0 1
						gameSceneLayout->removeChild(monsterHeros[lastLastIndex]->getMCurrentNode(), true);
						monsterHeros[lastLastIndex] = nullptr;
						existMonsterCount--;
						updateHeroCount();
						for (int k = j; k < MAX_COLUMN; k++){
							if (monsterHeros[k * MAX_ROW + i] != nullptr){
								HeroObj * mHero = monsterHeros[k * MAX_ROW + i];
								mHero->setIndexX(k - 1);
								mHero->updateNode();
								mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
								monsterHeros[(k - 1) * MAX_ROW + i] = mHero;
								monsterHeros[k * MAX_ROW + i] = nullptr;
							}
							else{
								break;
							}
						}
						//判断 准备和防御
						mHeroPrepareAndDef(true,monsterHeros);
						isActionRuning = true;
						isWalk = true;
						this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
							isActionRuning = false;
						}),NULL));
						break;
					}
					else if (mCurrrntId == monsterHeros[lastLastIndex]->getHero()->getId()
						&& mCurrrntId == monsterHeros[lastLastLastIndex]->getHero()->getId()){
						// 1 0 1 1 
						gameSceneLayout->removeChild(monsterHeros[lastIndex]->getMCurrentNode(), true);
						monsterHeros[lastIndex] = nullptr;
						existMonsterCount--;
						updateHeroCount();
						for (int k = j - 1; k < MAX_COLUMN; k++){
							if (monsterHeros[k * MAX_ROW + i] != nullptr){
								HeroObj * mHero = monsterHeros[k * MAX_ROW + i];
								mHero->setIndexX(k - 1);
								mHero->updateNode();
								mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
								monsterHeros[(k - 1) * MAX_ROW + i] = mHero;
								monsterHeros[k * MAX_ROW + i] = nullptr;
							}
							else{
								break;
							}
						}
						//判断 准备和防御
						mHeroPrepareAndDef(true, monsterHeros);
						isActionRuning = true;
						isWalk = true;
						this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
							isActionRuning = false;
						}), NULL));
						break;
					}
				}
			}
			if (isWalk){
				break;
			}
		}

		if (isWalk){
			return;
		}

		//横向 一步 移动
		for (int i = 0; i < MAX_COLUMN; i++){
			for (int j = MAX_COLUMN - 1; j > 0 ; j--){
				int index = j * MAX_ROW + i;
				if (monsterHeros[index] == nullptr){
					continue;
				}
				if (monsterHeros[index]->getActionType() != HeroActionType::Stand){
					break;
				}
				if (monsterHeros[index]->getHero()->getId() == monsterHeros[index - MAX_ROW]->getHero()->getId()){
					for (int k = 0; k < MAX_COLUMN; k++){
						bool isCan = false;
						HeroObj * mCurrentMonster = nullptr ;

						for (int l = MAX_COLUMN - 1; l >= 0; l--){
							int index2 = l * MAX_ROW + k;
							if (monsterHeros[index2] == nullptr){
								continue;
							}
							if (monsterHeros[index2]->getActionType() != HeroActionType::Stand){
								break;
							}
							/*if (l > 0){ // 有点复杂...
								if (monsterHeros[index]->getHero()->getId() == monsterHeros[index2]->getHero()->getId()
									&& monsterHeros[index]->getHero()->getId() != monsterHeros[index2 - MAX_ROW]->getHero()->getId()){

								}
								else{
									
								}
							}
							else{
								if (monsterHeros[index]->getHero()->getId() == monsterHeros[k]->getHero()->getId()){
									
								}
							}*/
							if (monsterHeros[index]->getHero()->getId() == monsterHeros[index2]->getHero()->getId()){
								isCan = true;
								mCurrentMonster = monsterHeros[index2];
								break;
							}
							else{
								break;
							}
						}
						if (isCan && i != k){
							isActionRuning = true;
							isWalk = true;
							mCurrentMonster->getMCurrentNode()->runAction(Sequence::create(FadeTo::create(0.2f, 125),DelayTime::create(0.3f),
								MoveTo::create(0.1f *(MAX_COLUMN - mCurrentMonster->getIndexX()), Vec2(mSceneSize.width + 60, mCurrentMonster->getpositionY())),
								MoveTo::create(0, Vec2(mSceneSize.width + 60, monsterHeros[index]->getpositionY())), MoveTo::create(0.1f * (MAX_COLUMN - i - 1),
								Vec2(monsterHeros[index]->getpositionX() + heroW, monsterHeros[index]->getpositionY())), FadeTo::create(0.2f, 255),
							CallFunc::create([=](){
								monsterHeros[mCurrentMonster->getId()] = nullptr;
								mCurrentMonster->setIndexX(j+1);
								mCurrentMonster->setIndexY(i);
								mCurrentMonster->updateNode();
								monsterHeros[index + MAX_ROW] = mCurrentMonster;
							}), CallFunc::create([=](){
									//判断 准备和防御
								mHeroPrepareAndDef(true, monsterHeros);
								this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
									isActionRuning = false;
								}), NULL));
							}), NULL));
							break;
						}
					}
				}
				else{
					break;
				}
			}
			if (isWalk){
				break;
			}
		}

		if (isWalk){
			return;
		}

		//横向 两步 消失
		for (int i = 0; i < MAX_ROW; i++){
			//
			int standCount = 0;
			for (int j = 0; j < MAX_COLUMN; j++){
				int index = j * MAX_ROW + i;
				if (monsterHeros[index] == nullptr){
					break;
				}
				if (monsterHeros[index]->getActionType() == HeroActionType::Stand){
					standCount++;
				}
				else{
					standCount = 0;
				}
				//mSelectObj
				if (standCount >= 5){
					mSelectObj = monsterHeros[(j - 4)* MAX_ROW + i];
					int lastIndex = (j - 3)* MAX_ROW + i;
					int lastLastIndex = (j - 2)* MAX_ROW + i;
					int lastLastLastIndex = (j - 1)* MAX_ROW + i;
					int lastLastLastLastIndex = j* MAX_ROW + i;
					int mCurrrntId = mSelectObj->getHero()->getId();
					if (mCurrrntId == monsterHeros[lastIndex]->getHero()->getId()
						&& mCurrrntId == monsterHeros[lastLastLastLastIndex]->getHero()->getId()){
						// 1 1 0 0 1
						gameSceneLayout->removeChild(monsterHeros[lastLastIndex]->getMCurrentNode(), true);
						monsterHeros[lastLastIndex] = nullptr;
						existMonsterCount--;
						updateHeroCount();
						for (int k = j; k < MAX_COLUMN; k++){
							if (monsterHeros[k * MAX_ROW + i] != nullptr){
								HeroObj * mHero = monsterHeros[k * MAX_ROW + i];
								mHero->setIndexX(k - 1);
								mHero->updateNode();
								mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
								monsterHeros[(k - 1) * MAX_ROW + i] = mHero;
								monsterHeros[k * MAX_ROW + i] = nullptr;
							}
							else{
								break;
							}
						}
						//判断 准备和防御
						mHeroPrepareAndDef(true, monsterHeros);
						isActionRuning = true;
						isWalk = true;
						this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
							isActionRuning = false;
						}), NULL));
						break;
					}
					else if (mCurrrntId == monsterHeros[lastLastLastLastIndex]->getHero()->getId()
						&& mCurrrntId == monsterHeros[lastLastLastIndex]->getHero()->getId()){
						// 1 0 0 1 1 
						gameSceneLayout->removeChild(monsterHeros[lastIndex]->getMCurrentNode(), true);
						monsterHeros[lastIndex] = nullptr;
						existMonsterCount--;
						updateHeroCount();
						for (int k = j - 1; k < MAX_COLUMN; k++){
							if (monsterHeros[k * MAX_ROW + i] != nullptr){
								HeroObj * mHero = monsterHeros[k * MAX_ROW + i];
								mHero->setIndexX(k - 1);
								mHero->updateNode();
								mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
								monsterHeros[(k - 1) * MAX_ROW + i] = mHero;
								monsterHeros[k * MAX_ROW + i] = nullptr;
							}
							else{
								break;
							}
						}
						//判断 准备和防御
						mHeroPrepareAndDef(true, monsterHeros);
						isActionRuning = true;
						isWalk = true;
						this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
							isActionRuning = false;
						}), NULL));
						break;
					}
					else if (mCurrrntId == monsterHeros[lastLastLastLastIndex]->getHero()->getId()
						&& mCurrrntId == monsterHeros[lastLastIndex]->getHero()->getId()){
						// 1 0 1 0 1 
						gameSceneLayout->removeChild(monsterHeros[lastIndex]->getMCurrentNode(), true);
						monsterHeros[lastIndex] = nullptr;
						existMonsterCount--;
						updateHeroCount();
						for (int k = j - 1; k < MAX_COLUMN; k++){
							if (monsterHeros[k * MAX_ROW + i] != nullptr){
								HeroObj * mHero = monsterHeros[k * MAX_ROW + i];
								mHero->setIndexX(k - 1);
								mHero->updateNode();
								mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
								monsterHeros[(k - 1) * MAX_ROW + i] = mHero;
								monsterHeros[k * MAX_ROW + i] = nullptr;
							}
							else{
								break;
							}
						}
						//判断 准备和防御
						mHeroPrepareAndDef(true, monsterHeros);
						isActionRuning = true;
						isWalk = true;
						this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
							isActionRuning = false;
						}), NULL));
						break;
					}
				}
			}
			if (isWalk){
				break;
			}
		}

		if (isWalk){
			return;
		}

		//横向 两步 移动
		for (int i = 0; i < MAX_COLUMN; i++){
			for (int j = MAX_COLUMN - 1; j > 1; j--){
				int index = j * MAX_ROW + i;
				if (monsterHeros[index] == nullptr){
					continue;
				}
				if (monsterHeros[index]->getActionType() != HeroActionType::Stand){
					break;
				}
				if (monsterHeros[index - MAX_ROW]->getHero()->getId() == monsterHeros[index - 2 * MAX_ROW]->getHero()->getId()){
					int moveX = 0;
					int moveY = 0;
					//移走
					bool isCan = false;
					for (int k = 0; k < MAX_COLUMN; k++){
						for (int l = MAX_COLUMN - 1; l >= 0; l--){
							int index2 = l * MAX_ROW + k;
							if (l == 0){
								if (monsterHeros[index2] == nullptr){
									moveX = l;
									moveY = k;
									isCan = true;
									break;
								}
							}
							else{
								if (monsterHeros[index2] == nullptr && monsterHeros[index2 - MAX_ROW] != nullptr &&
									monsterHeros[index2 - MAX_ROW]->getHero()->getId() != monsterHeros[index - MAX_ROW]->getHero()->getId()){
									moveX = l;
									moveY = k;
									isCan = true;
									break;
								}
							}
						}
						if (isCan){
							break;
						}
					}
					if (isCan){
						isActionRuning = true;
						HeroObj * mCurrentHero = monsterHeros[index];
						mCurrentHero->getMCurrentNode()->runAction(Sequence::create(FadeTo::create(0.2f, 125), DelayTime::create(0.3f),
							MoveTo::create(0.1f *(MAX_COLUMN - mCurrentHero->getIndexX()), Vec2(mSceneSize.width + 60, mCurrentHero->getpositionY())),
							CallFunc::create([=](){
							monsterHeros[mCurrentHero->getId()] = nullptr;
							mCurrentHero->setIndexX(moveX);
							mCurrentHero->setIndexY(moveY);
							mCurrentHero->updateNode();
							monsterHeros[mCurrentHero->getId()] = mCurrentHero;
						}),
							MoveTo::create(0, Vec2(mSceneSize.width + 60, mCurrentHero->getpositionY())), MoveTo::create(0.1f * (MAX_COLUMN - moveX),
							Vec2(mCurrentHero->getpositionX(), mCurrentHero->getpositionY())), FadeTo::create(0.2f, 255),
							CallFunc::create([=](){
							//判断 准备和防御
							mHeroPrepareAndDef(true, monsterHeros);
							this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
								isActionRuning = false;
							}), NULL));
						}), NULL));
					}
				}
				else{
					break;
				}
			}
			if (isWalk){
				break;
			}
		}

		if (isWalk){
			return;
		}

		//纵向 一步 消失 // 上到下查找
		for (int i = 0; i < MAX_COLUMN - 1; i++){
			for (int j = 0; j < MAX_ROW - 2; j++){
				int index = i * MAX_ROW + j;
				if (monsterHeros[index] == nullptr){
					continue;
				}
				if (monsterHeros[index]->getActionType() != HeroActionType::Stand){
					continue;
				}
				int next = index + MAX_ROW;
				int last = index + 1;
				int lastLast = index + 2;
				int nextLast = index + MAX_ROW + 1;
				int nextLastLast = index + MAX_ROW + 2;
				if (monsterHeros[last] != nullptr && monsterHeros[nextLastLast] != nullptr){
					if (monsterHeros[last]->getActionType() != HeroActionType::Stand ||
						monsterHeros[lastLast]->getActionType() != HeroActionType::Stand){
						continue;
					}
					if (monsterHeros[index]->getHero()->getId() == monsterHeros[last]->getHero()->getId()
						&& monsterHeros[index]->getHero()->getId() == monsterHeros[nextLastLast]->getHero()->getId()){
						gameSceneLayout->removeChild(monsterHeros[lastLast]->getMCurrentNode(), true);
						monsterHeros[lastLast] = nullptr;
						existMonsterCount--;
						updateHeroCount();
						for (int k = i + 1; k < MAX_COLUMN; k++){
							if (monsterHeros[k * MAX_ROW + j + 2] != nullptr){
								HeroObj * mHero = monsterHeros[k * MAX_ROW + j + 2];
								mHero->setIndexX(k - 1);
								mHero->updateNode();
								mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
								monsterHeros[(k - 1) * MAX_ROW + j + 2] = mHero;
								monsterHeros[k * MAX_ROW + j + 2] = nullptr;
							}
							else{

								break;
							}
						}
						this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=](){
							//判断 准备和防御
							mHeroPrepareAndDef(true, monsterHeros);
							isActionRuning = true;
							this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
								isActionRuning = false;
							}), NULL));
						}), NULL));
						isWalk = true;
						break;
					}
				}
				
				if (monsterHeros[nextLast] != nullptr && monsterHeros[lastLast] != nullptr){
					if (monsterHeros[nextLast]->getActionType() != HeroActionType::Stand ||
						monsterHeros[lastLast]->getActionType() != HeroActionType::Stand){
						continue;
					}
					if (monsterHeros[index]->getHero()->getId() == monsterHeros[nextLast]->getHero()->getId()
						&& monsterHeros[index]->getHero()->getId() == monsterHeros[lastLast]->getHero()->getId()){
						gameSceneLayout->removeChild(monsterHeros[last]->getMCurrentNode(), true);
						monsterHeros[last] = nullptr;
						existMonsterCount--;
						updateHeroCount();

						for (int k = i + 1; k < MAX_COLUMN; k++){
							if (monsterHeros[k * MAX_ROW + j + 1] != nullptr){
								HeroObj * mHero = monsterHeros[k * MAX_ROW + j + 1];
								mHero->setIndexX(k - 1);
								mHero->updateNode();
								mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
								monsterHeros[(k - 1) * MAX_ROW + j + 1] = mHero;
								monsterHeros[k * MAX_ROW + j + 1] = nullptr;
							}
							else{
								break;
							}
						}
						this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=](){
							//判断 准备和防御
							mHeroPrepareAndDef(true, monsterHeros);
							isActionRuning = true;
							this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
								isActionRuning = false;
							}), NULL));
						}), NULL));
						isWalk = true;
						break;
					}
				}
				
				if (monsterHeros[next] != nullptr && monsterHeros[lastLast] != nullptr
					&& monsterHeros[last] != nullptr){
					if (monsterHeros[next]->getActionType() != HeroActionType::Stand ||
						monsterHeros[lastLast]->getActionType() != HeroActionType::Stand ||
						monsterHeros[last]->getActionType() != HeroActionType::Stand){
						continue;
					}
					if (monsterHeros[next]->getHero()->getId() == monsterHeros[last]->getHero()->getId()
						&& monsterHeros[next]->getHero()->getId() == monsterHeros[lastLast]->getHero()->getId()){
						gameSceneLayout->removeChild(monsterHeros[index]->getMCurrentNode(), true);
						monsterHeros[index] = nullptr;
						existMonsterCount--;
						updateHeroCount();
						for (int k = i + 1; k < MAX_COLUMN; k++){
							if (monsterHeros[k * MAX_ROW + j] != nullptr){
								HeroObj * mHero = monsterHeros[k * MAX_ROW + j];
								mHero->setIndexX(k - 1);
								mHero->updateNode();
								mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
								monsterHeros[(k - 1) * MAX_ROW + j] = mHero;
								monsterHeros[k * MAX_ROW + j] = nullptr;
							}
							else{
								break;
							}
						}
						this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=](){
							//判断 准备和防御
							mHeroPrepareAndDef(true, monsterHeros);
							isActionRuning = true;
							this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
								isActionRuning = false;
							}), NULL));
						}), NULL));
						isWalk = true;
						break;
					}
				}
			}
			if (isWalk){
				break;
			}
		}				

		if (isWalk){
			return;
		}

		//纵向 一步 移动
		for (int i = 0; i < MAX_COLUMN ; i++){
			for (int j = 0; j < MAX_ROW - 2; j++){
				int index = i * MAX_ROW + j;
				if (monsterHeros[index] == nullptr){
					int lastLast = index - 2;
					int last = index - 1;
					int nextNext = index + 2;
					int next = index + 1;
					if (j == 0){
						isWalk = defNext(i, j, index, next, nextNext);
					}
					else if(j == 1){
						if (!isWalk){
							isWalk = defNext(i, j, index, last, next);
						}
						if (!isWalk){
							isWalk = defNext(i, j, index, next, nextNext);
						}
					}
					else if (j == MAX_ROW - 2){
						if (!isWalk){
							isWalk = defNext(i, j, index, last, next);
						}
						if (!isWalk){
							isWalk = defNext(i, j, index, last, lastLast);
						}
					}
					else if (j == MAX_ROW - 1){
						if (!isWalk){
							isWalk = defNext(i, j, index, last, lastLast);
						}
					}
					else{
						if (!isWalk){
							isWalk = defNext(i, j, index, last, lastLast);
						}
						if (!isWalk){
							isWalk = defNext(i, j, index, last, next);
						}
						if (!isWalk){
							isWalk = defNext(i, j, index, next, nextNext);
						}
					}
				}
				if (isWalk){
					break;
				}
			}
			if (isWalk){
				break;
			}
		}

		if (isWalk){
			return;
		}

		//出兵
		if (monstersAI->getHeroesCount() - existMonsterCount >= 1){
			isActionRuning = true;
			monsterEntry();
			monsterActCount--;
			updateActTime();
			if (monsterActCount <= 0){
				actRoundChage();
			}
			this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=](){
				isActionRuning = false;
			}), NULL));
		}
		else{
			this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=](){
				monsterActCount--;
				updateActTime();
				if (monsterActCount <= 0){
					actRoundChage();
				}
			}), NULL));
		}
	}
}

bool GameScene::defNext(int x, int y, int index, int next, int nextNext){
	if (index >= MAX_ROW && monsterHeros[index - MAX_ROW] == nullptr){
		return false;
	}
	if (monsterHeros[next] == nullptr || monsterHeros[nextNext] == nullptr){
		return false;
	}
	if (monsterHeros[next]->getActionType() != HeroActionType::Stand ||
		monsterHeros[nextNext]->getActionType() != HeroActionType::Stand){
		return false;
	}
	if (monsterHeros[next]->getHero()->getId() != monsterHeros[nextNext]->getHero()->getId()){
		return false;
	}

	for (int m = 0; m < MAX_ROW; m++){
		for (int n = MAX_COLUMN - 1; n > 0; n--){
			int item = n * MAX_ROW + m;
			if (monsterHeros[item] != nullptr && monsterHeros[item]->getActionType() == HeroActionType::Stand){
				if (monsterHeros[item]->getHero()->getId() == monsterHeros[next]->getHero()->getId()
					&& monsterHeros[item]->getId() != monsterHeros[next]->getId() &&
					monsterHeros[item]->getId() != monsterHeros[nextNext]->getId()){
					isActionRuning = true;
					HeroObj * mCurrentMonster = monsterHeros[item];
					mCurrentMonster->getMCurrentNode()->runAction(Sequence::create(FadeTo::create(0.2f, 125), DelayTime::create(0.3f),
						MoveTo::create(0.1f *(MAX_COLUMN - mCurrentMonster->getIndexX()), Vec2(mSceneSize.width + 60, mCurrentMonster->getpositionY())),
						CallFunc::create([=](){
						monsterHeros[mCurrentMonster->getId()] = nullptr;
						mCurrentMonster->setIndexX(x);
						mCurrentMonster->setIndexY(y);
						mCurrentMonster->updateNode();
						monsterHeros[index] = mCurrentMonster;
					}),
						MoveTo::create(0, Vec2(mSceneSize.width + 60, monsterHeros[index]->getpositionY())), MoveTo::create(0.1f * (MAX_COLUMN - x),
						Vec2(monsterHeros[index]->getpositionX(), monsterHeros[index]->getpositionY())), FadeTo::create(0.2f, 255),
						CallFunc::create([=](){
						//判断 准备和防御
						mHeroPrepareAndDef(true, monsterHeros);
						this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
							isActionRuning = false;
						}), NULL));
					}), NULL));
					return true;
				}
				break;
			}
		}
	}
}

void GameScene::timeSchedule(float delay){
	if (!isRoundChangeing){
		secCurrentTime--;
		if (secCurrentTime < 0){
			secCurrentTime = 0;
			actRoundChage();
		}
		secTimeAtlas->setString(String::createWithFormat("%d", secCurrentTime)->getCString());
	}
}

void GameScene::update(float dt){

	bool heroIsDeath = false;
	for (int i = 0; i < AllHeroCount; i++){ //遍历monster
		auto hero = monsterHeros[i];
		if (hero == nullptr){
			continue;
		}
		if (hero->getHeroHP() <= 0 || hero->getHeroATK() <= 0){//monster hp小于0
			//移除列表中的攻击duixiang
			for (auto preHeroes = prepareMonster.begin(); preHeroes != prepareMonster.end(); preHeroes++){
				for (auto atkHero = (*preHeroes).begin(); atkHero != (*preHeroes).end(); atkHero++){
					if ((*atkHero)->getId() == hero->getId()){
						(*preHeroes).clear();
						break;
					}
				}
				if ((*preHeroes).empty()){
					prepareMonster.erase(preHeroes);
					break;
				}
			}
			isActionRuning = false;
			existMonsterCount--;
			updateHeroCount();
			hero->death();
			hero = nullptr;
			monsterHeros[i] = nullptr;
			heroIsDeath = true;
		}
	}
	for (int i = 0; i < AllHeroCount; i++){ //遍历hero
		auto hero = mHeroes[i];
		if (hero == nullptr){
			continue;
		}
		if (hero->getHeroHP() <= 0 || hero->getHeroATK() <= 0){//hero hp小于0
			//移除列表中的攻击duixiang
			for (auto preHeroes = prepareHeroes.begin(); preHeroes != prepareHeroes.end(); preHeroes++){
				for (auto atkHero = (*preHeroes).begin(); atkHero != (*preHeroes).end(); atkHero++){
					if ((*atkHero)->getId() == hero->getId()){
						(*preHeroes).clear();
						break;
					}
				}
				if ((*preHeroes).empty()){
					prepareHeroes.erase(preHeroes);
					break;
				}
			}
			isActionRuning = false;
			existHeroCount--;
			updateHeroCount();
			hero->death();
			hero = nullptr;
			mHeroes[i] = nullptr;
			heroIsDeath = true;
		}
	}

	for (int i = 0; i < AllHeroCount; i++){ //遍历敌人
		auto monsterObj = monsterHeros[i];
		if (monsterObj == nullptr){
			continue;
		}
		if (monsterObj->getHeroHP() <= 0 || monsterObj->getHeroATK() <= 0){//敌人hp小于0
			monsterObj->death();
			monsterObj = nullptr;
			monsterHeros[i] = nullptr;
		}
	}

	for (int i = 0; i < AllHeroCount; i++){ //遍历
		auto heroObj = mHeroes[i];
		if (heroObj == nullptr){
			continue;
		}
		if (heroObj->getHeroHP() <= 0 || heroObj->getHeroATK() <= 0){//hp小于0
			heroObj->death();
			heroObj = nullptr;
			mHeroes[i] = nullptr;
		}
	}
	
	//有死亡
	if ( heroIsDeath && !isActionRuning){
		isActionRuning = true;
		//移动完后
		stopActionByTag(HeroRunActionTag);
		Action * action = Sequence::create(DelayTime::create(0.2f), CallFunc::create([=](){
			runHeroPosition(isAiRound ? monsterHeros : mHeroes);
			isActionRuning = false;
		}), NULL);
		action->setTag(HeroRunActionTag);
		runAction(action);
	}

	if (isAiRound){
		//判断怪物
		for (auto preHeroes : prepareMonster){
			for (auto atkHero : preHeroes){
				if (atkHero->getHeroACT() > 0){
					break;
				}
				isActionRuning = true;
				for (int i = 0; i < AllHeroCount; i++){ //遍历敌人
					auto monsterObj = mHeroes[i];
					if (monsterObj == nullptr || atkHero == nullptr){
						continue;
					}
					if (atkHero->collision(monsterObj)){//如果碰撞
						if (preHeroes.size() >= 1){//看看准备的对象中是否大于一个
							HeroObj * h1 = preHeroes.at(0);//获取这个主要的对象 通过他的攻击hp 改变其他的对象
							h1->attack([=](Node * node){
								int hp = h1->getHeroATK() - monsterObj->getHeroHP() - monsterObj->getHeroDEF();//攻击-血量和防御
								int monsHp = -hp;//怪物血量相反
								if (hp < 0){
									hp = 0;
									isActionRuning = false;
								}
								if (monsHp < 0){
									monsHp = 0;
								}
								int atkHp = monsterObj->getHeroHP() - monsHp;
								showCupBlood(atkHp, node->getPositionX(), node->getPositionY(), monsterObj->getIndexY());
								//h1->setHeroHP(hp);
								h1->setHeroATK(hp);
								h1->updateATK();
								monsterObj->setHeroHP(monsHp);
								if (monsterObj->getActionType() == HeroActionType::Prepare){
									HeroObj * hero1 = mHeroes[monsterObj->getId() + MAX_ROW];
									HeroObj * hero2 = mHeroes[monsterObj->getId() + 2 * MAX_ROW];
									hero1->setHeroHP(monsHp);
									hero1->setHeroATK(monsHp);
									hero1->updateATK();
									hero2->setHeroHP(monsHp);
									hero2->setHeroATK(monsHp);
									hero2->updateATK();
								}
								if (preHeroes.size() >= 2){//修改次要的对象
									HeroObj * h2 = preHeroes.at(1);
									//h2->setHeroHP(hp);
									h2->setHeroATK(hp);
									h2->updateATK();
								}
								if (preHeroes.size() >= 3){
									HeroObj * h3 = preHeroes.at(2);
									//h3->setHeroHP(hp);
									h3->setHeroATK(hp);
								}
							}, [=](){
								h1->attack([=](Node * node){
									int hp = 0;
									//打墙
									if (h1->getIsMonster()){
										hp = h1->getHeroATK() - mDEF;
										mCurrentHP = mCurrentHP - hp;
										showCupBlood(hp, 0 + heroW, node->getPositionY(), h1->getIndexY());
									}
									else{
										hp = h1->getHeroATK() - monsterDEF;
										monsterCurrentHP = monsterCurrentHP - hp;
										showCupBlood(hp, mSceneSize.width - heroW, node->getPositionY(), h1->getIndexY());
									}
									h1->setHeroHP(0);
									h1->setHeroATK(0);
									updateHPMP();
									isActionRuning = false;
								}, nullptr);
							});
							if (preHeroes.size() >= 2){//次要对象跟随动作
								HeroObj * h2 = preHeroes.at(1);
								h2->attack(nullptr, [=](){
									h2->attack([=](Node * node){
										//打墙
										h2->setHeroHP(0);
										h2->setHeroATK(0);
									}, nullptr);
								});
							}
							if (preHeroes.size() >= 3){
								HeroObj * h3 = preHeroes.at(2);
								h3->attack(nullptr, [=](){
									h3->attack([=](Node * node){
										//打墙
										h3->setHeroHP(0);
										h3->setHeroATK(0);
									}, nullptr);
								});
							}
						}
					}
				}
			}
		}
	}
	else{
		for (auto preHeroes : prepareHeroes){
			for (auto atkHero : preHeroes){
				if (atkHero->getHeroACT() > 0){
					break;
				}
				isActionRuning = true;
				for (int i = 0; i < AllHeroCount; i++){ //遍历敌人
					auto monsterObj = monsterHeros[i];
					if (monsterObj == nullptr || atkHero == nullptr){
						continue;
					}
					if (atkHero->collision(monsterObj)){//如果碰撞
						if (preHeroes.size() >= 1){//看看准备的对象中是否大于一个
							HeroObj * h1 = preHeroes.at(0);//获取这个主要的对象 通过他的攻击hp 改变其他的对象
							h1->attack([=](Node * node){
								int hp = h1->getHeroATK() - monsterObj->getHeroHP() - monsterObj->getHeroDEF();//攻击-血量和防御
								int monsHp = -hp;//怪物血量相反
								if (hp < 0){
									hp = 0;
								}
								if (monsHp < 0){
									monsHp = 0;
								}
								int atkHp = monsterObj->getHeroHP() - monsHp;
								showCupBlood(atkHp, node->getPositionX(), node->getPositionY(), monsterObj->getIndexY());
								//h1->setHeroHP(hp);
								h1->setHeroATK(hp);
								h1->updateATK();
								monsterObj->setHeroHP(monsHp);

								if (monsterObj->getActionType() == HeroActionType::Prepare){
									HeroObj * hero1 = monsterHeros[monsterObj->getId() + MAX_ROW];
									HeroObj * hero2 = monsterHeros[monsterObj->getId() + 2 * MAX_ROW];
									hero1->setHeroHP(monsHp);
									hero1->setHeroATK(monsHp);
									hero2->setHeroHP(monsHp);
									hero2->setHeroATK(monsHp);
								}

								if (preHeroes.size() >= 2){//修改次要的对象
									HeroObj * h2 = preHeroes.at(1);
									//h2->setHeroHP(hp);
									h2->setHeroATK(hp);
									h2->updateATK();
								}
								if (preHeroes.size() >= 3){
									HeroObj * h3 = preHeroes.at(2);
									//h3->setHeroHP(hp);
									h3->setHeroATK(hp);
								}
							}, [=](){
								h1->attack([=](Node * node){
									int hp = h1->getHeroATK() - monsterDEF;
									//打墙
									h1->setHeroHP(0);
									h1->setHeroATK(0);
									monsterCurrentHP = monsterCurrentHP - hp;
									showCupBlood(hp, mSceneSize.width - heroW, node->getPositionY(), h1->getIndexY());
									updateHPMP();
								}, nullptr);
							});
							if (preHeroes.size() >= 2){//次要对象跟随动作
								HeroObj * h2 = preHeroes.at(1);
								h2->attack(nullptr, [=](){
									h2->attack([=](Node * node){
										//打墙
										h2->setHeroHP(0);
										h2->setHeroATK(0);
									}, nullptr);
								});
							}
							if (preHeroes.size() >= 3){
								HeroObj * h3 = preHeroes.at(2);
								h3->attack(nullptr, [=](){
									h3->attack([=](Node * node){
										//打墙
										h3->setHeroHP(0);
										h3->setHeroATK(0);
									}, nullptr);
								});
							}
						}
					}
				}
			}
		}
	}
	
	
}

void GameScene::heroCountLCallBack(Ref * ref){
	if (existHeroCount < mUserInfo->getHeroesCount() && !isAiRound && mActCount >= 1){
		heroEntry();
		stopActionByTag(HeroEnterActionTag);
		Action * action = Sequence::create(DelayTime::create(2), CallFunc::create([=](){
			mHeroPrepareAndDef(true,mHeroes);
			if (mActCount == 0){
				//移动完后
				actRoundChage();
			}
			else{
				updateHeroCount();
			}
		}), NULL);
		action->setTag(HeroEnterActionTag);
		runAction(action);
	}
}

void GameScene::runHeroPosition(HeroObj * mHeros[]){
	for (int i = 0; i < MAX_ROW; i++){//正着来吧
		for (int j = 1; j < MAX_COLUMN; j++){
			int index = j * MAX_ROW + i;
			int nexIndex = index - MAX_ROW;
			if (mHeros[index] != nullptr){
				if (mHeros[nexIndex] == nullptr){
					for (int k = j; k > 0; k--){
						int posIndex = k * MAX_ROW + i;
						int lastPosIndex = (k - 1)* MAX_ROW + i;
						if (mHeros[lastPosIndex] == nullptr){
							swapHeroPosition(mHeros[posIndex], nullptr,mHeros);
						}
					}
				}
			}
		}
	}
}

void GameScene::showCupBlood(int cupBlood,int posX,int posY,int zOder){
	std::string atkStr = String::createWithFormat("%d", cupBlood)->getCString();
	TextAtlas * atkAtlas = TextAtlas::create(atkStr, "atk_hp_num.png", 30, 35, ".");
	atkAtlas->setAnchorPoint(Vec2(0.5, 0.5));
	atkAtlas->setPosition(Vec2(posX, posY));
	int x = CCRANDOM_0_1() * 30;
	int y = CCRANDOM_0_1() * 30;
	if (CCRANDOM_0_1() >= 0.5){
		x = -x;
	}
	if (CCRANDOM_0_1() >= 0.5){
		y = -y;
	}
	atkAtlas->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(x, y)), MoveBy::create(0.3f, Vec2(-y, x))
		, FadeOut::create(0.3f), CallFuncN::create([](Node * node){
		node->removeFromParentAndCleanup(true);
	}), NULL));
	gameSceneLayout->addChild(atkAtlas, zOder);
}

void GameScene::onExit(){
	Layer::onExit();
	unschedule(schedule_selector(GameScene::aiSchedule));
	unscheduleUpdate();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("HeroMuBei.plist");
	int index = 0;
	for (auto heros = baseHeroes.begin(); heros != baseHeroes.end(); heros++){
		//(*heros)->setType((HeroType)index);
		index++;
		std::string heroesFileName = String::createWithFormat("%s.plist", (*heros)->getName().c_str())->getCString();
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(heroesFileName);
	}
	index = 0;
	for (auto monster = baseMonsters.begin(); monster != baseMonsters.end(); monster++){
		//(*monster)->setType((HeroType)index);
		index++;
		std::string heroesFileName = String::createWithFormat("%s.plist", (*monster)->getName().c_str())->getCString();
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(heroesFileName);
	}
}