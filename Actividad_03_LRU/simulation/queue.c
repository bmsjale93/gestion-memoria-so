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

int dequeue(LRUQueue* queue, PageTable* pt) {
    if (queue->tail == NULL) {
        // La cola está vacía
        return -1;
    }

    Node* temp = queue->tail;
    int page_id = temp->page_id;

    // Recorrer hacia atrás desde tail hasta encontrar un nodo válido
    while (temp != NULL && !(pt->pages[page_id].valid && pt->pages[page_id].frame_id != -1)) {
        temp = temp->prev;  // Moverse al nodo anterior
        if (temp != NULL) {
            page_id = temp->page_id;  // Actualizar el page_id según el nuevo nodo
        }
    }

    if (temp == NULL) {
        // No se encontró ningún nodo válido para eliminar
        return -1;
    }

    // Desconectar el nodo encontrado de la cola
    if (temp == queue->head && temp == queue->tail) {
        // Es el único nodo en la cola
        queue->head = queue->tail = NULL;
    } else if (temp == queue->tail) {
        // Es el nodo tail, ajustar el puntero tail
        queue->tail = temp->prev;
        queue->tail->next = NULL;
    } else if (temp == queue->head) {
        // Es el nodo head, ajustar el puntero head
        queue->head = temp->next;
        queue->head->prev = NULL;
    } else {
        // El nodo está en el medio de la cola
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    free(temp); // Liberar el nodo eliminado
    return page_id;
}


void accessPage(LRUQueue* queue, int page_id) {
    Node* temp = queue->head;
    while (temp != NULL && temp->page_id != page_id) {
        temp = temp->next;
    }

    if (temp == NULL) return; // Si no se encuentra, no hacer nada.

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
