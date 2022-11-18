#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "elements/util.h"

char *datetime(char *fmt);
char *memory(void);
char *loadavg(char *fmt);
char *portage(void);
char *uptime(void);
char *keymap(void);
char *music(void);
char *command(char *command);
char *separator(char *s);
char *get_disk_rw(char *dev);

#endif
