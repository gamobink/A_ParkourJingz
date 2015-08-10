#include "NpcController.h"
#include "cocos2d.h"
#include <ctime>
#include "Obstacle.h"
#include "Coin.h"
#include "MapSequences.h"
#include "Player.h"
#include "MoveAbleElemManager.h"
#include "MoveAbleElemTypeDefines.h"

USING_NS_CC;


#define TRACK_0 0
#define TRACK_1 1
#define TRACK_2 2

#ifdef STD_VECTOR_ELEM
#define pushBack push_back
#define popBack pop_back
#endif

NpcController::NpcController() :_pCurMapSequences(nullptr), _pPlayer(nullptr), _pGameLayer(nullptr), _fRecationDt(1.0f)
{
	//��ʱ�亯����ʼ���������
	srand(static_cast<unsigned int>(clock()));
}

NpcController::~NpcController()
{
	Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	if (_pCurMapSequences !=nullptr)
	{
		_pCurMapSequences = nullptr;
	}

	for (auto it = _vSequencesPtr.begin(); it != _vSequencesPtr.end(); ++it)
	{
		delete (*it);
	}
	_vSequencesPtr.clear();

#ifdef STD_VECTOR_ELEM
	vpMoveableElems.clear();
#else
	vpMoveableElems.clear();
#endif


	CC_SAFE_RELEASE_NULL(_pPlayer);

	CC_SAFE_RELEASE_NULL(_pGameLayer);
}
//��ʹ�ã��ٵ����ģ��ṹ
bool NpcController::init(Player * pPlayer, cocos2d::Layer * pGameLayer)
{

	CC_SAFE_RETAIN(pPlayer);
	CC_SAFE_RETAIN(pGameLayer);

	CC_SAFE_RELEASE_NULL(_pPlayer);
	CC_SAFE_RELEASE_NULL(_pGameLayer);

	_pPlayer = pPlayer;
	_pGameLayer = pGameLayer;

	this->insertMapSequence(new MapSequences());

	//todo
	auto temp1 = MoveAbleElemManager::getInstance()->generateOneElem(OBSTACLE__ID);
	_diffObstacleWithPlay = calcuratePosWillHit(temp1, pPlayer, _fRecationDt * 4);
	MoveAbleElemManager::getInstance()->recycleElem(temp1);

	auto temp2 = MoveAbleElemManager::getInstance()->generateOneElem(COIN_TYPE_ID);
	_diffCoinWithPlay = calcuratePosWillHit(temp2, pPlayer, _fRecationDt * 4);
	MoveAbleElemManager::getInstance()->recycleElem(temp2);

	return true;
}

void NpcController::update(float delta)
{
	
	//todo �����ײ�ͷ���ײ����
	for (auto it = vpMoveableElems.begin(); it != vpMoveableElems.end();)
	{
		auto pMoveAbleElem = *it;
		CC_SAFE_RETAIN(pMoveAbleElem);
		auto obb = OBB(pMoveAbleElem->getCurSprite()->getAABB());
		auto obbPlayer = OBB(_pPlayer->getCurSprite()->getAABB());

		if (obb.intersects(obbPlayer))//����ײ
		{
			it = vpMoveableElems.erase(it);
			//ִ����ײ�߼�			 
			pMoveAbleElem->hitOthers(_pPlayer, [&pMoveAbleElem](){
				CC_SAFE_RETAIN(pMoveAbleElem);
				MoveAbleElemManager::getInstance()->recycleElem(pMoveAbleElem);
				CC_SAFE_RELEASE_NULL(pMoveAbleElem);
			});

			
			
		}
		//todo 50ֵ�����Ҳ�֪����λ�ȡģ�͵Ĵ�С
		else if ((*it)->getCurSprite()->getPositionZ() >= _pPlayer->getCurSprite()->getPositionZ() + 50.0f)//����ײ��Զ����ҽ�ɫ
		{
			//����
			auto pMoveAbleElem = *it;
			CC_SAFE_RETAIN(pMoveAbleElem);
			it = vpMoveableElems.erase(it);
			MoveAbleElemManager::getInstance()->recycleElem(pMoveAbleElem);
			CC_SAFE_RELEASE_NULL(pMoveAbleElem);
		}
		else
		{
			++it;
		}
		CC_SAFE_RELEASE_NULL(pMoveAbleElem);
	}

	randomGenElement(_pPlayer, _pGameLayer, delta);


}


void NpcController::randomGenElement(Player * pPlayer, cocos2d::Node * pRenderNode, float dt)
{
	static float fObstacleStepTime = 0.0f;

	fObstacleStepTime += dt;

	//ÿ��0.5������һ���ϰ���,ÿ������1��
	if (fObstacleStepTime > 0.5)
	{
		generateObstacle(pPlayer, pRenderNode, dt);
		fObstacleStepTime = 0.0f;
	}
}

