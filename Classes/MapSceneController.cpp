#include "MapSceneController.h"
#include "Player.h"

#define ROAD_SPRITE_MODEL_WIDTH 470.0f

USING_NS_CC;

//��������һ�γ��ֶ�ƴ�У���ȷ������Դ���⣬����ƴ������
//��Դ�������ϣ�����ƴ�ӵ������⣬����Ӱ������߼�

MapSceneController::MapSceneController() 
	:_pGameLayer(nullptr), _pPlayer(nullptr), _pEventListenerAfterUpdate(nullptr)
	, _fStepLength(1.0f), _fElapsed(0.0f), _fPerTime(1/60.0f)
	, _bMoveSceneMode(false)
{
	
}

MapSceneController::~MapSceneController()
{

	CC_SAFE_RELEASE_NULL(_pPlayer);
	CC_SAFE_RELEASE_NULL(_pGameLayer);
	
	Director::getInstance()->getEventDispatcher()->removeEventListener(_pEventListenerAfterUpdate);
	CC_SAFE_RELEASE_NULL(_pEventListenerAfterUpdate);

	_vpRoadSprites.clear();
}

bool MapSceneController::init(Player * pPlayer, cocos2d::Layer * pGameLayer)
{
	CC_SAFE_RETAIN(pPlayer);
	CC_SAFE_RETAIN(pGameLayer);

	if (_pPlayer!=nullptr)
	{
		CC_SAFE_RELEASE(_pPlayer);
	}

	if (_pGameLayer != nullptr)
	{
		CC_SAFE_RELEASE(_pGameLayer);
	}

	_pPlayer = pPlayer;
	_pGameLayer = pGameLayer;

	preGenerate();

	CC_SAFE_RELEASE_NULL(_pEventListenerAfterUpdate);

	//std::bind(MapSceneController::afferUpdateCallback, this, std::placeholders::_1) == CC_CALLBACK_1(MapSceneController::afferUpdateCallback, this)
	_pEventListenerAfterUpdate = Director::getInstance()->getEventDispatcher()->addCustomEventListener
		(
		Director::EVENT_AFTER_UPDATE, std::bind(&MapSceneController::afferUpdateCallback, this, std::placeholders::_1)
		);
	
	CC_SAFE_RETAIN(_pEventListenerAfterUpdate);

	_fStepLength = 5;

	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);

	return true;
}

void MapSceneController::preGenerate(void){
	//Ϊʲô��4��ʵ�����������ģ�û��� ����պ�����һ�����⣬doubleת��Ϊfloat���½ض�����
	//todo 
	for (int i = 0; i < 4; ++i)
	{
		auto pRoadSprite = Sprite3D::create("model/scene.c3b");
		pRoadSprite->setScale(0.2f);
		pRoadSprite->setRotation3D(Vec3(0, 90, 0));
		pRoadSprite->setPosition3D(Vec3(0, -5, -(ROAD_SPRITE_MODEL_WIDTH * i)));
		_pGameLayer->addChild(pRoadSprite, -100);

		_vpRoadSprites.pushBack(pRoadSprite);
	}
};

void MapSceneController::afferUpdateCallback(EventCustom * pEventCustom)
{
	//ÿ��update�꣬����Ĺ̶�λ�ƶ�������ɣ����ʱ���жϾ��鴦�ڵ�ͼ����һ��
	linkRoads();
}

void MapSceneController::linkRoads(void)
{
	//Ŀǰ��ɫ�ƶ�ֻ��ǰ���������������ʱֻ��Ҫ����z�������ʵ���߼�ʵ��
	auto pPlayer = _pPlayer;

	auto index = _vpRoadSprites.size();

	for each (auto pSprite in _vpRoadSprites)
	{
		auto pos = pSprite->getAABB().getCenter();
		pos.set(pos.x, pos.y, pPlayer->getCurPlayerSprite()->getPositionZ());
		if (pSprite->getAABB().containPoint(pos))
		{
			index = _vpRoadSprites.getIndex(pSprite);
			break;
		}
	}
	
	if (index >= _vpRoadSprites.size()/2)
	{
		auto size = _vpRoadSprites.size();
		for (decltype(index) i = 0; i < index; ++i)
		{
			auto pSprite = _vpRoadSprites.at(0);
			pSprite->retain();
			pSprite->setPosition3D(pSprite->getPosition3D() + Vec3(0, 0, -(ROAD_SPRITE_MODEL_WIDTH * static_cast<float>(size))));
			_vpRoadSprites.eraseObject(pSprite);
			_vpRoadSprites.pushBack(pSprite);
			pSprite->release();
		}
	}
	
}

void MapSceneController::update(float delta)
{
	if (_bMoveSceneMode)
	{
		_fElapsed += delta;
		if (_fElapsed >= _fPerTime)
		{
			moveScene(Vec3(0, 0, _fStepLength));
			_fElapsed = 0.0f;
		}
	}
	
}

void MapSceneController::moveScene(const Vec3 &pos)
{

	for each (auto pSprite in _vpRoadSprites)
	{
		pSprite->setPosition3D(pSprite->getPosition3D() + pos);
	}
	
}

void MapSceneController::beganMoveScene(void)
{
	_bMoveSceneMode = true;
}

void MapSceneController::stopMoveScene(void)
{
	_bMoveSceneMode = false;
}

