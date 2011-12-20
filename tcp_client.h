/**
 * @file	TcpClient.h
 * @brief	TCP客户端实现代码
 * @author	hrh <huangrh@landuntec.com>
 * @version	1.0.0
 * @date	2011-12-07
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


#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

#include "thread.h"
#include "ce_buffer.h"

//using namespace std;

struct _ClientInfo {

	char addr[32];
};


class TcpClient : public Thread
{
public:
	TcpClient();
	~TcpClient();

	void SetClient(struct _ClientInfo *info);
	void SetConnectFlag(bool flag);
	void ClearBuffer();

protected:
	void Run();

private:
	char client_addr[32];		//客户端IP地址

	bool _connected_status;		//是否已经连接到客户端
	int  local_sock;		//连接到客户端的socket

	bool _connect_flag;		//是否要连接到客户端

	void UploadClient();
	void ConnectClient();
	
	int SendData(struct cmemBuffer *image);
	int SendHeartBeat();

};

#endif

