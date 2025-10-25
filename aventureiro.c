#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5   // tamanho fixo da fila de peças futuras
#define TAM_PILHA 3  // capacidade máxima da pilha de reserva

// Estrutura que representa uma peça
typedef struct {
    char tipo; // tipo da peça ('I', 'O', 'T', 'L')
    int id;    // identificador único
} Peca;

// Estrutura da Fila (fila circular)
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

// Estrutura da Pilha (reserva)
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ---------------------- FUNÇÕES DE FILA ----------------------

// Inicializa a fila
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

// Adiciona uma peça no final da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("⚠️  Fila cheia! Não é possível adicionar.\n");
        return;
    }
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = p;
    f->qtd++;
}

// Remove uma peça da frente da fila (dequeue)
Peca desenfileirar(Fila *f) {
    Peca p = {' ', -1};
    if (filaVazia(f)) {
        printf("⚠️  Fila vazia! Nenhuma peça disponível.\n");
        return p;
    }
    p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

// ---------------------- FUNÇÕES DE PILHA ----------------------

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Empilha uma peça (push)
void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = nova;
}

// Desempilha uma peça (pop)
Peca desempilhar(Pilha *p) {
    Peca vazia = {' ', -1};
    if (pilhaVazia(p)) {
        printf("⚠️  Pilha vazia! Nenhuma peça reservada.\n");
        return vazia;
    }
    Peca removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// ---------------------- FUNÇÕES AUXILIARES ----------------------

// Gera uma nova peça aleatória com ID único
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = id;
    return p;
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n===== ESTADO ATUAL =====\n");

    // Fila
    printf("Fila de peças:\t");
    if (filaVazia(f)) printf("(vazia)");
    else {
        for (int i = 0; i < f->qtd; i++) {
            int idx = (f->frente + i) % TAM_FILA;
            printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
        }
    }
    printf("\n");

    // Pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) printf("(vazia)");
    else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
        }
    }

    printf("\n=========================\n");
}

// ---------------------- FUNÇÃO PRINCIPAL ----------------------

int main() {
    Fila fila;
    Pilha pilha;
    int opcao, idGlobal = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(idGlobal++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                enfileirar(&fila, gerarPeca(idGlobal++));
                break;
            }

            case 2: { // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("\n⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
                } else {
                    Peca reservada = desenfileirar(&fila);
                    if (reservada.id != -1) {
                        empilhar(&pilha, reservada);
                        printf("\n📦 Peça reservada: [%c %d]\n", reservada.tipo, reservada.id);
                        enfileirar(&fila, gerarPeca(idGlobal++));
                    }
                }
                break;
            }

            case 3: { // Usar peça reservada
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("\n🧩 Peça usada da reserva: [%c %d]\n", usada.tipo, usada.id);
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