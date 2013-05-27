#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "testcase.h"
#include "fifo.h"

uint job_index(testcase t, uint job) {
	return t->num_students + job + 1;
}

uint student_index(testcase t, uint student) {
	return student + 1;
}

testcase testcase_create(uint num_students, uint num_jobs) {
	uint i;
	uint num_vertices = num_students + num_jobs + 1;
	testcase t = malloc(sizeof(testcase_t));
	t->num_students = num_students;
	t->num_jobs = num_jobs;
	t->num_vertices = num_vertices;

	t->adjacencies = malloc(num_vertices*sizeof(fifo));
	for(i=0; i<num_vertices; i++) {
		t->adjacencies[i] = fifo_create();
	}

	t->pair_g1 = malloc(num_vertices*sizeof(uint));
	t->pair_g2 = malloc(num_vertices*sizeof(uint));
	t->dist = malloc(num_vertices*sizeof(uint));
	t->matching = 0;
	return t;
}

void testcase_destroy(testcase t) {
	uint i;
	for(i=0; i<t->num_vertices; i++) {
		fifo_destroy(t->adjacencies[i]);
	}
	free(t->adjacencies);
	free(t->pair_g1);
	free(t->pair_g2);
	free(t->dist);
	free(t);
}

void testcase_add_application(testcase t, uint student, uint job) {
	fifo_queue(t->adjacencies[student_index(t, student)], job_index(t, job));
	fifo_queue(t->adjacencies[job_index(t, job)], student_index(t, student));
}

void testcase_print(testcase t) {
	uint i;
	printf("==========[TESTCASE]==========\n");
	printf("num_students = %lu\n", t->num_students);
	printf("num_jobs = %lu\n", t->num_jobs);
	printf("matching = %lu\n", t->matching);
	printf("=========[ADJACENCIES]=========\n");
	for(i=0; i<t->num_vertices; i++) {
		printf("Adjacencies for vertex %lu: ", i);
		fifo_print(t->adjacencies[i]);
		printf("\n");
	}
	printf("====[OTHERS]==== (pair_g1, pair_g2, dist)\n");
	for(i=0; i<t->num_vertices; i++) {
		printf("Vertex %lu: pair_g1=%lu pair_g2=%lu dist=%lu\n", i, t->pair_g1[i], t->pair_g2[i], t->dist[i]);
	}
	
}

/**********************************************************************/

/*
 * G = G1 U G2 U NIL
 * where G1 and G2 are partition of graph and NIL
 * is a special null vertex
 */
#define NIL 0
#define INFINITE 1000000000

uint hopcroft_karp(testcase t) {
	uint v;
	/* for each v in G */
	for(v=0; v<t->num_vertices; v++) {
		/* pair_g1[v] = NIL */
		t->pair_g1[v] = NIL;
		/* pair_g2[v] = NIL */
		t->pair_g2[v] = NIL;
	}
	/* matching = 0 */
	t->matching = 0;
	/* while BFS() == true */
	while(hopcroft_karp_bfs(t)) {
		/* for each v in G1 */
		for(v=1; v<t->num_students+1; v++) {
			/* if pair_g1[v] == NIL */
			if(t->pair_g1[v] == NIL) {
				/*`if DFS(v) == true */
				if(hopcroft_karp_dfs(t, v)) {
					/* matching = matching + 1 */
					t->matching++;
				}
			}
		}
	}
	return t->matching;
}

bool hopcroft_karp_bfs(testcase t) {
	fifo q = fifo_create();
	fifo_item item;
	uint v, u;

	/* for each v in G1 */
	for(v=1; v<t->num_students+1; v++) {
		/* if Pair_G1[v] == NIL */
		if(t->pair_g1[v] == NIL) {
			/* Dist[v] = 0 */
			t->dist[v] = 0;
			/* Enqueue(Q, v) */
			fifo_queue(q, v);
		}
		/* else */
		else {
			/* Dist[v] = INFINITE */
			t->dist[v] = INFINITE;
		}
	}

	/* Dist[NIL] = INFINITE */
	t->dist[NIL] = INFINITE;

	/* while Empty(Q) == false */
	while(q->size != 0) {
		/* v = Dequeue(Q) */
		v = fifo_dequeue(q);
		/* if Dist[v] < Dist[NIL] */
		if(t->dist[v] < t->dist[NIL]) {
			/* for each u in Adv[v] */
			for(item = t->adjacencies[v]->first; item != NULL; item = item->next) {
				u = item->value;
				/* if Dist[ Pair_G2[u] ] == INFINITE */
				if(t->dist[t->pair_g2[u]] == INFINITE) {
					/* Dist{ Pair_G2[u] ] = Dist[v] + 1 */
					t->dist[t->pair_g2[u]] = t->dist[v] + 1;
					/* Enqueue(Q, Pair_G2[u]) */
					fifo_queue(q, t->pair_g2[u]);
				}
			}
		}
	}

	/* return Dist[NIL] != INFINITE */
	fifo_destroy(q);
	return (t->dist[NIL] != INFINITE);
}

bool hopcroft_karp_dfs(testcase t, uint v) {
	fifo_item item;
	uint u;

	/* if v != NIL */
	if(v == NIL) {
		/* return true */
		return true;
	}

	/* for each u in Adj[v] */
	for(item = t->adjacencies[v]->first; item != NULL; item = item->next) {
		u = item->value;
		/* if Dist[ Pair_G2[u] ] == Dist[v] + 1 */
		if(t->dist[t->pair_g2[u]] == t->dist[v] + 1) {
			/* if DFS(Pair_G2[u]) == true */
			if(hopcroft_karp_dfs(t, t->pair_g2[u])) {
				/* Pair_G2[u] = v */
				t->pair_g2[u] = v;
				/* Pair_G1[v] = u */
				t->pair_g1[v] = u;
				/* return true */
				return true;
			}
		}
	}
	/* dist[v] = INFINITE */
	t->dist[v] = INFINITE;
	/* return false */
	return false;
}
