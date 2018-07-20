#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <string>
using std::string;
using std::to_string;
USING_NS_CC;

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

	void update(float f);
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	void movePlayer(Sprite* player);
	void KeyArrayPush(int* keyArr, int num);
	void KeyArrayPop(int* keyArr, int num);
	void loadWaveAnimationHelper();
	void loadFrameHelper(string imagePath, string animationName, int frameNum, float frameDuration);
	void loadFrameReverselyHelper(string imagePath, string animationName, int frameNum, float frameDuration);
	void bombExplode(int wavePower, cocos2d::Vec2 position, int posX, int posY);
	void ExplosionWaveGenerator(string direction, int offsetX, int offsetY, int wavePower, cocos2d::Vec2 position, int posX,int posY);
	bool checkCanMove(int x, int y);
	bool checkObjectAndRemove(int x, int y);

	void HelloWorld::layBomb(Sprite* player);
	void flash(Sprite* player);
	// bomb explode relative
	void BombExploding(int BombPositionX, int BombPositionY, int power);
	void RecoverBombCount(int PlayerTag);
	int GetBombPowerByTag(int PlayerTag);
	bool CheckStopBomb(int posX, int posY);
private:
	cocos2d::Sprite* player1;
	cocos2d::Sprite* player2;
	float walkDuration; 

	float P1Speed;
	float P2Speed;
	float SpeedIncreasingDegree;
	float MaxSpeed;


	float dieDuration;
	float winDuration;
	float explosionHoldDuration;
	float waveGridSize;
	float explosionDuration;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	// move relative
	bool P1TryMoving;
	bool P1IsMoving;
	int P1KeyArray[5];
	int P1PositionX;
	int P1PositionY;
	int P1InitialX;
	int P1InitialY;
	int P1Dir;
	bool P2TryMoving;
	bool P2IsMoving;
	int P2KeyArray[5];
	int P2PositionX;
	int P2PositionY;
	int P2InitialX;
	int P2InitialY;
	int P2Dir;
	// skill relative

	// Bomb relative
	cocos2d::Sprite* BombMatrix[16][16];
	int P1BombMax;
	int P1BombLaid;
	int P1BombWavePower;
	int P2BombMax;
	int P2BombLaid;
	int P2BombWavePower;
	
	

	CCTMXLayer* layer1;
	CCTMXLayer* layer2;
	CCTMXLayer* layer3;

	CCTMXLayer* fortune;

	Layer* UIlayer;
  // implement the "static create()" method manually
  CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__