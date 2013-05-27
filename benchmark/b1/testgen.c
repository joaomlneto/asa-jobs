#include <stdio.h>

#define NUM_TESTS 10

int main(int argc, char* argv[]) {
	int i, j, k;
	int input_size = strtol(argv[1], NULL, 10);

	for(i=0; i<NUM_TESTS; i++) {

		/* print first line */
		printf("%d %d\n", input_size, input_size);
	
		/* print student lines */
		for(j=0; j<input_size; j++) {
			printf("%d %d\n", 1, j);
		}

	}

	return 0;
}
