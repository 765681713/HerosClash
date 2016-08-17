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

	mSceneSize = Director::getInstance()->getVisibleSize();
	gameSceneNode = CSLoader::createNode("GameScene.csb");
	gameSceneNode->setAnchorPoint(Vec2::ZERO);
	gameSceneNode->setPosition(Vec2::ZERO);
	this->addChild(gameSceneNode);
	gameSceneLayout = static_cast<Layout *>(gameSceneNode->getChildByName("GameSceneL"));
	//ImageView * jiNeng1;
	leftWall = static_cast<Sprite *>(gameSceneLayout->getChildByName("game_left_wall"));
	rightWall = static_cast<Sprite *>(gameSceneLayout->getChildByName("game_reight_wall"));
	Layout * mInfo = static_cast<Layout *>(gameSceneNode->getChildByName("JueSeInfoL"));
	Sprite * jueSeIcon = static_cast<Sprite *>(mInfo->getChildByName("JueSeIcon"));
	//juerSeIcon->load
	mHPLoadingBar = static_cast<LoadingBar *>(mInfo->getChildByName("HPLoadingBar"));
	mMPLoadingBar = static_cast<LoadingBar *>(mInfo->getChildByName("MPLoadingBar"));
	mHPAtlas = static_cast<TextAtlas *>(mInfo->getChildByName("HPAtlas"));
	mMPAtlas = static_cast<TextAtlas *>(mInfo->getChildByName("MPAtlas"));
	mLevel = static_cast<Text *>(mInfo->getChildByName("JueSeLevel"));

	Layout * monsterInfo = static_cast<Layout *>(gameSceneNode->getChildByName("MonsterInfoL"));
	Sprite * monsterIcon = static_cast<Sprite *>(monsterInfo->getChildByName("MIcon"));
	//monsterIcon->load
	monsterHPLoadingBar = static_cast<LoadingBar *>(monsterInfo->getChildByName("HPLoadingBar"));
	monsterMPLoadingBar = static_cast<LoadingBar *>(monsterInfo->getChildByName("MPLoadingBar"));
	monsterHPAtlas = static_cast<TextAtlas *>(monsterInfo->getChildByName("HPAtlas"));
	monsterMPAtlas = static_cast<TextAtlas *>(monsterInfo->getChildByName("MPAtlas"));
	monsterLevel = static_cast<Text *>(monsterInfo->getChildByName("MLevel"));
	roundCountAtlas = static_cast<TextAtlas *>(gameSceneNode->getChildByName("RoundL")->getChildByName("RoundCountAtlas"));
	secTimeAtlas = static_cast<TextAtlas *>(gameSceneNode->getChildByName("SecTimeAtlas"));
	mActCountAtlas = static_cast<TextAtlas *>(gameSceneNode->getChildByName("JActL")->getChildByName("ActCountAtlas"));
	monsterActCountAtlas = static_cast<TextAtlas *>(gameSceneNode->getChildByName("MActL")->getChildByName("ActCountAtlas"));
	mHeroAllCount = static_cast<TextAtlas *>(gameSceneNode->getChildByName("MHeroCountL")->getChildByName("HeroCountAtlas"));
	monsterAllCount = static_cast<TextAtlas *>(gameSceneNode->getChildByName("MonsterCountL")->getChildByName("monsterCountAtlas"));
	//事件监听
	eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto gameSceneListener = EventListenerTouchOneByOne::create();
	gameSceneListener->setSwallowTouches(true);
	gameSceneListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	gameSceneListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	gameSceneListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	eventDispatcher->addEventListenerWithSceneGraphPriority(gameSceneListener, gameSceneLayout);

	//加载资源
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("HeroMuBei.plist");
	for (auto heros = baseHeroes.begin(); heros != baseHeroes.end(); heros++){
		std::string heroesFileName = String::createWithFormat("%s.plist", (*heros)->getName())->getCString();
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(heroesFileName.c_str());
	}
	for (auto monster = baseMonsters.begin(); monster != baseMonsters.end(); monster++){
		std::string heroesFileName = String::createWithFormat("%s.plist", (*monster)->getName())->getCString();
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(heroesFileName.c_str());
	}
	for (int i = 0; i < MAX_ROW; i++){
		for (int j = 0; j < MAX_COLUMN; j++){
			mHeros[i*MAX_ROW + j] = nullptr;
		}
	}
	for (int i = 0; i < MAX_ROW; i++){
		for (int j = 0; j < MAX_COLUMN; j++){
			monsterHeros[i*MAX_ROW + j] = nullptr;
		}
	}
	return true;
}

