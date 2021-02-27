#include <stdio.h>
#include <time.h>

int main()
{
	clock_t a = clock();

	int i;

	for (i = 0; i < 1000000; i++) {

	};
	for (i = 0; i < 1000000; i++) {

	};
	for (i = 0; i < 1000000; i++) {

	};
	clock_t b = clock();
	double c = (double)b - a;
	printf("%f\n", c);
	return 0;
}
