#ifndef __SocketIO_H__
#define __SocketIO_H__

#include "cocos2d.h"

USING_NS_CC;


class SocketIO : public Ref
{
public:
	CREATE_FUNC(SocketIO);
	virtual bool init();

	CC_SYNTHESIZE(std::string, desc, Desc);

};

#endif