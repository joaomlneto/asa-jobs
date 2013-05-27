#ifndef __TESTCASE_H__
#define __TESTCASE_H__

#include <stdbool.h>
#include "fifo.h"

typedef struct testcase {
	uint num_students;
	uint num_jobs;
	uint num_vertices;
	fifo *adjacencies;
	uint *pair_g1;
	uint *pair_g2;
	uint *dist;
	uint matching;
} testcase_t;
typedef testcase_t* testcase;

testcase testcase_create(uint num_students, uint num_jobs);
void testcase_destroy(testcase t);

void testcase_add_application(testcase t, uint student, uint job);

void testcase_print(testcase t);

uint hopcroft_karp(testcase t);
bool hopcroft_karp_bfs(testcase t);
bool hopcroft_karp_dfs(testcase t, uint v);

#endif
