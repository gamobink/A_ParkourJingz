#pragma once
#include "cocos2d.h"
#include "MoveableElems.h"

//�ٶ�Ӧ����Ϊ��ҡ���ҡ���������ԣ������ֻ���������

class Player :public MoveableElems
{
	Player();
public:
	~Player();
	bool init(const std::string &pathName = "model/zhanshi_pao.c3b");

	CREATE_FUNC(Player);
	
private:
};

