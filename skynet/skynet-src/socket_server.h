#ifndef skynet_socket_server_h
#define skynet_socket_server_h

#include <stdint.h>

#define SOCKET_DATA     0
#define SOCKET_CLOSE    1
#define SOCKET_OPEN     2
#define SOCKET_ACCEPT   3
#define SOCKET_ERR      4
#define SOCKET_EXIT     5
#define SOCKET_UDP      6
#define SOCKET_WARNING  7

struct socket_server;

struct socket_message {
    int id;
    uintptr_t opaque;
    int ud;
    char *data;
}

struct socket_server *socket_server_create();
void socket_server_release(struct socket_server *);
init socket_server_poll(struct socket_server *, struct socket_message*, int more);

int socket_server_exit(struct socket_server *);
int socket_server_close(struct socket_server *, uintptr_t opaque, int id);
int socket_server_shutdown(struct socket_server *, uintptr_t opaque, int id);
int socket_server_start(struct socket_server *, uintptr_t opaque, int id);

int socket_server_send();
int socket_server_send_lowpriority();

int socket_server_listen(struct socket_server *, uintptr_t opaque, const char *addr, int port, int backlog);
int socket_server_connect(struct socket_server *, uintptr_t opaque, const char *addr, int port);
int socket_server_bind(struct socket_server *, uintptr_t opaque, int fd);

void socket_server_nodelay(struct socket_server *, int id);

struct socket_object_interface {
    void * (*buffer)(void *);
    int (*size)(void *);
    void (*free)(void *);
}

void socket_server_userobject(struct socket_server *, struct socket_object_interface *soi);

#endif
