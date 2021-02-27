#include <stdio.h>
#include <unistd.h>

int main(){

	int i = 0;
	for (i; i<4; i++){
		int j = fork();
		printf("%d\n",j);
	}

	return 0;
}
