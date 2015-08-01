#include "GameLayer.h"
#include "Player.h"

USING_NS_CC;

GameLayer::GameLayer() 
	:_pPlayer(nullptr)
{
}

GameLayer::~GameLayer()
{
	CC_SAFE_RELEASE_NULL(_pPlayer);
}

bool GameLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	CC_SAFE_RELEASE_NULL(_pPlayer);
	this->_pPlayer = Player::create();
	CC_SAFE_RETAIN(_pPlayer);

	this->addChild(_pPlayer->getCurSprite(), 20);//todo ��ʲô�ã�

	_pPlayer->getCurSprite()->setPosition3D(Vec3(0, 0, -30));//���λ�ÿ�����Ҫ���������ط�ȥ�趨

	scheduleUpdate();
	return true;
}

void GameLayer::update(float delta)
{

}


Player * GameLayer::getPlayer(void)
{
	return _pPlayer;
}

void GameLayer::setPlayer(Player * pPlayer)
{
	CC_SAFE_RETAIN(pPlayer);
	CC_SAFE_RELEASE_NULL(_pPlayer);
	this->_pPlayer = pPlayer;
}