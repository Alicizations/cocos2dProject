#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

Scene* HelloWorld::createScene() 
{
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) 
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() 
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) 
	{
	return false;
	}


	//////////////////////////////
	// 2.call initialization function
	initalizeParameters();
	loadAnimation();
	loadMap();
	addSprite();
	addEventListener();
	addScheduler();


	return true;
}

void HelloWorld::initalizeParameters()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	walkDuration = 0.5f;
	dieDuration = 0.4f;
	winDuration = 0.3f;
	// move relative
	P1TryMoving = false;
	P1IsMoving = false;
	P2TryMoving = false;
	P2IsMoving = false;
	for (int i = 0; i < 5; i++)
	{
		P1KeyArray[i] = P2KeyArray[i] = 0;
	}
	P1PositionX = 1;
	P1PositionY = 1;
	P2PositionX = 14;
	P2PositionY = 14;
}

void HelloWorld::loadAnimation()
{
	loadPlayerAnimationHelper("baobao", "player1");
}

void HelloWorld::loadPlayerAnimationHelper(string role, string player)
{
	cocos2d::Vector<SpriteFrame*> frameContainer;
	

	//walk up
	frameContainer.clear();
	for (int i = 1; i <= 5; i++)
	{
		auto texture = Director::getInstance()->getTextureCache()->addImage(role + "/walkUp ("+ to_string(i) +").png");
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
		frameContainer.pushBack(frame);
	}
	auto walkUpAnimation = Animation::createWithSpriteFrames(frameContainer, walkDuration / 5);
	AnimationCache::getInstance()->addAnimation(walkUpAnimation, player + "WalkUpAnimation");

	//walk down 
	frameContainer.clear();
	for (int i = 1; i <= 5; i++)
	{
		auto texture = Director::getInstance()->getTextureCache()->addImage(role + "/walkDown (" + to_string(i) + ").png");
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
		frameContainer.pushBack(frame);
	}
	auto walkDownAnimation = Animation::createWithSpriteFrames(frameContainer, walkDuration / 5);
	AnimationCache::getInstance()->addAnimation(walkDownAnimation, player + "WalkDownAnimation");

	//walk sideway
	frameContainer.clear();
	for (int i = 1; i <= 5; i++)
	{
		auto texture = Director::getInstance()->getTextureCache()->addImage(role + "/walkSideway (" + to_string(i) + ").png");
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
		frameContainer.pushBack(frame);
	}
	auto walkSidewayAnimation = Animation::createWithSpriteFrames(frameContainer, walkDuration / 5);
	AnimationCache::getInstance()->addAnimation(walkSidewayAnimation, player + "WalkSidewayAnimation");

	//die
	frameContainer.clear();
	for (int i = 1; i <= 4; i++)
	{
		auto texture = Director::getInstance()->getTextureCache()->addImage(role + "/die (" + to_string(i) + ").png");
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
		frameContainer.pushBack(frame);
	}
	auto dieAnimation = Animation::createWithSpriteFrames(frameContainer, dieDuration / 4);
	AnimationCache::getInstance()->addAnimation(dieAnimation, player + "DieAnimation");

	//win
	frameContainer.clear();
	for (int i = 1; i <= 3; i++)
	{
		auto texture = Director::getInstance()->getTextureCache()->addImage(role + "/win (" + to_string(i) + ").png");
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
		frameContainer.pushBack(frame);
	}
	auto winAnimation = Animation::createWithSpriteFrames(frameContainer, winDuration / 3);
	AnimationCache::getInstance()->addAnimation(winAnimation, player + "WinAnimation");

}

void HelloWorld::loadMap()
{

}

void HelloWorld::addSprite()
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("baobao/player.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
	player1 = Sprite::createWithSpriteFrame(frame);
	player1->setPosition(16+32, 16+32);
	this->addChild(player1, 1);
	//for debug
	//auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkDownAnimation"));
	//player1->runAction(RepeatForever::create(walkAction));

}

void HelloWorld::addEventListener()
{
	// KeyboardListener
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
}

void HelloWorld::addScheduler()
{
	schedule(schedule_selector(HelloWorld::update), 0.05f, kRepeatForever, 0);
}

void HelloWorld::update(float f)
{
	if (!P1IsMoving && P1TryMoving)
	{
		this->movePlayer(player1);
	}
	if (!P2IsMoving && P2TryMoving)
	{
		this->movePlayer(player2);
	}
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		P1TryMoving = true;
		KeyArrayPush(P1KeyArray, 1);
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		P1TryMoving = true;
		KeyArrayPush(P1KeyArray, 2);
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		P1TryMoving = true;
		KeyArrayPush(P1KeyArray, 3);
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		P1TryMoving = true;
		KeyArrayPush(P1KeyArray, 4);
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		// fire
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		break;
	}
}

// 1 up, 2 down, 3 left, 4 right
void HelloWorld::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		KeyArrayPop(P1KeyArray, 1);
		P1TryMoving = P1KeyArray[0] != 0;
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		KeyArrayPop(P1KeyArray, 2);
		P1TryMoving = P1KeyArray[0] != 0;
		break;
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
		KeyArrayPop(P1KeyArray, 3);
		P1TryMoving = P1KeyArray[0] != 0;
		break;
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
		KeyArrayPop(P1KeyArray, 4);
		P1TryMoving = P1KeyArray[0] != 0;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		break;
	}
}

void HelloWorld::movePlayer(Sprite* player) {
	auto speed = 10.0f;

	if (player == player1)
	{
		int dir = P1KeyArray[P1KeyArray[0]];
		int x = 0;
		int y = 0;
		Animate* walkAction;
		switch (dir)
		{
		case 1:
			y = 1;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkUpAnimation"));
			break;
		case 2:
			y = -1;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkDownAnimation"));
			break;
		case 3:
			x = -1;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkSidewayAnimation"));
			break;
		case 4:
			x = 1;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkSidewayAnimation"));
			break;
		default:
			break;
		}
		if (P1PositionX + x < 16 && P1PositionX + x >= 0 && P1PositionY + y < 16 && P1PositionY + y >= 0)
		{
			P1IsMoving = true;
			P1PositionX += x;
			P1PositionY += y;
			auto moveAC = Spawn::createWithTwoActions(MoveTo::create(0.5f, Vec2(16 + 32 * P1PositionX, 16 + 32 * P1PositionY)), walkAction);
			player1->setFlippedX(x == -1);
			auto ac = Sequence::create(moveAC, CallFunc::create([this]() {this->P1IsMoving = false; }), nullptr);
			player1->runAction(ac);
		}
	}
	else
	{

	}
}

void HelloWorld::KeyArrayPush(int * keyArr, int num)
{
	if (keyArr[0] >= 4)
	{
		KeyArrayPop(keyArr, num);
		keyArr[4] = num;
		keyArr[0]++;
	}
	else 
	{
		keyArr[++keyArr[0]] = num;
	}
}

void HelloWorld::KeyArrayPop(int * keyArr, int num)
{
	if (num > 4 || num < 1 || keyArr == nullptr)
	{
		return;
	}
	else
	{
		for (int i = 1; i < 5; i++)
		{
			if (keyArr[i] == num)
			{
				if (i != 4)
				{
					for (int j = i; j < 4; j++)
					{
						keyArr[i] = keyArr[i+1];
					}
				}
				keyArr[0]--;
				return;
			}
		}
	}
}
