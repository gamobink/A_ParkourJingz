#include "PlayerInputController.h"
#include "Player.h"

USING_NS_CC;

#define TURN_LEFT 1
#define TURN_RIGHT 2

#define MIDDLE_LINE_POS_X 0

PlayerInputController::PlayerInputController(Player * player) :_pPlayer(nullptr)
{
	this->_pPlayer = player;
	CC_SAFE_RETAIN(this->_pPlayer);
}

PlayerInputController::~PlayerInputController()
{
	CC_SAFE_RELEASE_NULL(this->_pPlayer);
}

PlayerInputController * PlayerInputController::create(Player * player)
{
	PlayerInputController *pRet = new(std::nothrow) PlayerInputController(player);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}

}

void PlayerInputController::reveiveTouchBegin(Vec2 pos, Node * pRenderNode)
{
	this->_touchBeginPos = std::move(pos);
}

void PlayerInputController::reveiveTouchEnd(Vec2 pos, Node * pRenderNode)
{
	this->_touchEndPos = std::move(pos);

	Vec2 diff = _touchEndPos - _touchBeginPos;

	diff.normalize();

	auto temp = Vec2(1, 0);//horizontal line

	auto result = Vec2::dot(diff, temp);

	//ת����������ֵ��ƫ�ƾ��������ڷ�ֵ���϶�Ϊ��Ч�������ƶ�
	const float Threshold = std::sqrt(2) / 2;//����պ��ƶ������ˮƽ��λ����45�Ƚǣ�-135�㣩ʱ�����ֵΪsqrt(2)/2����ƽ���ı��ε����

	if (std::abs(result) > Threshold)
	{
		if (result > 0) // right
		{
			//�����ƶ� �� ������ִ�������ƶ����� Gets an action from the running action list by its tag.
			if (_pPlayer->getCurPlayerSprite()->getPositionX() <= MIDDLE_LINE_POS_X
				&& !_pPlayer->getCurPlayerSprite()->getActionByTag(TURN_LEFT)
				&& !_pPlayer->getCurPlayerSprite()->getActionByTag(TURN_RIGHT)
				)
			{
				auto action = MoveBy::create(0.2f, Vec3(10, 0, 0));
				action->setTag(TURN_RIGHT);
				this->_pPlayer->getCurPlayerSprite()->runAction(action);
			}
		}
		else //left
		{
			//�����ƶ� �� ������ִ�������ƶ����� Gets an action from the running action list by its tag.
			if (_pPlayer->getCurPlayerSprite()->getPositionX() >= MIDDLE_LINE_POS_X
				&& !_pPlayer->getCurPlayerSprite()->getActionByTag(TURN_LEFT)
				&& !_pPlayer->getCurPlayerSprite()->getActionByTag(TURN_RIGHT)
				)
			{
				auto action = MoveBy::create(0.2f, Vec3(-10, 0, 0));
				action->setTag(TURN_LEFT);
				this->_pPlayer->getCurPlayerSprite()->runAction(action);
			}
		}
	}
	else //up or down
	{

#define MOVE_FORWARD(POS_X) \
		auto cameraMask = _pPlayer->getCurPlayerSprite()->getCameraMask();\
		\
		std::vector<Camera*> cameras = pRenderNode->getScene()->getCameras();\
		\
		auto func = [cameraMask](decltype(*cameras.begin()) targetCamera) ->bool\
			{\
				return ((unsigned short)targetCamera->getCameraFlag() & cameraMask) != 0;\
			};\
		\
		auto it = std::find_if(cameras.begin(), cameras.end(), func);\
		\
		while (it != cameras.end())\
		{\
			auto temp = (*it)->getPosition3D();\
			temp.add(Vec3(0, 0, POS_X));\
			(*it)->setPosition3D(temp);\
			std::find_if(++it, cameras.end(), func);\
		}\
		_pPlayer->getCurPlayerSprite()->setPositionZ(_pPlayer->getCurPlayerSprite()->getPositionZ()+POS_X);


		if (diff.y > 0)
		{
			MOVE_FORWARD(-5);
		}
		else
		{
			MOVE_FORWARD(5);
		}
	}
}