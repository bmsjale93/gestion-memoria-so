// simulation.h
#ifndef SIMULATION_H
#define SIMULATION_H

#include "defs.h"
#include "memory.h"
#include "queue.h"
#include "log.h"
#include "request.h"

void initSimulation(Frame frames[], int num_frames);
void simulate(PageRequest* requests, int num_requests, Frame frames[], int num_frames, ProcessPageTables* ppt, LRUQueue* queue);
void runTest(PageRequest* requests, int num_requests, const char* test_name);
void testRandomAccess(int value);
void testTemporalLocality(int num_pages, int num_accesses, int num_processes);
void testThrashing(int num_processes);

#endif // SIMULATION_H
