/**
 * @file	net_packet.cpp
 * @brief	处理PC客户端通过相机监听的TCP端口发来的数据
 * @author	hrh <huangrh@landuntec.com>
 * @version	1.0.0
 * @date	2011-12-19
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

#include <string.h>
#include <stdio.h>
#include "net_packet.h"
#include "sensor.h"
#include "gpio.h"
#include "tcp_client.h"
#include "image_process.h"


int NetPacket::Init(TcpClient *client)
{
	_client = client;
	processor = new ImageProcess();
	processor->Start();		//启动图像处理线程

	return 0;
}


NetPacket::NetPacket()
{
	_client = NULL;
	processor = NULL;
}


NetPacket::~NetPacket() {

	_client = NULL;
		
	if (processor) {
		delete processor;
		processor = NULL;
	}
}


static inline int get_auth_code(char *auth_code, int len)
{
	return 0;
}


static inline int match_auth_code(const char *auth_code, int len)
{
	return 0;
}

	
int NetPacket::ParsePacket(char *buf, int len)
{
	if ((unsigned int)len < sizeof(struct header_std)) {

		return -1;
	}
	
	struct header_std *head = (struct header_std *)buf;

	if (ProcessHeader(head, len)) {

		return -1;
	}

	switch (head->msg_type) {

	case MESSAGE_TYPE_REQ:
		ProcessRequest((struct payload_req *)(buf + sizeof(*head)),
				buf + sizeof(*head) + sizeof(struct payload_req));
		break;
	case MESSAGE_TYPE_ACK:
		ProcessAck((struct payload_ack *)(buf + sizeof(*head)),
				buf + sizeof(*head) + sizeof(struct payload_ack));
		break;
	default:
		break;
	}

	return len - head->msg_size;
}


int NetPacket::ProcessHeader(struct header_std *head, int packet_len)
{
	const char magic[sizeof(head->magic)] = PROTOCOL_MAGIC;
	printf("%s\n", __func__);

	if (memcmp(head->magic, magic, sizeof(magic)) != 0) {

		return -1;
	}

	if ((head->protocol_major != PROTOCOL_MAJOR) ||
		(head->protocol_minor != PROTOCOL_MINOR)) {

		return -1;
	}

	if (head->encoding != ENCODING_TYPE_RAW) {

		return -1;
	}

	if (match_auth_code(head->auth_code, sizeof(head->auth_code)) < 0) {

		return -1;
	}

	if (head->msg_size > (unsigned int)packet_len) {

		return -1;
	}

	return 0;
}


int NetPacket::ProcessRequest(struct payload_req *req, char *buf)
{
	printf("%s\n", __func__);
	switch (req->type & 0xFF000000) {

	case REQ_TYPE_HEARTBEAT:
		return ProcessHeartBeat(req, buf);
		break;
	case REQ_TYPE_RUNNING:
		return ProcessRunning(req, buf);
		break;
	case REQ_TYPE_MANUFACTURE:
		return ProcessMannufacture(req, buf);
		break;
	case REQ_TYPE_HW_DEBUG:
		break;
	case REQ_TYPE_SW_DEBUG:
		break;
	case REQ_TYPE_IMAGE:
		return ProcessImage(req, buf);
		break;
	default:
		break;

	}

	return -1;
}


int NetPacket::ProcessAck(struct payload_ack *ack, char *buf)
{
	return 0;
}


int NetPacket::ProcessHeartBeat(struct payload_req *req, char *buf)
{
	printf("%s\n", __func__);
	return 0;
}


int NetPacket::ProcessRunning(struct payload_req *req, char *buf)
{
	printf("%s\n", __func__);
	switch (req->type & 0x0000FFFF) {

	case REQ_RUN_CONNECT:
		printf("Get client REQ_RUN_CONNECT cmd\n");
		_client->SetConnectFlag(true);
		break;
	case REQ_RUN_DISCON:
		printf("Get client REQ_RUN_DISCON cmd\n");
		_client->SetConnectFlag(false);
		break;
	default:
		break;
	}
	
	return 0;
}


int NetPacket::ProcessImage(struct payload_req *req, char *buf)
{
	printf("%s\n", __func__);

	switch (req->type & 0x00FF0000) {

	case REQ_IMG_START:	//使能视频传输
		break;
	case REQ_IMG_STOP:	//停止视频传输
		break;
	case REQ_IMG_TRANS:	//图像传输
		break;
	case REQ_IMG_GPARM:	//获取图像参数
		break;
	case REQ_IMG_SPARM:	//设置图像参数
		return ProcessImgSetParm(req, buf);
		break;
	case REQ_IMG_SMODE:	//设置工作模式(live / trig)
		return ProcessImgMode(req, buf);
		break;
	case REQ_IMG_AEWMODE:	//设置AEW模式
		return ProcessAewMode(req, buf);
		break;
	case REQ_IMG_SNAP:	//手动抓拍
		return ProcessImgSnap();
		break;
	default:
		break;
	}

	return -1;
}


/**
 * @fn		int ProcessAewMode(struct payload_req *req, char *buf)
 *
 * @brief	处理AEW模式
 * @param	req	请求数据包
 * @param	buf	
 *
 * @return	
 */
