#include "Obstacle.h"
#include "MoveAbleElemTypeDefines.h"

USING_NS_CC;

Obstacle::Obstacle()
{
	_iElemTypeId = OBSTACLE__ID;
}

Obstacle::~Obstacle()
{
}

bool Obstacle::init(const std::string &szModelPath, const std::string &szTexturePath)
{

	
	CC_SAFE_RELEASE_NULL(_pSprite);
	_pSprite = Sprite3D::create(szModelPath, szTexturePath);
	CC_SAFE_RETAIN(_pSprite);
	_pSprite->setLocalZOrder(100);//���� //todo 
	//_pSprite->setRotation3D(Vec3(-90, 0, 90));
	_pSprite->setScale(0.1f);

	this->setMoveSpeed(1.0f*60.0f);

	_bIsSpriteInit = true;

	return true;
}

void Obstacle::update(float dt)
{
	auto moveStep = this->getMoveSpeed() * dt * this->getMoveDirNormal();
	_pSprite->setPosition3D(_pSprite->getPosition3D() + moveStep);
}

void Obstacle::beHitted(MoveAbleElem * pMoveAbleElem)
{
	//todo
	//������ӵ����ͣ��򲥷�������Ч

	//�������ң��򲻴���
}

void Obstacle::recycleSelf(void)
{
	_pSprite->setLocalZOrder(100);//���� //todo 
	//_pSprite->setRotation3D(Vec3(-90, 0, 90));
	_pSprite->setScale(0.1f);

	this->setMoveSpeed(1.0f*60.0f);

	_bIsSpriteInit = true;

	MoveAbleElem::recycleSelf();
}