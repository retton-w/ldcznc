/**
 * @file	udp_server.h
 * @brief	UDP服务器类,与客户端配合查找设备
 * @author	hrh <huangrh@landuntec.com>
 * @version	1.0.0
 * @date	2011-12-13
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


#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include "thread.h"

class UdpServer: public Thread
{

public:
	UdpServer();
	~UdpServer();

protected:
	void Run();


private:
	
	int  server_sock;	//服务器socket
	void Init();
	void Process(char *buf, int len);
	void BroadcastToClient();

};

#endif

