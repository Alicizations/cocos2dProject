#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <cmath>
#include <string>
USING_NS_CC;

 
#define P1WalkDuration (1.0f / P1Speed)
#define P2WalkDuration (1.0f / P2Speed)

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
	playBGM();
	addSprite();
	addEventListener();
	addScheduler();
}

void HelloWorld::initalizeParameters()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	walkDuration = 0.1f;
	dieDuration = 0.4f;
	winDuration = 0.3f;
	waveGridSize = 37.5;
	explosionDuration = 1.0f;
	explosionHoldDuration = 0.5f;

	absoluteDefenseTime = 1.5f;
	P1absoluteDefense = false;
	P2absoluteDefense = false;
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
	P1PositionY = 0;
	P2PositionX = 14;
	P2PositionY = 13;
	P1InitialX = 215;
	P1InitialY = 20;
	P2InitialX = 215;
	P2InitialY = 20;
	P1Dir = 3;
	P2Dir = 3;
	//  property matrix
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			PropertyMatrix[i][j] = nullptr;
		}
	}
	// bomb matrix
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			BombMatrix[i][j] = nullptr;
		}
	}
	// 最大放置数量
	P1BombMax = 1;
	// 目前放置的数量
	P1BombLaid = 0;
	// 炸弹威力
	P1BombWavePower = 1;
	P2BombMax = 1;
	P2BombLaid = 0;
	P2BombWavePower = 1;

	// 初始速度
	P1Speed = 2;
	P2Speed = 2;
	// 每检获一个速度道具提升的幅度
	SpeedIncreasingDegree = 2;
	// 最大速度
	MaxSpeed = 10;

	//决赛圈开始缩小时间
	fireGeneratingStartTime = 20;
	//决赛圈缩小间隔
	fireGeneratingGap = 10;
	fireTimeCount = 0;

	// skill relative
	SkillCount = 5;
	P1SkillIndex = 0;
	P2SkillIndex = 0;
	for (int i = 0; i < 5; i++)
	{
		P1SkillCDs[i] = P2SkillCDs[i] = 0;
	}
}

