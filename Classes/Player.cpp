#include "Player.h"
#include "MoveAbleElemTypeDefines.h"
#include "RegitsteredEvents.h"

USING_NS_CC;

Player::Player()
{
	_vMoveDir = Vec3(0, 0, -1.0f), _fMoveSpeed = 1.0f*60.0f;
	_iElemTypeId = PLAYER_TYPE_ID;
}

Player::~Player()
{
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

	_bIsSpriteInit = true;

	return true;
}

//�Լ�����Ч��
void Player::beHitted(MoveAbleElem * pMoveAbleElem)
{
	if (pMoveAbleElem->getElemType() == OBSTACLE__ID)
	{
		//TODO �˴�Ӧ�ò�������������ص��ɷ�������Ϣ����ʱ��֪����������
		_pSprite->stopAllActions();
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(RegitsteredEvents::SHOW_RESTART);
	}
}