#pragma once
#include "cocos2d.h"

//todo û��Ҫ������ĳ��Player������ʹ����������Ӳ����������ͬ���Ŀ����ԣ�ÿ�ε��ô���Player���󼴿�
class Player;

class PlayerController:public cocos2d::Ref
{
	PlayerController(Player * pPlayer);
public:
	~PlayerController();

	static PlayerController * create(Player * pPlayer);
	
	bool init();

	bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

	void beganMovePlayer(void);

	void stopMovePlayer(void);

	void update(float delta);

	void stopGame(void);

	void startGame(void);

private:
	CC_DISALLOW_COPY_AND_ASSIGN(PlayerController);

private:
	cocos2d::EventListenerTouchOneByOne * _pTouchListenerOneByOne;
	Player * _pPlayer;
	cocos2d::Vec2 _touchBeginPos;
	cocos2d::Vec2 _touchEndPos;

	float _fStepLength;
	float _fElapsed;
	float _fPerTime;
	bool _bMovePlayerMode;
};


