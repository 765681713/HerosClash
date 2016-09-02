#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	//Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//使用cocostudio 加载csb文件
	Node * sceneNode = CSLoader::createNode("loadingScene.csb");
	sceneNode->setAnchorPoint(Vec2::ZERO);
	sceneNode->setPosition(Vec2::ZERO);
	this->addChild(sceneNode);
	//获得loadingbar
	loadingBar = static_cast <ui::LoadingBar *>(sceneNode->getChildByName("LoadingBar_1"));
	this->schedule(schedule_selector(HelloWorld::udpateProgress), 0.5);
	//获得菜单背景纹理
	//Texture2D * texture = Director::getInstance()->getTextureCache()->addImage("menu_bg.png");
	//Rect rect = Rect::ZERO;
	//rect.size = texture->getContentSize();
	//得到精灵帧   通过纹理和大小
	//SpriteFrame * frame = SpriteFrame::createWithTexture(texture, rect);
	//添加到缓存
	//SpriteFrameCache::getInstance()->addSpriteFrame(frame, "menu_bg");

	//auto loading = Director::getInstance()->getTextureCache()->addImage("loading_bg2.png");
	//Rect r = Rect::ZERO;
	//r.size = loading->getContentSize();
	//SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::createWithTexture(loading, r), "loading_bg2");

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("loading_bg.plist");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("main_scene.mp3");

	return true;
}

void HelloWorld::onEnterTransitionDidFinish(){

	//rapidjson::Document document;
	//document.SetObject();
	//rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	//rapidjson::Value array(rapidjson::kArrayType);
	//rapidjson::Value object(rapidjson::kObjectType);
	//object.AddMember("int", 1, allocator);
	//object.AddMember("double", 1.0, allocator);
	//object.AddMember("bool", true, allocator);
	//object.AddMember("hello", "HelloWorld", allocator);
	//array.PushBack(object, allocator);
	//StringBuffer buffer;
	//rapidjson::Writer<StringBuffer> writer(buffer);
	//document.Accept(writer);

	/*document.AddMember("username", "BingYanDeXuanLv", allocator);
	document.AddMember("userpwd", "123456789", allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);*/
	//log(" buffer = %s " , buffer.GetString());
	std::string params = String::createWithFormat("username=%s&userpwd=%s",userName,userPwd)->getCString();
	HttpRequest * req = new HttpRequest();
	req->setUrl(loginUrl);
	req->setRequestType(HttpRequest::Type::POST);
	req->setResponseCallback(this,httpresponse_selector(HelloWorld::login));
	req->setRequestData(params.c_str(), strlen(params.c_str()));
	HttpClient::getInstance()->send(req);
	req->release();
}

void HelloWorld::login(HttpRequest * req, HttpResponse * res){
	if (!res){
		return;
	}
	int code = res->getResponseCode();
	auto dataChar = res->getResponseData();
	std::string data = std::string(dataChar->begin(),dataChar->end());
	rapidjson::Document document;
	document.Parse<0>(data.c_str());
	if (!document.HasParseError()){
		int resultCode = document["resultCode"].GetInt();
		if (resultCode == ResultCodeSuccess){
			isLogin = true;
		}
	}
}

void HelloWorld::udpateProgress(float time){
	if (loadingBar != nullptr){
		int loading = loadingBar->getPercent();
		loading += 20;
		if (loading >= 100){
			loadingBar->setPercent(100);
			if (isLogin){
				Scene * ms = MenuScene::createScene();
				Director::getInstance()->replaceScene(ms);
			}
		}
		else{
			loadingBar->setPercent(loading);
		}
	}
}

void HelloWorld::onExit(){
	Layer::onExit();
	this->unschedule(schedule_selector(HelloWorld::udpateProgress));

}