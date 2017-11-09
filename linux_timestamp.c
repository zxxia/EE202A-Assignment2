#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define POLL_TIMEOUT_INF -1

#define BILLION 1000000000.0


int running = 1;

void signal_handler(int sig)
{
    running = 0;
}

int main (int argc, char* argv[])
{
    struct sigaction act;
    struct pollfd pollfdset;
    const int nfds = 1;
    int timeout, rv;
    unsigned int gpio;
    int len;
    clockid_t clk_id = CLOCK_MONOTONIC;
    struct timespec start_tp;
    struct timespec event_tp;
    int timestamping_started = 0;

    act.sa_handler = signal_handler;
    rv = sigaction(SIGINT, &act, NULL);

    if (rv == -1) {
        perror("signaction");
        exit(EXIT_FAILURE);
    }

    gpio_export(gpio);
    gpio_set_dir(gpio, GPIO_DIR_IN);

    memset((void*) &pollfdset, 0, sizeof(pollfdset));

    pollfdset.fd = gpio_fd_open(gpio);
    pollfdset.events = POLLPRI;
    gpio_set_edge(gpio, GPIO_RISING_EDGE);

    while(running) {
        pollfdset.revents = 0;
        rv = poll(&pollfdset, nfds, POLL_TIMEOUT_INF);

        if (rv < 0) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if (pollfdset.revents & POLLPRI) {
            // Rising arrived
            clock_gettime(clk_id, &event_tp);
            if (!timestamping_started) {
                memcpy(&start_tp, &event_tp, sizeof(event_tp));
                timestamping_started = 1;
            }

            float timestamp = (float)(event_tp.tv_sec - start_tp.tv_sec) + (event_tp.tv_nsec - start_tp.tv_nsec) / BILLION;

            fprintf(stdout, "%f\n", timestamp);
        }
    }
    gpio_fd_close(pollfdset.fd);
    gpio_unexport(gpio);
    exit(EXIT_SUCCESS);
}