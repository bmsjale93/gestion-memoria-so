// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include "defs.h"

void initQueue(LRUQueue* queue);
void enqueue(LRUQueue* queue, int page_id);
int dequeue(LRUQueue* queue, PageTable* pt);
void accessPage(LRUQueue* queue, int page_id);
#endif // QUEUE_H
