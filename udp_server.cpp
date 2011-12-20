/**
 * @file 	udp_server.cpp
 * @brief	UDP服务类的实现	
 * @author	hrh <huangrh@landuntec.com>
 * @version 	1.0.0
 * @date 	2011-12-13
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

#include <stdio.h>
#include <string.h>
#include "udp_server.h"
#include "socket.h"
#include "ldczn_protocol.h"

UdpServer::UdpServer()
{

}


UdpServer::~UdpServer()
{

}


void UdpServer::BroadcastToClient()
{
	
	int  broadcast_sock;	//广播回复socket
	
	broadcast_sock = Socket::CreateUdp();
	if (broadcast_sock < 0) {

		printf("func: %s, Create Udp failed\n", __func__);
		return;
	}

	struct packet_search_ack packet;
	char	auth_code[sizeof(packet.head)];
	fill_std_header(&packet.head, ENCODING_TYPE_RAW, auth_code,
			MESSAGE_TYPE_ACK, sizeof(packet));

	packet.ack.id	= 0;
	packet.ack.type	= REQ_TYPE_SEARCH;
	packet.ack.sts	= ACK_SUCCESS;

	Socket::SendBroadcast(broadcast_sock, 39001, (char *)&packet, sizeof(packet));

	Socket::Close(broadcast_sock);
}


void UdpServer::Init()
{
	server_sock = Socket::CreateUdp();
	if (server_sock < 0) {
	
		printf("func: %s, Create udp failed\n", __func__);
		return;
	}
	
	if (Socket::Bind(server_sock, 39000) < 0) {
	
		printf("func: %s, Bind port 39000 failed\n", __func__);
		return;
	}
	
}


void UdpServer::Process(char *buf, int len)
{
	struct header_std *head = (struct header_std *)buf;

	if (len < sizeof(struct header_std)) {

		printf("len of recv buf is too short\n");
		return;
	}

	struct payload_req *req = (struct payload_req *)(buf + sizeof(*head));
	if ((req->type & 0xFF000000) == REQ_TYPE_SEARCH) {

		BroadcastToClient();
	}
	printf("func: %s, %d\n", __func__, req->type);
}


#define RECV_BUF_LENGTH 1024

void UdpServer::Run()
{
	Init();
	printf("start udp server \n");

	int ret = -1;
	int port = 0;
	int bytes = -1;
	char addr[32];
	char buf[RECV_BUF_LENGTH];

	while (!IsTerminated()) {

		ret = Socket::CheckRead(server_sock, 5000);
		if (ret < 0) {
			printf("func: %s, Check socket error\n", __func__);
			break;
		}
		
		if (ret > 0) {
			if ((bytes = Socket::RecvFrom(server_sock, buf, addr, &port)) < 0) {
				printf("udpserver recvfrom error\n");
				break;
			}
		
			printf("func %s, recv from %s:%d\n", __func__, addr, port);
			Process(buf, bytes);
		}
	}

	Socket::Close(server_sock);
	Terminate();

	printf("func: %s exit\n", __func__);
}

