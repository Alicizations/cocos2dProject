#include "OverScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#define database UserDefault::getInstance()
USING_NS_CC;

Scene* OverScene::createScene()
{
    return OverScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void OverScene::restartMenuCallback(cocos2d::Ref* pSender)
{
	CCScene *s = HelloWorld::createScene();
	CCTransitionScene *reScene = CCTransitionProgressRadialCCW::create(1.2, s);
	CCDirector::sharedDirector()->replaceScene(reScene);
}
// on "init" you need to initialize your instance
bool OverScene::init()
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

	auto box = Sprite::create("ui/box.png");
	box->setScaleY(2);
	box->setScaleX(1.6);
	box->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(box, 1);

	winner = database->getIntegerForKey("winner");
	string p = winner == 1 ? "P1" : "P2";
	string player = winner == 1 ? "baobao" : "pidan";

	auto pLabel = Label::createWithTTF(p, "fonts/arial.ttf", 30);
	pLabel->enableBold();
	pLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 100));
	pLabel->setColor(Color3B::RED);
	this->addChild(pLabel, 2);

	auto playerImage = Sprite::create(player + "/player.png");
	playerImage->setScale(2);
	playerImage->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 30));
	this->addChild(playerImage, 2);

	auto info = Label::createWithTTF(player + " WIN!", "fonts/arial.ttf", 30);
	info->enableBold();
	info->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 70));
	this->addChild(info, 2);

	auto restartLabel = Label::createWithTTF("Restart", "fonts/arial.ttf", 50);
	auto restartItem = MenuItemLabel::create(restartLabel, CC_CALLBACK_1(OverScene::restartMenuCallback, this));
	restartItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 100));
	auto menu = Menu::create(restartItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);	

    return true;
}