void HelloWorld::loadAnimation()
{
	loadPlayerAnimationHelper("baobao", "player1");
	loadPlayerAnimationHelper("pidan", "player2");
	loadWaveAnimationHelper();
	loadFlashAnimationHelper();
	loadFireAnimationHelper();

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
	loadFrameHelper("bomb/centerWave", "centerWaveVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/upWave", "upWaveVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/upWaveTail", "upWaveTailVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/downWave", "downWaveVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/downWaveTail", "downWaveTailVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/leftWave", "leftWaveVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/leftWaveTail", "leftWaveTailVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/rightWave", "rightWaveVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/rightWaveTail", "rightWaveTailVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));

	loadFrameReverselyHelper("bomb/centerWave", "centerWaveGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/upWave", "upWaveGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/upWaveTail", "upWaveTailGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/downWave", "downWaveGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/downWaveTail", "downWaveTailGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/leftWave", "leftWaveGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/leftWaveTail", "leftWaveTailGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/rightWave", "rightWaveGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/rightWaveTail", "rightWaveTailGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
}

void HelloWorld::loadFlashAnimationHelper()
{
	loadFrameHelper("flash/flash", "flashAnimation", 4, 0.15f);
}

void HelloWorld::loadFireAnimationHelper()
{
	loadFrameHelper("fire/fire", "fireAnimation", 3, 1.0f/3.0f);
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

void HelloWorld::loadSound()
{
	SimpleAudioEngine::getInstance()->preloadEffect("sound/lay.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/explode.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/flash.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/get.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/die.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/win.wav");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/bg.wav");
}

void HelloWorld::playBGM()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/bg.wav", true);
}

void HelloWorld::loadMap()
{
	TMXTiledMap *tmx = TMXTiledMap::create("map/map.tmx");
	tmx->setPosition(96 + visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));

	//除背景 以外 其他的都用它缩放（具体看情况）
	//double scale_x = visibleSize.width / 512; 
	//背景用它缩放（具体看情况）
	//double scale_y = visibleSize.height / 512;
	tmx->setScaleX(1.5);
	tmx->setScaleY(1.5);
	addChild(tmx, 0);

	layer1 = tmx->layerNamed("meta");
	layer2 = tmx->layerNamed("meta2");
	layer3 = tmx->layerNamed("pool");

	fortune = tmx->layerNamed("fortune");

	UIlayer = Layer::create();
	UIlayer->setPosition(0, 0);
	UIlayer->setAnchorPoint(origin);
	addChild(UIlayer, 0);
}

void HelloWorld::addSprite() 
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("baobao/player.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
	player1 = Sprite::createWithSpriteFrame(frame);
	player1->setPosition(P1InitialX + P1PositionX * waveGridSize, P1InitialY + P1PositionY * waveGridSize);
	this->addChild(player1, 1);

	texture = Director::getInstance()->getTextureCache()->addImage("pidan/player.png");
	frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
	player2 = Sprite::createWithSpriteFrame(frame);
	player2->setPosition(P2InitialX + P2PositionX * waveGridSize, P2InitialY + P2PositionY * waveGridSize);
	this->addChild(player2, 1);
	//for debug
	//auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("upWaveGeneratingAnimation"));
	//auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("upWaveTailGeneratingAnimation"));
	//walkAction->setS
	//player1->runAction(RepeatForever::create(walkAction));
	//bombExplode(10, Vec2(visibleSize.width / 2, visibleSize.height / 2));

	auto ui = Sprite::create("ui/UI.jpg");
	ui->setPosition(Vec2(100, visibleSize.height / 2 + origin.y));
	ui->setScaleX(200 / ui->getTextureRect().getMaxX());
	ui->setScaleY(visibleSize.height / ui->getTextureRect().getMaxY());
	UIlayer->addChild(ui, 0);

	auto box1 = Sprite::create("ui/box.png");
	box1->setPosition(Vec2(100, visibleSize.height / 2 + origin.y + 100));
	UIlayer->addChild(box1, 1);

	auto player1 = Sprite::create("baobao/player.png");
	player1->setPosition(Vec2(70, visibleSize.height / 2 + origin.y + 100));
	player1->setScale(1.5);
	UIlayer->addChild(player1, 2);

	auto bomb1 = Sprite::create("bomb/bomb.png");
	bomb1->setPosition(Vec2(130, visibleSize.height / 2 + origin.y + 140));
	bomb1->setScale(0.8);
	UIlayer->addChild(bomb1, 2);

	P1bombNumLabel = Label::createWithTTF(std::to_string(P1BombMax - P1BombLaid), "fonts/arial.ttf", 20);
	P1bombNumLabel->setPosition(Vec2(155, visibleSize.height / 2 + origin.y + 140));
	P1bombNumLabel->setColor(Color3B::BLACK);
	P1bombNumLabel->enableBold();
	UIlayer->addChild(P1bombNumLabel, 2);

	auto power1 = Sprite::create("ui/power.png");
	power1->setPosition(Vec2(130, visibleSize.height / 2 + origin.y + 110));
	power1->setScale(0.8);
	UIlayer->addChild(power1, 2);

	P1powerLabel = Label::createWithTTF(std::to_string(P1BombWavePower), "fonts/arial.ttf", 20);
	P1powerLabel->setPosition(Vec2(155, visibleSize.height / 2 + origin.y + 110));
	P1powerLabel->setColor(Color3B::BLACK);
	P1powerLabel->enableBold();
	UIlayer->addChild(P1powerLabel, 2);

	auto speed1 = Sprite::create("ui/speed.png");
	speed1->setPosition(Vec2(130, visibleSize.height / 2 + origin.y + 80));
	speed1->setScale(0.8);
	UIlayer->addChild(speed1, 2);

	P1speedLabel = Label::createWithTTF(std::to_string(P1Speed), "fonts/arial.ttf", 20);
	P1speedLabel->setPosition(Vec2(155, visibleSize.height / 2 + origin.y + 80));
	P1speedLabel->setColor(Color3B::BLACK);
	P1speedLabel->enableBold();
	UIlayer->addChild(P1speedLabel, 2);

	auto box2 = Sprite::create("ui/box.png");
	box2->setPosition(Vec2(100, visibleSize.height / 2 + origin.y -50));
	UIlayer->addChild(box2, 1);

	auto player2 = Sprite::create("pidan/player.png");
	player2->setPosition(Vec2(70, visibleSize.height / 2 + origin.y -50));
	player2->setScale(1.5);
	UIlayer->addChild(player2, 2);

	auto bomb2 = Sprite::create("bomb/bomb.png");
	bomb2->setPosition(Vec2(130, visibleSize.height / 2 + origin.y - 10));
	bomb2->setScale(0.8);
	UIlayer->addChild(bomb2, 2);

	P2bombNumLabel = Label::createWithTTF(std::to_string(P2BombMax - P2BombLaid), "fonts/arial.ttf", 20);
	P2bombNumLabel->setPosition(Vec2(155, visibleSize.height / 2 + origin.y - 10));
	P2bombNumLabel->setColor(Color3B::BLACK);
	P2bombNumLabel->enableBold();
	UIlayer->addChild(P2bombNumLabel, 2);

	auto power2 = Sprite::create("ui/power.png");
	power2->setPosition(Vec2(130, visibleSize.height / 2 + origin.y - 40));
	power2->setScale(0.8);
	UIlayer->addChild(power2, 2);

	P2powerLabel = Label::createWithTTF(std::to_string(P2BombWavePower), "fonts/arial.ttf", 20);
	P2powerLabel->setPosition(Vec2(155, visibleSize.height / 2 + origin.y - 40));
	P2powerLabel->setColor(Color3B::BLACK);
	P2powerLabel->enableBold();
	UIlayer->addChild(P2powerLabel, 2);

	auto speed2 = Sprite::create("ui/speed.png");
	speed2->setPosition(Vec2(130, visibleSize.height / 2 + origin.y - 70));
	speed2->setScale(0.8);
	UIlayer->addChild(speed2, 2);

	P2speedLabel = Label::createWithTTF(std::to_string(P2Speed), "fonts/arial.ttf", 20);
	P2speedLabel->setPosition(Vec2(155, visibleSize.height / 2 + origin.y - 70));
	P2speedLabel->setColor(Color3B::BLACK);
	P2speedLabel->enableBold();
	UIlayer->addChild(P2speedLabel, 2);

	Sprite* sp0 = Sprite::create("ui/hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp1 = Sprite::create("ui/hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("ui/hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	pT1 = ProgressTimer::create(sp);
	pT1->setScaleX(36);
	pT1->setScaleY(0.65);
	pT1->setAnchorPoint(Vec2(0, 0));
	pT1->setType(ProgressTimerType::BAR);
	pT1->setBarChangeRate(Point(1, 0));
	pT1->setMidpoint(Point(0, 1));
	pT1->setPercentage(100);
	pT1->setPosition(Vec2(52, visibleSize.height / 2 + origin.y + 47));
	UIlayer->addChild(pT1, 2);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setScale(0.4);
	sp0->setPosition(Vec2(35, visibleSize.height / 2 + origin.y + 45));
	UIlayer->addChild(sp0, 1);

	pT2 = ProgressTimer::create(sp);
	pT2->setScaleX(36);
	pT2->setScaleY(0.65);
	pT2->setAnchorPoint(Vec2(0, 0));
	pT2->setType(ProgressTimerType::BAR);
	pT2->setBarChangeRate(Point(1, 0));
	pT2->setMidpoint(Point(0, 1));
	pT2->setPercentage(100);
	pT2->setPosition(Vec2(52, visibleSize.height / 2 + origin.y - 103));
	UIlayer->addChild(pT2, 2);
	sp1->setAnchorPoint(Vec2(0, 0));
	sp1->setScale(0.4);
	sp1->setPosition(Vec2(35, visibleSize.height / 2 + origin.y - 105));
	UIlayer->addChild(sp1, 1);
}


void HelloWorld::bombExplode(int wavePower, Vec2 position, int posX, int posY)
{

	ExplosionWaveGenerator("up", 0, 1, wavePower, position, posX, posY);
	ExplosionWaveGenerator("down", 0, -1, wavePower, position, posX, posY);
	ExplosionWaveGenerator("left", -1, 0, wavePower, position, posX, posY);
	ExplosionWaveGenerator("right", 1, 0, wavePower, position, posX, posY);
	
	checkAndChangeBlood(posX, posY);
	
	auto centerWave = Sprite::create();
	centerWave->setPosition(position.x + wavePower * waveGridSize * 0, position.y + wavePower * waveGridSize * 0);
	this->addChild(centerWave, 1);

	SimpleAudioEngine::getInstance()->playEffect("sound/explode.wav");
	auto centerSequence = Sequence::create(
		Animate::create(AnimationCache::getInstance()->getAnimation("centerWaveGeneratingAnimation")),
		DelayTime::create(explosionHoldDuration),
		Animate::create(AnimationCache::getInstance()->getAnimation("centerWaveVanishAnimation")),
		CallFunc::create([centerWave, this]()
		{
			centerWave->removeFromParentAndCleanup(true);
		}),
		nullptr
	);
	centerWave->runAction(centerSequence);
	centerWave->setScale(1.2f);
}

void HelloWorld::ExplosionWaveGenerator(string direction, int offsetX, int offsetY, int wavePower, cocos2d::Vec2 position, int posX, int posY)
{
	size_t i;
	bool flag = false;
	for ( i = 0; i < wavePower; i++)
	{
		//判断是否有物体
		if (flag)
			break;
		if (posX + (i + 1) * offsetX < 0 || posX + (i + 1) * offsetX > 15 || posY + (i + 1) * offsetY < 0 || posY + (i + 1) * offsetY > 15)
			break;
		flag = checkObjectAndRemove(posX + (i+1) * offsetX, posY + (i+1) * offsetY);
		if (!flag && !checkCanMove(posX + (i + 1) * offsetX, posY + (i + 1) * offsetY))
			break;
		if (i == 0)
			continue;

		//判断角色是否处于爆炸范围内
		checkAndChangeBlood(posX + i * offsetX, posY + i * offsetY);

		auto wave = Sprite::create();
		wave->setPosition(position.x + i * waveGridSize * offsetX, position.y + i * waveGridSize * offsetY);
		this->addChild(wave, 1);
		auto waveSequence = Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation(direction + "WaveGeneratingAnimation")),
			DelayTime::create(explosionHoldDuration),
			Animate::create(AnimationCache::getInstance()->getAnimation(direction + "WaveVanishAnimation")),
			CallFunc::create([wave, this]()
			{
				wave->removeFromParentAndCleanup(true);
			}),
			nullptr
		);
		wave->runAction(waveSequence);
		wave->setScale(1.20f);
	}
	checkAndChangeBlood(posX + i * offsetX, posY + i * offsetY);

	auto waveTail = Sprite::create();
	waveTail->setPosition(position.x + i * waveGridSize * offsetX, position.y + i * waveGridSize * offsetY);
	this->addChild(waveTail, 1);
	auto waveSequence = Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation(direction + "WaveTailGeneratingAnimation")),
		DelayTime::create(explosionHoldDuration),
		CallFunc::create([i, flag, position, offsetX, offsetY, posX, posY, this]()
		{
			checkAndChangeBlood(posX + i * offsetX, posY + i * offsetY);
			if (flag)
			{
				int randomNumber = rand() % 100;
				if (randomNumber <= 9)
				{
					auto pro = Sprite::create("res/item_25.png");
					this->PropertyMatrix[posX + i * offsetX][posY + i * offsetY] = pro;
					pro->setPosition(position.x + i * waveGridSize * offsetX, position.y + i * waveGridSize * offsetY);
					pro->setTag(1);
					this->addChild(pro, 0);
				}
				else if (randomNumber <= 26)
				{
					auto pro = Sprite::create("res/power.png");
					this->PropertyMatrix[posX + i * offsetX][posY + i * offsetY] = pro;
					pro->setPosition(position.x + i * waveGridSize * offsetX, position.y + i * waveGridSize * offsetY);
					pro->setTag(2);
					this->addChild(pro, 0);
				}
				else if (randomNumber <= 43)
				{
					auto pro = Sprite::create("res/speed.png");
					this->PropertyMatrix[posX + i * offsetX][posY + i * offsetY] = pro;
					pro->setPosition(position.x + i * waveGridSize * offsetX, position.y + i * waveGridSize * offsetY);
					pro->setTag(3);
					this->addChild(pro, 0);
				}
				else if (randomNumber <= 60)
				{
					auto pro = Sprite::create("res/item_61.png");
					this->PropertyMatrix[posX + i * offsetX][posY + i * offsetY] = pro;
					pro->setPosition(position.x + i * waveGridSize * offsetX, position.y + i * waveGridSize * offsetY);
					pro->setTag(4);
					this->addChild(pro, 0);
				}
			}
		}),
		Animate::create(AnimationCache::getInstance()->getAnimation(direction + "WaveTailVanishAnimation")),
		CallFunc::create([waveTail, this]()
		{
			waveTail->removeFromParentAndCleanup(true);
		}),
		nullptr
	);
	waveTail->runAction(waveSequence);
	waveTail->setScale(1.20f);
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
	schedule(schedule_selector(HelloWorld::CountCD), 1.0f, kRepeatForever, 0);
}

