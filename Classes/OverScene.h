#pragma once
#include <stdio.h>
#include "cocos2d.h"

class OverScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
	int winner;
    virtual bool init();
    
	// a selector callback
	void restartMenuCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(OverScene);
};
