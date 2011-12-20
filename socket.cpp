/**
 * @file	Socket.cpp
 * @brief	
 * @author	hrh <huangrh@landuntec.com>
 * @version	1.0.0
 * @date	2011-11-30
 *
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
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include "ldczn_protocol.h"
#include "socket.h"


int Socket::CreateTcp()
{

	int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	return sockfd;

}


void Socket::SetNonblock(int sockfd)
{
	int flags = fcntl(sockfd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(sockfd, F_SETFL, flags);

	flags = fcntl(sockfd, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	fcntl(sockfd, F_SETFD, flags);

}


int Socket::CreateUdp()
{
	int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	return sockfd;

}


int Socket::Bind(int sockfd, int port)
{

	int share = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &share, sizeof(int));

	struct sockaddr_in servAddr;	
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family		= AF_INET;
	servAddr.sin_addr.s_addr	= htonl(INADDR_ANY);	//Any IP address available locally
	servAddr.sin_port		= htons(port);

	return bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr));

}


int Socket::Listen(int sockfd, int max_request)
{
	
	return listen(sockfd, max_request);

}


int Socket::Accept(int sockfd, int msec)
{
	struct sockaddr_in clntAddr;
	socklen_t clntAddrLen = sizeof(clntAddr);
	int newSocket = -1;

	int sn;
	fd_set fdr;
	FD_ZERO(&fdr);
	FD_SET(sockfd, &fdr);
	struct timeval tv;
	tv.tv_sec = msec / 1000;
	tv.tv_usec = (msec % 1000) * 1000;
	sn = select(sockfd + 1, &fdr, NULL, NULL, &tv);

	if (sn > 0)
	{
		newSocket = accept(sockfd, (struct sockaddr *)&clntAddr, &clntAddrLen);
	}

	return newSocket;

}


int Socket::GetClient(int sockfd, char *addr)
{
	struct sockaddr_in clntAddr;
	socklen_t clntAddrLen = sizeof(clntAddr);

	if (getpeername(sockfd, (struct sockaddr *)&clntAddr, &clntAddrLen) < 0) {

		return -1;
	}
	
	int size = strlen(inet_ntoa(clntAddr.sin_addr));
	memcpy(addr, inet_ntoa(clntAddr.sin_addr), size + 1);
	printf("connect from address : %s, port : %d, len is %d\n",
			inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port), size + 1);

	return 0;

}


int Socket::TcpKeepalive(int sockfd, int start, int interval, int count)
{

	int keepAlive = 1;

	if (sockfd < 0 || start < 0 ||
		interval < 0 || count < 0) {

		return -1;
	}

	/* 启动心跳机制,设置为零即关闭 */
	if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive,
				sizeof(keepAlive)) == -1) {

		return -1;
	}

	/* 启动心跳机制到第一次心跳侦测包发送的时间间隔 */
	if (setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, (void *)&start,
				sizeof(start)) == -1) {

		return -1;
	}
	
	/* 两次心跳侦测包之间的时间间隔 */
	if (setsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&interval,
				sizeof(interval)) == -1) {

		return -1;
	}

	/* 探测次数,即几次失败之后认为TCP连接已断开 */
	if (setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&count,
				sizeof(count)) == -1) {

		return -1;
	}

	return 0;

}


int Socket::Writen(int sockfd, void *buf, int len, int msec)
{
        
	char *ptr = (char *)buf;
        int sLen = 0;
	int sn = 0;
	int n = 0;

        fd_set fdw;
        struct timeval tv;

        while (sLen < len) {

                FD_ZERO(&fdw);
                FD_SET(sockfd, &fdw);

                tv.tv_sec = msec / 1000;
                tv.tv_usec = (msec % 1000) * 1000;

                sn = select(sockfd + 1, NULL, &fdw, NULL, &tv);

                if (sn > 0) {

                        n = send(sockfd, ptr + sLen, len - sLen, 0);
                        if (n == -1) {

				return -1;

                        } else if (n == 0) {

                                break;
                        }
                        sLen += n;

                } else {

                        return sn;
                }
        }

        return sLen;

}


int Socket::CheckRead(int sockfd, int msec)
{	
	struct timeval timeout;
        fd_set readfd;
        int ret = 0;

        if (sockfd < 0) {

                return -1;
        }

        timeout.tv_sec  = msec / 1000;
        timeout.tv_usec = (msec % 1000) * 1000;

        FD_ZERO(&readfd);
        FD_SET(sockfd, &readfd);

        ret = select(sockfd + 1, &readfd, NULL, NULL, &timeout);
        if (ret < 0) {

                ret = -1;

        } else if (ret == 0) {

                ret = 0;

        } else {

                if (FD_ISSET(sockfd, &readfd)) {

                        ret = 1;

                } else {

                        ret = 0;
                }
        }

        return ret;

}


