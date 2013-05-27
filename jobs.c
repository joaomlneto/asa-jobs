#include <stdio.h>
#include <stdlib.h>
#include "testcase.h"
#include "defs.h"

testcase read_input() {
	testcase t;
	uint num_students, num_jobs;
	uint student, application, num_applications, job;

	/* read header line */
	scanf("%lu", &num_students);
	scanf("%lu", &num_jobs);
	t = testcase_create(num_students, num_jobs);

	/* read student lines */
	for(student=0; student<num_students; student++) {
		/* how many applications for current student */
		scanf("%lu", &num_applications);
		for(application=0; application<num_applications; application++) {
			scanf("%lu", &job);
			testcase_add_application(t, student, job);
		}
	}

	return t;
}

int main() {
	testcase t = read_input();
	hopcroft_karp(t);
	printf("%lu\n", t->matching);
	testcase_destroy(t);
	return 0;
}
