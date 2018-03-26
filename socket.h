#ifndef SOCKET_H
#define SOCKET_H

extern int socket_tcp();
extern int socket_bind_reuse_listen();
extern int socket_accept();
extern int socket_connect_host();
extern int socket_connect_ulong();
extern int socket_connect_timeout();
extern int socket_connect_ndelay();

#endif
