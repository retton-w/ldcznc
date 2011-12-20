#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include "image_buffer.h"

#include "tcp_server.h"
#include "udp_server.h"


int main(int argc, char *argv[])
{

	TcpClient *tcp_client = new TcpClient();
	TcpServer *tcp_server = new TcpServer(tcp_client);
	UdpServer udp_server;

	struct env_image_buffer env_image_buffer;

        env_image_buffer.buf_len          = 1024 * 1024;
        env_image_buffer.buf_num          = 8;
        env_image_buffer.resd_buf_num_max = 1;
        env_image_buffer.capt_buf_num_max = 4;
        env_image_buffer.tran_buf_num_min = 0;
        env_image_buffer.tran_buf_num_max = 3;

        image_buffer_init(&env_image_buffer);

	tcp_client->Start();
	tcp_server->Start();
	printf("start Tcp Server\n");
	udp_server.Start();

	while(1) {

		sleep(10);
	}

	return 0;
}

