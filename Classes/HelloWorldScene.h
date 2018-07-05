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

private:
	cocos2d::Sprite* player1;
	cocos2d::Sprite* player2;
	float walkDuration; 
	float dieDuration;
	float winDuration;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

  void update(float f);
  void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
  void onKeyReleased(EventKeyboard::KeyCode code, Event* event);


  // implement the "static create()" method manually
  CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__