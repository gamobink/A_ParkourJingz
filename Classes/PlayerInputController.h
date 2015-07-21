#pragma once
#include "cocos2d.h"

//todo û��Ҫ������ĳ��Player������ʹ����������Ӳ����������ͬ���Ŀ����ԣ�ÿ�ε��ô���Player���󼴿�
class Player;

class PlayerInputController:public cocos2d::Ref
{
	PlayerInputController(Player * pPlayer);
public:
	~PlayerInputController();

	bool init() {return true;}

	void reveiveTouchBegin(cocos2d::Vec2 pos, cocos2d::Node * pRenderNode);

	void reveiveTouchEnd(cocos2d::Vec2 pos, cocos2d::Node * pRenderNode);

	static PlayerInputController * create(Player * pPlayer);
private:
	CC_DISALLOW_COPY_AND_ASSIGN(PlayerInputController);

private:
	Player * _pPlayer;
	cocos2d::Vec2 _touchBeginPos;
	cocos2d::Vec2 _touchEndPos;
};


