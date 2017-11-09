CC=gcc
TSPOBJS=gpio.o linux_timestamp.o
PWMOBJS=gpio.o linux_pwm.o
all:linux_timestamp linux_pwm

linux_timestamp: $(TSPOBJS)
	$(CC) -o $@ $(TSPOBJS)
linux_pwm: $(PWMOBJS)
	$(CC) -o $@ $(PWMOBJS)
clean:
	rm *.o linux_timestamp linux_pwm
