main: main.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -Wall -Wextra -g3 -c $^ -o $@
