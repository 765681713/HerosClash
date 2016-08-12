#ifndef __GUANQIA_BEAN_H__
#define __GUANQIA_BEAN_H__

#include "cocos2d.h"
#include "GuanQiaItemBean.h"

USING_NS_CC;
class GuanQiaBean : public Ref
{
public:
	CREATE_FUNC(GuanQiaBean);
	virtual bool init();
	CC_SYNTHESIZE(std::string, name, Name);
	CC_SYNTHESIZE(std::vector<GuanQiaItemBean * >, content, Content);
};

#endif