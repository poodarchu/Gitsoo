#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "Listen.h"

using namespace std;

Listen::Listen(const char * _port) {
	sscanf(_port,"%d",&port);
}

void Listen::CreateSocket() {
	sockaddr_in server;
	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd<0) {
		cerr<<"Socket() Error.\n";
		exit(1);
	}

	memset(&server,0,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	server.sin_port=htons(port);

	if(bind(socket_fd,(sockaddr*)&server,sizeof(server))<0) {
		cerr<<"Bind Error.\n";
		exit(1);
	}

	if(listen(socket_fd,10)<0) {
		cerr<<"Listen Error.\n";
		exit(1);
	}
}

void Listen::Solve(const Process & pro) {
	CreateSocket();

	while(1) {
		int acc_fd;
		sockaddr_in client;
		unsigned int sin_size=sizeof(sockaddr_in);

		if((acc_fd=accept(socket_fd,(sockaddr*)&client,&sin_size))==-1) {
			cerr<<"Accept Error.\n";
			continue;
		}

		pro(acc_fd);
		close(acc_fd);
	}
}

Listen::~Listen() {
	close(socket_fd);
}
