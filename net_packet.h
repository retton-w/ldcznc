
#ifndef _NET_PACKET_H_
#define _NET_PACKET_H_

#include "ldczn_protocol.h"

class TcpClient;

class NetPacket {

public:
	NetPacket();
	~NetPacket();

	int Init(TcpClient *client);

	int ParsePacket(char *buf, int len);

private:


	int ProcessHeader(struct header_std *head, int packet_len);

	int ProcessRequest(struct payload_req *req, char *buf);
	int ProcessHeartBeat(struct payload_req *req, char *buf);
	int ProcessRunning(struct payload_req *req, char *buf);
	int ProcessImage(struct payload_req *req, char *buf);
	int ProcessAewMode(struct payload_req *req, char *buf);
	int ProcessMannufacture(struct payload_req *req, char *buf);

	int ProcessImgMode(struct payload_req *req, char *buf);
	int ProcessImgSnap();
	int ProcessImgSetParm(struct payload_req *req, char *buf);

	int ProcessAck(struct payload_ack *ack, char *buf);
	

	TcpClient *_client;
};

#endif

