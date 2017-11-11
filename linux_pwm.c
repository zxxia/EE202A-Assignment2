#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BILLION 1000000000.0

int count_timestamps(FILE* fp)
{
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;

    /* count the number of lines in the file */
    count = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        count++;
    }
    /* go back to the start of the file so that the data can be read */
    rewind(fp);
    return count;
}

int main (int argc, char* argv[])
{
    int rv;
    unsigned int gpio;
    FILE *fp;
    char* line;
    int n_timestamps;
    size_t len;
    ssize_t read;
    float* timestamps;
    int i;
    char* filename;
    struct timespec interval;
    struct timespec rem;

    if (argc < 2) {
        fprintf(stdout, "Usage: ./linux_pwm timestampFile\n");
        exit(EXIT_FAILURE);
    }

    // Parse input file
    filename = argv[1];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    n_timestamps = count_timestamps(fp);

    timestamps = (float*) malloc(sizeof(float) * n_timestamps);
    i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        rv = sscanf(line, "%f\n", &timestamps[i]);
        if (rv != 1) {
            fprintf(stderr, "Failed to read line %d \'%s\' in %s.\n", i, line, filename);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }

    // Prepare gpio pin
    gpio_export(gpio);
    gpio_set_direction(gpio, GPIO_DIR_OUTPUT);

    // Generate pwm
    i = 1;
    while(i < n_timestamps) {
        gpio_set_value(gpio, 1);
        interval.tv_sec = 0;
        interval.tv_nsec = 1000; // pulse length is at least 10us
        nanosleep(&interval, &rem);

        float diff = timestamps[i] - timestamps[i - 1];
        interval.tv_sec = diff;
        interval.tv_nsec = (long)(diff * BILLION) - (long)(interval.tv_sec * BILLION);
        gpio_set_value(gpio, 0);
        nanosleep(&interval, &rem);
    }


    // Recycle resources
    free(timestamps);
    gpio_unexport(gpio);
    fclose(fp);
    exit(EXIT_SUCCESS);
}