int NetPacket::ProcessAewMode(struct payload_req *req, char *buf)
{
	unsigned int mode = 0x00;
        switch (req->type & REQ_TYPE_CMD_MASK) {
	
	case REQ_IMG_AEWMODE_DISABLE:
		Sensor::GetInstance()->SetSensorAEManual();
		break;
	case REQ_IMG_AEWMODE_GAIN:
		Sensor::GetInstance()->SetSensorAEAuto();
		mode = 0x01;
		Sensor::GetInstance()->SetSensorAEMethod(mode);
		break;
	case REQ_IMG_AEWMODE_EXP:
		Sensor::GetInstance()->SetSensorAEAuto();
		mode = 0x02;
		Sensor::GetInstance()->SetSensorAEMethod(mode);
		break;
	case REQ_IMG_AEWMODE_AUTO:
		Sensor::GetInstance()->SetSensorAEAuto();
		mode = 0x00;
		Sensor::GetInstance()->SetSensorAEMethod(mode);
		break;
	default:
		break;
	}

	return 0;
}


int NetPacket::ProcessImgSnap()
{
	printf("%s\n", __func__);
	GpioCtl::MannualSnap();
	return 0;
}


int NetPacket::ProcessImgMode(struct payload_req *req, char *buf)
{
	printf("%s\n", __func__);
        switch (req->type & REQ_TYPE_CMD_MASK) {
		case REQ_IMG_MODE_LIVE :
			Sensor::GetInstance()->SetSensorVideo();
			processor->SetStatus(VIDEO);
			break;
		case REQ_IMG_MODE_TRIG :
			processor->SetStatus(CAPTURE);
			Sensor::GetInstance()->SetSensorCapture();
			break;
		default:
			break;
	}

	return 0;
}


int NetPacket::ProcessImgSetParm(struct payload_req *req, char *buf)
{
        printf("%s\n", __func__);

        struct payload_img_parm *parm = (struct payload_img_parm *)buf;
        switch (req->type & REQ_TYPE_CMD_MASK) {
		case REQ_IMG_PARM_EXP :
			Sensor::GetInstance()->SetSensorExposure(parm->value);
			break;
		case REQ_IMG_PARM_EXP_MIN:
			Sensor::GetInstance()->SetSensorMinExp(parm->value);
			break;
		case REQ_IMG_PARM_EXP_MAX:
			Sensor::GetInstance()->SetSensorMaxExp(parm->value);
			break;
		case REQ_IMG_PARM_GAIN :
			Sensor::GetInstance()->SetSensorGain(parm->value);
			break;
		case REQ_IMG_PARM_GAIN_MIN:
			Sensor::GetInstance()->SetSensorMinGain(parm->value);
			break;
		case REQ_IMG_PARM_GAIN_MAX:
			Sensor::GetInstance()->SetSensorMaxGain(parm->value);
			break;
		case REQ_IMG_PARM_WB_R :
			Sensor::GetInstance()->SetSensorRgain(parm->value);
			break;
		case REQ_IMG_PARM_WB_B :
			Sensor::GetInstance()->SetSensorBgain(parm->value);
			break;
		case REQ_IMG_PARM_V_TARGET:
			Sensor::GetInstance()->SetSensorVideoTargetGray(parm->value);
			break;
		case REQ_IMG_PARM_T_TARGET:
			Sensor::GetInstance()->SetSensorCaptureTargetGray(parm->value);
			break;
		case REQ_IMG_PARM_AEZONE:
			Sensor::GetInstance()->SetSensorAEZone(parm->value);
			break;
		default :
			break;
        }
        return 0;
}

	
int NetPacket::ProcessMannufacture(struct payload_req *req, char *buf)
{
	return 0;
}

