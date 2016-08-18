#include "HeroObj.h"


bool HeroObj::init(){
	//CC_SAFE_DELETE
	return true;
}

int HeroObj::getId(){
	return id;
}

HeroActionType HeroObj::getActionType(){
	return this->mActionType;
}

int HeroObj::getIndexX(){
	return this->indexX;
}

//修改index  同事改 position
void HeroObj::setIndexX(int indexX){
	this->indexX = indexX;
	if (isMonster){
		setpositionX(mStartX + indexX * heroW);
	}
	else{
		setpositionX(hStartX - indexX * heroW);
	}
	this->id = indexX * MAX_ROW + indexY;
	//updateNode();
}

int HeroObj::getIndexY(){
	return this->indexY;
}

void HeroObj::setIndexY(int indexY){
	this->indexY = indexY;
	setpositionY(hStartY - indexY * heroH);
	this->id = indexX * MAX_ROW + indexY;
	//updateNode();
}

Node * HeroObj::getMCurrentNode(){
	return this->mCurrentNode;
}

//添加node
void HeroObj::addNode(Layout * layout, Node * node){
	this->mCurrentLayout = layout;
	this->mCurrentNode = node;
	mCurrentLayout->addChild(mCurrentNode, indexY, id);
}

void HeroObj::setAction(ActionTimeline * action){
	mCurrentAction = action;
}

void HeroObj::runAction(HeroActionType actionType){
	if (mCurrentAction == nullptr){
		return;
	}
	mCurrentAction->stop();
	this->mActionType = actionType;
	switch (actionType){
	case HeroActionType::Stand:{
		mCurrentAction->play("stand", true);
		break;
	}
	case HeroActionType::Prepare:{
		mCurrentAction->play("prepare", true);
		break;
	}
	case HeroActionType::Run:{
		mCurrentAction->play("run", true);
		break;
	}
	case HeroActionType::Attack:{
		mCurrentAction->play("attack", false);
		break;
	}
	case HeroActionType::Hit:{
		mCurrentAction->play("hit", false);
		break;
	}
	case HeroActionType::Death:{
		mCurrentAction->play("death", false);
		break;
	}
	default:
		mCurrentAction->gotoFrameAndPlay(0, false);
		break;
	}

}

void HeroObj::updateNode(){
	//修改画的时候的层 //修改 层值和tag
	if (mCurrentNode != nullptr){
		mCurrentNode->setLocalZOrder(indexY);
		mCurrentNode->setTag(id);
	}
}

void HeroObj::removeNode(){
	mCurrentLayout->removeChild(mCurrentNode,true);
}

HeroObj * HeroObj::cloneThis(int indexX){
	HeroObj * hero = HeroObj::create();
	hero->retain();
	hero->setIsMonster(isMonster);
	hero->setIndexY(indexY);
	hero->setIndexX(indexX);
	hero->setHero(this->hero);
	hero->isClone = true;
	std::string nodeName = String::createWithFormat("%s.csb", this->hero->getName().c_str())->getCString();
	auto heroNode = CSLoader::createNode(nodeName.c_str());
	heroNode->setPosition(hero->getpositionX(), hero->getpositionY());
	heroNode->setScaleX(mCurrentNode->getScaleX());
	heroNode->setScaleY(mCurrentNode->getScaleY());
	hero->addNode(this->mCurrentLayout, heroNode);
	ActionTimeline * action = mCurrentAction->clone();
	hero->setAction(action);
	heroNode->runAction(action);
	hero->runAction(HeroActionType::Stand);
	return hero;
}

void HeroObj::prepare(bool showNum){
	auto defNode = CSLoader::createNode("EffectBg.csb");
	defNode->setAnchorPoint(Vec2(0, 0));
	defNode->setPosition(Vec2(0, 0));
	defNode->setScale(0.4f);
	ActionTimeline * effectDef = CSLoader::createTimeline("EffectBg.csb");
	defNode->runAction(effectDef);
	effectDef->play("prebody1", true);
	mCurrentNode->addChild(defNode, 0, 0);
	runAction(HeroActionType::Prepare);
	if (showNum){
		//添加进度条和攻击数字回合
		TextAtlas * atkAtlas = TextAtlas::create("35", "hero_atk_num.png", 30, 35, ".");
		atkAtlas->setAnchorPoint(Vec2(0.5, 1));
		atkAtlas->setScaleX(-0.5f);
		atkAtlas->setScaleY(0.5f);
		atkAtlas->setPosition(Vec2(0, 35));
		TextAtlas * roundAtlas = TextAtlas::create("5", "hero_round_count_num.png", 50, 65, ".");
		roundAtlas->setScaleX(-0.3f);
		roundAtlas->setAnchorPoint(Vec2(0.5, 1));
		roundAtlas->setScaleY(0.3f);
		roundAtlas->setPosition(Vec2(0, 0));
		Sprite *atkProOff = Sprite::create("hero_atk_pre_off.png");
		Sprite *atkProOn = Sprite::create("hero_atk_pre_on.png");
		ProgressTimer * atkPro = ProgressTimer::create(atkProOn);
		atkPro->setType(ProgressTimer::Type::BAR);
		//从左到右
		atkPro->setMidpoint(ccp(0, 0.5));
		atkPro->setBarChangeRate(ccp(1, 0));
		atkPro->setPercentage(30);
		atkPro->setScaleX(-0.5f);
		atkPro->setScaleY(0.5f);
		atkPro->setAnchorPoint(Vec2(0.5, 1));
		atkPro->setPosition(Vec2(0, 10));
		atkProOff->setScaleX(-0.5f);
		atkProOff->setScaleY(0.5f);
		atkProOff->setAnchorPoint(Vec2(0.5, 1));
		atkProOff->setPosition(Vec2(0, 10));
		mCurrentNode->addChild(atkAtlas, 2, 1);
		mCurrentNode->addChild(atkProOff, 1, 2);
		mCurrentNode->addChild(atkPro, 1, 3);
		mCurrentNode->addChild(roundAtlas, 2, 4);
	}
}

void HeroObj::def(){
	auto node = CSLoader::createNode("HeroMuBei.csb");
	auto defNode = CSLoader::createNode("EffectBg.csb");
	defNode->setAnchorPoint(Vec2(0, 0));
	defNode->setPosition(Vec2(0, 0));
	defNode->setScaleX(-0.3f);
	defNode->setScaleY(0.3f);
	ActionTimeline * effectDef = CSLoader::createTimeline("EffectBg.csb");
	defNode->runAction(effectDef);
	effectDef->play("def1", true);
	node->addChild(defNode, 0, id);
	node->setPosition(positionX, positionY);
	node->setScaleX(-1);
	removeNode();
	addNode(this->mCurrentLayout, node);
	//防御动画
	ActionTimeline * action = CSLoader::createTimeline("HeroMuBei.csb");
	action->setTag(hero->getId());
	node->runAction(action);
	setAction(action);
	runAction(HeroActionType::Def);
	hero->setPDef(hero->getPDef() * 2);//增加防御
}

void HeroObj::attact(){
		
	
}