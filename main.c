#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/personality.h>
#include <unistd.h>

extern void _start(void);

static const char* PROC_SELF_EXE = "/proc/self/exe";

__attribute__((used, naked)) void _start2(void) {
	asm volatile(
		///////////////////////////////////////////////////////////////////////////

		// getuid
		"mov $0x66, %%rax\n"
		"syscall\n"

		"cmp $0x0, %%rax\n" // TODO: replace with the actual uid
		"je .uid_setup_done\n"

		// setresuid
		"mov $0x0, %%rdx\n" // TODO: replace with the actual uid
		"mov %%rdx, %%rsi\n"
		"mov %%rdx, %%rdi\n"
		"mov $0x75, %%rax\n"
		"syscall\n"

		".uid_setup_done:\n"

		///////////////////////////////////////////////////////////////////////////

		// getgid
		"mov $0x68, %%rax\n"
		"syscall\n"

		"cmp $0x0, %%rax\n" // TODO: replace with actual gid
		"je .gid_setup_done\n"

		// setresgid
		"mov $0x0, %%rdx\n" // TODO: replace with actual gid
		"mov %%rdx, %%rsi\n"
		"mov %%rdx, %%rdi\n"
		"mov $0x77, %%rax\n"
		"syscall\n"

		".gid_setup_done:\n"

		///////////////////////////////////////////////////////////////////////////

		// personality(0xffffffff)
		"xor %%rdi, %%rdi\n"
		"not %%rdi\n"
		"mov $0x87, %%rax\n"
		"syscall\n"

		"mov %%rax, %%rdi\n"
		"and %[ADDR_NO_RANDOMIZE], %%eax\n"
		"test %%rax, %%rax\n"
		"jnz .personality_setup_done\n"

		// personality(persona | ADDR_NO_RANDOMIZE)
		"or %[ADDR_NO_RANDOMIZE], %%edi\n"
		"mov $0x87, %%rax\n"
		"syscall\n"

		// argc
		"mov (%%rsp), %%eax\n"
		"inc %%rax\n"

		// execve("/proc/self/exe", argv, envp)
		"lea 8(%%rsp, %%rax, 8), %%rdx\n" // envp
		"lea 8(%%rsp), %%rsi\n"           // argv
		"mov %[PROC_SELF_EXE], %%rdi\n"
		"mov $0x3b, %%rax\n"
		"syscall\n"

		"ud2\n"

		".personality_setup_done:\n"
		:
		: [ADDR_NO_RANDOMIZE] "i"(ADDR_NO_RANDOMIZE),
		  [PROC_SELF_EXE] "m"(PROC_SELF_EXE)
		: "rax", "rdi", "rsi", "rdx", "cc");

	// "return" to _start
	asm volatile("jmp *%0" : : "r"(_start));
}

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
