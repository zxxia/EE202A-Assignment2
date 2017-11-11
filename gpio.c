#include "gpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

int gpio_export(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYSFS_GPIO_DIR"/export", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    if (len >= 0 && len < sizeof(buf)) {
        write(fd, buf, len);
    }

    close(fd);
}



int gpio_unexport(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYSFS_GPIO_DIR"/unexport", O_WRONLY);
    if (fd < 0) {
        perror("gpio/unexport");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    if (len >= 0 && len < sizeof(buf)) {
        write(fd, buf, len);
    }

    close(fd);
}



int gpio_set_direction(unsigned int gpio, unsigned int dir)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);

    if (fd < 0) {
        perror("gpio/direction");
        return fd;
    }

    if (dir == GPIO_DIR_OUTPUT) {
        write(fd, "out", 3);
    }
    else {
        write(fd, "in", 2);
    }
}


int gpio_set_edge(unsigned int gpio, const char* edge)
{

    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/edge", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-edge");
        return fd;
    }

    write(fd, edge, strlen(edge));
    close(fd);
    return 0;
}


int gpio_set_value(unsigned int gpio, unsigned int val)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-value");
        return fd;
    }

    if (val) {
        write(fd, "1", 1);
    }
    else {
        write(fd, "0", 2);
    }
    close(fd);
    return 0;
}


int gpio_fd_open(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/value", gpio);
    fd = open(buf, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("gpio/fd_open");
    }
    return fd;
}


int gpio_fd_close(int fd)
{
    return close(fd);
}
