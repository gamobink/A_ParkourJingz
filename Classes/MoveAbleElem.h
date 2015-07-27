#pragma once

#include "cocos2d.h"


/*
������˼����һ��C++ע����ƣ�
�������Ѿ�ʵ�������ƵĻ��ƣ�����û�������Զ�������ע��Ĵ��룬����и���˼

�ڼ̳н׶Σ������ĳ�ʼ���б������ӵ����������static��������ĳЩ�ض������Ŀ����ԣ������Ӿ����ü̳�֮��ʵ��ע�ᡣ
������������������ʱ�����޸�ԭ�ȵĴ��룬ֻ��Ҫ���±��뼴��

*/

class MoveAbleElem
{
protected:
	MoveAbleElem() :_pSprite(nullptr), _vMoveDir(0, 0, 1), _fMoveSpeed(1.0f*60.0f)
		, _iElemTypeId(0), _bIsSpriteInit(false)
	{
		
	}

public:

	virtual ~MoveAbleElem()
	{
		CC_SAFE_RELEASE_NULL(_pSprite);
		cocos2d::Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	}

	virtual cocos2d::Sprite3D * getCurSprite(void)
	{
		return _pSprite;
	}

	virtual float getMoveSpeed(void)
	{
		return _fMoveSpeed;
	}
	virtual void setMoveSpeed(float fSpeed)
	{
		_fMoveSpeed = fSpeed;
	}

	cocos2d::Vec3 getMoveDirNormal(void)
	{
		return _vMoveDir;
	}

	void setMoveDir(cocos2d::Vec3 dir)
	{
		_vMoveDir = dir;
		_vMoveDir.normalize();
	}

	virtual void update(float dt)
	{

	}

	unsigned short getElemType()
	{
		return _iElemTypeId;
	}

	void setElemType(unsigned short iTypeId)
	{
		_iElemTypeId = iTypeId;
	}

	virtual void recycleSelf(void)
	{
		cocos2d::Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	}

	//��ʹ��״̬������ʱ����������״̬
	virtual void setUsed(void)
	{
		if (!_bIsSpriteInit)
		{
			initSprite();;
		}
		cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
	}

	//!!!! You must call this function when you init your new MoveAbleElem
	/*static void registerSelf(void)
	{
		MoveAbleElemManager::getInstance()->registerElemAndFactory(0, MoveAbleElemFactory<MoveAbleElem>::create);
	}*/
	
	virtual void initSprite()
	{
		CC_SAFE_RELEASE_NULL(_pSprite);
		_pSprite = cocos2d::Sprite3D::create();
		CC_SAFE_RETAIN(_pSprite);
	}

protected:
	cocos2d::Sprite3D * _pSprite;
	float _fMoveSpeed;
	cocos2d::Vec3 _vMoveDir;
	bool _bIsSpriteInit;
	unsigned short _iElemTypeId;
};