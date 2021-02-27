#include <stdio.h>
#include <unistd.h>
int main() {

	int a = fork();

	if (a == 0) {
		printf("Equis de\n");
	}
	else
	{
		while (true) {};
	}
}
