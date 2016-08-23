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

//�޸�index  ͬ�¸� position
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

void HeroObj::setHeroHP(int hp){
	this->mCurrentHP = hp;
}
void HeroObj::setHeroATK(int atk){
	this->mCurrentATK = atk;
}

int * HeroObj::getHeroATK(){
	return &mCurrentATK;
}

Node * HeroObj::getMCurrentNode(){
	return this->mCurrentNode;
}

Node * HeroObj::getMWuQi(){
	return mWuQi;
}
//���node
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
	//�޸Ļ���ʱ��Ĳ� //�޸� ��ֵ��tag
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
		//��ӽ������͹������ֻغ�
		std::string atk = String::createWithFormat("%d",mCurrentATK)->getCString();
		TextAtlas * atkAtlas = TextAtlas::create(atk, "hero_atk_num.png", 30, 35, ".");
		atkAtlas->setAnchorPoint(Vec2(0.5, 1));
		atkAtlas->setScaleX(-0.5f);
		atkAtlas->setScaleY(0.5f);
		atkAtlas->setPosition(Vec2(0, 35));
		std::string act = String::createWithFormat("%d", mCurrentACT)->getCString();
		TextAtlas * roundAtlas = TextAtlas::create(act, "hero_round_count_num.png", 50, 65, ".");
		roundAtlas->setScaleX(-0.3f);
		roundAtlas->setAnchorPoint(Vec2(0.5, 1));
		roundAtlas->setScaleY(0.3f);
		roundAtlas->setPosition(Vec2(0, 0));
		Sprite *atkProOff = Sprite::create("hero_atk_pre_off.png");
		Sprite *atkProOn = Sprite::create("hero_atk_pre_on.png");
		ProgressTimer * atkPro = ProgressTimer::create(atkProOn);
		atkPro->setType(ProgressTimer::Type::BAR);
		//������
		atkPro->setMidpoint(ccp(0, 0.5));
		atkPro->setBarChangeRate(ccp(1, 0));
		int hp = mCurrentATK * 100 / (mCurrentATK + mCurrentACT * mCurrentADD);
		atkPro->setPercentage(hp);
		atkPro->setScaleX(-0.5f);
		atkPro->setScaleY(0.5f);
		atkPro->setAnchorPoint(Vec2(0.5, 1));
		atkPro->setPosition(Vec2(0, 10));
		atkProOff->setScaleX(-0.5f);
		atkProOff->setScaleY(0.5f);
		atkProOff->setAnchorPoint(Vec2(0.5, 1));
		atkProOff->setPosition(Vec2(0, 10));
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
	//��������
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
		int hp = mCurrentATK * 100 / (mCurrentATK + mCurrentACT * mCurrentADD);
		atkPro->setPercentage(hp);
	}
}

void HeroObj::hit(int * atk){
	if ((*atk) >= mCurrentHP + mCurrentDEF){
		//
		(*atk) = (*atk) - mCurrentHP - mCurrentDEF;
		mCurrentHP = 0;
	}
	else if((*atk) <= mCurrentDEF){
		//��ʾ0 
		(*atk) = 0;
		return;
	}
	else {
		//
		mCurrentHP = mCurrentHP + mCurrentDEF - (*atk);
		if (mActionType == HeroActionType::Prepare){
			mCurrentATK = mCurrentHP;
			if (mCurrentNode->getChildren().size() > 2){
				TextAtlas * atkAtlas = static_cast<TextAtlas *>(mCurrentNode->getChildByTag(1));
				std::string atk = String::createWithFormat("%d", mCurrentATK)->getCString();
				atkAtlas->setString(atk);
				ProgressTimer * atkPro = static_cast<ProgressTimer *>(mCurrentNode->getChildByTag(3));
				atkPro->setPercentage((int(mCurrentATK / (mCurrentATK + mCurrentACT * mCurrentADD) * 100)));
			}
		}
		(*atk) = 0;
	}
}

//HeroObj * targetHero, int count, bool isCallFun, std::function<void()> callFun
void HeroObj::atkEnemy(int width, int height, int index){
	//�����ж�text
	//�ж�Ӣ������  ��������
	switch (hero->getAtkType()){
	case HeroAtkType::ZhanShi:{
		mWuQi = mCurrentNode;
		runAction(HeroActionType::Run);
		float time = 0.1 * (this->indexX + MAX_COLUMN + 3);//�м� �����ߵ��ƶ�ʱ��
		int didtanceX = !isMonster ? width - (index + 1) * heroW : 0 + (index + 1) * heroW;
		auto action = Sequence::create(MoveTo::create(time, Vec2(didtanceX, positionY)), NULL);
		action->setTag(111);
		mCurrentNode->runAction(action);
		break;
	}
	case HeroAtkType::FaShi:{
		mWuQi = mCurrentNode;
		break;
	}
	case HeroAtkType::GongJianShou:{
		mWuQi = mCurrentNode;
		break;
	}
	case HeroAtkType::Other:{
		mWuQi = mCurrentNode;
		break;
	}
	}
}

bool HeroObj::collision(HeroObj * target){
	//��ͬ������ ��ͬ����ײ���
	switch (hero->getAtkType()){
	case HeroAtkType::ZhanShi:{
		if (mCurrentNode->getPosition().x + heroW / 2 > target->getMCurrentNode()->getPosition().x - heroW / 2
			&& target->getMCurrentNode()->getPosition().y < mCurrentNode->getPosition().y + heroH / 2 &&
			target->getMCurrentNode()->getPosition().y > mCurrentNode->getPosition().y - heroH / 2){
			return true;
		}
		return false;
		break;
	}
	case HeroAtkType::FaShi:{

		return false;
		break;
	}
	case HeroAtkType::GongJianShou:{

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

void HeroObj::attack(HeroObj * target , bool isAtt){
	if (isAttacking){
		return;
	}
	isAttacking = true;
	//���Լ�����������Ӣ��
	switch (hero->getAtkType()){
	case HeroAtkType::ZhanShi:{
		auto action = mCurrentNode->getActionByTag(111);
		mCurrentNode->stopAction(action);
		runAction(HeroActionType::Attack);
		mCurrentAction->setLastFrameCallFunc([=](){
			if (isAtt){
				target->hit(&mCurrentATK);
			}
			mCurrentNode->getActionManager()->resumeTarget(mCurrentNode);
			//�����󶯻�������
			//mCurrentNode->runAction(action);
			isAttacking = false;
			mCurrentAction->clearLastFrameCallFunc();
		});
		break;
	}
	case HeroAtkType::FaShi:{
		break;
	}
	case HeroAtkType::GongJianShou:{
		break;
	}
	case HeroAtkType::Other:{
		break;
	}
	}
}

void HeroObj::death(){
	/*if (mWuQi != nullptr){
		mWuQi->stopAllActions();
		mWuQi->removeFromParentAndCleanup(true);
	}*/
	mCurrentNode->stopAllActions();
	mCurrentNode->removeAllChildren();
	mCurrentNode->removeFromParentAndCleanup(true);
	this->release();
}