#include <stdio.h>
#include <time.h>

int main()
{
	clock_t a = clock();
	clock_t b;

	int i;

	for (i = 0; i < 1000000; i++) {
            printf("%d\n", i);
    };
	for (i = 0; i < 1000000; i++) {
            printf("%d\n", i);
    };
	for (i = 0; i < 1000000; i++) {
            printf("%d\n", i);
    };
	b = clock();
	double c = (double)(b - a)/CLOCKS_PER_SEC;
	printf("%f\n", c);
	return 0;
}