void HelloWorld::CountCD(float f)
{
	for (int i = 0; i < 5; i++)
	{
		P1SkillCDs[i] = P1SkillCDs[i] > 0 ? P1SkillCDs[i] - 1 : 0;
		P2SkillCDs[i] = P2SkillCDs[i] > 0 ? P2SkillCDs[i] - 1 : 0;
	}
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

	//决赛圈相关
	fireTimeCount++;
	//决赛圈最多4层
	for (int i = 0; i <= 3; i++)
	{
		if (fireTimeCount == (fireGeneratingStartTime + i * fireGeneratingGap) * 20 )
		{
			fireGenerator(i);
		}
	}
}
//决赛圈
void HelloWorld::fireGenerator(int layer)
{
	//upper row
	for (int i = layer; i <= 15 - layer; i++)
	{
		auto fire = Sprite::create();
		fire->setPosition(Vec2(P1InitialX + waveGridSize * i, P1InitialY + waveGridSize * (15 - layer)));
		this->addChild(fire, 4);
		fire->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("fireAnimation"))));
	}

	//lower row
	for (int i = layer; i < 15 - layer; i++)
	{
		auto fire = Sprite::create();
		fire->setPosition(Vec2(P1InitialX + waveGridSize * i, P1InitialY + waveGridSize * layer));
		this->addChild(fire, 4);
		fire->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("fireAnimation"))));
	}

	//left column
	for (int i = layer; i < 15 - layer; i++)
	{
		auto fire = Sprite::create();
		fire->setPosition(Vec2(P1InitialX + waveGridSize * layer, P1InitialY + waveGridSize * i));
		this->addChild(fire, 4);
		fire->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("fireAnimation"))));
	}

	for (int i = layer; i < 15 - layer; i++)
	{
		auto fire = Sprite::create();
		fire->setPosition(Vec2(P1InitialX + waveGridSize * (15 - layer), P1InitialY + waveGridSize * i));
		this->addChild(fire, 4);
		fire->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("fireAnimation"))));
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
		layBomb(player1);
		break;
	case EventKeyboard::KeyCode::KEY_F:
	case EventKeyboard::KeyCode::KEY_CAPITAL_F:
		UseSkill(player1, P1SkillIndex);
		break;
	case EventKeyboard::KeyCode::KEY_R:
	case EventKeyboard::KeyCode::KEY_CAPITAL_R:
		P1SkillIndex = (P1SkillIndex + 1) % SkillCount;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		P2TryMoving = true;
		KeyArrayPush(P2KeyArray, 1);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		P2TryMoving = true;
		KeyArrayPush(P2KeyArray, 2);
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		P2TryMoving = true;
		KeyArrayPush(P2KeyArray, 3);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		P2TryMoving = true;
		KeyArrayPush(P2KeyArray, 4);
		break;
	case EventKeyboard::KeyCode::KEY_ENTER:
	case EventKeyboard::KeyCode::KEY_KP_ENTER:
		layBomb(player2);
		break;
	case EventKeyboard::KeyCode::KEY_0:
		UseSkill(player2, P2SkillIndex);
		break;
	case EventKeyboard::KeyCode::KEY_2:
		P2SkillIndex = (P2SkillIndex + 1) % SkillCount;
		break;
	}
}

