typedef struct {
    int x;
    int y;
} PontoSkyline;

typedef struct {
    PontoSkyline* pontos;
    int tamanho;
    int capacidade;
} ArraySkyline;

void inicializarArraySkyline(ArraySkyline* arr, int capacidadeInicial) {
    arr->pontos = (PontoSkyline*)malloc(capacidadeInicial * sizeof(PontoSkyline));
    arr->tamanho = 0;
    arr->capacidade = capacidadeInicial;
}

void liberarArraySkyline(ArraySkyline* arr) {
    free(arr->pontos);
    arr->pontos = NULL;
    arr->tamanho = 0;
    arr->capacidade = 0;
}

void adicionarPontoArraySkyline(ArraySkyline* arr, int x, int y) {
    if (arr->tamanho > 0 && arr->pontos[arr->tamanho - 1].x == x) {
        arr->pontos[arr->tamanho - 1].y = y;
        return;
    }
    if (arr->tamanho > 0 && arr->pontos[arr->tamanho - 1].y == y) {
        return;
    }

    if (arr->tamanho == arr->capacidade) {
        arr->capacidade *= 2;
        arr->pontos = (PontoSkyline*)realloc(arr->pontos, arr->capacidade * sizeof(PontoSkyline));
        if (arr->pontos == NULL) {
            perror("Erro ao realocar memoria para ArraySkyline");
            exit(EXIT_FAILURE); 
        }
    }
    arr->pontos[arr->tamanho].x = x;
    arr->pontos[arr->tamanho].y = y;
    arr->tamanho++;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

ArraySkyline calcularSkylineRecursivo(int** edificios, int inicio, int fim);

ArraySkyline mesclarSkylines(ArraySkyline* skylineEsquerdo, ArraySkyline* skylineDireito) {
    ArraySkyline resultadoSkyline;
    inicializarArraySkyline(&resultadoSkyline, skylineEsquerdo->tamanho + skylineDireito->tamanho + 2);

    int i = 0;
    int j = 0;
    int alturaEsquerdaAtual = 0;
    int alturaDireitaAtual = 0;

    while (i < skylineEsquerdo->tamanho && j < skylineDireito->tamanho) {
        if (skylineEsquerdo->pontos[i].x < skylineDireito->pontos[j].x) {
            alturaEsquerdaAtual = skylineEsquerdo->pontos[i].y;
            adicionarPontoArraySkyline(&resultadoSkyline, skylineEsquerdo->pontos[i].x, max(alturaEsquerdaAtual, alturaDireitaAtual));
            i++;
        } else if (skylineDireito->pontos[j].x < skylineEsquerdo->pontos[i].x) {
            alturaDireitaAtual = skylineDireito->pontos[j].y;
            adicionarPontoArraySkyline(&resultadoSkyline, skylineDireito->pontos[j].x, max(alturaEsquerdaAtual, alturaDireitaAtual));
            j++;
        } else {
            alturaEsquerdaAtual = skylineEsquerdo->pontos[i].y;
            alturaDireitaAtual = skylineDireito->pontos[j].y;
            adicionarPontoArraySkyline(&resultadoSkyline, skylineEsquerdo->pontos[i].x, max(alturaEsquerdaAtual, alturaDireitaAtual));
            i++;
            j++;
        }
    }

    while (i < skylineEsquerdo->tamanho) {
        adicionarPontoArraySkyline(&resultadoSkyline, skylineEsquerdo->pontos[i].x, max(skylineEsquerdo->pontos[i].y, alturaDireitaAtual));
        i++;
    }

    while (j < skylineDireito->tamanho) {
        adicionarPontoArraySkyline(&resultadoSkyline, skylineDireito->pontos[j].x, max(skylineDireito->pontos[j].y, alturaEsquerdaAtual));
        j++;
    }

    return resultadoSkyline;
}

ArraySkyline calcularSkylineRecursivo(int** edificios, int inicio, int fim) {
    if (inicio > fim) {
        ArraySkyline vazia;
        inicializarArraySkyline(&vazia, 0);
        return vazia;
    }
    
    if (inicio == fim) {
        ArraySkyline skylineUnitario;
        inicializarArraySkyline(&skylineUnitario, 2);

        int esquerda = edificios[inicio][0];
        int direita = edificios[inicio][1];
        int altura = edificios[inicio][2];

        adicionarPontoArraySkyline(&skylineUnitario, esquerda, altura);
        adicionarPontoArraySkyline(&skylineUnitario, direita, 0);

        return skylineUnitario;
    }

    int meio = inicio + (fim - inicio) / 2;
    ArraySkyline skylineEsquerdo = calcularSkylineRecursivo(edificios, inicio, meio);
    ArraySkyline skylineDireito = calcularSkylineRecursivo(edificios, meio + 1, fim);

    ArraySkyline resultadoMesclado = mesclarSkylines(&skylineEsquerdo, &skylineDireito);

    liberarArraySkyline(&skylineEsquerdo);
    liberarArraySkyline(&skylineDireito);

    return resultadoMesclado;
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** getSkyline(int** buildings, int buildingsSize, int* buildingsColSize, int* returnSize, int** returnColumnSizes) {
    if (buildingsSize == 0) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    ArraySkyline resultadoTemporario = calcularSkylineRecursivo(buildings, 0, buildingsSize - 1);

    *returnSize = resultadoTemporario.tamanho;
    *returnColumnSizes = (int*)malloc(*returnSize * sizeof(int));
    int** finalSkyline = (int**)malloc(*returnSize * sizeof(int*));

    if (*returnColumnSizes == NULL || finalSkyline == NULL) {
        perror("Erro ao alocar memoria para o resultado final do skyline");
        liberarArraySkyline(&resultadoTemporario);
        if (*returnColumnSizes != NULL) { free(*returnColumnSizes); *returnColumnSizes = NULL; }
        if (finalSkyline != NULL) { free(finalSkyline); finalSkyline = NULL; }
        *returnSize = 0;
        return NULL;
    }

    for (int i = 0; i < *returnSize; ++i) {
        finalSkyline[i] = (int*)malloc(2 * sizeof(int));
        if (finalSkyline[i] == NULL) {
            perror("Erro ao alocar memoria para um ponto do skyline");
            for (int k = 0; k < i; ++k) free(finalSkyline[k]);
            free(finalSkyline);
            free(*returnColumnSizes);
            liberarArraySkyline(&resultadoTemporario);
            *returnSize = 0;
            *returnColumnSizes = NULL;
            return NULL;
        }
        finalSkyline[i][0] = resultadoTemporario.pontos[i].x;
        finalSkyline[i][1] = resultadoTemporario.pontos[i].y;
        (*returnColumnSizes)[i] = 2;
    }

    liberarArraySkyline(&resultadoTemporario);

    return finalSkyline;
}