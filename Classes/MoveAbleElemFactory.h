/***************************************************
ʵ�ֵ������ߺ�������ģ�͡�
�������߳�һֱ�ں�ִ̨�С�������ҵ����ʱ��ִ�У�û����ҵ����ʱֱ�ӷ�����ǰcpuʱ��Ƭ
����������������ELEM_MIN_REMAIN�Ǽ�����������ˮ����ҵ������PRE_CREATE_BASE��
���������У����߳�����������������Լ����������������˵����̨���������߳�ӵ����ע���ˣ���
��Ϊȷ��AutoReleasePool�ǲ�֧�ֶ��߳���������ʹ�����ֶ�release��ͬʱ��elem��create��������ΪcreateWithAutoRealse

������Ϸ�߼�����Ҫ����ƿ�����ڣ�
1����ʱ�����ɴ�����elem����ʼ�����飬��Ȼ���Ѿ���elem�����ɲ�ɶ��߳��ˣ�����Sprite3D�ĳ�ʼ����ʹ����AutoRelease��ص�
���Ƶ����޷��ڷ����߳������ɡ�ĿǰΪʹ��ǰ��ʼ�������������ƿ��֮һ
2����ײ����߼�û�н���AOI����
3��������ʹ�����麯�����������һ���������⣬֮���ǽ��޹ص��麯����������Ϊ���麯��









***************************************************/
#pragma once
#include "cocos2d.h"
#include "MoveAbleElemBaseFactory.h"
#include <thread>
#include <future>
#include <atomic>
#include <exception>
#include <mutex>

#include "MoveAbleElem.h"

#define INCREASEMENT 40
#define PRE_CREATE_INCREASEMENT_TIMES 1
#define PRE_CREATE_BASE 30
#define ELEM_MIN_REMAIN 15
#define ELEM_MAX_REMAIN 80
//Ӧ�ø�������ȥ��ȡʵ�ʿ����������
#define ELEM_MAX_NEED 100

#ifdef STD_VECTOR_ELEM
#define pushBack push_back
#define popBack pop_back
#define RELSASE_TARTGET(TAR) 
#else
#define RELSASE_TARTGET(TAR) TAR->release()
#endif

//todo bug STD_VECTOR_ELEM
#define ADD_MOVEABLE_ELEM(CONTAINER,SIZE) \
		for (size_t i = 0; i < SIZE; ++i)\
		{\
			auto p = T::createWithAutoRealse();\
			CONTAINER.pushBack(p); \
			RELSASE_TARTGET(p);	\
		}\

		

template<class T>
class MoveAbleElemFactory :public MoveAbleElemBaseFactory
{
	typedef T* T_Ptr;
protected:
	MoveAbleElemFactory() :_bPreCreateFinshed(false), _bThreedNeedToExit(false), _iCount(0)
	{
		auto & _locked = _mutexLock;
		auto & _pPreGenElem = this->_pPreGenElem;
		auto & _bPreCreateFinshed = this->_bPreCreateFinshed;
		auto & _bThreedNeedToExit = this->_bThreedNeedToExit;
		auto & curStep = this->_iCount;
		auto totalStep = PRE_CREATE_INCREASEMENT_TIMES; //todo
		//�����߳�ȥ��ɼ��أ�����������Ψһ��
		_theadPreload = std::thread(
			[&_locked, &_pPreGenElem, &_bPreCreateFinshed, &_bThreedNeedToExit, &curStep,&totalStep]()->void
		{
			while (true)
			{
				if (_bPreCreateFinshed)
				{
					std::this_thread::yield();
				}
				else
				{
					while (!_locked.try_lock())
					{
						//����ȡ��
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
					}
					ADD_MOVEABLE_ELEM(_pPreGenElem, INCREASEMENT);//�л��̵߳�ʱ��ʹ���INCREASEMENT����ļ�������õ����
					++curStep;
					if (curStep >= totalStep)
					{
						_bPreCreateFinshed = true;
						curStep = 0;
					}
					//����
					_locked.unlock();
					std::this_thread::yield();
					
				}

				if (_bThreedNeedToExit)
				{
					return;
				}
				
			}
			
		}
		);
		_theadPreload.detach();
	}
public:
	
	virtual ~MoveAbleElemFactory()
	{
		while (!_mutexLock.try_lock())
		{

		}
		_bThreedNeedToExit.store(true);
		_mutexLock.unlock();
		if (_theadPreload.joinable())
		{
			_theadPreload.join();
		}
	}

	bool init() override
	{
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

	T_Ptr getMoveAbleElem()
	{
		if (_vReadyElem.size() <= ELEM_MIN_REMAIN && _bPreCreateFinshed)
		{
			_mutexLock.lock();
			if (_vReadyElem.size() <= ELEM_MAX_REMAIN)
			{
				_bPreCreateFinshed = false;
			}
			for (auto it = _pPreGenElem.begin(); it != _pPreGenElem.end(); ++it)
			{
				_vReadyElem.pushBack(*it);
			}
			_pPreGenElem.clear();
			_iCount = 0;
			_mutexLock.unlock();
		}

		if (_vReadyElem.empty())
		{		
			ADD_MOVEABLE_ELEM(_vReadyElem, 1);
		}
		//�ѹ��ã����ٹ����߳�
		else if (_vReadyElem.size() >= ELEM_MAX_NEED)
		{
			int i = 0;
			while (!_mutexLock.try_lock() && i < 5)
			{
				++i;
				//����ȡ��
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
			if (i < 5)
			{
				_bThreedNeedToExit.store(true);
				_mutexLock.unlock();
			}
		}
		

		//todo 
		T_Ptr pElem = dynamic_cast<T_Ptr >(_vReadyElem.back());//����auto ��Ϊ��ȷ�Ϸ��ط�const T *
		_vUsedElem.pushBack(pElem);
		pElem->setUsed();
		_vReadyElem.popBack();

		return pElem;
	}

	void forceToPreCreate()
	{
		_mutexLock.lock();
		for (auto it = _pPreGenElem.begin(); it != _pPreGenElem.end(); ++it)
		{
			_vReadyElem.pushBack(*it);
		}
		_pPreGenElem.clear();
		_bPreCreateFinshed = false;
		_iCount = 0;
		_mutexLock.unlock();
	}

	void recycleElem(MoveAbleElem * pElem)
	{
		_vReadyElem.pushBack(pElem);
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
	std::thread _theadPreload;
	std::atomic<bool> _bPreCreateFinshed;
	std::atomic<bool> _bThreedNeedToExit;
	std::atomic<int> _iCount;
	std::mutex _mutexLock;
private:
};

#ifdef STD_VECTOR_ELEM
#undef pushBack
#undef popBack pop_back
#endif

#undef RELSASE_TARTGET(TAR)