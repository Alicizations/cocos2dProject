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
	player1->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(player1, 1);
	//for debug
	auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkDownAnimation"));
	//auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("rightWaveTailReverselyAnimation"));
	player1->runAction(RepeatForever::create(walkAction));

}

void HelloWorld::addEventListener()
{

}

void HelloWorld::addScheduler()
{

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

bool HelloWorld::checkObjectAndRemove(int x, int y)
{
	if (fortune->tileAt(ccp(x, y)))
	{
		fortune->removeTileAt(ccp(x, y));
		return true;
	}
	return false;
}