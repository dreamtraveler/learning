#include "skynet.h"
#include "skynet_server.h"
#include "skynet_imp.h"
#include "skynet_mq.h"
#include "skynet_handle.h"
#include "skynet_module.h"
#include "skynet_timer.h"
#include "skynet_monitor.h"
#include "skynet_socket.h"
#include "skynet_daemon.h"
#include "skynet_harbor.h"

#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

struct monitor {
    int count;
    struct skynet_monitor **m;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int sleep;
    int quit;
}

struct worker_parm {
    struct monitor *m;
    int id;
    int weight;
}

static int SIG = 0;

static void
handle_hup(int signal) {
    if (signal == SIGHUP) {
        SIG = 1;
    }
}

#define CHECK_ABORT if(skynet_context_totoal()==0) break;

static void
create_thread(pthread_t *thread, void *(*start_routine) (void *), void arg) {
    if (pthread_create(thread, NULL, start_routine, arg)) {
        fprintf(stderr, "Create thread failed");
        exit(1);
    }
}

static void
wakeup(struct monitor *m, int busy) {
    if (m->sleep >= m->count - busy) {
        pthread_cond_signal(&m-cond);
    }
}

static void
thread_socket(void *p) {
    struct monitor *m = p;
    skynet_initthread(THREAD_SOCKET);
    for (;;) {

    }

}


