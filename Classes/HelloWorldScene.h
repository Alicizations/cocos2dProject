#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <string>
using std::string;
using std::to_string; 
USING_NS_CC;
using namespace CocosDenshion;

class HelloWorld : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback

	void initalizeParameters();
	void loadAnimation();
	void loadMap();
	void loadSound();
	void playBGM();
	void addSprite();
	void addEventListener();
	void addScheduler();
	void loadPlayerAnimationHelper(string role, string player);

	void fireGenerator(int layer);//决赛圈

	void update(float f);
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	void movePlayer(Sprite* player);
	void KeyArrayPush(int* keyArr, int num);
	void KeyArrayPop(int* keyArr, int num);
	void loadWaveAnimationHelper();
	void loadFlashAnimationHelper();
	void loadFireAnimationHelper();
	void loadFrameHelper(string imagePath, string animationName, int frameNum, float frameDuration);
	void loadFrameReverselyHelper(string imagePath, string animationName, int frameNum, float frameDuration);
	void bombExplode(int wavePower, cocos2d::Vec2 position, int posX, int posY);
	void ExplosionWaveGenerator(string direction, int offsetX, int offsetY, int wavePower, cocos2d::Vec2 position, int posX,int posY);
	bool checkCanMove(int x, int y);
	bool checkObjectAndRemove(int x, int y);

	void HelloWorld::layBomb(Sprite* player);
	// skill relative
	void UseSkill(Sprite* player, int SkillIndex);
	void flash(Sprite* player);
	void recover(Sprite* player);
	void BombUp(Sprite* player);
	void PowerUp(Sprite* player);
	void SpeedUp(Sprite* player);
	void CountCD(float f);
	// bomb explode relative
	void BombExploding(int BombPositionX, int BombPositionY, int power);
	void RecoverBombCount(int PlayerTag);
	int GetBombPowerByTag(int PlayerTag);
	bool CheckStopBomb(int posX, int posY);

	void checkAndHandleProperty(int playerID);
	void checkAndChangeBlood(int posX, int posY);
private:
	cocos2d::Sprite* player1;
	cocos2d::Sprite* player2;
	float walkDuration; 

	int P1Speed;
	int P2Speed;
	int SpeedIncreasingDegree;
	int MaxSpeed;

	//决赛圈开始缩小时间
	float fireGeneratingStartTime;
	//决赛圈缩小时间
	float fireGeneratingGap;
	//决赛圈计时器
	float fireTimeCount;

	float dieDuration;
	float winDuration;
	float explosionHoldDuration;
	float waveGridSize;
	float explosionDuration;

	bool P1absoluteDefense;
	bool P2absoluteDefense;
	float absoluteDefenseTime;
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
	int P1SkillCDs[5];
	int P2SkillCDs[5];
	// 0 flash, 1 recover, 2 bomb, 3 power, 4 speed
	int P1SkillIndex;
	int P2SkillIndex;
	// number of skills
	int SkillCount;
	// Bomb relative
	cocos2d::Sprite* BombMatrix[16][16];
	int P1BombMax;
	int P1BombLaid;
	int P1BombWavePower;
	int P2BombMax;
	int P2BombLaid;
	int P2BombWavePower;
	
	cocos2d::Sprite* PropertyMatrix[16][16];

	CCTMXLayer* layer1;
	CCTMXLayer* layer2;
	CCTMXLayer* layer3;

	CCTMXLayer* fortune;

	Layer* UIlayer;
	Label* P1bombNumLabel;
	Label* P2bombNumLabel;
	Label* P1powerLabel;
	Label* P2powerLabel;
	Label* P1speedLabel;
	Label* P2speedLabel;
	cocos2d::ProgressTimer* pT1;
	cocos2d::ProgressTimer* pT2;

  // implement the "static create()" method manually
  CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__