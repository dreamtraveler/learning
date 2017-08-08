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
    struct epoll_event ev;
    ev.events = EPOLLIN | (enable ? EPOLLOUT : 0);
    ev.data.ptr = ud;
    epoll_ctl(efd, EPOLL_CTL_MOD, sock, &ev);
}

static int
sp_wait(poll_fd, struct event *e, int max) {
    struct epoll_event ev[max];
    int n = epoll_wait(efd, ev, max, -1);
    int i;
    for (i = 0; i < n; i++) {
        unsigned flag = ev[i].events;
        e[i].s     = ev[i].data.ptr;
        e[i].write = (flag & EPOLLOUT) != 0;
        e[i].read  = (flag & EPOLLIN | EPOLLHUP) != 0;
        e[i].error = (flag & EPOLLERR) != 0;
    }
    return n;
}

static void
sp_nonblocking(int fd) {
    int flag = fcntl(fd, F_GETEL, 0);
    if (-1 != flag) {
        fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    }
}





static void
start(int thread) {
    pthread_t pid[thread+3];
    struct monitor *m = skynet_malloc(sizeof(*m));
    memset(m, 0, sizeof(*m));
    m->count = thread;
    m->sleep = 0;

    m->m = skynet_malloc(thread * sizeof(struct skynet_monitor *));
    int i;
    for(i = 0; i < thread; i++) {
        m->m[i] = skynet_monitor_new();
    }
    if (pthread_mutex_init(&m->mutex, NULL)) {
        fprintf(stderr, "Init cond error");
        exit(1);
    }
    if (pthread_cond_init(&m->cond, NULL)) {
        fprintf(stderr, "Init cond error");
        exit(1);
    }
    create_thread(&pid[0], thread_monitor, m);
    create_thread(&pid[1], thread_timer, m);
    create_thread(&pid[2], thread_socket, m);

    static int weight[] = {
        -1, -1, -1, -1, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3,
    };
    struct worker_parm wp[thread];
    for (i = 0; i < thread; i++) {
        wp[i].m = m;
        wp[i].id = i;
        if (i < sizeof(weight)/sizeof(weight[0])) {
            wp[i].weight = weight[i];
        } else {
            wp[i].weight = 0;
        }
    }
    for (i = 0; i < thread; i++) {
        create_thread(pid[i], NULL);
    }

    for (i = 0; i < thread + 3; i++) {
        pthread_join(pid[i], NULL);
    }

    free_monitro(m);
}

#endif
