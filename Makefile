CFLAGS := -Wall -Wextra -fPIE -pie -fstack-protector-all -D_FORTIFY_SOURCE=2 -g3 -D__EVILCC_PROMOTE_UID -D__EVILCC_PROMOTE_GID
LFLAGS := -Wl,-z,now -Wl,-z,relro

all: main main2
	sudo chown -v root:root main main2
	sudo chmod +s main main2

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

main2: main.c start2.c
	$(CC) $(CFLAGS) -Wl,-e"__evilcc_entry" $^ -o $@ $(LFLAGS)
