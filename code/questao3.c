#include <stdio.h>
#include <stdlib.h>


typedef struct {
    struct ListNode** elementos;
    int tamanho;
    int capacidade;
} MinHeap;

void inicializarMinHeap(MinHeap* heap, int capacidadeInicial) {
    heap->elementos = (struct ListNode**)malloc(capacidadeInicial * sizeof(struct ListNode*));
    if (heap->elementos == NULL) {
        perror("Erro ao alocar memoria para MinHeap elementos");
        exit(EXIT_FAILURE);
    }
    heap->tamanho = 0;
    heap->capacidade = capacidadeInicial;
}

void liberarMinHeap(MinHeap* heap) {
    if (heap->elementos != NULL) {
        free(heap->elementos);
    }
    heap->elementos = NULL;
    heap->tamanho = 0;
    heap->capacidade = 0;
}

void trocarHeapElementos(struct ListNode** a, struct ListNode** b) {
    struct ListNode* temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int indice) {
    int pai = (indice - 1) / 2;
    while (indice > 0 && heap->elementos[pai]->val > heap->elementos[indice]->val) {
        trocarHeapElementos(&heap->elementos[pai], &heap->elementos[indice]);
        indice = pai;
        pai = (indice - 1) / 2;
    }
}

void heapifyDown(MinHeap* heap, int indice) {
    int menor = indice;
    int filhoEsquerdo = 2 * indice + 1;
    int filhoDireito = 2 * indice + 2;

    if (filhoEsquerdo < heap->tamanho && heap->elementos[filhoEsquerdo]->val < heap->elementos[menor]->val) {
        menor = filhoEsquerdo;
    }

    if (filhoDireito < heap->tamanho && heap->elementos[filhoDireito]->val < heap->elementos[menor]->val) {
        menor = filhoDireito;
    }

    if (menor != indice) {
        trocarHeapElementos(&heap->elementos[indice], &heap->elementos[menor]);
        heapifyDown(heap, menor);
    }
}

void minHeap_push(MinHeap* heap, struct ListNode* no) {
    if (heap->tamanho == heap->capacidade) {
        int novaCapacidade = (heap->capacidade == 0) ? 1 : heap->capacidade * 2;
        heap->capacidade = novaCapacidade;
        
        heap->elementos = (struct ListNode**)realloc(heap->elementos, heap->capacidade * sizeof(struct ListNode*));
        if (heap->elementos == NULL) {
            perror("Erro ao realocar memoria para MinHeap push");
            exit(EXIT_FAILURE);
        }
    }
    heap->elementos[heap->tamanho] = no;
    heap->tamanho++;
    heapifyUp(heap, heap->tamanho - 1);
}

struct ListNode* minHeap_pop(MinHeap* heap) {
    if (heap->tamanho == 0) {
        return NULL;
    }
    struct ListNode* raiz = heap->elementos[0];
    heap->elementos[0] = heap->elementos[heap->tamanho - 1];
    heap->tamanho--;
    heapifyDown(heap, 0);
    return raiz;
}

struct ListNode* minHeap_top(MinHeap* heap) {
    if (heap->tamanho == 0) {
        return NULL;
    }
    return heap->elementos[0];
}

int minHeap_empty(MinHeap* heap) {
    return heap->tamanho == 0;
}

struct ListNode* mergeKLists(struct ListNode** listas, int tamanhoListas) {
    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = NULL;
    struct ListNode* atual = &dummy;
    MinHeap minHeapDeListas;
    inicializarMinHeap(&minHeapDeListas, tamanhoListas > 0 ? tamanhoListas : 1);
    for (int i = 0; i < tamanhoListas; ++i) {
        if (listas[i] != NULL) {
            minHeap_push(&minHeapDeListas, listas[i]);
        }
    }
    while (!minHeap_empty(&minHeapDeListas)) {
        struct ListNode* noMinimo = minHeap_pop(&minHeapDeListas);  
        if (noMinimo->next != NULL) {
            minHeap_push(&minHeapDeListas, noMinimo->next);
        }
        atual->next = noMinimo;
        atual = atual->next;
    }
    liberarMinHeap(&minHeapDeListas);
    return dummy.next;
}