void GameScene::onEnterTransitionDidFinish(){
	DelayTime * dTime = DelayTime::create(0);
	Sequence * action = Sequence::create(dTime, CallFunc::create(CC_CALLBACK_0(GameScene::initGame, this)), NULL);
	this->runAction(action);
	srand(time(NULL));
}

void GameScene::heroEntry(){
	for (; existHeroCount < mUserInfo->getHeroesCount();){
		int indexY = (int)(CCRANDOM_0_1() * MAX_ROW);
		int indexType = (int)(CCRANDOM_0_1() * baseHeroes.size());
		Heroes * heroes = baseHeroes.at(indexType);
		if (heroes->getType() == HeroType::Hero){
			for (int j = 0; j < MAX_COLUMN; j++){
				int index = j * MAX_ROW + indexY;
				if (mHeros[index] == nullptr){
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
					hObj->addNode(gameSceneLayout,hero);
					//出场动画
					ActionTimeline * action = CSLoader::createTimeline(nodeName.c_str());
					hero->runAction(action);
					hObj->setAction(action);
					hObj->runAction(HeroActionType::Run);
					hero->runAction(Sequence::create(DelayTime::create(0.4f * j), MoveTo::create(0.3f * (MAX_COLUMN + 1 - j), Vec2(hObj->getpositionX(), hObj->getpositionY())), CallFuncN::create([hObj](Node * node){
						hObj->runAction(HeroActionType::Stand);
					}), NULL));
					mHeros[index] = hObj;
					existHeroCount++;
					break;
				}
			}
		}
		else if (heroes->getType() == HeroType::Boss){
			for (int j = 0; j < MAX_COLUMN - 1; j++){
				int index = j * MAX_ROW + indexY;
				if (indexY == 0){//上边界
					if (mHeros[index] == nullptr && mHeros[index + 1] == nullptr
						&& mHeros[index + MAX_ROW] == nullptr && mHeros[index + MAX_ROW + 1]){//占四个地方
						addBoss(index, j, indexY, hStartX - j * heroW - heroW / 2, hStartY - indexY * heroH - heroH / 2, heroes);
						existHeroCount++;
						break;
					}
				}
				else if (indexY == MAX_ROW - 1){//下边界
					if (mHeros[index] == nullptr && mHeros[index - 1] == nullptr
						&& mHeros[index + MAX_ROW] == nullptr && mHeros[index + MAX_ROW - 1]){//占四个地方
						addBoss(index - 1, j, indexY - 1, hStartX - j * heroW - heroW / 2, hStartY - indexY * heroH + heroH / 2, heroes);
						existHeroCount++;
						break;
					}
				}
				else{//中间
					if (mHeros[index] == nullptr && mHeros[index + 1] == nullptr
						&& mHeros[index + MAX_ROW] == nullptr && mHeros[index + MAX_ROW + 1]){//占四个地方
						addBoss(index, j, indexY, hStartX - j * heroW - heroW / 2, hStartY - indexY * heroH - heroH / 2, heroes);
						existHeroCount++;
						break;
					}
					else if (mHeros[index] == nullptr && mHeros[index - 1] == nullptr
						&& mHeros[index + MAX_ROW] == nullptr && mHeros[index + MAX_ROW - 1]){//占四个地方
						//tag , (0,0) , px py , hero
						addBoss(index - 1, j, indexY - 1, hStartX - j * heroW - heroW / 2, hStartY - indexY * heroH + heroH / 2, heroes);
						existHeroCount++;
						break;
					}
				}
			}
		}
	}
}