void HelloWorld::layBomb(Sprite* player)
{
	if (player == player1 && P1BombLaid < P1BombMax)
	{
		
		int posX = P1PositionX;
		int posY = P1PositionY;
		if (BombMatrix[posX][posY] != nullptr)
		{
			return;
		}
		else
		{
			P1BombLaid++;
			P1bombNumLabel->setString(std::to_string(P1BombMax - P1BombLaid));
			auto bomb = Sprite::create();
			SimpleAudioEngine::getInstance()->playEffect("sound/lay.wav");
			bomb->setPosition(Vec2(P1PositionX * waveGridSize + P1InitialX, P1PositionY * waveGridSize + P1InitialY));
			bomb->setTag(11);
			this->addChild(bomb, 0);
			auto bombSequence = Sequence::create(
				Repeat::create(Animate::create(AnimationCache::getInstance()->getAnimation("bombAnimation")), 2),
				CallFunc::create([bomb, posX, posY, this]()
			{
				this->BombMatrix[posX][posY] = nullptr;
				bomb->removeFromParentAndCleanup(true);
				bombExplode(P1BombWavePower, bomb->getPosition(), posX, posY);
				BombExploding(posX, posY, P1BombWavePower);
				P1BombLaid--;
				P1bombNumLabel->setString(std::to_string(P1BombMax - P1BombLaid));
			}),
				nullptr
				);

			bomb->runAction(bombSequence);
			BombMatrix[posX][posY] = bomb;
		}
	}
	else if (player == player2 && P2BombLaid < P2BombMax)
	{
		int posX = P2PositionX;
		int posY = P2PositionY;
		if (BombMatrix[posX][posY] != nullptr)
		{
			return;
		}
		else
		{
			P2BombLaid++;
			P2bombNumLabel->setString(std::to_string(P2BombMax - P2BombLaid));
			auto bomb = Sprite::create();
			SimpleAudioEngine::getInstance()->playEffect("sound/lay.wav");
			bomb->setPosition(Vec2(P2PositionX * waveGridSize + P2InitialX, P2PositionY * waveGridSize + P2InitialY));
			bomb->setTag(22);
			this->addChild(bomb, 0);
			auto bombSequence = Sequence::create(
				Repeat::create(Animate::create(AnimationCache::getInstance()->getAnimation("bombAnimation")), 2),
				CallFunc::create([bomb, posX, posY, this]()
			{
				this->BombMatrix[posX][posY] = nullptr;
				bomb->removeFromParentAndCleanup(true);
				P2BombLaid--;
				P2bombNumLabel->setString(std::to_string(P2BombMax - P2BombLaid));
				bombExplode(P2BombWavePower, bomb->getPosition(), posX, posY);
				BombExploding(posX, posY, P2BombWavePower);
			}),
				nullptr
				);

			bomb->runAction(bombSequence);
			BombMatrix[posX][posY] = bomb;
		}
	}
	
}

