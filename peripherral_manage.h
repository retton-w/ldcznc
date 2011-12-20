/**
 * @file	peripherral_manage.h
 * @brief	外设管理模块	
 * @author	hrh <huangrh@landuntec.com>
 * @version	1.0.0
 * @date	2011-12-20
 * 
 * @verbatim
 * ============================================================================
 * Copyright (c) Shenzhen Landun technology Co.,Ltd. 2011
 * All rights reserved. 
 * 
 * Use of this software is controlled by the terms and conditions found in the
 * license agreenment under which this software has been supplied or provided.
 * ============================================================================
 * 
 * @endverbatim
 * 
 */

#ifndef _PERIPHERRAL_MANAGE_H_
#define _PERIPHERRAL_MANAGE_H_

#include "ldczn_base.h"

class Mutex;

class PeripherralManage {

public:
	PeripherralManage();
	~PeripherralManage();

	bool GetTriggerInfo(trigger_info *info);
	bool PutTriggerInfo(const trigger_info *info);

private:

	std::list<trigger_info*> _trigger_list;
	Mutex	_list_mutex;
};

#endif

