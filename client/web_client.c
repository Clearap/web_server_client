#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

#define BUF_LEN 128

int main(int argc, char* argv[]){
	int s, n, len, len_out;
	struct sockaddr_in server_addr;
	char* haddr;
	char buf[BUF_LEN + 1];
	int port;

	if(argc == 3){
		port = 80;
	}else if(argc == 4){
		port = atoi(argv[3]);
	}else{
		printf("usage : webclient server_addr URL port_nunber");
		return (-1);
	}
	haddr = argv[1];
	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		printf("cannot create socket");
		return -1;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(haddr);
	server_addr.sin_port = htons(port);

	if(connect(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		printf("cannot connect");
		return -1;
	}

	sprintf(buf, "GET %s HTTP/1.0\r\n\r\n", argv[2]);
	write(s, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	while((n = read(s, buf, BUF_LEN)) > 0){
		printf("%s", buf);
		memset(buf, 0, sizeof(buf));
	}

	close(s);
}