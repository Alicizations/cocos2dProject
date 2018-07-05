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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//////////////////////////////
	// 2.call initialization function
	loadAnimation();
	loadMap();
	addSprite();
	addEventListener();
	addScheduler();


	return true;
}

void HelloWorld::loadAnimation()
{

}

void HelloWorld::loadMap()
{

}

void HelloWorld::addSprite()
{

}

void HelloWorld::addEventListener()
{

}

void HelloWorld::addScheduler()
{

}
