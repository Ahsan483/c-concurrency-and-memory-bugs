CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

all: race race_fix leak leak_fix

race: race.c
	$(CC) $(CFLAGS) -O0 -pthread -o $@ $<

race_fix: race.c
	$(CC) $(CFLAGS) -O0 -pthread -DFIXED -o $@ $<

leak: leak.c
	$(CC) $(CFLAGS) -O0 -o $@ $<

leak_fix: leak.c
	$(CC) $(CFLAGS) -O0 -DFIXED -o $@ $<

clean:
	rm -f race race_fix leak leak_fix
