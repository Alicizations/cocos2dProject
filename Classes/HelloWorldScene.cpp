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
	P2PositionY = 14;
	P1InitialX = 215;
	P1InitialY = 20;
	P2InitialX = 0;
	P2InitialY = 0;
	P1Dir = 3;
	P2Dir = 3;
	// bomb matrix
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			BombMatrix[i][j] = nullptr;
		}
	}
}

void HelloWorld::loadAnimation()
{
	loadPlayerAnimationHelper("baobao", "player1");

	loadWaveAnimationHelper();

}

void HelloWorld::loadPlayerAnimationHelper(string role, string player)
{
	loadFrameHelper(role + "/walkUp", player + "WalkUpAnimation", 4, walkDuration / 5.0f);
	loadFrameHelper(role + "/walkDown", player + "WalkDownAnimation", 4, walkDuration / 5.0f);
	loadFrameHelper(role + "/walkSideway", player + "WalkSidewayAnimation", 5, walkDuration / 5.0f);
	loadFrameHelper(role + "/die", player + "DieAnimation", 4, dieDuration / 4.0f);
	loadFrameHelper(role + "/win", player + "WinAnimation", 3, winDuration / 3.0f);
}

void HelloWorld::loadWaveAnimationHelper()
{
	loadFrameHelper("bomb/bomb", "bombAnimation", 3, 1.0f / 3.0f);
	loadFrameHelper("bomb/explosion", "explosionVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/upWave", "upWaveVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/upWaveTail", "upWaveTailVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/downWave", "downWaveVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/downWaveTail", "downWaveTailVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/leftWave", "leftWaveVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/leftWaveTail", "leftWaveTailVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/rightWave", "rightWaveVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameHelper("bomb/rightWaveTail", "rightWaveTailVanishAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));

	loadFrameReverselyHelper("bomb/explosion", "explosionGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/upWave", "upWaveGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/upWaveTail", "upWaveTailGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/downWave", "downWaveGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/downWaveTail", "downWaveTailGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/leftWave", "leftWaveGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/leftWaveTail", "leftWaveTailGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/rightWave", "rightWaveGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
	loadFrameReverselyHelper("bomb/rightWaveTail", "rightWaveTailGeneratingAnimation", 4, (explosionDuration - explosionHoldDuration) / (2 * 4.0f));
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

	auto box = Sprite::create("ui/box.png");
	box->setPosition(Vec2(100, visibleSize.height / 2 + origin.y + 100));
	UIlayer->addChild(box, 1);

	auto player = Sprite::create("baobao/player.png");
	player->setPosition(Vec2(70, visibleSize.height / 2 + origin.y + 100));
	player->setScale(1.5);
	UIlayer->addChild(player, 2);

	auto bomb = Sprite::create("bomb/bomb.png");
	bomb->setPosition(Vec2(130, visibleSize.height / 2 + origin.y + 150));
	bomb->setScale(0.8);
	UIlayer->addChild(bomb, 2);

	auto bombNum = Label::createWithTTF("1", "fonts/arial.ttf", 20);
	bombNum->setPosition(Vec2(155, visibleSize.height / 2 + origin.y + 150));
	bombNum->setColor(Color3B::BLACK);
	bombNum->enableBold();
	UIlayer->addChild(bombNum, 2);
}


