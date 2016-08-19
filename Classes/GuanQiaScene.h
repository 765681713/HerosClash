#ifndef __GUANQIA_SCENE_H__
#define __GUANQIA_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "cocostudio\CocoStudio.h"
#include "bean\GuanQiaBean.h"
#include "json\document.h"
#include "bean\UserInfo.h"
#include "bean\Heroes.h"
#include "bean\Monster.h"
#include "bean\WuPin.h"
#include "bean\AI.h"
#include "GameScene.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;

class GuanQiaScene : public Layer
{
public:
	CREATE_FUNC(GuanQiaScene);
	virtual bool init();
	static Scene * createScene(UserInfo * info, std::vector<Heroes *> heroes);

	void decodeGuanQiaJson();
	void decodeMonsterJson();
	void decodeWuPinJson();
	void decodeAIJson();

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

	bool onBoosGQTouchBegan(Touch *pTouch, Event *pEvent);
	void onBoosGQTouchEnded(Touch *pTouch, Event *pEvent);

	//����ؿ��ص�
	void downEnd(Node *  node);
	void startGame(Ref *  node);
	void guanQiaBack(Ref *  node);
	void preBack(Ref *  node);

	//Ӣ���б����¼�void addEventListenerListView(Ref* target, SEL_ListViewEvent selector);
	//void onItemClickListener(Ref * pSender, ListView::EventType type);
	//void onItemClickListener(Ref * pSender);

	void removeListener();
	void addListener(int index);

	virtual void onExit();
	~GuanQiaScene();
public:
	PageView * mPageView;
	Layout * mPrepareLayout;
	Layout * prepareLayout;

	Layout * mGuanQia1;
	Layout * mGuanQia2;

	Text * mTitle;

	Layout * mGuanQiaBiaoZhi;
	int mCurrentIndexPage = 0;
	int mGuanQiaCount = 1;//���ص�����

	UserInfo * mInfo;
	//static std::vector<Heroes *> mHeroes;
	//CC_SYNTHESIZE(UserInfo * ,mInfo,UserInfo);
	CC_SYNTHESIZE(std::vector<Heroes *>,mHeroes,Heroes);//���е�Ӣ��
	std::vector<WuPin *> mWuPins;
	std::vector<Monster *> mCurrentMonsters;
	std::vector<Heroes *> mCurrentHeroes;//ѡ���
	GuanQiaItemBean * mCurrentItem;
	GuanQiaItemBean * mGuanQiaItem;
	std::vector<GuanQiaItemBean * > mGuanQiaItems;
	GuanQiaBean * mGuanQia;
	std::vector<GuanQiaBean * > mGuanQias;
	AI * monsterAi;

	EventListenerTouchOneByOne * xiaoGuanListener;
	EventListenerTouchOneByOne * bossListener;

};

#endif