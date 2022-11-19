#include "elements.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/signal.h>
#include <X11/Xlib.h>

Display *dpy;
static char *element_buffers[LEN(elements)];
static int element_intervals[LEN(elements)];

void setstatus(char *str)
{
        if (XStoreName(dpy, DefaultRootWindow(dpy), str) < 0)
                die("XStoreName\n");
        XFlush(dpy);
}

int main(int argc, char **argv)
{
	bool use_stdout = false;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-s") == 0 ||
				strcmp(argv[i], "--stdout") == 0)
			use_stdout = true;
	}

        if (!(dpy = XOpenDisplay(NULL)))
                die("XOpenDisplay\n");

        size_t status_len = 0;
        size_t status_max = 4096;
        char *status = safe_malloc(sizeof(char) * status_max);
        bzero(status, status_max);

        int n = LEN(elements);

        for (int i = 0; i < n; i++) {
               element_intervals[i] = 0;
               element_buffers[i] = NULL;
        }

        for (;;) {
                bool changes_made = false;
                for (int i = 0; i < n; i++) {
                        Block element = elements[i];
                        if (element.interval != element_intervals[i] &&
                                        element_intervals[i] != 0) {
                                continue;
                        }

                        free(element_buffers[i]);

                        if (element.arg == NULL)
                                element_buffers[i] = element.func();
                        else
                                element_buffers[i] = element.func(element.arg);

                        element_intervals[i] = 0;
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

			if (use_stdout)
				printf("%s\n", status);
			else
				setstatus(status);
                }

                sleep(1);
                for (int i = 0; i < n; i++)
                        element_intervals[i]++;
        }

        free(status);
        if (XCloseDisplay(dpy) < 0)
                die("XCloseDisplay\n");
}
