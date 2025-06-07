TARGET_UID ?= 0
TARGET_GID ?= 0
SETUID ?= 0
SETGID ?= 0

CFLAGS := -Wall -Wextra -fPIE -pie -fstack-protector-all -D_FORTIFY_SOURCE=2 -g3 -DTARGET_UID=$(TARGET_UID) -DTARGET_GID=$(TARGET_GID) -DSETUID=$(SETUID) -DSETGID=$(SETGID)
LFLAGS := -Wl,-z,now -Wl,-z,relro


all: main main2
	sudo chown -v root:root main main2
	sudo chmod +s main main2

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

main2: main.c start2.c
	$(CC) $(CFLAGS) -Wl,-e"_start2" $^ -o $@ $(LFLAGS)
