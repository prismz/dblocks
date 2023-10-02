#include "elements.h"

typedef struct {
        char *(*func)();
        char *arg;
        char *fmt;
        int interval;
} Block;

/*
 * music         cmus metadata           arg: NULL
 * get_disk_rw   disk r/w speed in MB/s  arg: device name
 * keymap        X11 keymap              arg: NULL
 * uptime        system uptime           arg: NULL
 * command       runs a command          arg: command
 * portage       gentoo packages         arg: NULL
 * loadavg       CPU load average        arg: 1, 5, 15 or all (man 3 getloadavg)
 * memory        memory usage in MiB     arg: NULL
 * datetime      date and time           arg: time format (man 3 strftime)
 */

static const Block elements[] = {
        { music,       NULL,                         "[%s]", 1 },
        { get_disk_rw, "nvme0n1",                        "[%s]", 1 },
        { keymap,      NULL,                         "[%s]", 1 },
        { uptime,      NULL,                         "[%s]", 50 },
/*        { command,
                "awk -F\"[][]\" '/Left:/ { print $2 }' <(amixer sget Master)",
                "[vol: %s]", 5 }, */
        { portage,     NULL,                         "[%s pkgs]", 45 },
        { loadavg,     "1",                          "[%s]", 5 },
        { memory,      NULL,                         "[%sMiB]", 3 },
        { datetime,    "%a %b %d %T",       "[%s]", 1 },
};
