#pragma once
#include "cocos2d.h"
#include "MoveAbleElemBaseFactory.h"
#include <thread>
#include <future>
#include <atomic>
#include <exception>
#include <iostream>

static std::atomic_flag s_lock = ATOMIC_FLAG_INIT;

#define INCREASEMENT 10

#define ADD_MOVEABLE_ELEM(SIZE) \
		for (size_t i = 0; i < SIZE; ++i)\
		{\
			_vUsedElem.push_back(MoveAbleElem::create()); \
		}

template<class MoveAbleElem>
class MoveAbleElemFactory :public MoveAbleElemBaseFactory
{
	typedef MoveAbleElem* MoveAbleElemPtr;
protected:
	MoveAbleElemFactory()
	{
		_pPreGenElem.clear();

		auto & _lock = s_lock;
		auto & _vUsedElem = this->_vUsedElem;
		//�����߳�ȥ��ɼ��أ�����������Ψһ��
		_theadPreload = std::async(
			[&_lock, &_vUsedElem]()->void
		{
			while (_lock.test_and_set(std::memory_order_acquire))
			{
				//����ȡ��
			}
			ADD_MOVEABLE_ELEM(INCREASEMENT * 2);
			_lock.clear();
			//����
		}
		);
	}
public:
	
	virtual ~MoveAbleElemFactory()
	{

	}

	bool init() override
	{
		//ADD_MOVEABLE_ELEM(INCREASEMENT);
		return true;
	}

	static MoveAbleElemBaseFactory * create()
	{
		MoveAbleElemFactory *pRet = new(std::nothrow) MoveAbleElemFactory();
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

	virtual MoveAbleElemPtr getMoveAbleElem()
	{
		try
		{
			_theadPreload.get();
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
		

		if (!_pPreGenElem.empty())
		{
			while (s_lock.test_and_set(std::memory_order_acquire))
			{
				//����ȡ��
			}
			for each (auto pMoveAbleElem in _pPreGenElem)
			{
				_vUsedElem.push_back(pMoveAbleElem);
			}
			_pPreGenElem.clear();
			s_lock.clear();
		}

		if (_vUsedElem.size() <= 0)
		{
			ADD_MOVEABLE_ELEM(INCREASEMENT);
		}

		//todo 
		MoveAbleElemPtr pElem = dynamic_cast<MoveAbleElemPtr >(_vUsedElem.back());//����auto ��Ϊ��ȷ�Ϸ��ط�const MoveAbleElem *
		_vReadyElem.push_back(pElem);
		pElem->setUsed();
		_vUsedElem.pop_back();
		return pElem;
	}

	virtual void recycleElem(MoveAbleElemPtr pElem)
	{
		_vReadyElem.push_back(pElem);
		for (auto it = _vUsedElem.begin(); it != _vUsedElem.end(); ++it)
		{
			if (*it == pElem)
			{
				_vUsedElem.erase(it);
				break;
			}
		}
		pElem->recycleSelf();
	}

	
protected:

private:
	std::future<void> _theadPreload;
};

