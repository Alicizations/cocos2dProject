#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void MenuScene::startMenuCallback(cocos2d::Ref* pSender)
{
	CCScene *s = HelloWorld::createScene();
	CCTransitionScene *reScene = CCTransitionProgressRadialCCW::create(1.2, s);
	CCDirector::sharedDirector()->replaceScene(reScene);
}
// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("ui/bg.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width;
	float winh = mywinsize.height;
	float spx = bg->getTextureRect().getMaxX();
	float spy = bg->getTextureRect().getMaxY();
	bg->setScaleX(winw / spx);
	bg->setScaleY(winh / spy);
	this->addChild(bg, 0);

	auto box1 = Sprite::create("ui/box.png");
	box1->setScaleY(2);
	box1->setScaleX(1.6);
	box1->setPosition(Vec2(visibleSize.width / 2 + origin.x - 150, visibleSize.height / 2 + origin.y));
	this->addChild(box1, 1);

	auto p1Label = Label::createWithTTF("P1", "fonts/arial.ttf", 30);
	p1Label->enableBold();
	p1Label->setPosition(Vec2(visibleSize.width / 2 + origin.x - 150, visibleSize.height / 2 + origin.y + 100));
	p1Label->setColor(Color3B::RED);
	this->addChild(p1Label, 2);

	auto baobao = Sprite::create("baobao/player.png");
	baobao->setScale(2);
	baobao->setPosition(Vec2(visibleSize.width / 2 + origin.x - 150, visibleSize.height / 2 + origin.y + 30));
	this->addChild(baobao, 2);

	auto p1Introduce = Label::createWithTTF("move:WASD\nbomb:space\nuse skill:F\nchange skill:R", "fonts/arial.ttf", 20);
	p1Introduce->enableBold();
	p1Introduce->setPosition(Vec2(visibleSize.width / 2 + origin.x - 150, visibleSize.height / 2 + origin.y - 70));
	p1Introduce->setColor(Color3B::RED);
	this->addChild(p1Introduce, 2);

	auto box2 = Sprite::create("ui/box.png");
	box2->setScaleY(2);
	box2->setScaleX(1.6);
	box2->setPosition(Vec2(visibleSize.width / 2 + origin.x + 150, visibleSize.height / 2 + origin.y));
	this->addChild(box2, 1);

	auto p2Label = Label::createWithTTF("P2", "fonts/arial.ttf", 30);
	p2Label->enableBold();
	p2Label->setPosition(Vec2(visibleSize.width / 2 + origin.x + 150, visibleSize.height / 2 + origin.y + 100));
	p2Label->setColor(Color3B::BLUE);
	this->addChild(p2Label, 2);

	auto pidan = Sprite::create("pidan/player.png");
	pidan->setScale(2);
	pidan->setPosition(Vec2(visibleSize.width / 2 + origin.x + 150, visibleSize.height / 2 + origin.y + 30));
	this->addChild(pidan, 2);

	auto p2Introduce = Label::createWithTTF("move:direction\nbomb:enter\nuse skill:0\nchange skill:2", "fonts/arial.ttf", 20);
	p2Introduce->enableBold();
	p2Introduce->setPosition(Vec2(visibleSize.width / 2 + origin.x + 150, visibleSize.height / 2 + origin.y - 70));
	p2Introduce->setColor(Color3B::BLUE);
	this->addChild(p2Introduce, 2);

	auto startLabel = Label::createWithTTF("Start", "fonts/arial.ttf", 50);
	auto startItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(MenuScene::startMenuCallback, this));
	startItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 100));
	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);	

    return true;
}

