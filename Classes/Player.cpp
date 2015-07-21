#include "Player.h"

USING_NS_CC;

Player * Player::s_pPlayer = nullptr;

Player::Player() :_pSprite(nullptr)
{
}

Player::~Player()
{
	CC_SAFE_RELEASE_NULL(_pSprite);
}

bool Player::init(const std::string &pathName)
{
	if (_pSprite != nullptr)
	{
		CC_SAFE_RELEASE_NULL(_pSprite);
	}
	_pSprite = Sprite3D::create(pathName);
	_pSprite->setRotation3D(Vec3(0, 180, 0));

	CC_SAFE_RETAIN(_pSprite);

	//ģ����Դ��һ��͹�������ͨ��һ�𵼳����ڶ��������ǵ����Ķ���ͨ���������Ҳ����ԭ��Դ��ͨ������
	//������������
	auto animation = Animation3D::create(pathName);
	auto animate = Animate3D::create(animation);
	_pSprite->runAction(RepeatForever::create(animate));
	return true;
}

Player * Player::getInstance()
{
	if (s_pPlayer == nullptr || s_pPlayer == NULL)
	{
		s_pPlayer = Player::create();
		CC_SAFE_RETAIN(s_pPlayer);
	}
	return s_pPlayer;
}

void Player::destroyInstance()
{
	CC_SAFE_RELEASE_NULL(s_pPlayer);
}

Sprite3D * Player::getCurPlayerSprite()
{
	return _pSprite;
}