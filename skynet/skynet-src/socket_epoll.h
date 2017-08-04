#ifndef poll_socket_epoll_h
#define poll_socket_epoll_h

#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

static bool
sp_invalid(int efd) {
    return efd == -1;
}

static int sp_create {
    return epoll_create(1024);
}

static void
sp_release(poll_fd fd) {
    close(efd);
}

static int
sp_add(poll_fd efd, int sock, void *ud) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = ud;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, sock, &ev) == -1) {
        return 1;
    } else {
        return 0;
    }
}

static int
sp_del(poll_fd fd, int sock) {
    epoll_ctl(efd, EPOLL_CTL_DEL, sock, NULL);
}

static void
sp_write(poll_fd, int sock, void *ud, bool enable) {

}

static int
sp_wait(poll_fd, struct event *e, int max) {

}

static void
sp_nonblocking(int sock) {

}

#endif
