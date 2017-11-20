#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	printf("Real user id = %d, effective user id = %d\n", getuid(), geteuid());

	seteuid(1001);
	printf("Real user id = %d, effective user id = %d\n", getuid(), geteuid());

	seteuid(0);
	printf("Real user id = %d, effective user id = %d\n", getuid(), geteuid());
	
	return 0;
}
