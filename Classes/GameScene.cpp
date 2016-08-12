#include "GameScene.h"

Scene * GameScene::createScene(UserInfo * info, std::vector<Heroes *> heroes, std::vector<Monster *> monsters, AI * ai){
	auto scene = Scene::create();
	auto layer = GameScene::create();
	layer->mUserInfo = info;
	layer->baseHeroes = heroes;
	layer->baseMonsters = monsters;
	layer->monstersAI = ai;
	scene->addChild(layer);
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
	//¼ÓÔØ×ÊÔ´
	for (auto heros = baseHeroes.begin(); heros != baseHeroes.end(); heros++){
		std::string heroesFileName = String::createWithFormat("%s.plist",(*heros)->getName())->getCString();
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(heroesFileName.c_str());
	}
	for (auto monster = baseMonsters.begin(); monster != baseMonsters.end(); monster++){
		std::string heroesFileName = String::createWithFormat("%s.plist", (*monster)->getName())->getCString();
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(heroesFileName.c_str());
	}
	initGame();


	/*SpriteFrameCache::getInstance()->addSpriteFramesWithFile("HeroZhanShi_1.plist");
	auto heroZhanShi1 = CSLoader::createNode("HeroZhanShi_1.csb");
	heroZhanShi1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(heroZhanShi1);
	auto heroZhanShi2 = CSLoader::createNode("HeroZhanShi_1.csb");
	heroZhanShi2->setPosition(visibleSize.width / 2 - 200, visibleSize.height / 2);
	this->addChild(heroZhanShi2);
	timeline::ActionTimeline * action1 = CSLoader::createTimeline("HeroZhanShi_1.csb");
	timeline::ActionTimeline * action2 = CSLoader::createTimeline("HeroZhanShi_1.csb");
	heroZhanShi1->runAction(action1);
	heroZhanShi2->runAction(action2);
	action1->play("attack", true);
	action2->play("attack", true);*/
	return true;
}

void GameScene::initGame(){
	auto heroZhanShi1 = CSLoader::createNode("HeroZhanShi_1.csb");
	heroZhanShi1->setPosition(580, 450);
	gameSceneLayout->addChild(heroZhanShi1);

	ActionTimeline * action1 = CSLoader::createTimeline("HeroZhanShi_1.csb");
	heroZhanShi1->runAction(action1);
	action1->play("stand", true);
}