#include "elements.h"

typedef struct {
        char *(*func)();
        char *arg;
        char *fmt;
        long interval_ms;
} Block;

/*
 * (playerctl required for these)
 * music_tidal   tidal-hifi track info         arg: NULL
 * music_spotify spotify track info            arg: NULL
 * music_cmus    cmus track info               arg: NULL
 *
 * command       runs a command                arg: command
 *
 * get_disk_rw   disk r/w speed in MB/s        arg: device name (dmcrypt => dm-0,1,2,etc)
 * keymap        X11 keymap                    arg: NULL
 * uptime        system uptime                 arg: NULL
 * portage       number of gentoo packages     arg: NULL
 * loadavg       CPU load average              arg: 1, 5, 15 or all (man 3 getloadavg)
 * memory        memory usage in MiB           arg: NULL
 * datetime      date and time                 arg: time format (man 3 strftime)
 * temp_sensor   temp sensor reading           arg: hwmon path (/sys/class/hwmon/hwmon1/temp1_input)
 * temp_sensor_f  (same as above, just in farenheit)
 */

/* global update interval in ms */
#define UPDATE_INTERVAL_MS 250

static const Block elements[] = {
//      { music_tidal,   NULL,                                       "[%s]",            1000 },
//      { music_cmus,    NULL,                                       "[%s]",            1000 },
        { music_spotify, NULL,                                       "[%s]",            300 },
//      { get_disk_rw,   "dm-0",                                     "[%s]",            3000 },
        { keymap,        NULL,                                       "[%s]",            500 },
//      { uptime,        NULL,                                       "[%s]",            50000 },
//      { command,       "qlop -r",                                  "[%s]",            30000 },
        { command,       "pulseaudio-volume",                        "[vol: %s]",       200 },

//      { portage,       NULL,                                       "[portage: %s]",   45000 },
//      { temp_sensor,   "/sys/class/hwmon/hwmon3/temp1_input",      "[GPU: %s]",       2000 },
//      { temp_sensor,   "/sys/class/hwmon/hwmon1/temp1_input",      "[CPU: %s]",       2000 },
        { loadavg,       "1",                                        "[%s]",            2000 },
        { memory,        NULL,                                       "[%sMiB]",         200 },
        { datetime,      "%a %b %d %I:%M:%S %p %Y",                  "[%s]",            500 },
};
