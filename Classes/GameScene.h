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
#define MAX_ROW  8
#define MAX_COLUMN 8

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace timeline;

class GameScene : public Layer
{
public:
	CREATE_FUNC(GameScene);
	virtual bool init();
	void initGame();
	static Scene * createScene(UserInfo * info, std::vector<Heroes *> heroes,std::vector<Monster *> monsters,AI * ai);





public:
	UserInfo * mUserInfo;
	std::vector<Heroes *> baseHeroes;
	std::vector<Monster *> baseMonsters;
	AI * monstersAI;

	std::vector<HeroObj *> mHeros;
	std::vector<HeroObj *> monsterHeros;

	Node * gameSceneNode;//node
	Layout * gameSceneLayout;//��Ϸ����
	Sprite * leftWall; //��������
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
	TextAtlas * roundCountAtlas;//�غ���
	TextAtlas * secTimeAtlas;//ʣ��ʱ��
	TextAtlas * mActCountAtlas;//�ж�����
	TextAtlas * monsterActCountAtlas;//�ж�����
protected:
	Size mSceneSize;
};

typedef enum {
	Stand,
	Prepare,
	Run,
	Attack,
	Hit,
	Death
}HeroActionType;

#endif