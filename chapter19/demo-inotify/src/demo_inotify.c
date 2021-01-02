#include "demo_inotify.h"
#include <sys/inotify.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static void displayInotifyEvent(struct inotify_event *);

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int demoInotify(int argc, char *argv[]) {
    int inotifyFd, wd, j;
    char buf[BUF_LEN];
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s pathname...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    inotifyFd = inotify_init();
    if (inotify_init() == -1) {
        fprintf(stderr, "failed to call inotify_init(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (j = 1; j < argc; j++) {
        wd = inotify_add_watch(inotifyFd, argv[j], IN_ALL_EVENTS);
        if (wd == -1) {
            fprintf(stderr, "failed to call inotify_add_watch(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        fprintf(stdout, "Watching %s using wd %d\n", argv[j], wd);
    }

    for (;;) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0) {
            fprintf(stderr, "read() from inotify fd return 0!");
            break;
        }

        if (numRead == -1) {
            fprintf(stderr, "failed to call read(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        fprintf(stdout, "Read %ld bytes from inotify fd\n", numRead);

        for (p = buf; p < buf + numRead;) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    exit(EXIT_SUCCESS);
}

static void displayInotifyEvent(struct inotify_event *i) {
    fprintf(stdout, "   wd=%2d; ", i->wd);

    if (i->cookie > 0)
        fprintf(stdout, "cookie =%4d; ", i->cookie);

    fprintf(stdout, "mask = ");
    if (i->mask & IN_ACCESS)
        fprintf(stdout, "IN_ACCESS ");

    if (i->mask & IN_ATTRIB)
        fprintf(stdout, "IN_ATTRIB ");

    if (i->mask & IN_CLOSE_NOWRITE)
        fprintf(stdout, "IN_CLOSE_NOWRITE ");

    if (i->mask & IN_CLOSE_WRITE)
        fprintf(stdout, "IN_CLOSE_WRITE ");

    if (i->mask & IN_CREATE)
        fprintf(stdout, "IN_CREATE ");

    if (i->mask & IN_DELETE)
        fprintf(stdout, "IN_DELETE ");

    if (i->mask & IN_DELETE_SELF)
        fprintf(stdout, "IN_DELETE_SELF ");

    if (i->mask & IN_IGNORED)
        fprintf(stdout, "IN_IGNORED ");

    if (i->mask & IN_ISDIR)
        fprintf(stdout, "IN_ISDIR ");

    if (i->mask & IN_MODIFY)
        fprintf(stdout, "IN_MODIFY ");

    if (i->mask & IN_MOVE_SELF)
        fprintf(stdout, "IN_MOVE_SELF ");

    if (i->mask & IN_MOVED_FROM)
        fprintf(stdout, "IN_MOVED_FROM ");

    if (i->mask & IN_MOVED_TO)
        fprintf(stdout, "IN_MOVED_TO ");

    if (i->mask & IN_OPEN)
        fprintf(stdout, "IN_OPEN ");

    if (i->mask & IN_Q_OVERFLOW)
        fprintf(stdout, "IN_Q_OVERFLOW ");

    if (i->mask & IN_UNMOUNT)
        fprintf(stdout, "IN_UNMOUNT ");
    fprintf(stdout, "\n");

    if (i->len > 0)
        fprintf(stdout, "   name = %s\n", i->name);
}