void HelloWorld::BombExploding(int BombPositionX, int BombPositionY, int power)
{
	if (BombPositionX < 0 || BombPositionX > 15 || BombPositionY < 0 || BombPositionY > 15 || power <= 0)
	{
		return;
	}
	// check Y+ Y- X- X+
	for (int i = 1; i <= power; i++)
	{
		// Y+
		if (BombPositionX < 0 || BombPositionX > 15 || BombPositionY + i < 0 || BombPositionY + i > 15)
		{
			// do nothing
		}
		else
		{
			if (CheckStopBomb(BombPositionX, BombPositionY + i) && BombMatrix[BombPositionX][BombPositionY + i] != nullptr)
			{
				int ttag = this->BombMatrix[BombPositionX][BombPositionY + i]->getTag();
				this->BombMatrix[BombPositionX][BombPositionY + i]->stopAllActions();
				RecoverBombCount(ttag);
				bombExplode(P2BombWavePower, this->BombMatrix[BombPositionX][BombPositionY + i]->getPosition(), BombPositionX, BombPositionY + i);
				this->BombMatrix[BombPositionX][BombPositionY + i]->removeFromParentAndCleanup(true);
				this->BombMatrix[BombPositionX][BombPositionY + i] = nullptr;
				BombExploding(BombPositionX, BombPositionY + i, GetBombPowerByTag(ttag));
			}
		}
		// Y-
		if (BombPositionX < 0 || BombPositionX > 15 || BombPositionY - i < 0 || BombPositionY - i > 15)
		{
			// do nothing
		}
		else
		{
			if (CheckStopBomb(BombPositionX, BombPositionY - i) && BombMatrix[BombPositionX][BombPositionY - i] != nullptr)
			{
				int ttag = this->BombMatrix[BombPositionX][BombPositionY - i]->getTag();
				this->BombMatrix[BombPositionX][BombPositionY - i]->stopAllActions();
				RecoverBombCount(ttag);
				bombExplode(P2BombWavePower, this->BombMatrix[BombPositionX][BombPositionY - i]->getPosition(), BombPositionX, BombPositionY - i);
				this->BombMatrix[BombPositionX][BombPositionY - i]->removeFromParentAndCleanup(true);
				this->BombMatrix[BombPositionX][BombPositionY - i] = nullptr;
				BombExploding(BombPositionX, BombPositionY - i, GetBombPowerByTag(ttag));
			}
		}
		// X-
		if (BombPositionX - i < 0 || BombPositionX - i > 15 || BombPositionY < 0 || BombPositionY > 15)
		{
			// do nothing
		}
		else
		{
			if (CheckStopBomb(BombPositionX - i, BombPositionY) && BombMatrix[BombPositionX - i][BombPositionY] != nullptr)
			{
				int ttag = this->BombMatrix[BombPositionX - i][BombPositionY]->getTag();
				this->BombMatrix[BombPositionX - i][BombPositionY]->stopAllActions();
				RecoverBombCount(ttag);
				bombExplode(P2BombWavePower, this->BombMatrix[BombPositionX - i][BombPositionY]->getPosition(), BombPositionX - i, BombPositionY);
				this->BombMatrix[BombPositionX - i][BombPositionY]->removeFromParentAndCleanup(true);
				this->BombMatrix[BombPositionX - i][BombPositionY] = nullptr;
				BombExploding(BombPositionX - i, BombPositionY, GetBombPowerByTag(ttag));
			}
		}
		// X+
		if (BombPositionX + i < 0 || BombPositionX + i > 15 || BombPositionY < 0 || BombPositionY > 15)
		{
			// do nothing
		}
		else
		{
			if (CheckStopBomb(BombPositionX + i, BombPositionY) && BombMatrix[BombPositionX + i][BombPositionY] != nullptr)
			{
				int ttag = this->BombMatrix[BombPositionX + i][BombPositionY]->getTag();
				this->BombMatrix[BombPositionX + i][BombPositionY]->stopAllActions();
				RecoverBombCount(ttag);
				bombExplode(P2BombWavePower, this->BombMatrix[BombPositionX + i][BombPositionY]->getPosition(), BombPositionX + i, BombPositionY);
				this->BombMatrix[BombPositionX + i][BombPositionY]->removeFromParentAndCleanup(true);
				this->BombMatrix[BombPositionX + i][BombPositionY] = nullptr;
				BombExploding(BombPositionX + i, BombPositionY, GetBombPowerByTag(ttag));
			}
		}
	}
}

