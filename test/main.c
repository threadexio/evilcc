#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <sys/personality.h>
#include <unistd.h>

static void print_array(const char** arr, size_t limit) {
	size_t i = 0;
	const char** p = arr;

	if (*p == NULL) {
		printf("[ ]");
		return;
	}

	printf("[ \"%s\"", *p++);
	i++;

	for (; *p != NULL && i < limit; p++, i++)
		printf(", \"%s\"", *p);

	if (*p != NULL && i == limit)
		printf(", ...");

	printf(" ]");
}

int main(int argc, const char** argv, const char** envp) {
	printf("Hello world!\n");
	printf("pid: %d\n", getpid());
	printf("argv: ");
	print_array(argv, argc);
	printf("\n");
	printf("envp: ");
	print_array(envp, 2);
	printf("\n");
	printf("\n");

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

	if (persona & ADDR_NO_RANDOMIZE)
		printf("\e[1;31m[*]\e[0m ASLR disabled!\n");
	else
		printf("\e[1;32m[*]\e[0m ASLR enabled!\n");

	return 0;
}
