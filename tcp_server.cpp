/**
 * @file	TcpServer.cpp
 * @brief	Tcp服务端类实现
 * @author	hrh <huangrh@landuntec.com>
 * @version 	1.0.0
 * @date 	2011-12-07
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


#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "tcp_server.h"
#include "net_packet.h"
#include "sensor.h"

#define RECV_BUF_LENGTH 1024


TcpServer::TcpServer(TcpClient *client)
{
	_client = client;

	packet_parse = new NetPacket();
	packet_parse->Init(_client);
	bClient = false;
	server_sock = -1;
}


TcpServer::~TcpServer()
{
	if (packet_parse) {
		delete packet_parse;
		packet_parse = NULL;
	}

	bClient = false;
	server_sock = -1;
}


/**
 * @function	void Init()
 * @brief	初始化tcp server，并监听39002端口
 *
 */
void TcpServer::Init()
{
	
	server_sock = Socket::CreateTcp();
	if (server_sock < 0) {

		printf("Create Nonblock Tcp failed\n");
		return;
	}

	Socket::SetNonblock(server_sock);

	int ret = Socket::Bind(server_sock, 39002);
	if (ret < 0) {

		printf("Bind port 39002 failed\n");
		Socket::Close(server_sock);
		return;
	}

	ret = Socket::Listen(server_sock, 1);
	if (ret < 0) {

		printf("Listen port failed\n");
		Socket::Close(server_sock);
		return;
	}

	Socket::TcpKeepalive(server_sock, 5, 2, 1);
	printf("start listen 39002 port\n");

}


void TcpServer::Run()
{
	
	char buf[RECV_BUF_LENGTH];
	
	Init();

	while (!IsTerminated()) {

		if (bClient == false) {

			clnt_sock = Socket::Accept(server_sock, 10000);
			if (clnt_sock < 0) {

				printf("no connection right now\n");
				continue;

			} else {
			
				printf("An PC client connect to the camera\n");
				bClient = true;
				struct _ClientInfo info;
				Socket::GetClient(clnt_sock, info.addr); 
				_client->SetClient(&info);			//设置PC客户端的IP地址
			}
		}

		int ret = Socket::Read(clnt_sock, buf, RECV_BUF_LENGTH, 5000); 
		if (ret == 0) {

			if (bClient) {

				//client.Terminate();
				printf("after call terminate client\n");
				//client.Join();
				Socket::Close(clnt_sock);
				bClient = false;
				Sensor::GetInstance()->SetSensorCapture();
				printf("-----------------Connection disconnect----------\n");
			}

		} else if (ret > 0) {

			printf("got %d packet\n", ret);
			Process(buf, ret);
		}
		
	}

	Socket::Close(clnt_sock);
	Socket::Close(server_sock);
}


int TcpServer::Process(char *buf, int len)
{
	printf("%s\n", __func__);
	return packet_parse->ParsePacket(buf, len);
}


