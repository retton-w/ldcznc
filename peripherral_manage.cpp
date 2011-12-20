#include <list>
#include "peripherral_manage.h"

using namespace std;

PeripherralManage::PeripherralManage()
{

}


PeripherralManage::~PeripherralManage()
{

}


trigger_info* PeripherralManage::GetTriggerInfo()
{
	trigger_info *tmpInfo = NULL;
	if ((tmpInfo = _trigger_list.front()) == NULL) {
		return NULL;
	}

	_trigger_list.pop_front();
}


bool PeripherralManage::PutTriggerInfo(trigger_info *info)
{
	trigger_info *tmpInfo = new trigger_info;
	memcpy(tmpInfo, info, sizeof(trigger_info));
	_trigger_list.push_back(tmpInfo);	

	return true;
}

