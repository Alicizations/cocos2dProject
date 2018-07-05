#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene {
public:
  static cocos2d::Scene* createScene();

  virtual bool init();

  // a selector callback

  void loadAnimation();
  void loadMap();
  void addSprite();
  void addEventListener();
  void addScheduler();

  void update(float f);
  void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
  void onKeyReleased(EventKeyboard::KeyCode code, Event* event);


  // implement the "static create()" method manually
  CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__