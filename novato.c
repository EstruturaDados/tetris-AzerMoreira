#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5  // tamanho fixo da fila

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo;  // tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente; // índice do primeiro elemento
    int tras;   // índice do último elemento
    int qtd;    // quantidade atual de elementos
} Fila;

// Função para inicializar a fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->qtd == 0;
}

// Gera uma nova peça com tipo aleatório e id sequencial
Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.tipo = tipos[rand() % 4]; // escolhe tipo aleatório
    p.id = id;
    return p;
}

// Insere uma peça no final da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n⚠️  Fila cheia! Não é possível inserir nova peça.\n");
        return;
    }
    f->tras = (f->tras + 1) % TAM_FILA; // posição circular
    f->itens[f->tras] = p;
    f->qtd++;
}

// Remove a peça da frente da fila (dequeue)
Peca desenfileirar(Fila *f) {
    Peca p = {' ', -1};
    if (filaVazia(f)) {
        printf("\n⚠️  Fila vazia! Nenhuma peça para jogar.\n");
        return p;
    }
    p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA; // posição circular
    f->qtd--;
    return p;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("\n=== Fila de Peças ===\n");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    int i, idx;
    for (i = 0; i < f->qtd; i++) {
        idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\n=====================\n");
}

int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL));

    int opcao, idGlobal = 0;

    // Inicializa fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(idGlobal++));
    }

    do {
        exibirFila(&fila);
        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                break;
            }
            case 2: {
                Peca nova = gerarPeca(idGlobal++);
                enfileirar(&fila, nova);
                printf("\n🧩 Nova peça gerada: [%c %d]\n", nova.tipo, nova.id);
                break;
            }
            case 0:
                printf("\nSaindo do jogo... 👋\n");
                break;
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}