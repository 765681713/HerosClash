#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "LoadingScene.h"
#include "cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GuanQiaScene.h"
#include "bean\UserInfo.h"
#include "bean\Heroes.h"
#include "bean\WuPin.h"
#include "network\SocketIO.h"

#define MUSIC_KEY "music_key"
#define SOUND_KEY "sound_key"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;
using namespace CocosDenshion;
using namespace network;

class MenuScene : public Layer,SocketIO::SIODelegate
{
public:
	CREATE_FUNC(MenuScene);
	virtual bool init();
	~MenuScene();

	static cocos2d::Scene* createScene();

	virtual void onEnterTransitionDidFinish();
	virtual void cleanup();
	//������Ϣ
	void onInfoLayoutClick(Ref * ref);
	//����������ť
	void onAddGoldClick(Ref * ref);
	void onAddZuanShiClick(Ref * ref);
	void onAddTiLiClick(Ref * ref);
	//Ӣ�ۣ���Ʒ������
	void onOtherBtnClick(Ref * ref);
	//scrll  �е����о���
	bool spriteTouchBegan(Touch * touch, Event* ev);
	void spriteTouchMove(Touch* touch, Event* ev);
	void spriteTouchEnd(Touch* touch, Event* ev);
	//ǩ�����ֵ��ť�ص�
	bool otherTouchBegan(Touch * touch, Event* ev);
	void otherTouchEnd(Touch* touch, Event* ev);
	//�������ص�
	void downEnd(Node *  node);
	void decodeUserInfo();
	void decodeHeroList();
	void decodeWuPinList();
	virtual void onExit();

	//Ӣ���б�
	void onYingXiongLanClick();
	//�����б�
	void onBeiBaoLanClick();

	//����
	virtual void onConnect(SIOClient* client);
	virtual void onMessage(SIOClient* client, const std::string& data);
	virtual void onClose(SIOClient* client);
	virtual void onError(SIOClient* client, const std::string& data);
	//�ص�
	virtual void getUserInfo(SIOClient* client, const std::string& data);


	std::string getCurrentTime();
public:
	EventDispatcher * eventDispatcher;
	Layout * layer1;
	Node * sceneNode;
	ui::ScrollView * scrollView;
	EventListenerTouchOneByOne * spriteListener;
	EventListenerTouchOneByOne * otherListener;
	EventListenerTouchOneByOne * backEvent;
	Layout * settingLayout;
	Layout * yingXiongLayout;
	ScrollView * yingXiongList;
	Layout * beiBaoLayout;
	ScrollView * wuPinList;
	std::vector<Heroes *> mHeroes;
	std::vector<WuPin *> mWuPins;
	UserInfo * userInfo;
	SIOClient * sioClient;

};

typedef enum {
	//�������еľ���
	ScrollOpenGame,
	ScrollYouXiang,
	ScrollYuanZheng,
	ScrollShangDian,
	ScrollPaiHang,
	ScrollGongHui,
	ScrollYingXiongWu,
	ScrollShiLianu,
	//TopPanel
	QianDao,
	HuoDong,
	ChongZhi,
	//other
	YingXiongLan,
	BeiBao,
	RenWu,
	//���ذ�ť
	SettingRightBack,
	YXLeftBack,
	BBLeftBack

}MenuType;

#endif