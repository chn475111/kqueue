#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "event_kqueue.h"

event_kqueue_t* event_kqueue_create(int max)
{
    event_kqueue_t *h = (event_kqueue_t*)malloc(sizeof(event_kqueue_t));
    if(h == NULL)
        return NULL;
    memset(h, 0, sizeof(event_kqueue_t));

    h->events = (struct kevent*)malloc(max*sizeof(struct kevent));
    if(h->events == NULL)
        goto ErrP;
    memset(h->events, 0, max*sizeof(struct kevent));

    h->kq = kqueue();
    if(h->kq == -1)
        goto ErrP;

    h->num = 0;
    h->max = max;
    return h;
ErrP:
    if(h)
    {
        if(h->events)
            free(h->events);
        if(h->kq > 0)
            close(h->kq);
        free(h);
    }
    return NULL;
}

int event_kqueue_add(event_kqueue_t *h, int fd, short event, void *data)
{
    struct kevent kev;

    kev.ident = fd;
    kev.filter = event;
    kev.flags = EV_ADD;
    kev.fflags = 0;
    kev.data = 0;
    kev.udata = data;

    return kevent(h->kq, &kev, 1, NULL, 0, NULL);
}

int event_kqueue_del(event_kqueue_t *h, int fd, short event, void *data)
{
    struct kevent kev;

    kev.ident = fd;
    kev.filter = event;
    kev.flags = EV_DELETE;
    kev.fflags = 0;
    kev.data = 0;
    kev.udata = data;

    return kevent(h->kq, &kev, 1, NULL, 0, NULL);
}

int event_kqueue_wait(event_kqueue_t *h, int timeout)
{
    struct timespec ts;

    ts.tv_sec = timeout/1000;
    ts.tv_nsec = (timeout%1000)*1000*1000;

    h->num = kevent(h->kq, NULL, 0, h->events, h->max, &ts);
    return h->num;
}

void event_kqueue_proc(event_kqueue_t *h, event_queue_cb handler)
{
    int i;
    for(i = 0; i < h->num; i++)
    {
        if(handler) handler(h->events[i].ident, h->events[i].filter, h->events[i].udata);
    }
}

void event_kqueue_destroy(event_kqueue_t *h)
{
    if(h)
    {
        if(h->events)
            free(h->events);
        if(h->kq > 0)
            close(h->kq);
        free(h);
    }
}
