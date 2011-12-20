/**
 * @file	TcpServer.h
 * @brief	TCP服务类声明
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


#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include "thread.h"
#include "tcp_client.h"
#include "ldczn_protocol.h"

class NetPacket;

class TcpServer: public Thread
{

public:
	int Process(char *buf, int len);
	TcpServer(TcpClient *client);
	~TcpServer();

protected:
	void Run();


private:
	bool bClient;		//是否有客户端已连接服务器
	int  server_sock;	//服务器socket
	int  clnt_sock;		//客户端socket
	
	TcpClient *_client;	//相机客户端线程对象指针

	NetPacket *packet_parse;

	void Init();

#if 0
	int ProcessHeartBeat(struct payload_req *req, char *buf);
	int ProcessRunning(struct payload_req *req, char *buf);
	int ProcessImage(struct payload_req *req, char *buf);
	int ProcessRequest(struct payload_req *req, char *buf);
	int ProcessAck(struct payload_ack *ack, char *buf);
	int ProcessHeader(struct header_std *head, int packet_len);
	int ProcessImgStart();
	int ProcessImgStop();
	int ProcessImgSnap();
	int ProcessImgMode(struct payload_req *req, char *buf);
	int ProcessAewMode(struct payload_req *req, char *buf);
	int ProcessImgSetParm(struct payload_req *req, char *buf);
#endif
};

#endif

