#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main()
{
	clock_t a, b;
	pid_t c, d, e;

	a = clock();
	c = fork();
	if (c == 0){
		d = fork();
		if (d == 0){
			e = fork();
			if (e == 0){
                int i;
				for (i = 0; i < 1000000; i++);
			}
			else{
                int i;
				for (i = 0; i < 1000000; i++);
				wait(NULL);
			}
		}
		else{
            int i;
			for (i = 0; i < 1000000; i++);
			wait(NULL);
		}
	}
	else{
		wait(NULL);
		b = clock();
		printf("%f\n", (double)b - a);
	}
	return 0;
}
