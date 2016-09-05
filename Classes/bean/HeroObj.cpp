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

int HeroObj::getHeroACT(){
	return mCurrentACT;
}
int HeroObj::getHeroHP(){
	return mCurrentHP;
}
int HeroObj::getHeroDEF(){
	return mCurrentDEF;
}
void HeroObj::setHeroHP(int hp){
	this->mCurrentHP = hp;
}
void HeroObj::setHeroATK(int atk){
	this->mCurrentATK = atk;
}

int HeroObj::getHeroATK(){
	return mCurrentATK;
}

void HeroObj::setHero(BaseHeroes * h){
	this->hero = h;
	mCurrentHP = h->getHP();
	mCurrentATK = h->getSAtk() + h->getPAtk();
	mCurrentDEF = h->getSDef() + h->getPDef();
	mCurrentADD = h->getAAtk();
	mCurrentACT = h->getRound();
	mCurrentDouble = h->getDoubleAtk();
}
BaseHeroes * HeroObj::getHero(){
	return this->hero;
}

Node * HeroObj::getMCurrentNode(){
	return this->mCurrentNode;
}

Node * HeroObj::getMWuQi(){
	return mWuQi;
}
//添加node
void HeroObj::addNode(Layout * layout, Node * node){
	this->mCurrentLayout = layout;
	this->mCurrentNode = node;
	this->mWuQi = this->mCurrentNode;
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
		mCurrentHP = hero->getHP();
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
	mCurrentADD = Multiple * hero->getAAtk();
	mCurrentHP = mCurrentATK = Multiple * (hero->getPAtk() + hero->getSAtk());
	mCurrentDEF = hero->getPDef() > hero->getSDef() ? hero->getPDef() : hero->getSDef();
	mCurrentACT = hero->getRound();
	if (showNum){
		//添加进度条和攻击数字回合
		std::string atk = String::createWithFormat("%d",mCurrentATK)->getCString();
		TextAtlas * atkAtlas = TextAtlas::create(atk, "hero_atk_num.png", 30, 35, ".");
		atkAtlas->setAnchorPoint(Vec2(0.5, 1));
		atkAtlas->setPosition(Vec2(0, 35));
		std::string act = String::createWithFormat("%d", mCurrentACT)->getCString();
		TextAtlas * roundAtlas = TextAtlas::create(act, "hero_round_count_num.png", 50, 65, ".");
		roundAtlas->setAnchorPoint(Vec2(0.5, 1));
		roundAtlas->setPosition(Vec2(0, 0));
		Sprite *atkProOff = Sprite::create("hero_atk_pre_off.png");
		Sprite *atkProOn = Sprite::create("hero_atk_pre_on.png");
		ProgressTimer * atkPro = ProgressTimer::create(atkProOn);
		atkPro->setType(ProgressTimer::Type::BAR);
		//从左到右
		atkPro->setMidpoint(ccp(0, 0.5));
		atkPro->setBarChangeRate(ccp(1, 0));
		int hp = mCurrentATK * 100 / (mCurrentATK + mCurrentACT * mCurrentADD);
		atkPro->setPercentage(hp);
		atkPro->setAnchorPoint(Vec2(0.5, 1));
		atkPro->setPosition(Vec2(0, 10));
		atkProOff->setAnchorPoint(Vec2(0.5, 1));
		atkProOff->setPosition(Vec2(0, 10));
		if (isMonster){
			atkAtlas->setScaleX(0.5f);
			atkAtlas->setScaleY(0.5f);
			roundAtlas->setScaleX(0.3f);
			roundAtlas->setScaleY(0.3f);
			atkPro->setScaleX(0.5f);
			atkPro->setScaleY(0.5f);
			atkProOff->setScaleX(0.5f);
			atkProOff->setScaleY(0.5f);
		}
		else{
			atkAtlas->setScaleX(-0.5f);
			atkAtlas->setScaleY(0.5f);
			roundAtlas->setScaleX(-0.3f);
			roundAtlas->setScaleY(0.3f);
			atkPro->setScaleX(-0.5f);
			atkPro->setScaleY(0.5f);
			atkProOff->setScaleX(-0.5f);
			atkProOff->setScaleY(0.5f);
		}
		mCurrentNode->addChild(atkAtlas, 2, 100);
		mCurrentNode->addChild(atkProOff, 1, 200);
		mCurrentNode->addChild(atkPro, 1, 300);
		mCurrentNode->addChild(roundAtlas, 2, 400);
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
	mCurrentHP = Multiple * hero->getHP();
	mCurrentDEF = Multiple * (hero->getPDef() + hero->getSDef());
}

void HeroObj::updateRound(){
	if (mActionType == HeroActionType::Prepare){
		mCurrentACT--;
		mCurrentHP = mCurrentATK = mCurrentATK + mCurrentADD;
		if (mCurrentNode->getChildrenCount() > 3){
			TextAtlas * atkAtlas = static_cast<TextAtlas *>(mCurrentNode->getChildByTag(100));
			std::string atk = String::createWithFormat("%d", mCurrentATK)->getCString();
			atkAtlas->setString(atk);
			ProgressTimer * atkPro = static_cast<ProgressTimer *>(mCurrentNode->getChildByTag(300));
			int hp = mCurrentATK * 100 / (mCurrentATK + mCurrentACT * mCurrentADD);
			atkPro->setPercentage(hp);
			std::string act = String::createWithFormat("%d", mCurrentACT)->getCString();
			TextAtlas * actAtlas = static_cast<TextAtlas *>(mCurrentNode->getChildByTag(400));
			actAtlas->setString(act);
			if (mCurrentACT <= 0){
				actAtlas->setVisible(false);
			}
		}
	}
}

void HeroObj::updateATK(){
	if (mCurrentNode->getChildrenCount() > 3){
		TextAtlas * atkAtlas = static_cast<TextAtlas *>(mCurrentNode->getChildByTag(100));
		std::string atk = String::createWithFormat("%d", mCurrentATK)->getCString();
		atkAtlas->setString(atk);
		ProgressTimer * atkPro = static_cast<ProgressTimer *>(mCurrentNode->getChildByTag(300));
		int hp = mCurrentATK * 100 / mCurrentHP;
		atkPro->setPercentage(hp);
	}
}

//HeroObj * targetHero, int count, bool isCallFun, std::function<void()> callFun
void HeroObj::atkEnemy(std::function<void()> hitWall){
	//隐藏行动text
	//判断英雄类型  攻击动画
	switch (hero->getAtkType()){
	case HeroAtkType::ZhanShi:{
		runAction(HeroActionType::Run);
		float time = 0.1 * (abs(mWuQi->getPosition().x - targetPosX) / heroW);//从现在的位置到两边的移动时间
		auto action = Sequence::create(MoveTo::create(time, Vec2(targetPosX, targetPosY)), CallFunc::create(hitWall), NULL);
		action->setTag(ZhanShiAtkActionTag);
		mWuQi->runAction(action);
		break;
	}
	case HeroAtkType::FaShi:{

		std::string wuQiName = String::createWithFormat("%s.csb", hero->getAtkEffect().c_str())->getCString();
		mWuQi = CSLoader::createNode(wuQiName.c_str());
		mWuQi->setPosition(positionX + heroW/2, positionY);
		mWuQi->setRotation(-90);
		mWuQi->setVisible(false);
		mCurrentLayout->addChild(mWuQi, indexY, 100 + id);
		//出场动画
		ActionTimeline * action = CSLoader::createTimeline(wuQiName.c_str());
		mWuQi->runAction(action);
		runAction(HeroActionType::Attack);
		mCurrentAction->setLastFrameCallFunc([=](){
			action->gotoFrameAndPlay(0,true);
			mWuQi->setVisible(true);
			float time = 0.1 * (abs(mWuQi->getPosition().x - targetPosX - heroW/ 2) / heroW);
			auto action = Sequence::create(MoveTo::create(time, Vec2(targetPosX + heroW / 2, targetPosY)), CallFunc::create(hitWall), NULL);
			action->setTag(FaShiAtkActionTag);
			mWuQi->runAction(action);
		});
		break;
	}
	case HeroAtkType::GongJianShou:{
		std::string wuQiName = String::createWithFormat("%s.csb", hero->getAtkEffect().c_str())->getCString();
		mWuQi = CSLoader::createNode(wuQiName.c_str());
		mWuQi->setPosition(positionX+ heroW/2, positionY);
		mWuQi->setRotation(-90);
		mWuQi->setVisible(false);
		mCurrentLayout->addChild(mWuQi, indexY, 100 + id);
		//出场动画
		ActionTimeline * action = CSLoader::createTimeline(wuQiName.c_str());
		mWuQi->runAction(action);
		runAction(HeroActionType::Attack);
		mCurrentAction->setLastFrameCallFunc([=](){
			action->gotoFrameAndPlay(0, true);
			mWuQi->setVisible(true);
			float time = 0.1 * (abs(mWuQi->getPosition().x - targetPosX - heroW / 2) / heroW);
			auto action = Sequence::create(MoveTo::create(time, Vec2(targetPosX + heroW / 2, targetPosY)), CallFunc::create(hitWall), NULL);
			action->setTag(GJSAtkActionTag);
			mWuQi->runAction(action);
		});
		break;
	}
	case HeroAtkType::Other:{
		break;
	}
	}
}

bool HeroObj::collision(HeroObj * target){
	if (target == nullptr || this->mWuQi == nullptr){
		return false;
	}
	//不同的武器 不同的碰撞检测
	if (hero == nullptr){
		return false;
	}
	switch (hero->getAtkType()){
	case HeroAtkType::ZhanShi:{
		if (mWuQi->getPosition().x + heroW / 2 > target->getMCurrentNode()->getPosition().x - heroW / 2
			&& target->getMCurrentNode()->getPosition().y < mWuQi->getPosition().y + heroH / 2 &&
			target->getMCurrentNode()->getPosition().y > mWuQi->getPosition().y - heroH / 2){
			return true;
		}
		return false;
		break;
	}
	case HeroAtkType::FaShi:{
		if (mWuQi->getPosition().x  > target->getMCurrentNode()->getPosition().x - heroW / 2
			&& target->getMCurrentNode()->getPosition().y < mWuQi->getPosition().y + heroH / 2 &&
			target->getMCurrentNode()->getPosition().y > mWuQi->getPosition().y - heroH / 2){
			return true;
		}
		return false;
		break;
	}
	case HeroAtkType::GongJianShou:{
		if (mWuQi->getPosition().x  > target->getMCurrentNode()->getPosition().x - heroW / 2
			&& target->getMCurrentNode()->getPosition().y < mWuQi->getPosition().y + heroH / 2 &&
			target->getMCurrentNode()->getPosition().y > mWuQi->getPosition().y - heroH / 2){
			return true;
		}
		return false;
		break;
	}
	case HeroAtkType::Other:{

		return false;
		break;
	}
	}
	return false;
}

void HeroObj::attack(std::function<void(Node *)> callback,std::function<void()> hitWall){
	if (isAttacking){
		return;
	}
	isAttacking = true;
	//用自己的武器攻击英雄
	switch (hero->getAtkType()){
	case HeroAtkType::ZhanShi:{
		auto action = mWuQi->getActionByTag(ZhanShiAtkActionTag);
		mWuQi->stopAction(action);
		runAction(HeroActionType::Attack);
		mCurrentAction->setLastFrameCallFunc([=](){
			

			if (hitWall != nullptr){
				runAction(HeroActionType::Run);
				float time = 0.1 * (abs(mWuQi->getPosition().x - targetPosX) / heroW);//从现在的位置到两边的移动时间
				auto action = Sequence::create(MoveTo::create(time, Vec2(targetPosX, targetPosY)), CallFunc::create(hitWall), NULL);
				action->setTag(ZhanShiAtkActionTag);
				mWuQi->runAction(action);
			}
			//回调更新血量
			if (callback != nullptr){
				/*std::string atkEffect = String::createWithFormat("%s.csb", hero->getAtkEffect().c_str())->getCString();
				auto atkEffNode = CSLoader::createNode(atkEffect.c_str());
				atkEffNode->setPosition(Vec2(mWuQi->getPosition().x + heroW, positionY));
				ActionTimeline * effectAtk = CSLoader::createTimeline(atkEffect.c_str());
				atkEffNode->runAction(effectAtk);
				mCurrentLayout->addChild(atkEffNode, indexY);
				effectAtk->gotoFrameAndPlay(0, false);
				effectAtk->setLastFrameCallFunc([=](){
					atkEffNode->removeFromParentAndCleanup(true);
				});*/
				callback(mWuQi);
			}
			isAttacking = false;
			mCurrentAction->clearLastFrameCallFunc();
		});
		break;
	}
	case HeroAtkType::FaShi:{
		//回调更新血量
		if (callback != nullptr){
			callback(mWuQi);
		}
		isAttacking = false;
		break;
	}
	case HeroAtkType::GongJianShou:{
		//回调更新血量
		if (callback != nullptr){
			callback(mWuQi);
		}
		isAttacking = false;
		break;
	}
	case HeroAtkType::Other:{
		break;
	}
	}
}

void HeroObj::death(){
	if (hero->getAtkType() != HeroAtkType::ZhanShi){
		mWuQi->stopAllActions();
		mWuQi->removeFromParentAndCleanup(true);
	}
	mCurrentNode->stopAllActions();
	mCurrentNode->removeAllChildren();
	mCurrentNode->removeFromParentAndCleanup(true);
	this->release();
}