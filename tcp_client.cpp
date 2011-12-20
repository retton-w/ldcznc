/**
 * @file	TcpClient.cpp
 * @brief	TCP客户端
 * @author 	hrh <huangrh@landuntec.com>
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
#include "tcp_client.h"
#include "ldczn_protocol.h"
#include "image_buffer.h"


int TcpClient::SendHeartBeat()
{
	
	if (local_sock == -1) {
                return -1;
        }
        int bytes = -1;

	struct packet_heartbeat packet;
        char auth_code[sizeof(packet.head.auth_code)];
        fill_std_header(&packet.head,
                        ENCODING_TYPE_RAW, auth_code,
                        MESSAGE_TYPE_REQ,
                        sizeof(packet));

        //packet.req.id   = get_req_id();
        packet.req.id   = 0;
        packet.req.type = REQ_TYPE_HEARTBEAT;

        printf("send heart beat\n");

        bytes = Socket::Writen(local_sock, &packet, sizeof(packet), 0);
        if (bytes < 0) {
                return -1;
        } else if (bytes == 0) {
		local_sock = -1;
		_connected_status = false;
		_connect_flag = false;
                printf("peer closed\n");
                return -1;
        } else if ((unsigned int)bytes < sizeof(packet)) {
                return -1;
        }
}


int TcpClient::SendData(struct cmemBuffer *image)
{
        if (local_sock == -1) {
                return -1;
        }
        int bytes = -1;

        struct packet_img_trans_req packet;
        char auth_code[sizeof(packet.head.auth_code)];
        fill_std_header(&packet.head,
                        ENCODING_TYPE_RAW, auth_code,
                        MESSAGE_TYPE_REQ,
                        sizeof(packet) + image->size);

        //packet.req.id   = get_req_id();
        packet.req.id   = 0;
        packet.req.type = REQ_TYPE_IMAGE | REQ_IMG_TRANS;

        packet.trans.info_len = 0;
        packet.trans.data_len = image->size;

        bytes = Socket::Writen(local_sock, &packet, sizeof(packet), 0);
        printf("send image : header %d bytes, jpeg %d bytes\n", sizeof(packet), image->size);

        if (bytes < 0) {
                return -1;
        } else if (bytes == 0) {
                printf("peer closed\n");
		local_sock = -1;
		_connected_status = false;
		_connect_flag = false;
                return -1;
        } else if ((unsigned int)bytes < sizeof(packet)) {
                return -1;
        }

	bytes = Socket::Writen(local_sock, (void *)image->vir_addr, image->size, 3000);
        if (bytes < 0) {
                return -1;
        } else if (bytes == 0) {
                printf("peer closed\n");
                return -1;
        }
        
        return bytes;
}


TcpClient::TcpClient()
{
	_connected_status = false;
	local_sock = -1;
	memset(client_addr, 0, sizeof(client_addr));

	_connect_flag = false;
}


TcpClient::~TcpClient()
{
	Terminate();
	if (local_sock >= 0) {

		Socket::Close(local_sock);
		printf("Close socket\n");
        }

	local_sock = -1;
	_connected_status = false;

	_connect_flag = false;
	printf("deconstrution\n");
}


void TcpClient::ConnectClient()
{
	int client_port = 39003;

	if (local_sock == -1) {

		if ((local_sock = Socket::CreateTcp()) < 0) {

			printf("create Tcp socket failed\n");
			return;
		}
	}

	if (Socket::Connect(local_sock, client_addr, client_port, 2000)) {

		printf("connect failed, addr : %s, port: %d\n", client_addr, client_port);

		return;
	}

	printf("connect OK\n");
	_connected_status = true;
}


void TcpClient::UploadClient()
{
        struct image_buffer_description *image = NULL;
	if (transport_buffer_get(&image) == 0) {

		if (!_connected_status) {
		
			transport_buffer_free(image);
			image = NULL;
			return;
		}

		if (SendData(&image->info) <= 0) {

			printf("-------can't send data----------\n");
			if (local_sock >= 0) {
				Socket::Close(local_sock);
				local_sock = -1;
				_connected_status = false;
			}
		}
		transport_buffer_free(image);
		image = NULL;
	}
}


void TcpClient::ClearBuffer()
{
	usleep(100000);
        struct image_buffer_description *image = NULL;
	while (transport_buffer_get(&image) == 0) {

		transport_buffer_free(image);
		image = NULL;
	}

	printf("after free buffer\n");
}


void TcpClient::Run()
{

	int cnt = 0;

	while (!IsTerminated()) {

		if (_connect_flag) {
	
			if (!_connected_status) {
				ConnectClient();
			}

		} else {

			if (_connected_status) {
				
				if (local_sock) {
					Socket::Close(local_sock);
					local_sock = -1;
				}

				_connected_status = false;
			}
		}
		UploadClient();

		usleep(10000);

		if (_connected_status) {
			cnt++;
			if (cnt > 500) {
				cnt = 0;
				SendHeartBeat();
			}
		}
        }
	
	Terminate();
	ClearBuffer();
	local_sock = -1;
	_connected_status = false;
	_connect_flag = false;
}


void TcpClient::SetClient(struct _ClientInfo *info)
{

	memcpy(client_addr, info->addr, sizeof(info->addr));
	printf("client addr is %s\n", client_addr);
}


void TcpClient::SetConnectFlag(bool flag)
{
	_connect_flag = flag;
}

