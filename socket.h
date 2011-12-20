/**
 * @file	Socket.h
 * @brief	
 * @author 	hrh <huangrh@landuntec.com>
 * @version 	1.0.0
 * @date 	2011-11-30
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

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <arpa/inet.h>


class Socket
{

public:
	static int CreateTcp();
	static int CreateUdp();
	static void SetNonblock(int sockfd);
	static int Bind(int sockfd, int port);
	static int Listen(int sockfd, int max_request);
	static int Accept(int sockfd, int msec);
	static int Writen(int sockfd, void *buf, int len, int msec);	//写入len字节，少于也返回错误
	static int CheckRead(int sockfd, int msec);
	static int Read(int sockfd, void *buf, int len, int msec);
	static int Readn(int sockfd, void *buf, int len, int msec);	//读取len字节，少于也返回错误
	static int Connect(int sockfd, char *destAddr, int destPort, int msec);
	static int TcpKeepalive(int sockfd, int start, int interval, int count);
	static int GetClient(int sockfd, char *addr);

	static void Close(int sockfd);
	static int RecvFrom(int sockfd, char *buf, char *addr, int *port);
	static int SendTo(int sockfd, char *buf, int len, char *addr, int port);
	static int SendBroadcast(int sockfd, int remote_port, char *buf, int len);

private:
	Socket() { }
	~Socket() { }

};

#endif

