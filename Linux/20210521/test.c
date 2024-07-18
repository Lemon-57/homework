#include <func.h>

#define LOCALIP "127.0.0.1"

int main(int argc, char **argv)
{
	struct sockaddr_in srvr;
	memset(&srvr, 0, sizeof(srvr));
	srvr.sin_family = AF_INET;
	srvr.sin_port = htons(88);
	srvr.sin_addr.s_addr = inet_addr("192.168.2.80"); // 0L;

	printf("srvr.sin_addr.s_addr = %d\n", srvr.sin_addr.s_addr);

}
