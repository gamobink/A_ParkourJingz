#pragma once
#include "cocos2d.h"
#include "MoveAbleElem.h"

//�ٶ�Ӧ����Ϊ��ҡ���ҡ���������ԣ������ֻ���������


#ifdef STD_VECTOR_ELEM
class Player :public MoveAbleElem, public cocos2d::Ref
#else
class Player :public MoveAbleElem
#endif
{
#define DEFAULT_MODEL_RES_PATH "model/zhanshi_pao.c3b"
	Player();
public:
	~Player();
	bool init(const std::string &pathName = DEFAULT_MODEL_RES_PATH);

	static Player* createWithAutoRealse(const std::string &szModelPath = DEFAULT_MODEL_RES_PATH)
	{
		Player *pRet = new(std::nothrow) Player();
		if (pRet)
		{
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
	

	static Player* create(const std::string &szModelPath = DEFAULT_MODEL_RES_PATH)
	{
		Player *pRet = new(std::nothrow) Player();
		if (pRet && pRet->init(szModelPath))
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

	virtual void initSprite()
	{
		init();
	}

	
private:
};

