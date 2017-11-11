#ifndef _GPIO_H_
#define _GPIO_H

#define GPIO_DIR_OUTPUT 1
#define GPIO_DIR_INPUT 0

static const char* GPIO_RISING_EDGE = "rising";
static const char* GPIO_FALING_EDGE = "falling";
static const char* GPIO_BOTH_EDGE = "both";
/*
 * @ return 0 on sucess
 *
 */

/*
 * gpio_export
 * export a gpio pin for use in the user space.
 * must be aclled before the pin can be used.
 */
int gpio_export(unsigned int gpio);

/*
 * gpio_unexport
 * undo the export action.
 */
int gpio_unexport(unsigned int gpio);

/*
 * gpio_set_dir
 * set direction of gpio pin either as input or output
 */
int gpio_set_direction(unsigned int gpio, unsigned int dir);

int gpio_set_value(unsigned int gpio, unsigned int val);

int gpio_set_edge(unsigned int gpio, const char *edge);

int gpio_fd_open(unsigned int gpio);

int gpio_fd_close(int fd);

#endif
