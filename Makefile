main: main.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -Wall -Wextra -c $^ -o $@
