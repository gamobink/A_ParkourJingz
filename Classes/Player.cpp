#include "Player.h"
#include "MoveAbleElemManager.h"

USING_NS_CC;

Player::Player()
{
	_vMoveDir = Vec3(0, 0, -1.0f), _fMoveSpeed = 1.0f*60.0f;
	_iElemTypeId = 4;
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

	//模型资源中一般和骨骼动作通道一起导出，第二个参数是导出的动画通道，可以我不清楚原资源的通道内容
	//创建动作对象
	auto animation = Animation3D::create(pathName);
	auto animate = Animate3D::create(animation);
	_pSprite->runAction(RepeatForever::create(animate));

	_bIsSpriteInit = true;

	return true;
}

int Player::registerSelf(void)
{
	MoveAbleElemManager::getInstance()->registerElemAndFactory(4, MoveAbleElemFactory<Player>::create);
	return 0;
}

//static int a = Player::registerSelf();//注册，同时完成预生成