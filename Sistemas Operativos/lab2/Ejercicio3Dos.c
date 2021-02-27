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
				for (i = 0; i < 1000000; i++) { printf("%d\n", i); };
			}
			else{
            	int i;
				for (i = 0; i < 1000000; i++) { printf("%d\n", i); };
				wait(NULL);
			}

		}
		else{
        	int i;
			for (i = 0; i < 1000000; i++) { printf("%d\n", i); };
			wait(NULL);

		}
	}
	else{

		wait(NULL);
		b = clock();
		double c = (double)(b - a) / CLOCKS_PER_SEC;
		printf("%f\n", c);

	}
	return 0;
}
