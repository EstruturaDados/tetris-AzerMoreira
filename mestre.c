#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ---------------------------
// Estrutura de dados
// ---------------------------
typedef struct {
    char tipo;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ---------------------------
// Funções utilitárias
// ---------------------------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) { return f->qtd == 0; }
int filaCheia(Fila *f) { return f->qtd == TAM_FILA; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }

// ---------------------------
// Gerar peças automaticamente
// ---------------------------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = id;
    return p;
}

// ---------------------------
// Operações da Fila
// ---------------------------
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = p;
    f->qtd++;
}

Peca desenfileirar(Fila *f) {
    Peca vazio = {' ', -1};
    if (filaVazia(f)) return vazio;
    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return removida;
}

// ---------------------------
// Operações da Pilha
// ---------------------------
void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    Peca vazio = {' ', -1};
    if (pilhaVazia(p)) return vazio;
    Peca removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// ---------------------------
// Exibição visual
// ---------------------------
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

// ---------------------------
// Troca entre estruturas
// ---------------------------

// Troca direta: frente da fila <-> topo da pilha
void trocarTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\n⚠️  Impossível trocar: uma das estruturas está vazia.\n");
        return;
    }

    int idxFrente = f->frente;
    Peca temp = f->itens[idxFrente];
    f->itens[idxFrente] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\n🔄 Troca realizada entre a frente da fila e o topo da pilha!\n");
}

// Troca múltipla: 3 primeiras da fila <-> 3 da pilha
void trocarMultiplas(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("\n⚠️  Não há peças suficientes para troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % TAM_FILA;
        int idxPilha = p->topo - i;

        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[idxPilha];
        p->itens[idxPilha] = temp;
    }

    printf("\n🔁 Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha!\n");
}

// ---------------------------
// Função principal
// ---------------------------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, idGlobal = 0;
    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++)
        enfileirar(&fila, gerarPeca(idGlobal++));

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça (remove da fila)\n");
        printf("2 - Reservar peça (mover fila -> pilha)\n");
        printf("3 - Usar peça reservada (remover da pilha)\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Troca múltipla (3 primeiras da fila <-> 3 da pilha)\n");
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

            case 3: { // Usar peça da pilha
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("\n🧩 Peça usada da reserva: [%c %d]\n", usada.tipo, usada.id);
                break;
            }

            case 4: // Troca direta
                trocarTopo(&fila, &pilha);
                break;

            case 5: // Troca múltipla
                trocarMultiplas(&fila, &pilha);
                break;

            case 0:
                printf("\nSaindo do jogo... 👋\n");
                break;

            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}