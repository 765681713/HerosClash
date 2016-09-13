#ifndef __HeroObj_REF_H__
#define __HeroObj_REF_H__

#include "cocos2d.h"
#include "BaseHeroes.h"
#include "cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#define MAX_ROW  8
#define MAX_COLUMN 8
#define hStartX   580
#define hStartY   450
#define mStartX   700
#define mStartY   450
#define heroW   60
#define heroH   55
#define Multiple 3
#define ZhanShiAtkActionTag 101
#define FaShiAtkActionTag 102
#define GJSAtkActionTag 103

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace timeline;

typedef enum {
	Stand,
	Prepare,
	Run,
	Attack,
	Hit,
	Death,
	Def
}HeroActionType;

class HeroObj : public Ref
{
public:
	CC_SYNTHESIZE(int, positionX, positionX);
	CC_SYNTHESIZE(int, targetPosX, TargetPosX);
	CC_SYNTHESIZE(int, positionY, positionY);
	CC_SYNTHESIZE(int, targetPosY, TargetPosY);
	CC_SYNTHESIZE(bool, isMonster, IsMonster);
	CREATE_FUNC(HeroObj);
	virtual bool init();
	int getIndexX();
	void setIndexX(int indexX);
	int getIndexY();
	void setIndexY(int indexY);
	int getId();
	int getHeroACT();
	int getHeroHP();
	int getHeroDEF();
	int getHeroATK();
	int getHeroAdd();
	void setHeroAdd(int add);
	void setHeroHP(int hp);
	void setHeroATK(int atk);
	void setHero(BaseHeroes * h);
	BaseHeroes * getHero();

	Node * getMCurrentNode();
	void addNode(Layout * layout, Node * node);
	void setAction(ActionTimeline * action);
	void runAction(HeroActionType actionType);
	void updateNode();
	HeroActionType getActionType();
	void removeNode();
	HeroObj * cloneThis(int indexX);
	BaseHeroes * hero;
	void prepare(bool showNum);
	void def();
	//HeroObj * targetHero,int count, bool isCallFun, std::function<void()> callFun
	void atkEnemy(std::function<void()> hitWall);//不同的攻击类型 不同的开始
	bool collision(HeroObj * target);
	void attack(std::function<void(Node *)> callback, std::function<void()> hitWall);//不同的攻击类型 不同的攻击检测 bool  是否真实攻击 少血的
	void updateRound();
	void updateATK();
	void death();
	Node * getMWuQi();


public :
	bool isClone = false;
	bool isAttacking = false;
private:
	Layout * mCurrentLayout;
	Node * mCurrentNode;
	Node * mWuQi;
	int id = 0;
	int indexX = 0;
	int indexY = 0;
	ActionTimeline* mCurrentAction;
	HeroActionType mActionType = HeroActionType::Run;

	int mCurrentHP = 0;
	int mCurrentATK = 0;
	int mCurrentDEF = 0;
	int mCurrentADD = 0;
	int mCurrentACT = 0;
	double mCurrentDouble = 0;

	
};

#endif