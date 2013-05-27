#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef __FIFO_H__
#define __FIFO_H__

#define FIFO_VALUE int
#define FIFO_VALUE_FORMAT_STR "%d"
#define FIFO_VALUE_NULL -1
#define FIFO_SAFE 1

typedef struct fifo_item {
	struct fifo_item *next;
	FIFO_VALUE value;
} fifo_item_t;
typedef fifo_item_t* fifo_item;

typedef struct fifo {
	int size;
	fifo_item first;
	fifo_item last;
} fifo_t;
typedef fifo_t* fifo;

fifo fifo_create();
fifo_item fifo_item_create(FIFO_VALUE value);
void fifo_destroy(fifo f);
FIFO_VALUE fifo_item_destroy(fifo_item item);

void fifo_queue(fifo f, FIFO_VALUE value);
FIFO_VALUE fifo_dequeue(fifo f);

void fifo_item_print(fifo_item item);
void fifo_print(fifo f);

#endif
#ifndef __TESTCASE_H__
#define __TESTCASE_H__

typedef struct testcase {
	int num_students;
	int num_jobs;
	int num_vertices;
	fifo *adjacencies;
	int *pair_g1;
	int *pair_g2;
	int *dist;
	int matching;
} testcase_t;
typedef testcase_t* testcase;

testcase testcase_create(int num_students, int num_jobs);
void testcase_destroy(testcase t);

void testcase_add_edge(testcase t, int from, int to);

void testcase_print(testcase t);

int hopcroft_karp(testcase t);
bool hopcroft_karp_bfs(testcase t);
bool hopcroft_karp_dfs(testcase t, int v);

#endif

fifo fifo_create() {
	fifo f = malloc(sizeof(fifo_t));
	f->size = 0;
	f->first = NULL;
	f->last = NULL;
	return f;
}

fifo_item fifo_item_create(FIFO_VALUE value) {
	fifo_item item = malloc(sizeof(fifo_item_t));
	item->next = NULL;
	item->value = value;
	return item;
}

void fifo_destroy(fifo f) {
	fifo_item item = f->first;
	fifo_item aux;
	while(item != NULL) {
		aux = item->next;
		fifo_item_destroy(item);
		item = aux;
	}
	free(f);
}

FIFO_VALUE fifo_item_destroy(fifo_item item) {
	FIFO_VALUE value = item->value;
	free(item);
	return value;
}

void fifo_queue(fifo f, FIFO_VALUE value) {
	fifo_item item = fifo_item_create(value);
	/* queue is empty */
	if(f->first == NULL) {
		f->first = item;
	}
	/* queue is not empty */
	else {
		f->last->next = item;
	}
	f->last = item;
	f->size++;
}

FIFO_VALUE fifo_dequeue(fifo f) {
	fifo_item item = f->first;
#ifdef FIFO_SAFE
	if(f->first == NULL) {
		return FIFO_VALUE_NULL;
	}
#endif
	f->first = item->next;
	f->size--;
	return fifo_item_destroy(item);
}

void fifo_item_print(fifo_item item) {
	printf(FIFO_VALUE_FORMAT_STR, item->value);
}

void fifo_print(fifo f) {
	fifo_item p = f->first;
	printf("[FIFO size=%d] ", f->size);
	while(p != NULL) {
		fifo_item_print(p);
		printf(" ");
		p = p->next;
	}
	printf("[/FIFO]");
}

int job_index(testcase t, int job) {
	return t->num_students + job + 1;
}

int student_index(testcase t, int student) {
	return student + 1;
}

testcase testcase_create(int num_students, int num_jobs) {
	int i;
	int num_vertices = num_students + num_jobs + 1;
	testcase t = malloc(sizeof(testcase_t));
	t->num_students = num_students;
	t->num_jobs = num_jobs;
	t->num_vertices = num_vertices;

	t->adjacencies = malloc(num_vertices*sizeof(fifo));
	for(i=0; i<num_vertices; i++) {
		t->adjacencies[i] = fifo_create();
	}

	t->pair_g1 = malloc(num_vertices*sizeof(int));
	t->pair_g2 = malloc(num_vertices*sizeof(int));
	t->dist = malloc(num_vertices*sizeof(int));
	t->matching = 0;
	return t;
}

void testcase_destroy(testcase t) {
	int i;
	for(i=0; i<t->num_vertices; i++) {
		fifo_destroy(t->adjacencies[i]);
	}
	free(t->adjacencies);
	free(t->pair_g1);
	free(t->pair_g2);
	free(t->dist);
	free(t);
}

void testcase_add_edge(testcase t, int student, int job) {
	fifo_queue(t->adjacencies[student_index(t, student)], job_index(t, job));
	fifo_queue(t->adjacencies[job_index(t, job)], student_index(t, student));
}

void testcase_print(testcase t) {
	int i;
	printf("==========[TESTCASE]==========\n");
	printf("num_students = %d\n", t->num_students);
	printf("num_jobs = %d\n", t->num_jobs);
	printf("matching = %d\n", t->matching);
	printf("=========[ADJACENCIES]=========\n");
	for(i=0; i<t->num_vertices; i++) {
		printf("Adjacencies for vertex %d: ", i);
		fifo_print(t->adjacencies[i]);
		printf("\n");
	}
	printf("====[OTHERS]==== (pair_g1, pair_g2, dist)\n");
	for(i=0; i<t->num_vertices; i++) {
		printf("Vertex %d: pair_g1=%d pair_g2=%d dist=%d\n", i, t->pair_g1[i], t->pair_g2[i], t->dist[i]);
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

int hopcroft_karp(testcase t) {
	int v;
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
	int v, u;

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

bool hopcroft_karp_dfs(testcase t, int v) {
	fifo_item item;
	int u;

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

testcase read_input() {
	testcase t;
	int num_students, num_jobs;
	int student, application, num_applications, job;

	/* read header line */
	scanf("%d", &num_students);
	scanf("%d", &num_jobs);
	t = testcase_create(num_students, num_jobs);

	/* read student lines */
	for(student=0; student<num_students; student++) {
		/* how many applications for current student */
		scanf("%d", &num_applications);
		for(application=0; application<num_applications; application++) {
			scanf("%d", &job);
			testcase_add_edge(t, student, job);
		}
	}

	return t;
}

int main() {
	testcase t;
	t = read_input();
	hopcroft_karp(t);
	printf("%d\n", t->matching);
	testcase_destroy(t);
	return 0;
}
