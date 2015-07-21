#include "GameLayer.h"
#include "Player.h"

#include "MapSequences.h"

#include "GameMainController.h"

USING_NS_CC;

GameLayer::GameLayer() 
	:_pPlayer(nullptr), _pTouchListenerOneByOne(nullptr), _pGameMainController(nullptr)
{
}

GameLayer::~GameLayer()
{
	if (_pTouchListenerOneByOne != nullptr)
	{
		this->getEventDispatcher()->removeEventListener(_pTouchListenerOneByOne);
		_pTouchListenerOneByOne = nullptr;
	}
	CC_SAFE_RELEASE_NULL(_pPlayer);
	CC_SAFE_RELEASE_NULL(_pGameMainController);
}

bool GameLayer::init()
{


	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->_pPlayer = Player::create();
	CC_SAFE_RETAIN(_pPlayer);

	this->addChild(_pPlayer->getCurSprite(), 20);//todo ��ʲô�ã�

	_pPlayer->getCurSprite()->setPosition3D(Vec3(0, 0, -30));//���λ�ÿ�����Ҫ���������ط�ȥ�趨

	CC_SAFE_RELEASE(_pGameMainController);
	_pGameMainController = GameMainController::create(_pPlayer, this);
	CC_SAFE_RETAIN(_pGameMainController);

	scheduleUpdate();

	return true;
}

void GameLayer::update(float delta)
{

}

void GameLayer::onEnter()
{
	
	if (_pTouchListenerOneByOne != nullptr)
	{
		this->getEventDispatcher()->removeEventListener(_pTouchListenerOneByOne);
		_pTouchListenerOneByOne = nullptr;
	}

	_pTouchListenerOneByOne = EventListenerTouchOneByOne::create();

	_pTouchListenerOneByOne->setSwallowTouches(true);

	_pTouchListenerOneByOne->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	_pTouchListenerOneByOne->onTouchEnded = std::bind(&GameLayer::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);//ʵ��ʹ�õ�Ȼ��cocos��װ�Ľӿڷ��㣬����Ҫ��ϤC++11�ͱ����ʹ��

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_pTouchListenerOneByOne, this);

	CCLayer::onEnter();

}

void GameLayer::onExit()
{
	this->getEventDispatcher()->removeEventListener(_pTouchListenerOneByOne);
	_pTouchListenerOneByOne = nullptr;

	CCLayer::onExit();
}

bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	_pGameMainController->onTouchBegan(touch, unused_event);
	return true;
}

void GameLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	_pGameMainController->onTouchEnded(touch, unused_event);
}