SequenceInfo NpcController::pumpSequence()
{
	if (!_pCurMapSequences || _pCurMapSequences->isEnd())
	{

		unsigned int index = rand() % this->_vSequencesPtr.size();
		_pCurMapSequences = this->_vSequencesPtr[index];

	}
	return _pCurMapSequences->pumpSequence();
}


void NpcController::generateObstacle(Player * pPlayer, cocos2d::Node * pRenderNode, float dt)
{
	auto seq = pumpSequence();

#define POS_LEFT -10
#define POS_MIDDLE 0
#define POS_RIGHT 10

#define GEN_ELEMENTS(ELEM_ID,POS_X) switch(ELEM_ID) \
					{\
	case EMPTY:\
		break;\
		\
	case OBSTACLE__ID:\
						{\
			auto temp = MoveAbleElemManager::getInstance()->generateOneElem(ELEM_ID);\
			vpMoveableElems.pushBack(temp);\
			pRenderNode->addChild(temp->getCurSprite(),100);\
			if(false) \
			{ \
				temp->getCurSprite()->setPosition3D(pPlayer->getCurSprite()->getPosition3D() + _diffObstacleWithPlay );\
			} \
			else \
			{\
				temp->getCurSprite()->setPosition3D( \
					 Vec3(POS_X, 0 + _diffObstacleWithPlay.y, pPlayer->getCurSprite()->getPositionZ() + _diffObstacleWithPlay.z)); \
			}\
		}\
		break;\
	   \
	case COIN_TYPE_ID:\
		{\
			auto temp = MoveAbleElemManager::getInstance()->generateOneElem(ELEM_ID); \
			vpMoveableElems.pushBack(temp);\
			pRenderNode->addChild(temp->getCurSprite(),50); \
			temp->getCurSprite()->setPosition3D( \
					 Vec3(POS_X, 0 + _diffObstacleWithPlay.y, pPlayer->getCurSprite()->getPositionZ() + _diffObstacleWithPlay.z));  \
			temp->getCurSprite()->setRotation3D(Vec3(90.0f, 0.0f, 180.0f));\
		}\
		break;\
	}

	if (seq.oper == OPER_ID::NOW_GEN)
	{
		GEN_ELEMENTS(seq.left, POS_LEFT);
		GEN_ELEMENTS(seq.middle, POS_MIDDLE);
		GEN_ELEMENTS(seq.right, POS_RIGHT);
	}
	else if (seq.oper == OPER_ID::PRE_GEN)
	{
		//todo 70 Ӧ�������ñ����Ŷ�
		if (MoveAbleElemManager::getInstance()->getElemFactory(seq.left)->getReadyElemSize() < 70)
		{
			MoveAbleElemManager::getInstance()->forceToPreCreate(seq.left);
		}
		if (MoveAbleElemManager::getInstance()->getElemFactory(seq.middle)->getReadyElemSize() < 70)
		{
			MoveAbleElemManager::getInstance()->forceToPreCreate(seq.middle);
		}
		if (MoveAbleElemManager::getInstance()->getElemFactory(seq.right)->getReadyElemSize() < 70)
		{
			MoveAbleElemManager::getInstance()->forceToPreCreate(seq.right);
		}
	}
	
}

cocos2d::Vec3 && NpcController::calcuratePosWillHit(MoveAbleElem * pElemSrc, Player * pElemTar, float fDtToHit)
{

#ifndef STD_VECTOR_ELEM
	CC_SAFE_RETAIN(pElemSrc);
#endif

	CC_SAFE_RETAIN(pElemTar);

	auto pSpriteSrc =  pElemSrc->getCurSprite();
	auto pSpriteTar = pElemTar->getCurSprite();

	auto srcNormalDir = pElemSrc->getMoveDirNormal();
	srcNormalDir.normalize();
	
	auto tarNormalDir = pElemTar->getMoveDirNormal();
	tarNormalDir.normalize();

	srcNormalDir = srcNormalDir * pElemSrc->getMoveSpeed();
	tarNormalDir = tarNormalDir * pElemTar->getMoveSpeed();
	auto tartDir = tarNormalDir - srcNormalDir;

	tartDir *= fDtToHit;

	pSpriteSrc->setPosition3D(pSpriteTar->getPosition3D() + tartDir + Vec3(0,-5,0));

	CC_SAFE_RELEASE(pElemTar);

#ifndef STD_VECTOR_ELEM
	CC_SAFE_RELEASE(pElemSrc);
#endif

	return std::move(tartDir);

	
}

void NpcController::stopGame(void)
{
	Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	for (auto it = vpMoveableElems.begin(); it != vpMoveableElems.end();++it)
	{
		auto pMoveAbleElem = *it;
		cocos2d::Director::getInstance()->getScheduler()->unscheduleUpdate(pMoveAbleElem);
		pMoveAbleElem->getCurSprite()->pause();
	}
	
}

void NpcController::startGame(void)
{
	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

#ifdef STD_VECTOR_ELEM
#undef pushBack
#undef popBack pop_back
#endif