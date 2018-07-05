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
	waveGridSize = 32;
	explosionDuration = 1.0f;
	explosionHoldDuration = 0.5f;
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

}

void HelloWorld::addSprite() 
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("baobao/player.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh())));
	player1 = Sprite::createWithSpriteFrame(frame);
	player1->setPosition(visibleSize.width/2, visibleSize.height/2);
	//this->addChild(player1, 1);
	//for debug
	auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("player1WinAnimation"));
	//auto walkAction = Animate::create(AnimationCache::getInstance()->getAnimation("upWaveTailGeneratingAnimation"));
	//walkAction->setS
	//player1->runAction(RepeatForever::create(walkAction));
	bombExplode(10, Vec2(visibleSize.width / 2, visibleSize.height / 2));

}

void HelloWorld::bombExplode(int wavePower, Vec2 position)
{

	ExplosionWaveGenerator("up", 0, 1, wavePower, position);
	ExplosionWaveGenerator("down", 0, -1, wavePower, position);
	ExplosionWaveGenerator("left", -1, 0, wavePower, position);
	ExplosionWaveGenerator("right", 1, 0, wavePower, position);

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
}

void HelloWorld::ExplosionWaveGenerator(string direction, int offsetX, int offsetY, int wavePower, cocos2d::Vec2 position)
{
	for (size_t i = 1; i < wavePower; i++)
	{
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
		//wave->setScale(1.0f);
	}
	auto waveTail = Sprite::create();
	waveTail->setPosition(position.x + wavePower * waveGridSize * offsetX, position.y + wavePower * waveGridSize * offsetY);
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
	//waveTail->setScale(0.8f);
}

void HelloWorld::addEventListener()
{

}

void HelloWorld::addScheduler()
{

}
