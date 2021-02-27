#include <stdio.h>
#include <unistd.h>
int main() {

	int a = fork();
	if (a == 0) {
        int i;
		for (i = 0; i < 1000000; ++i) {
			printf("%d\n", i);
		}
	}
	else
	{
		while (1);
	}
}
