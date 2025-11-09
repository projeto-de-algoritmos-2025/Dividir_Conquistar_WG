int obterComprimentoDaLista(struct ListNode* cabeca) {
    int comprimento = 0;
    struct ListNode* atual = cabeca;
    while (atual != NULL) {
        comprimento++;
        atual = atual->next;
    }
    return comprimento;
}

struct ListNode* dividirLista(struct ListNode* cabeca, int k) {
    while (--k > 0 && cabeca != NULL) {
        cabeca = cabeca->next;
    }
    struct ListNode* resto = (cabeca != NULL) ? cabeca->next : NULL;
    if (cabeca != NULL) {
        cabeca->next = NULL;
    }
    return resto;
}

typedef struct {
    struct ListNode* cabecaMesclada;
    struct ListNode* caudaMesclada;
} ResultadoMesclagem;

void trocarPonteirosListNode(struct ListNode** a, struct ListNode** b) {
    struct ListNode* temp = *a;
    *a = *b;
    *b = temp;
}

ResultadoMesclagem mesclarListas(struct ListNode* lista1, struct ListNode* lista2) {
    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = NULL;

    struct ListNode* cauda = &dummy;

    while (lista1 != NULL && lista2 != NULL) {
        if (lista1->val > lista2->val) {
            trocarPonteirosListNode(&lista1, &lista2);
        }
        cauda->next = lista1;
        lista1 = lista1->next;
        cauda = cauda->next;
    }

    cauda->next = (lista1 != NULL) ? lista1 : lista2;

    while (cauda->next != NULL) {
        cauda = cauda->next;
    }

    ResultadoMesclagem resultado;
    resultado.cabecaMesclada = dummy.next;
    resultado.caudaMesclada = cauda;
    return resultado;
}

struct ListNode* sortList(struct ListNode* cabeca) {
    if (cabeca == NULL || cabeca->next == NULL) {
        return cabeca;
    }

    int comprimentoTotal = obterComprimentoDaLista(cabeca);

    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = cabeca;

    for (int k = 1; k < comprimentoTotal; k *= 2) {
        struct ListNode* ponteiroAtual = dummy.next;
        struct ListNode* caudaAnterior = &dummy;

        while (ponteiroAtual != NULL) {
            struct ListNode* listaEsquerda = ponteiroAtual;
            struct ListNode* listaDireita = dividirLista(listaEsquerda, k);
            ponteiroAtual = dividirLista(listaDireita, k);

            ResultadoMesclagem resultado = mesclarListas(listaEsquerda, listaDireita);

            caudaAnterior->next = resultado.cabecaMesclada;
            caudaAnterior = resultado.caudaMesclada;
        }
    }

    return dummy.next;
}