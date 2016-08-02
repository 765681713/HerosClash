#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//调整开发时设计分辨率
		glview = GLViewImpl::createWithRect("HerosClash", cocos2d::Rect(0, 0, designResolutionSize.width * 2 / 3, designResolutionSize.height * 2 / 3));
#else
        glview = GLViewImpl::create("HerosClash");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
#else
	auto frameSize = glview->getFrameSize();
	glview->setDesignResolutionSize(frameSize.width, frameSize.height, ResolutionPolicy::NO_BORDER);
	director->setContentScaleFactor(MIN(frameSize.height / designResolutionSize.height, frameSize.width / designResolutionSize.width));
#endif

	//资源路径
	std::vector<std::string> resPaths;
	resPaths.push_back("json/");
	resPaths.push_back("fonts/");
	resPaths.push_back("res/");
	resPaths.push_back("res/audio/");
	resPaths.push_back("res/beijing/");
	resPaths.push_back("res/effect/");
	resPaths.push_back("res/heroos/");
	resPaths.push_back("res/ui/");
	resPaths.push_back("audio/");
	resPaths.push_back("audio/effect");
	FileUtils::getInstance()->setSearchPaths(resPaths);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