void HelloWorld::RecoverBombCount(int PlayerTag)
{
	if (PlayerTag == 11)
	{
		P1BombLaid--;
	}
	else if (PlayerTag == 22)
	{
		P2BombLaid--;
	}
}

int HelloWorld::GetBombPowerByTag(int PlayerTag)
{
	if (PlayerTag == 11)
	{
		return P1BombWavePower;
	}
	else if (PlayerTag == 22)
	{
		return P2BombWavePower;
	}
	return 0;
}

bool HelloWorld::CheckStopBomb(int posX, int posY)
{
	int MapX = posX;
	int MapY = 15 - posY;
	if (layer1->tileAt(ccp(MapX, MapY)))
	{
		return false;
	}
	else if (layer2->tileAt(ccp(MapX, MapY)))
	{
		return false;
	}
	else if (layer3->tileAt(ccp(MapX, MapY)))
	{
		return false;
	}
	else if (fortune->tileAt(ccp(MapX, MapY)))
	{
		return false;
	}
	return true;
}

bool HelloWorld::checkCanMove(int posX, int posY)
{
	int MapX = posX;
	int MapY = 15 - posY;
	if (layer1->tileAt(ccp(MapX, MapY)))
	{
		return false;
	}
	else if (layer2->tileAt(ccp(MapX, MapY)))
	{
		return false;
	}
	else if (layer3->tileAt(ccp(MapX, MapY)))
	{
		return false;
	}
	else if (fortune->tileAt(ccp(MapX, MapY)))
	{
		return false;
	}
	else if (BombMatrix[posX][posY] != nullptr)
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
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		KeyArrayPop(P2KeyArray, 1);
		P2TryMoving = P2KeyArray[0] != 0;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		KeyArrayPop(P2KeyArray, 2);
		P2TryMoving = P2KeyArray[0] != 0;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		KeyArrayPop(P2KeyArray, 3);
		P2TryMoving = P2KeyArray[0] != 0;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		KeyArrayPop(P2KeyArray, 4);
		P2TryMoving = P2KeyArray[0] != 0;
		break;	
	}
}

void HelloWorld::UseSkill(Sprite* player, int SkillIndex)
{
	switch (SkillIndex){
	case 0:
		flash(player);
		break;
	case 1:
		recover(player);
		break;
	case 2:
		BombUp(player);
		break;
	case 3:
		PowerUp(player);
		break;
	case 4:
		SpeedUp(player);
		break;
	default:
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
			P1Dir = 1;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkUpAnimation"));
			break;
		case 2:
			y = -1;
			P1Dir = 2;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkDownAnimation"));
			break;
		case 3:
			x = -1;
			P1Dir = 3;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkSidewayAnimation"));
			break;
		case 4:
			x = 1;
			P1Dir = 4;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkSidewayAnimation"));
			break;
		default:
			break;
		}
		walkAction->setDuration(P1WalkDuration);
		if (P1PositionX + x < 16 && P1PositionX + x >= 0 && P1PositionY + y < 16 && P1PositionY + y >= 0 && checkCanMove(P1PositionX + x, P1PositionY + y))
		{
			player1->stopAllActions();
			P1IsMoving = true;
			P1PositionX += x;
			P1PositionY += y;
			auto moveAC = Spawn::createWithTwoActions(MoveTo::create(P1WalkDuration, Vec2(P1InitialX + waveGridSize * P1PositionX, P1InitialY + waveGridSize * P1PositionY)), walkAction);
			player1->setFlippedX(x == -1);
			auto ac = Sequence::create(moveAC, CallFunc::create([this]() {checkAndHandleProperty(1); if (this->P1TryMoving == true) { this->movePlayer(this->player1); } else { this->P1IsMoving = false; } }), nullptr);
			player1->runAction(ac);
		}
		else
		{
			player1->setFlippedX(x == -1);
			player1->runAction(walkAction);
			this->P1IsMoving = false;
		}
	}
	else
	{
		int dir = P2KeyArray[P2KeyArray[0]];
		int x = 0;
		int y = 0;
		Animate* walkAction;
		switch (dir)
		{
		case 1:
			y = 1;
			P2Dir = 1;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player2WalkUpAnimation"));
			break;
		case 2:
			y = -1;
			P2Dir = 2;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player2WalkDownAnimation"));
			break;
		case 3:
			x = -1;
			P2Dir = 3;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player2WalkSidewayAnimation"));
			break;
		case 4:
			x = 1;
			P2Dir = 4;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player2WalkSidewayAnimation"));
			break;
		default:
			break;
		}
		walkAction->setDuration(P2WalkDuration);
		if (P2PositionX + x < 16 && P2PositionX + x >= 0 && P2PositionY + y < 16 && P2PositionY + y >= 0 && checkCanMove(P2PositionX + x, P2PositionY + y))
		{
			player2->stopAllActions();
			P2IsMoving = true;
			P2PositionX += x;
			P2PositionY += y;
			auto moveAC = Spawn::createWithTwoActions(MoveTo::create(P2WalkDuration, Vec2(P2InitialX + waveGridSize * P2PositionX, P2InitialY + waveGridSize * P2PositionY)), walkAction);
			player2->setFlippedX(x == -1);
			auto ac = Sequence::create(moveAC, CallFunc::create([this]() {checkAndHandleProperty(2); if (this->P2TryMoving == true) { this->movePlayer(this->player2); } else { this->P2IsMoving = false; } }), nullptr);
			player2->runAction(ac);
		}
		else
		{
			player2->setFlippedX(x == -1);
			player2->runAction(walkAction);
			this->P2IsMoving = false;
		}
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
						keyArr[j] = keyArr[j+1];
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
	y = 15 - y;
	if (fortune->tileAt(ccp(x, y)))
	{
		fortune->removeTileAt(ccp(x, y));
		return true;
	}
	return false;
}