void HelloWorld::bombExplode(int wavePower, Vec2 position, int posX, int posY)
{

	ExplosionWaveGenerator("up", 0, 1, wavePower, position, posX, posY);
	ExplosionWaveGenerator("down", 0, -1, wavePower, position, posX, posY);
	ExplosionWaveGenerator("left", -1, 0, wavePower, position, posX, posY);
	ExplosionWaveGenerator("right", 1, 0, wavePower, position, posX, posY);
	
	
	auto centerWave = Sprite::create();
	centerWave->setPosition(position.x + wavePower * waveGridSize * 0, position.y + wavePower * waveGridSize * 0);
	this->addChild(centerWave, 1);
	
	auto centerSequence = Sequence::create(
		Animate::create(AnimationCache::getInstance()->getAnimation("upWaveGeneratingAnimation")),
		DelayTime::create(explosionHoldDuration),
		Animate::create(AnimationCache::getInstance()->getAnimation("upWaveVanishAnimation")),
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
		if (flag)
			break;
		//判断是否有物体
		if (posX + (i + 1) * offsetX < 0 || posX + (i + 1) * offsetX > 15 || posY + (i + 1) * offsetY < 0 || posY + (i + 1) * offsetY > 15)
			break;
		flag = checkObjectAndRemove(posX + (i+1) * offsetX, posY + (i+1) * offsetY);
		if (!flag && !checkCanMove(posX + (i + 1) * offsetX, posY + (i + 1) * offsetY))
			break;
		if (i == 0)
			continue;


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
	auto waveTail = Sprite::create();
	waveTail->setPosition(position.x + i * waveGridSize * offsetX, position.y + i * waveGridSize * offsetY);
	this->addChild(waveTail, 1);
	auto waveSequence = Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation(direction + "WaveTailGeneratingAnimation")),
		DelayTime::create(explosionHoldDuration),
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
		layBomb();
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		break;
	}
}

void HelloWorld::layBomb()
{
	int posX = P1PositionX;
	int posY = P1PositionY;
	if (BombMatrix[posX][posY] != nullptr)
	{
		return;
	}
	else
	{
		auto bomb = Sprite::create();
		bomb->setPosition(Vec2(P1PositionX * waveGridSize + P1InitialX, P1PositionY * waveGridSize + P1InitialY));
		this->addChild(bomb, 0);
		auto bombSequence = Sequence::create(
			Repeat::create(Animate::create(AnimationCache::getInstance()->getAnimation("bombAnimation")), 2),
			CallFunc::create([bomb, posX, posY, this]()
			{
				this->BombMatrix[posX][posY] = nullptr;
				bomb->removeFromParentAndCleanup(true);
				bombExplode(3.0, bomb->getPosition(), posX, posY);
			}),
			nullptr
			);

		bomb->runAction(bombSequence);
		BombMatrix[posX][posY] = bomb;
	}
}

bool HelloWorld::checkCanMove(int x, int y)
{
	y = 15 - y;
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
	else if (fortune->tileAt(ccp(x, y)))
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
	case EventKeyboard::KeyCode::KEY_F:
	case EventKeyboard::KeyCode::KEY_CAPITAL_F:
		flash(player1);
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
			P1Dir = 1;
			P2Dir = 1;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkUpAnimation"));
			break;
		case 2:
			y = -1;
			P1Dir = 2;
			P2Dir = 2;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkDownAnimation"));
			break;
		case 3:
			x = -1;
			P1Dir = 3;
			P2Dir = 3;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkSidewayAnimation"));
			break;
		case 4:
			x = 1;
			P1Dir = 4;
			P2Dir = 4;
			walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WalkSidewayAnimation"));
			break;
		default:
			break;
		}
		if (P1PositionX + x < 16 && P1PositionX + x >= 0 && P1PositionY + y < 16 && P1PositionY + y >= 0 && checkCanMove(P1PositionX + x, P1PositionY + y))
		{
			player1->stopAllActions();
			P1IsMoving = true;
			P1PositionX += x;
			P1PositionY += y;
			auto moveAC = Spawn::createWithTwoActions(MoveTo::create(walkDuration, Vec2(P1InitialX + waveGridSize * P1PositionX, P1InitialY + waveGridSize * P1PositionY)), walkAction);
			player1->setFlippedX(x == -1);
			auto ac = Sequence::create(moveAC, CallFunc::create([this]() { if (this->P1TryMoving == true) { this->movePlayer(this->player1); } else { this->P1IsMoving = false; } }), nullptr);
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
			P1PositionX += x;
			P1PositionY += y;
			player->setPosition(Vec2(P1InitialX + waveGridSize * P1PositionX, P1InitialY + waveGridSize * P1PositionY));
		}
		else
		{
			
		}
	}
	else
	{
		P2IsMoving = false;
	}
}
