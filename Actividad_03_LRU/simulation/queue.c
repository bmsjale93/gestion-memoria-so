// queue.c
#include "queue.h"
#include <stdlib.h>

void initQueue(LRUQueue* queue) {
    queue->head = queue->tail = NULL;
}

void enqueue(LRUQueue* queue, int page_id) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->page_id = page_id;
    newNode->next = queue->head;
    newNode->prev = NULL;

    if (queue->head == NULL) {
        queue->head = queue->tail = newNode;
    } else {
        queue->head->prev = newNode;
        queue->head = newNode;
    }
}

int dequeue(LRUQueue* queue, PageTable* pt){
    if (queue->tail == NULL) {
        // La cola está vacía
        return -1;
    }

    Node* temp = queue->tail;
    int page_id = temp->page_id;

    // Si es el único nodo en la cola
    if (queue->head == queue->tail) {
        queue->head = queue->tail = NULL;
    } else {
        // Ajustar el puntero `tail` al penúltimo nodo
        queue->tail = temp->prev;
        queue->tail->next = NULL;
    }

    free(temp); // Liberar el nodo eliminado
    return page_id;
}

void accessPage(LRUQueue* queue, int page_id) {
    Node* temp = queue->head;
    while (temp != NULL && temp->page_id != page_id) {
        temp = temp->next;
    }

    if (temp == NULL) return; // Si no se encuentra, no hacer nada (esto no debería pasar en uso normal).

    if (temp == queue->head) return; // Ya es el más reciente, no hacer nada.

    // Desconectar el nodo de su posición actual
    if (temp->prev) {
        temp->prev->next = temp->next;
    }
    if (temp->next) {
        temp->next->prev = temp->prev;
    }
    if (temp == queue->tail) {
        queue->tail = temp->prev;
    }

    // Mover el nodo al frente
    temp->next = queue->head;
    temp->prev = NULL;
    queue->head->prev = temp;
    queue->head = temp;
}