int Socket::Readn(int sockfd, void *buf, int len, int msec)
{

	char *ptr = (char *)buf;
        int rLen = 0;
	int n = 0;

        while (rLen < len) {

		if (CheckRead(sockfd, msec) > 0) {

			n = recv(sockfd, ptr + rLen, len - rLen, 0);
			if (n == -1) {

				return -1;

			} else if (n == 0) {

				break;
			}
			rLen += n;

		} else {

			printf("check read error\n");
			return -1;
		}
        }

        return rLen;

}


int Socket::Read(int sockfd, void *buf, int len, int msec)
{
	
	char *ptr = (char *)buf;
	int n = 0;

	if (CheckRead(sockfd, msec) > 0) {

		n = recv(sockfd, ptr, len, 0);
		if (n == -1) {

			return -1;
		}

		return n;

	} else {

		return -1;
        }

}


int Socket::Connect(int sockfd, char *destAddr, int destPort, int msec)
{

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family		= PF_INET;
	sin.sin_port		= htons((u_short)destPort);
	sin.sin_addr.s_addr	= inet_addr(destAddr);

#if 0
	int flag;
	fcntl(sockfd, F_GETFL, &flag);
	fcntl(sockfd, F_SETFL, flag | O_NONBLOCK);
#endif

	if (connect(sockfd, (struct sockaddr *)&sin, sizeof(sin)) == 0) {

		return 0;
	}
	
	printf("errno is %s\n", strerror(errno));
	if (errno != EINPROGRESS) {

		return -1;
	}

	fd_set fdw;
	FD_ZERO(&fdw);
	FD_SET(sockfd, &fdw);

	struct timeval tv;
	tv.tv_sec = msec / 1000;
	tv.tv_usec = (msec % 1000) * 1000;

	if (select(sockfd + 1, NULL, &fdw, NULL, &tv) <= 0) {

		printf("timeout\n");
		return -1;
	}

	if (FD_ISSET(sockfd, &fdw)) {

		int error = 0;
		socklen_t len = sizeof(error);
		getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
		if (error == 0) {

			return 0;
		}

		return -1;
	}

	return -1;

}


void Socket::Close(int sockfd)
{
	if (sockfd != -1)
	{
		close(sockfd);
	}
}


int Socket::RecvFrom(int sockfd, char *buf, char *addr, int *port)
{
	struct sockaddr_in clntAddr;
	socklen_t clntAddrLen = sizeof(clntAddr);

	int ret = -1;
	ret = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *)&clntAddr, &clntAddrLen);
	
	if (ret > 0) {

		printf("recv %d bytes from %s:%d\n", ret, inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port));
	
		int size = strlen(inet_ntoa(clntAddr.sin_addr));
		memcpy(addr, inet_ntoa(clntAddr.sin_addr), size + 1);
		*port = ntohs(clntAddr.sin_port);

		return ret;
	}

	return -1;
}


int Socket::SendTo(int sockfd, char *buf, int len, char *addr, int port)
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family		= PF_INET;
	sin.sin_port		= htons((u_short)port);
	sin.sin_addr.s_addr	= inet_addr(addr);

	socklen_t sinLen = sizeof(sin);
	
	int ret = -1;
	ret = sendto(sockfd, buf, len, 0, (struct sockaddr *)&sin, sinLen);

	if (ret > 0) {

		printf("send %d bytes to %s:%d\n", ret,
				inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
		return ret;
	}

	return -1;
}


int Socket::SendBroadcast(int sockfd, int remote_port, char *buf, int len)
{

        int b = 1;
        struct sockaddr_in addr;

        if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&b, sizeof(b)) < 0) {
                printf("Can't set BROADCAST\n");
                return -1;
        }

        addr.sin_family      = PF_INET;
        addr.sin_port        = htons(remote_port);
        addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	
	socklen_t sinLen = sizeof(addr);

        if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                printf("Can't connect addr\n");
                printf("errno = %d(%s)\n", errno, strerror(errno));
                return -1;
        }
	
	if (sendto(sockfd, buf, len, 0, (struct sockaddr *)&addr, sinLen) < 0) {

		printf("func: is %s, send error\n", __func__);
		printf("error msg: %s, errno is: %d\n", strerror(errno), errno);
		return -1;
	}

	return 0;
}

