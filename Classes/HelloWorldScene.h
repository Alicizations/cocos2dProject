#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <string>
using std::string;
using std::to_string;

class HelloWorld : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback

	void initalizeParameters();
	void loadAnimation();
	void loadMap();
	void addSprite();
	void addEventListener();
	void addScheduler();
	void loadPlayerAnimationHelper(string role, string player);
	void loadWaveAnimationHelper();
	void loadFrameHelper(string imagePath, string animationName, int frameNum, float frameDuration);
	void loadFrameReverselyHelper(string imagePath, string animationName, int frameNum, float frameDuration);
	bool checkCanMove(int x, int y);
	bool checkObjectAndRemove(int x, int y);
private:
	cocos2d::Sprite* player1;
	cocos2d::Sprite* player2;
	float walkDuration; 
	float dieDuration;
	float winDuration;
	float waveGridSize;
	float explosionDuration;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

	CCTMXLayer* layer1;
	CCTMXLayer* layer2;
	CCTMXLayer* layer3;

	CCTMXLayer* fortune;
  // implement the "static create()" method manually
  CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__