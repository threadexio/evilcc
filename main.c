#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <sys/personality.h>
#include <unistd.h>

void evilcc_init(void);

int main() {
	printf("Hello world!\n");

	printf("pid: %d\n", getpid());

	uid_t ruid, euid, suid;
	assert(getresuid(&ruid, &euid, &suid) == 0);

	gid_t rgid, egid, sgid;
	assert(getresgid(&rgid, &egid, &sgid) == 0);

	printf("           |   UID     GID\n");
	printf("-----------+---------------\n");
	printf(" Real      |  %4d    %4d\n", ruid, rgid);
	printf(" Effective |  %4d    %4d\n", euid, egid);
	printf(" Saved     |  %4d    %4d\n", suid, sgid);

	int persona = personality(0xffffffff);
	assert(persona != -1);

	printf("\n");
	printf("personality: 0x%08x\n", persona);

	if (persona & ADDR_NO_RANDOMIZE)
		printf("\e[1;32m[*]\e[0m ASLR disabled!\n");
	else
		printf("\e[1;31m[*]\e[0m ASLR enabled!\n");

	return 0;
}
