#include <stdio.h>

int main(int argc, char* argv[]) {
	int i, j, k;
	int input_size = strtol(argv[1], NULL, 10);

	/* print first line */
	printf("%d %d\n", input_size, input_size);

	/* print student lines */
	for(j=0; j<input_size; j++) {
		printf("%d", input_size);
		for(k=0; k<input_size; k++) {
			printf(" %d", k);
		}
		printf("\n");
	}


	return 0;
}
