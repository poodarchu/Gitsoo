/*
   Listen the TCP from server2 through socket.
*/

#ifndef LISTEN_H
#define LISTEN_H

#include "Process.h"

class Listen {
	private:
		int port;
		int socket_fd;

		void CreateSocket();
	public:
		Listen(const char *);
		~Listen();

		void Solve(const Process &);
};

#endif
