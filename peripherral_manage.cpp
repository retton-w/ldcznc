/**
 * @file	peripherral_manage.cpp
 * @brief	外设管理模块实现
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

#include <list>
#include "mutex.h"
#include "peripherral_manage.h"

using namespace std;

PeripherralManage::PeripherralManage()
{

}


PeripherralManage::~PeripherralManage()
{

}


/**
 *
 * @fn		bool GetTriggerInfo(trigger_info *info)
 * @brief	从触发数据队列获取触发数据
 *
 * @param	[out] info	从队列中获取触发数据
 *
 * @return	是否有触发数据
 * @retval	true | false
 */
bool PeripherralManage::GetTriggerInfo(trigger_info *info)
{
	if (_list_mutex.lock()) {

		if (_trigger_list.empty()) {
			return false;
		}

		info = _trigger_list.front();
		_trigger_list.pop_front();
		_list_mutex.unlock();
		return true;

	} else {
		return false;
	}
}


/**
 * @fn		bool PutTriggerInfo(const trigger_info *info)
 * @brief	将触发数据放入队列中
 *
 * @param	[in] info	传入触发数据
 *
 * @return	是否传入触发数据成功
 * @retval	true | false
 */
bool PeripherralManage::PutTriggerInfo(const trigger_info *info)
{
	if (_list_mutex.lock()) {
		trigger_info *tmpInfo = new trigger_info;
		memcpy(tmpInfo, info, sizeof(trigger_info));
		_trigger_list.push_back(tmpInfo);
		_list_mutex.unlock();
		return true;

	} else {
		return false;
	}
}

