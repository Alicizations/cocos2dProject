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

	//3. load the tilemap


	TMXTiledMap *tmx = TMXTiledMap::create("map/map.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));

	//除背景 以外 其他的都用它缩放（具体看情况）
	//double scale_x = visibleSize.width / 512; 
	//背景用它缩放（具体看情况）
	//double scale_y = visibleSize.height / 512;
	tmx->setScaleX(2.0);
	tmx->setScaleY(1.5);
	addChild(tmx, 0);

	layer1 = tmx->layerNamed("meta");
	layer2 = tmx->layerNamed("meta2");
	layer3 = tmx->layerNamed("pool");

	fortune = tmx->layerNamed("fortune");
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

	waveGridSize = 32;
	explosionDuration = 0.5f;
}

void HelloWorld::loadAnimation()
{
	loadPlayerAnimationHelper("baobao", "player1");

	loadWaveAnimationHelper();

}

void HelloWorld::loadPlayerAnimationHelper(string role, string player)
{
	loadFrameHelper(role + "/walkUp", player + "WalkUpAnimation", 5, walkDuration / 5.0f);
	loadFrameHelper(role + "/walkDown", player + "WalkDownAnimation", 5, walkDuration / 5.0f);
	loadFrameHelper(role + "/walkSideway", player + "WalkSidewayAnimation", 5, walkDuration / 5.0f);
	loadFrameHelper(role + "/die", player + "DieAnimation", 4, dieDuration / 4.0f);
	loadFrameHelper(role + "/win", player + "WinAnimation", 3, winDuration / 3.0f);
}

void HelloWorld::loadWaveAnimationHelper()
{
	loadFrameHelper("bomb/bomb", "bombAnimation", 3, 1.0f / 3.0f);
	loadFrameHelper("bomb/explosion", "explosionAnimation", 4, explosionDuration / 4.0f);
	loadFrameHelper("bomb/upWave", "upWaveAnimation", 4, explosionDuration / 4.0f);
	loadFrameHelper("bomb/upWaveTail", "upWaveTailAnimation", 4, explosionDuration / 4.0f);
	loadFrameHelper("bomb/downWave", "downWaveAnimation", 4, explosionDuration / 4.0f);
	loadFrameHelper("bomb/downWaveTail", "downWaveTailAnimation", 4, explosionDuration / 4.0f);
	loadFrameHelper("bomb/leftWave", "leftWaveAnimation", 4, explosionDuration / 4.0f);
	loadFrameHelper("bomb/leftWaveTail", "leftWaveTailAnimation", 4, explosionDuration / 4.0f);
	loadFrameHelper("bomb/rightWave", "rightWaveAnimation", 4, explosionDuration / 4.0f);
	loadFrameHelper("bomb/rightWaveTail", "rightWaveTailAnimation", 4, explosionDuration / 4.0f);
	loadFrameHelper("bomb/rightWaveTail", "rightWaveTailAnimation", 4, explosionDuration / 4.0f);
	loadFrameReverselyHelper("bomb/rightWaveTail", "rightWaveTailReverselyAnimation", 4, explosionDuration / 4.0f);
}

void HelloWorld::loadFrameHelper(string imagePath, string animationName, int frameNum, float frameDuration)
{
	cocos2d::Vector<SpriteFrame*> frameContainer;
	frameContainer.clear();
	for (int i = 1; i <= frameNum; i++)
	{
		auto texture = Director::getInstance()->getTextureCache()->addImage(imagePath + " (" + to_string(i) + ").png");
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
		frameContainer.pushBack(frame);
	}
	auto Animation = Animation::createWithSpriteFrames(frameContainer, frameDuration);
	AnimationCache::getInstance()->addAnimation(Animation, animationName);
}

void HelloWorld::loadFrameReverselyHelper(string imagePath, string animationName, int frameNum, float frameDuration)
{
	cocos2d::Vector<SpriteFrame*> frameContainer;
	frameContainer.clear();
	for (int i = 1; i <= frameNum; i++)
	{
		auto texture = Director::getInstance()->getTextureCache()->addImage(imagePath + " (" + to_string(i) + ").png");
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
		frameContainer.pushBack(frame);
	}
	frameContainer.reverse();
	auto Animation = Animation::createWithSpriteFrames(frameContainer, frameDuration);
	AnimationCache::getInstance()->addAnimation(Animation, animationName);
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
	auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkDownAnimation"));
	//auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("rightWaveTailReverselyAnimation"));
	player1->runAction(RepeatForever::create(walkAction));

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

bool HelloWorld::checkCanMove(int x, int y)
{
	if (layer1->tileAt(ccp(x, y)))
	{
		return false;
	}
	else if (layer2->tileAt(ccp(x, y)))
	{
		return false;
	}
	else if (layer3->tileAt(ccp(x, y)))
	{
		return false;
	}
	return true;
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
bool HelloWorld::checkObjectAndRemove(int x, int y)
{
	if (fortune->tileAt(ccp(x, y)))
	{
		fortune->removeTileAt(ccp(x, y));
		return true;
	}
	return false;
}