void HelloWorld::flash(Sprite * player)
{
	player->stopAllActions();
	if (player == player1)
	{
		P1IsMoving = false;
		int x = 0;
		int y = 0;
		switch (P1Dir)
		{
		case 1:
			y = 2;
			break;
		case 2:
			y = -2;
			break;
		case 3:
			x = -2;
			break;
		case 4:
			x = 2;
			break;
		}
		if (P1PositionX + x < 16 && P1PositionX + x >= 0 && P1PositionY + y < 16 && P1PositionY + y >= 0 && checkCanMove(P1PositionX + x, P1PositionY + y))
		{
			auto flashGleam = Sprite::create();
			flashGleam->setPosition(Vec2(P1InitialX + waveGridSize * P1PositionX, P1InitialY + waveGridSize * P1PositionY));
			this->addChild(flashGleam, 3);
			flashGleam->runAction(
				Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("flashAnimation")) ,
				CallFunc::create([flashGleam, this]() {flashGleam->removeFromParentAndCleanup(true); }),
				nullptr));
			P1PositionX += x;
			P1PositionY += y;
			checkAndHandleProperty(1);
			player->setPosition(Vec2(P1InitialX + waveGridSize * P1PositionX, P1InitialY + waveGridSize * P1PositionY));
			SimpleAudioEngine::getInstance()->playEffect("sound/flash.wav");
		}
		else
		{
			
		}
	}
	else
	{
		P2IsMoving = false;
		int x = 0;
		int y = 0;
		switch (P2Dir)
		{
		case 1:
			y = 2;
			break;
		case 2:
			y = -2;
			break;
		case 3:
			x = -2;
			break;
		case 4:
			x = 2;
			break;
		}
		if (P2PositionX + x < 16 && P2PositionX + x >= 0 && P2PositionY + y < 16 && P2PositionY + y >= 0 && checkCanMove(P2PositionX + x, P2PositionY + y))
		{
			auto flashGleam = Sprite::create();
			flashGleam->setPosition(Vec2(P2InitialX + waveGridSize * P2PositionX, P2InitialY + waveGridSize * P2PositionY));
			this->addChild(flashGleam, 3);
			flashGleam->runAction(
				Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("flashAnimation")),
					CallFunc::create([flashGleam, this]() {flashGleam->removeFromParentAndCleanup(true); }),
					nullptr));
			P2PositionX += x;
			P2PositionY += y;
			checkAndHandleProperty(2);
			player->setPosition(Vec2(P2InitialX + waveGridSize * P2PositionX, P2InitialY + waveGridSize * P2PositionY));
			SimpleAudioEngine::getInstance()->playEffect("sound/flash.wav");
		}
		else
		{

		}
	}
}

void HelloWorld::recover(Sprite* player)
{
	if (player == player1)
	{
		if (P1SkillCDs[1] > 0)
		{
			return;
		}
		float f = pT1->getPercentage() + 40 > 100 ? 100 : pT1->getPercentage() + 40;
		auto animate = CCProgressTo::create(0.1, f);
		pT1->runAction(animate);
		P1SkillCDs[1] = 30;
	}
	else
	{
		if (P2SkillCDs[1] > 0)
		{
			return;
		}
		float f = pT2->getPercentage() + 40 > 100 ? 100 : pT2->getPercentage() + 40;
		auto animate = CCProgressTo::create(0.1, f);
		pT2->runAction(animate);
		P2SkillCDs[1] = 30;
	}
}

void HelloWorld::BombUp(Sprite* player)
{
	if (player == player1)
	{
		if (P1SkillCDs[2] > 0)
		{
			return;
		}
		P1BombMax++;
		P1bombNumLabel->setString(std::to_string(P1BombMax));
		P1SkillCDs[2] = 30;
		auto ac = Sequence::create(DelayTime::create(15.0f), CCCallFunc::create([this]() {P1BombMax--;P1bombNumLabel->setString(std::to_string(P1BombMax - P1BombLaid));}), nullptr);
		this->runAction(ac);
	}
	else
	{
		if (P2SkillCDs[2] > 0)
		{
			return;
		}
		P2BombMax++;
		P2bombNumLabel->setString(std::to_string(P2BombMax));
		P1SkillCDs[2] = 30;
		auto ac = Sequence::create(DelayTime::create(15.0f), CCCallFunc::create([this]() {P2BombMax--; P2bombNumLabel->setString(std::to_string(P2BombMax - P2BombLaid)); }), nullptr);
		this->runAction(ac);
	}
}

