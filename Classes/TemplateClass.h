#ifndef __TEMPLATE_CLASS_H__
#define __TEMPLATE_CLASS_H__

#include "cocos2d.h"
#include "iostream"
//#include "ui\CocosGUI.h"
//#include "cocostudio\CocoStudio.h"
USING_NS_CC;
using namespace std;
//using namespace ui;
//using namespace cocostudio;

class TemplateClass : public Layer
{
public:
	CREATE_FUNC(TemplateClass);
	virtual bool init();
};

#endif