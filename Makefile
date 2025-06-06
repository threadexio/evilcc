CFLAGS := -Wall -Wextra -fPIE -pie -fstack-protector-all -D_FORTIFY_SOURCE=2 -g3
LFLAGS := -Wl,-z,now -Wl,-z,relro

all: main main2
	sudo chown -v kat:users main main2
	# sudo chmod +s main main2

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

main2: main.c
	$(CC) $(CFLAGS) -Wl,-e"_start2" $^ -o $@ $(LFLAGS)
