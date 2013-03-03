
# Add -DPROGRESS to add a progress indicator (of sorts)
# PROGRESS = -DPROGRESS

all: euler413

euler413: euler413.c
	gcc -Wall -O3 -o euler413 $(PROGRESS) euler413.c


clean:
	rm euler413

