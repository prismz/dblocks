#include "elements.h"

typedef struct {
        char *(*func)();
        char *arg;
        char *fmt;
        int interval;
} Block;

static const Block elements[] = {
        { music,       NULL,                         "[%s]", 1 },
        { get_disk_rw, "sdb",                        "[%s]", 1 },
        { keymap,      NULL,                         "[%s]", 5 },
        { uptime,      NULL,                         "[%s]", 50 },
        { command,
                "awk -F\"[][]\" '/Left:/ { print $2 }' <(amixer sget Master)",
                "[vol: %s]", 5 },
        { portage,     NULL,                         "[%s pkgs]", 45 },
        { loadavg,     "1",                          "[%s]", 5 },
        { memory,      NULL,                         "[%sMiB]", 3 },
        { datetime,    "%a %b %d %I:%M:%S %p",       "[%s]", 1 },
};
