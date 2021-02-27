#include <stdio.h>
#include <unistd.h>

int main(){
    int a = fork();
	int b = fork();
	int c = fork();
	int d = fork();
	printf("%d\n%d\n%d\n%d\n", a, b, c, d);
    return 0;
}
