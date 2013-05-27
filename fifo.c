#include "fifo.h"
#include <stdlib.h>
#include <stdio.h>

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
