#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int r() {
	time_t s;
	time(&s);
	srand((unsigned int) s);
}

int main(int argc, char* argv[]) {
	int i, j, k;
	int input_size = strtol(argv[1], NULL, 10);
	int num_applications;

	/* initialize random number generator */
	r();

	/* print first line */
	printf("%d %d\n", input_size, input_size);

	/* print student lines */
	for(i=0; i<input_size; i++) {
		num_applications = rand()%input_size;
		printf("%d", num_applications);

		/* print student applications */
		for(j=0; j<num_applications; j++) {
			printf(" %d", rand()%input_size);
		}

		printf("\n");
	}


	return 0;
}
