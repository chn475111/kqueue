/**
 * @brief kqueue wrapper
 * @author lijk@infosec.com.cn
 * @version 0.0.1
 * @date 2017-9-26 15:25:33
 */
#ifndef __EVENT_KQUEUE_H__
#define __EVENT_KQUEUE_H__

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct event_kqueue_s
{
    int kq;
    int num;
    int max;
    struct  kevent *events;
}event_kqueue_t;

typedef void (*event_queue_cb)(int fd, short event, void *data);

/// return - null: failed; non-null: succeed
event_kqueue_t* event_kqueue_create(int max);

/// return - -1: failed; others: succeed
int event_kqueue_add(event_kqueue_t *h, int fd, short event, void *data);

/// return - -1: failed; others: succeed
int event_kqueue_del(event_kqueue_t *h, int fd, short event, void *data);

/// return - -1: failed; 0: timeout; others: succeed
int event_kqueue_wait(event_kqueue_t *h, int timeout);

/// return - non-return
void event_kqueue_proc(event_kqueue_t *h, event_queue_cb handler);

/// return - non-return
void event_kqueue_destroy(event_kqueue_t *h);

#ifdef __cplusplus
}
#endif

#endif
