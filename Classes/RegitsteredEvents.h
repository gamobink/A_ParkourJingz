#pragma once
#include <string>
#include <vector>

//todo ���ͨ������ע����Ϣ�¼�����
class RegitsteredEvents
{
public:
	static const std::string GOLD_CHANGED;
	static const std::string SHOW_RESTART;

private:
	std::vector<std::string> _vRegisteredEvents;
};