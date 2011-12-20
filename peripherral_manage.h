#ifndef _PERIPHERRAL_MANAGE_H_
#define _PERIPHERRAL_MANAGE_H_

#include "ldczn_base.h"

class PeripherralManage {

public:
	PeripherralManage();
	~PeripherralManage();

	trigger_info* GetTriggerInfo();
	bool PutTriggerInfo(trigger_info *info);

private:

	std::list<trigger_info*> _trigger_list;

};

#endif

