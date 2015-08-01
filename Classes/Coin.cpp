#include "Coin.h"
#include "MoveAbleElemTypeDefines.h"
#include "RegitsteredEvents.h"

USING_NS_CC;

#define COMMON_COIN 10;

Coin::Coin()
{
	_iElemTypeId = COIN_TYPE_ID;
	_iGold = COMMON_COIN;
}

Coin::~Coin()
{
}

bool Coin::init(const std::string &pathName)
{
	CC_SAFE_RELEASE_NULL(_pSprite);

	this->_pSprite = Sprite3D::create(pathName);
	//��x����ת-90��Ϊ��ʼ���鳯��
	this->_pSprite->setRotation3D(Vec3(-90, 0, 0));
	this->_pSprite->setScale(0.3f);
	CC_SAFE_RETAIN(_pSprite);

	
	CC_SAFE_RELEASE_NULL(_pAction);
	//����Ϊ��y����ת90��
	_pAction = RepeatForever::create(RotateBy::create(1.0f, Vec3(0, 270, 0)));
	CC_SAFE_RETAIN(_pAction);
	_pSprite->runAction(_pAction);

	this->setMoveSpeed(0.0f);
	_bIsSpriteInit = true;
	return true;
}

void Coin::update(float dt)
{

}

void Coin::beHitted(MoveAbleElem * pMoveAbleElem)
{
	//todo ��Ǯ
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(RegitsteredEvents::GOLD_CHANGED, new int(_iGold));
}