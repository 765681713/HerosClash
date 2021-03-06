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
#define RoundTime 30
#define SwapActionTag 1001
#define HeroRunActionTag 1002
#define HeroEnterActionTag 1003

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
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();

	void initGame();
	void updateUi();
	void updateHeroCount();
	void updateHPMP();
	void updateActTime();
	void updateHeroAct();
	bool initNotRepeatForHero(int indexX, int indexY, int id);
	bool initNotRepeatForMonster(int indexX, int indexY, int id);
	void addBoss(int index, int indexX, int indexY, int posionY, BaseHeroes * heroes);
	void heroEntry();
	void heroCountLCallBack(Ref * ref);
	void monsterEntry();
	void mHeroPrepareAndDef(bool isClick, HeroObj * mHeros[]);
	void mHeroPrepareAction(std::vector<std::vector<HeroObj *>> allHHeroes, HeroObj * mHeros[]);
	void mHeroDefAction(std::vector<std::vector<HeroObj *>> allVHeroes, HeroObj * mHeros[]);
	void swapHeroPosition(HeroObj * from, HeroObj * to, HeroObj * mHeros[]);
	void runHeroPosition(HeroObj * mHeros[]);
	void showCupBlood(int cupBlood, int posX, int posY, int zOder);
	void actRoundChage();
	void gameWin(bool isMy);
	/*void monsterPrepareAndDef(bool isClick, HeroObj * mHeros[]);
	void monsterPrepareAction(std::vector<std::vector<HeroObj *>> allHHeroes);
	void monsterDefAction(std::vector<std::vector<HeroObj *>> allVHeroes);
	void swapMonsterPosition(HeroObj * from, HeroObj * to);
	void runMonsterPosition();*/

	bool onTouchBegan(Touch * pTouch, Event * pEvent);
	void onTouchMoved(Touch * pTouch, Event * pEvent);
	void onTouchEnded(Touch * pTouch, Event * pEvent);
	void onLongTouchDown(float delay);
	
	//AI
	void aiSchedule(float delay);
	void timeSchedule(float delay);
	virtual void update(float dt);
	//def
	bool defNext(int x, int y, int index,int next, int nextNext);

	virtual void onExit();
private:
	bool isInit;
	bool isActionRuning = false;
	bool isLongPress = false;
	bool isAiRound;
	bool isMWinGame = false;
	bool isMonsterWinGame = false;
	bool isRoundChangeing = false;
	static const int AllHeroCount = MAX_ROW * MAX_COLUMN;

	int mCurrentHP = 0;
	int mCurrentMP = 0;
	int mDEF = 0;
	int roundCount = 0;
	int secCurrentTime = 0;
	int mActCount = 0;
	int monsterCurrentHP = 0;
	int monsterCurrentMP = 0;
	int monsterDEF = 0;
	int monsterActCount = 0;
	int existHeroCount = 0;
	int existMonsterCount = 0;
	const int currentHeroTag = 520;
	int touchDownY;
	int longTouchDownX;
	int longTouchDownY;

	HeroObj * mCurrentHero = nullptr;
	UserInfo * mUserInfo;
	//std::map<int, ActionTimeline *> animations;
	std::vector<Heroes *> baseHeroes;
	std::vector<Monster *> baseMonsters;
	std::vector<std::vector<HeroObj * >> prepareHeroes;
	std::vector<std::vector<HeroObj * >> prepareMonster;

	AI * monstersAI;
	HeroObj * mHeroes[AllHeroCount];
	HeroObj * monsterHeros[AllHeroCount];

	EventDispatcher * eventDispatcher;

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
protected:
	Size mSceneSize;
};

#endif