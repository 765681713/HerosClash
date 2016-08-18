#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "cocostudio\CocoStudio.h"
#include "bean\UserInfo.h"
#include "bean\Heroes.h"
#include "bean\Monster.h"
#include "bean\AI.h"
#include "bean\HeroObj.h"
//#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace timeline;

class GameScene : public Layer
{
public:
	//CREATE_FUNC(GameScene);
	static Scene * createScene(UserInfo * info, std::vector<Heroes *> heroes, std::vector<Monster *> monsters, AI * ai);

	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	void initGame();
	bool initNotRepeatForHero(int indexX, int indexY, int id);
	bool initNotRepeatForMonster(int indexX, int indexY, int id);
	void addBoss(int index, int indexX, int indexY, int posionX, int posionY, BaseHeroes * heroes);
	void heroEntry();
	void monsterEntry();
	void mHeroPrepareAndDef(); 
	void mHeroPrepareAction(std::vector<std::vector<HeroObj *>> allHHeroes);
	void mHeroDefAction(std::vector<std::vector<HeroObj *>> allVHeroes);
	void swapHeroPosition(HeroObj * from, HeroObj * to);

	bool onTouchBegan(Touch * pTouch, Event * pEvent);
	void onTouchMoved(Touch * pTouch, Event * pEvent);
	void onTouchEnded(Touch * pTouch, Event * pEvent);
	void onLongTouchDown(float delay);
	
	//AI
	void aiSchedule(float delay);

	virtual void onExit();
public:
	bool isInit;
	bool isActionRuning = false;
	bool isLongPress = false;
	bool isAiRound;
	
	const int currentHeroTag = 520;
	int touchDownY;
	int longTouchDownX;
	int longTouchDownY;

	HeroObj * mCurrentHero = nullptr;
	UserInfo * mUserInfo;
	std::vector<Heroes *> baseHeroes;
	std::vector<Monster *> baseMonsters;
	std::vector<std::vector<HeroObj * >> prepareHeroes;
	AI * monstersAI;

	int existHeroCount = 0;
	int existMonsterCount = 0;

	//std::vector<HeroObj *> mHeros;
	//std::vector<HeroObj *> monsterHeros;
	HeroObj * mHeros[MAX_ROW * MAX_COLUMN];
	HeroObj * monsterHeros[MAX_ROW * MAX_COLUMN];

	Node * gameSceneNode;//node
	Layout * gameSceneLayout;//游戏场景
	Sprite * leftWall; //左右两边
	Sprite * rightWall;
	ImageView * jiNeng1;
	LoadingBar * mHPLoadingBar;
	LoadingBar * monsterHPLoadingBar;
	TextAtlas * mHPAtlas;
	TextAtlas * monsterHPAtlas;
	TextAtlas * mMPAtlas;
	TextAtlas * monsterMPAtlas;
	TextAtlas * mHeroAllCount;
	TextAtlas * monsterAllCount;
	LoadingBar * mMPLoadingBar;
	LoadingBar * monsterMPLoadingBar;
	Text * mLevel;
	Text * monsterLevel;
	TextAtlas * roundCountAtlas;//回合数
	TextAtlas * secTimeAtlas;//剩余时间
	TextAtlas * mActCountAtlas;//行动次数
	TextAtlas * monsterActCountAtlas;//行动次数

	EventDispatcher * eventDispatcher;
protected:
	Size mSceneSize;
};

#endif