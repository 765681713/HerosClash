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
//void HeroObj::setId(int id){
//	//setActionTimelineID(id);
//	this->id = id;
//	//�޸� ��ֵ��tag
//	if (mCurrentLayout != nullptr && mCurrentNode != nullptr){
//		mCurrentLayout->removeChild(mCurrentNode);
//		mCurrentLayout->addChild(mCurrentNode, indexX, id);
//	}
//}

////�޸�action id
//void HeroObj::setActionTimelineID(int tag){
//	if (this->mCurrentNode != nullptr){
//		Action * action = this->mCurrentNode->getActionByTag(this->id);
//		if (action != nullptr){
//			action->setTag(tag);
//		}
//	}
//}

int HeroObj::getIndexX(){
	return this->indexX;
}

//�޸�index  ͬ�¸� position
void HeroObj::setIndexX(int indexX){
	this->indexX = indexX;
	log("setIndexX  %d", hStartX - indexX * heroW);
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

//���node
void HeroObj::addNode(Layout * layout, Node * node){
	this->mCurrentLayout = layout;
	this->mCurrentNode = node;
	mCurrentLayout->addChild(mCurrentNode, indexX, id);
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
	//�޸Ļ���ʱ��Ĳ� //�޸� ��ֵ��tag
	if (mCurrentNode != nullptr){
		mCurrentNode->setLocalZOrder(indexX);
		mCurrentNode->setTag(id);
	}
}

void HeroObj::removeNode(){
	mCurrentLayout->removeChild(mCurrentNode,true);
}