void GameScene::monsterEntry(){
	for (; existMonsterCount < monstersAI->getHeroesCount();){
		int indexY = (int)(CCRANDOM_0_1() * MAX_ROW);
		int indexType = (int)(CCRANDOM_0_1() * baseMonsters.size());
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
					hero->setPosition(mSceneSize.width + 60, hObj->getpositionY());
					hObj->addNode(gameSceneLayout,hero);
					//出场动画
					ActionTimeline * action = CSLoader::createTimeline(nodeName.c_str());
					hero->runAction(action);
					hObj->setAction(action);
					hObj->runAction(HeroActionType::Run);
					hero->runAction(Sequence::create(DelayTime::create(0.4f * j), MoveTo::create(0.3f * (MAX_COLUMN + 1 - j), Vec2(hObj->getpositionX(), hObj->getpositionY())), CallFuncN::create([hObj](Node * node){
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
			if (mHeros[lastLastX * MAX_ROW + indexY] != nullptr && mHeros[lastX * MAX_ROW + indexY] != nullptr){
				if (id == mHeros[lastLastX * MAX_ROW + indexY]->getHero()->getId() &&
					id == mHeros[lastX * MAX_ROW + indexY]->getHero()->getId()){
					return false;
				}
			}
		}

		if (indexY == 0){
			if (mHeros[indexX * MAX_ROW + nextNextY] == nullptr || mHeros[indexX * MAX_ROW + nextY] == nullptr){
				return true;
			}
			if (id == mHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
				id == mHeros[indexX * MAX_ROW + nextNextY]->getHero()->getId()){
				return false;
			}
		}
		else if (indexY == 1){
			if (mHeros[indexX * MAX_ROW + nextY] != nullptr && mHeros[indexX * MAX_ROW + lastY] != nullptr){
				if (id == mHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == mHeros[indexX * MAX_ROW + lastY]->getHero()->getId()){
					return false;
				}
			}
			if (mHeros[indexX * MAX_ROW + nextNextY] == nullptr || mHeros[indexX * MAX_ROW + nextY] == nullptr){
				return true;
			}
			if (id == mHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
				id == mHeros[indexX * MAX_ROW + nextNextY]->getHero()->getId()){
				return false;
			}
		}
		else if (indexY == MAX_ROW - 1){
			if (mHeros[indexX * MAX_ROW + lastLastY] == nullptr || mHeros[indexX * MAX_ROW + lastY] == nullptr){
				return true;
			}
			if (id == mHeros[indexX * MAX_ROW + lastY]->getHero()->getId() &&
				id == mHeros[indexX * MAX_ROW + lastLastY]->getHero()->getId()){
				return false;
			}
		}
		else if (indexY == MAX_ROW - 2){
			if (mHeros[indexX * MAX_ROW + nextY] != nullptr && mHeros[indexX * MAX_ROW + lastY] != nullptr){
				if (id == mHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == mHeros[indexX * MAX_ROW + lastY]->getHero()->getId()){
					return false;
				}
			}
			
			if (mHeros[indexX * MAX_ROW + lastLastY] != nullptr && mHeros[indexX * MAX_ROW + lastY]!= nullptr){
				if (id == mHeros[indexX * MAX_ROW + lastY]->getHero()->getId() &&
					id == mHeros[indexX * MAX_ROW + lastLastY]->getHero()->getId()){
					return false;
				}
			}
		}
		else{
			if (mHeros[indexX * MAX_ROW + nextY] != nullptr && mHeros[indexX * MAX_ROW + lastY] != nullptr){
				if (id == mHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == mHeros[indexX * MAX_ROW + lastY]->getHero()->getId()){
					return false;
				}
			}
			if (mHeros[indexX * MAX_ROW + lastLastY] != nullptr && mHeros[indexX * MAX_ROW + lastY]!= nullptr){
				if (id == mHeros[indexX * MAX_ROW + lastY]->getHero()->getId() &&
					id == mHeros[indexX * MAX_ROW + lastLastY]->getHero()->getId()){
					return false;
				}
			}
			if (mHeros[indexX * MAX_ROW + nextNextY] != nullptr &&  mHeros[indexX * MAX_ROW + nextY]!= nullptr){
				if (id == mHeros[indexX * MAX_ROW + nextY]->getHero()->getId() &&
					id == mHeros[indexX * MAX_ROW + nextNextY]->getHero()->getId()){
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
			if (monsterHeros[indexX * MAX_ROW + lastLastY] == nullptr || monsterHeros[indexX * MAX_ROW + lastY]==nullptr){
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
			if (monsterHeros[indexX * MAX_ROW + lastLastY] != nullptr && monsterHeros[indexX * MAX_ROW + lastY] != nullptr ){
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

void GameScene::addBoss(int index, int indexX, int indexY, int posionX, int posionY, BaseHeroes* heroes){
	HeroObj * hObj = HeroObj::create();
	hObj->retain();
	hObj->setIndexX(indexX);
	hObj->setIndexY(indexY);
	hObj->setHero(heroes);
	std::string name = heroes->getName();
	std::string nodeName = String::createWithFormat("%s.csb", name.c_str())->getCString();
	auto hero = CSLoader::createNode(nodeName.c_str());
	hero->setPosition(-120, hObj->getpositionY());
	hero->setScaleX(-2);
	hero->setScaleY(2);
	hObj->addNode(gameSceneLayout,hero);
	//出场动画
	ActionTimeline * action = CSLoader::createTimeline(nodeName.c_str());
	hero->runAction(action);
	hObj->setAction(action);
	hObj->runAction(HeroActionType::Run);
	hero->runAction(Sequence::create(DelayTime::create(0.4f * indexX), MoveTo::create(0.3f * (MAX_COLUMN + 1 - indexX), Vec2(hObj->getpositionX(), hObj->getpositionY())), CallFuncN::create([hObj](Node * node){
		hObj->runAction(HeroActionType::Stand);
	}), NULL));
	mHeros[index] = hObj;
	mHeros[index + 1] = hObj;
	mHeros[index + MAX_ROW] = hObj;
	mHeros[index + MAX_ROW + 1] = hObj;
}

bool GameScene::onTouchBegan(Touch * pTouch, Event * pEvent){
	if (isActionRuning){
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
				if (mHeros[i * MAX_COLUMN + index] != nullptr){
					indexHero = i * MAX_COLUMN + index;
				}
				else{
					break;
				}
			}
			if (indexHero == -1){
				return;
			}
			if (mHeros[indexHero]->getActionType() == HeroActionType::Stand){
				mCurrentHero = mHeros[indexHero];
				mCurrentHero->getMCurrentNode()->runAction(FadeTo::create(0.2f, 125));
				return;
			}
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
				if (mHeros[i * MAX_COLUMN + index] == nullptr){
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
				mHeros[mCurrentHero->getIndexX() * MAX_COLUMN + mCurrentHero->getIndexY()] = nullptr;
				mCurrentHero->setIndexX(indexXHero);
				mCurrentHero->setIndexY(index);
				mCurrentHero->updateNode();
				mHeros[indexXHero * MAX_COLUMN + index] = mCurrentHero;
			}), CallFunc::create([=](){
				//判断 准备和防御
				mHeroPrepareAndDef();
				mCurrentHero = nullptr;
				isActionRuning = false;
			}), NULL));
		}
	}
}

void GameScene::onLongTouchDown(float delay){
	isLongPress = true;
	int posX = abs(longTouchDownX - (hStartX + heroW / 2));
	int posY = abs(longTouchDownY - (hStartY + heroH / 2));
	int indexX = posX / heroW;
	int indexY = posY / heroH;
	if (indexX >= 0 && indexX < MAX_COLUMN && indexY >= 0 && indexY < MAX_ROW){
		HeroObj* hero = mHeros[indexX * MAX_ROW + indexY];
		if (hero != nullptr &&( hero->getActionType() == HeroActionType::Stand || hero->getActionType() == HeroActionType::Def)){
			//ActionTimeline * action = static_cast<ActionTimeline *>(hero->getMNode()->getActionByTag(hero->getID()));
			//action->play("hit",false);
			//action->setLastFrameCallFunc([=](){
			gameSceneLayout->removeChild(mHeros[indexX * MAX_ROW + indexY]->getMCurrentNode(), true);
				mHeros[indexX * MAX_ROW + indexY] = nullptr;
				for (int i = indexX + 1; i < MAX_COLUMN; i++){
					if (mHeros[i * MAX_ROW + indexY] != nullptr){
						HeroObj * mHero = mHeros[i * MAX_ROW + indexY];
						mHero->setIndexX(i-1);
						mCurrentHero->updateNode();
						mHero->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(mHero->getpositionX(), mHero->getpositionY())));
						mHeros[(i - 1) * MAX_ROW + indexY] = mHero;
						mHeros[i * MAX_ROW + indexY] = nullptr;
					}
					else{
						break;
					}
				}
				//判断 准备和防御
				mHeroPrepareAndDef();

			//});
			hero = nullptr;
		}
	}
}

void GameScene::mHeroPrepareAndDef(){
	std::vector<std::vector<HeroObj *>> allHHeroes;
	std::vector<std::vector<HeroObj *>> allVHeroes;
	int id;
	for (int i = 0; i < MAX_ROW; i++){//行
		std::vector<HeroObj *> hHeroes;
		for (int j = 0; j < MAX_COLUMN; j++){//列
			HeroObj * hero = mHeros[j * MAX_ROW + i];
			if (hero != nullptr && hero->getActionType() == HeroActionType::Stand){
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
				}
			}
			else{
				if (hHeroes.size() >= 3){
					allHHeroes.push_back(hHeroes);
				}
				hHeroes.clear();
				break;
			}
		}	
	}
	for (int i = 0; i < MAX_COLUMN; i++){//列
		std::vector<HeroObj *> vHeroes;
		for (int j = 0; j < MAX_ROW; j++){//行
			HeroObj * hero = mHeros[i * MAX_ROW + j];
			if (hero != nullptr && hero->getActionType() == HeroActionType::Stand){
				if (vHeroes.empty()){
					id = hero->getHero()->getId();
					vHeroes.push_back(hero);
					continue;
				}
				if (id == hero->getHero()->getId()){
					vHeroes.push_back(hero);
				}
				else{
					if (vHeroes.size() >= 3){
						allVHeroes.push_back(vHeroes);
					}
					vHeroes.clear();
				}
			}
			else{
				if (vHeroes.size() >= 3){
					allVHeroes.push_back(vHeroes);
				}
				vHeroes.clear();
				break;
			}
		}
	}
	if (allHHeroes.empty() && allVHeroes.empty()){
		return;
	}
	else if (!allHHeroes.empty() && !allVHeroes.empty()){
		//筛选重复的hero


		//mHeroDefAction(allVHeroes);
		//mHeroPrepareAction(allHHeroes);
	}
	else if(!allHHeroes.empty()){
		mHeroPrepareAction(allHHeroes);
	}
	else if(!allVHeroes.empty()){
		mHeroDefAction(allVHeroes);
	}
}

void GameScene::swapHeroPosition(HeroObj * from, HeroObj * to){
	int indexX ;
	if (to != nullptr && to->getActionType() == HeroActionType::Def){
		return;
	}
	else if(to != nullptr){
		indexX = from->getIndexX();
		from->setIndexX(to->getIndexX());
		from->updateNode();
		mHeros[to->getId()] = from;
		to->setIndexX(indexX);
		to->updateNode();
		mHeros[from->getId()] = to;
		from->getMCurrentNode()->runAction(MoveTo::create(0.1f* (indexX - from->getIndexX()), Vec2(from->getpositionX(), from->getpositionY())));
		to->getMCurrentNode()->runAction(MoveTo::create(0.1f* (indexX - from->getIndexX()), Vec2(to->getpositionX(), to->getpositionY())));
	}
	else{
		mHeros[from->getId()] = nullptr;
		from->setIndexX(from->getIndexX() - 1);
		from->updateNode();
		mHeros[from->getId()] = from;
		from->getMCurrentNode()->runAction(MoveTo::create(0.1f, Vec2(from->getpositionX(), from->getpositionY())));
	}
}

void GameScene::mHeroPrepareAction(std::vector<std::vector<HeroObj *>> allHHeroes){
	for (auto hHeroes = allHHeroes.begin(); hHeroes != allHHeroes.end(); hHeroes++){
		std::vector<HeroObj *>  heroes;
		for (unsigned int i = 0; i < (*hHeroes).size(); i++){
			heroes.push_back((*hHeroes).at(i));
			if ((i + 1) % 3 == 0){//3个为一组
				for (int j = 0; j < 3; j++){
					//准备动画
					heroes[j]->runAction(HeroActionType::Prepare);
					if (j == 1){
						Node * node = heroes[j]->getMCurrentNode();
						//添加进度条和攻击数字
					}
				}
				//准备好后 移动
				for (int i = heroes[0]->getIndexX() - 1; i >= 0; i--){
					HeroObj * obj = mHeros[i * MAX_ROW + heroes[0]->getIndexY()];
					if (obj != nullptr && obj->getActionType() == HeroActionType::Prepare  //pre 并且相同
						&& strcmp(obj->getHero()->getName().c_str(), heroes[0]->getHero()->getName().c_str())){
						for (int i = 0; i < 3; i++){
							HeroObj * heroItem = heroes[i];
							heroItem->getMCurrentNode()->runAction(Sequence::create(MoveTo::create(0.3f,
								Vec2(heroItem->getpositionX() + 3 * heroW, heroItem->getpositionY())),
							CallFuncN::create([=](Node * node){
								gameSceneLayout->removeChild(node);
								mHeros[heroItem->getId()] = nullptr;
								//最后一个移动 判断后面有没有hero
								if (i == 2){
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
					}
					else if (obj != nullptr && obj->getActionType() == HeroActionType::Prepare){ //pre
						break;
					}
					else {
						for (int i = 0; i < 3; i++){
							swapHeroPosition(heroes[i], obj);
						}
					}
				}
				prepareHeroes.push_back(heroes);//添加到已经准备的英雄组中
			}
		}
	}

}

void GameScene::mHeroDefAction(std::vector<std::vector<HeroObj *>> allVHeroes){
	for (auto vHeroes = allVHeroes.begin(); vHeroes != allVHeroes.end(); vHeroes++){
		for (unsigned int i = 0; i < (*vHeroes).size(); i++){
			HeroObj * hero = (*vHeroes).at(i);
			auto node = CSLoader::createNode("HeroMuBei.csb");
			node->setPosition(hero->getpositionX(), hero->getpositionY());
			node->setScaleX(-1);
			hero->removeNode();
			hero->addNode(gameSceneLayout,node);
			//防御动画
			ActionTimeline * action = CSLoader::createTimeline("HeroMuBei.csb");
			action->setTag(hero->getId());
			node->runAction(action);
			hero->setAction(action);
			hero->runAction(HeroActionType::Def);
			hero->getHero()->setPDef(hero->getHero()->getPDef() * 2);//增加防御

			//防御后移动
			if (hero->getIndexX() == 0){
				break;
			}
			else{
				//HeroObj * swapHero;
				for (int i = hero->getIndexX(); i > 0; i--){
					swapHeroPosition(mHeros[i * MAX_ROW + hero->getIndexY()], mHeros[(i - 1) * MAX_ROW + hero->getIndexY()]);
				}
			}
		}
	}
}





void GameScene::onExit(){
	Layer::onExit();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("HeroMuBei.plist");
	int index = 0;
	for (auto heros = baseHeroes.begin(); heros != baseHeroes.end(); heros++){
		(*heros)->setType((HeroType)index);
		index++;
		std::string heroesFileName = String::createWithFormat("%s.plist", (*heros)->getName())->getCString();
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(heroesFileName);
	}
	index = 0;
	for (auto monster = baseMonsters.begin(); monster != baseMonsters.end(); monster++){
		(*monster)->setType((HeroType)index);
		index++;
		std::string heroesFileName = String::createWithFormat("%s.plist", (*monster)->getName())->getCString();
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(heroesFileName);
	}
}