void HelloWorld::PowerUp(Sprite* player) 
{
	if (player == player1)
	{
		if (P1SkillCDs[3] > 0)
		{
			return;
		}
		P1BombWavePower++;
		P1powerLabel->setString(std::to_string(P1BombWavePower));
		P1SkillCDs[3] = 30;
		auto ac = Sequence::create(DelayTime::create(15.0f), CCCallFunc::create([this]() {P1BombWavePower--; P1powerLabel->setString(std::to_string(P1BombWavePower)); }), nullptr);
		this->runAction(ac);
	}
	else
	{
		if (P2SkillCDs[3] > 0)
		{
			return;
		}
		P2BombWavePower++;
		P2powerLabel->setString(std::to_string(P2BombWavePower));
		P1SkillCDs[3] = 30;
		auto ac = Sequence::create(DelayTime::create(15.0f), CCCallFunc::create([this]() {P2BombWavePower--; P2powerLabel->setString(std::to_string(P2BombWavePower)); }), nullptr);
		this->runAction(ac);
	}
}

void HelloWorld::SpeedUp(Sprite* player)
{
	if (player == player1)
	{
		if (P1SkillCDs[4] > 0)
		{
			return;
		}
		P1Speed++;
		P1speedLabel->setString(std::to_string(P1Speed));
		P1SkillCDs[4] = 30;
		auto ac = Sequence::create(DelayTime::create(15.0f), CCCallFunc::create([this]() {P1Speed--; P1speedLabel->setString(std::to_string(P1Speed)); }), nullptr);
		this->runAction(ac);
	}
	else
	{
		if (P2SkillCDs[4] > 0)
		{
			return;
		}
		P2Speed++;
		P2speedLabel->setString(std::to_string(P2Speed));
		P1SkillCDs[4] = 30;
		auto ac = Sequence::create(DelayTime::create(15.0f), CCCallFunc::create([this]() {P2Speed--; P2speedLabel->setString(std::to_string(P2Speed)); }), nullptr);
		this->runAction(ac);
	}
}

void HelloWorld::checkAndHandleProperty(int playerID)
{
	int i;
	if (playerID == 1)
	{
		if (PropertyMatrix[P1PositionX][P1PositionY] != nullptr)
		{
			SimpleAudioEngine::getInstance()->playEffect("sound/get.wav");
			auto pro = PropertyMatrix[P1PositionX][P1PositionY];
			if (pro->getTag() == 1)
			{
				auto animate = CCProgressTo::create(0.1, pT1->getPercentage() + 10);
				pT1->runAction(animate);
			}
			else if (pro->getTag() == 2)
			{
				if (P1BombWavePower < 10)
					P1BombWavePower++;
				P1powerLabel->setString(std::to_string(P1BombWavePower));
			}
			else if (pro->getTag() == 3)
			{
				if(P1Speed < 10)
					P1Speed++;
				P1speedLabel->setString(std::to_string(P1Speed));
			}
			else if (pro->getTag() == 4)
			{
				P1BombMax++;
				P1bombNumLabel->setString(std::to_string(P1BombMax - P1BombLaid));
			}
			pro->removeFromParentAndCleanup(true);
			PropertyMatrix[P1PositionX][P1PositionY] = nullptr;
		}
	}
	else
	{
		if (PropertyMatrix[P2PositionX][P2PositionY] != nullptr)
		{
			SimpleAudioEngine::getInstance()->playEffect("sound/get.wav");
			auto pro = PropertyMatrix[P2PositionX][P2PositionY];
			if (pro->getTag() == 1)
			{
				auto animate = CCProgressTo::create(0.1, pT2->getPercentage() + 10);
				pT2->runAction(animate);
			}
			else if (pro->getTag() == 2)
			{
				if (P2BombWavePower < 10)
					P2BombWavePower++;
				P2powerLabel->setString(std::to_string(P2BombWavePower));
			}
			else if (pro->getTag() == 3)
			{
				if (P2Speed < 10)
					P2Speed++;
				P2speedLabel->setString(std::to_string(P2Speed));
			}
			else if (pro->getTag() == 4)
			{
				P2BombMax++;
				P2bombNumLabel->setString(std::to_string(P2BombMax - P2BombLaid));
			}
			pro->removeFromParentAndCleanup(true);
			PropertyMatrix[P2PositionX][P2PositionY] = nullptr;
		}
	}
}

void HelloWorld::checkAndChangeBlood(int posX, int posY)
{
	if (posX == P1PositionX && posY== P1PositionY && !P1absoluteDefense)
	{
		this->P1absoluteDefense = true;
		auto bloodSequence = Sequence::create(CCProgressTo::create(absoluteDefenseTime, pT1->getPercentage() - 20),
			CallFunc::create([this]()
		{
			this->P1absoluteDefense = false;
		}),
			nullptr
		);
		pT1->runAction(bloodSequence);
	}
	if (posX == P2PositionX && posY  == P2PositionY && !P2absoluteDefense)
	{
		this->P2absoluteDefense = true;
		auto bloodSequence = Sequence::create(CCProgressTo::create(absoluteDefenseTime, pT2->getPercentage() - 20),
			CallFunc::create([this]()
		{
			this->P2absoluteDefense = false;
		}),
			nullptr
		);
		pT2->runAction(bloodSequence);
	}
}
