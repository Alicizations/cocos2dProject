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
	player1->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(player1, 1);
	//for debug
	auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkDownAnimation"));
	player1->runAction(RepeatForever::create(walkAction));

}

void HelloWorld::addEventListener()
{

}

void HelloWorld::addScheduler()
{

}
