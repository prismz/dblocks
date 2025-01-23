#include "elements.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/signal.h>
#include <X11/Xlib.h>

Display *dpy;

/* output for each element */
static char *element_buffers[LEN(elements)];

/* time since element last updated in ms */
static float element_intervals_ms[LEN(elements)];

static void setdwmstatus(char *str)
{
        if (XStoreName(dpy, DefaultRootWindow(dpy), str) < 0)
                die("XStoreName\n");
        XFlush(dpy);
}

static int sleep_ms(long ms)
{
        struct timespec ts;
        int res;

        if (ms < 0) {
                errno = EINVAL;
                return 1;
        }

        ts.tv_sec = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000;

        do {
                res = nanosleep(&ts, &ts);
        } while (res && errno == EINTR);

        return res;
}

int main(int argc, char **argv)
{
	bool use_stdout = false;
        bool show_time = false;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--stdout") == 0) {
			use_stdout = true;
                } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--showtime") == 0) {
                        show_time = true;
                } else {
                        fprintf(stderr, "unknown option %s\n", argv[i]);
                        return 1;
                }
	}

        if (!(dpy = XOpenDisplay(NULL)))
                die("XOpenDisplay\n");

        size_t status_len = 0;
        size_t status_max = 4096;
        char *status = safe_malloc(sizeof(char) * status_max);
        bzero(status, status_max);

        int n = LEN(elements);

        for (int i = 0; i < n; i++) {
               element_intervals_ms[i] = 0;
               element_buffers[i] = NULL;
        }

        for (;;) {
                clock_t start = clock();
                bool changes_made = false;

                /* updates buffers for elements that need to be updated */
                for (int i = 0; i < n; i++) {
                        Block element = elements[i];
                        if (element.interval_ms >= element_intervals_ms[i] &&
                                        element_intervals_ms[i] != 0) {
                                continue;
                        }

                        free(element_buffers[i]);

                        if (element.arg == NULL)
                                element_buffers[i] = element.func();
                        else
                                element_buffers[i] = element.func(element.arg);

                        element_intervals_ms[i] = 0;
                        changes_made = true;
                }

                char buf[1024];
                if (changes_made) {
                        bzero(status, status_max);
                        for (int i = 0; i < n; i++) {
                                if (element_buffers[i] == NULL)
                                        continue;
                                snprintf(buf, 1024, elements[i].fmt,
                                                element_buffers[i]);

                                size_t bufsize = strlen(buf);
                                if (bufsize + status_len + 1 > status_max)
                                        break;

                                strncat(status, buf, 1024);
                        }

                        clock_t end = clock();

                        double generation_time_ms = (((double)(end - start)) / CLOCKS_PER_SEC) * 1000;
                        if (show_time) {
                                char time_str[32];
                                snprintf(time_str, 32, "[%fms]", generation_time_ms);
                                strncat(status, time_str, 32);
                        }

			if (use_stdout)
				printf("%s\n", status);
			else
				setdwmstatus(status);
                }

                sleep_ms(250);
                for (int i = 0; i < n; i++)
                        element_intervals_ms[i] += 250;
        }

        free(status);
        if (XCloseDisplay(dpy) < 0)
                die("XCloseDisplay\n");
}
