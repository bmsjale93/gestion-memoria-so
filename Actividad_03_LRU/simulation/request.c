// request.c
#include "request.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void processPageRequest(PageTable* pt, Frame frames[], LRUQueue* queue, PageRequest request) {
    char logMessage[256];

    // Log the request being processed
    sprintf(logMessage, "Procesando solicitud de página %d para el proceso %d", request.page_id, request.process_id);
    logEvent(logMessage);

    // Log the initial state
    sprintf(logMessage, "Estado inicial: Página %d es %s", request.page_id, pt->pages[request.page_id].valid ? "válida" : "no válida");
    logEvent(logMessage);

    if (pt->pages[request.page_id].valid) {
        // Access the page, updating its position in the LRU queue
        accessPage(queue, request.page_id);
        sprintf(logMessage, "Página %d accedida y movida a MRU.", request.page_id);
        logEvent(logMessage);
        return;
    }

    int frameIndex = findFreeFrame(frames, NUM_FRAMES);
    if (frameIndex == -1) {
        // No free frames, need to evict the least recently used page
        int evictedPageId = dequeue(queue, pt);
        if (evictedPageId == -1) {
            sprintf(logMessage, "No hay páginas válidas en la cola LRU para desalojar.");
            logEvent(logMessage);
            return;
        }

        frameIndex = pt->pages[evictedPageId].frame_id;
        pt->pages[evictedPageId].valid = 0;
        pt->pages[evictedPageId].frame_id = -1;
        frames[frameIndex].page_id = -1;
        frames[frameIndex].occupied = 0;

        sprintf(logMessage, "Fallos de página: la página %d desalojada del marco %d", evictedPageId, frameIndex);
        logEvent(logMessage);
    }

    // Place the new page in the frame and update the LRU queue
    frames[frameIndex].page_id = request.page_id;
    frames[frameIndex].occupied = 1;
    pt->pages[request.page_id].frame_id = frameIndex;
    pt->pages[request.page_id].valid = 1;

    enqueue(queue, request.page_id);

    sprintf(logMessage, "Página %d cargada en el marco %d", request.page_id, frameIndex);
    logEvent(logMessage);

    // Log the final state
    sprintf(logMessage, "Estado final: Página %d en marco %d", request.page_id, frameIndex);
    logEvent(logMessage);
}
