/*

����ʵ�ֽ�ɫ��������Ķ����������ƶ�������Ч���ܻ��Ȳ�����

*/

#pragma once

#include "cocos2d.h"

class Player;
class MapSceneController;
class PlayerInputController;

class GameMainController:public cocos2d::Ref
{
	GameMainController();
public:
	
	~GameMainController();

	bool init(Player * pPlayer, cocos2d::Layer * pGameLayer);

	static GameMainController* create(Player * pPlayer, cocos2d::Layer * pGameLayer)
	{
		GameMainController *pRet = new(std::nothrow) GameMainController();
		if (pRet && pRet->init(pPlayer, pGameLayer))
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

	bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

	void onAfterDraw(cocos2d::EventCustom * pEvent);

private:
	MapSceneController * _pMapSceneController;
	PlayerInputController * _pInputController;
	cocos2d::Layer * _pGameLayer;
	cocos2d::EventListenerCustom * _pEventListenerAfterDraw;
};
