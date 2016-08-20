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
	CC_SYNTHESIZE(int, positionY, positionY);
	CC_SYNTHESIZE(bool, isMonster, IsMonster);
	CC_SYNTHESIZE(BaseHeroes *, hero, Hero);
	CREATE_FUNC(HeroObj);
	virtual bool init();
	int getIndexX();
	void setIndexX(int indexX);
	int getIndexY();
	void setIndexY(int indexY);
	int getId();
	int getHeroACT();
	Node * getMCurrentNode();
	void addNode(Layout * layout, Node * node);
	void setAction(ActionTimeline * action);
	void runAction(HeroActionType actionType);
	void updateNode();
	HeroActionType getActionType();
	void removeNode();
	HeroObj * cloneThis(int indexX);

	void prepare(bool showNum);
	void def();
	void attact(HeroObj * targetHero, bool isCallFun, std::function<void()> callFun);
	void updateRound();
	void hit(int * atk);
	void death();


public :
	bool isClone = false;
	
private:
	Layout * mCurrentLayout;
	Node * mCurrentNode;
	int id = 0;
	int indexX = 0;
	int indexY = 0;
	ActionTimeline* mCurrentAction;
	HeroActionType mActionType;

	int mCurrentHP = 0;
	int mCurrentATK = 0;
	int mCurrentDEF = 0;
	int mCurrentADD = 0;
	int mCurrentACT = 0;
	double mCurrentDouble = 0;
};

#endif