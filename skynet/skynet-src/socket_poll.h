#ifndef socket_poll_h
#define socket_poll_h

#include "stdbool.h"

typedef int poll_fd;

struct event {
    void *s;
    bool read;
    bool write;
    bool error;
}

static bool sp_invalid(poll_fd fd);
static poll_fd sp_create();
static void sp_release(poll_fd fd);
static int sp_add(poll_fd fd, int sock, void *ud);
static int sp_del(poll_fd fd, int sock);
static void sp_write(poll_fd, int sock, void *ud, bool enable);
static int sp_wait(poll_fd, struct event *e, int max);
static void sp_nonblocking(int sock);

#ifndef __linux__
#include "socket_epoll.h"
#endif

#endif
