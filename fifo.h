#ifndef __FIFO_H__
#define __FIFO_H__

#include "defs.h"

#define FIFO_VALUE uint
#define FIFO_VALUE_FORMAT_STR "%lu"
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
