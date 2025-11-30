#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarTerritorios(Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);

        printf("Nome: ");
        scanf(" %29[^\n]", mapa[i].nome);
        limparBuffer();

        printf("Cor do exercito: ");
        scanf(" %9[^\n]", mapa[i].cor);
        limparBuffer();

        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();
    }
}

void exibirMapa(Territorio* mapa, int qtd) {
    printf("\n===== MAPA =====\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d) %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("\nAtaque!\n");
    printf("Dado atacante: %d\n", dadoA);
    printf("Dado defensor: %d\n", dadoD);

    if (dadoA > dadoD) {
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        printf("Territorio conquistado!\n");
    } else {
        atacante->tropas -= 1;
        printf("Ataque falhou! Atacante perdeu 1 tropa.\n");
    }
}

void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    int contador = 0;

    if (strstr(missao, "3 territorios")) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 0) contador++;
        }
        if (contador >= 3) return 1;
    }

    if (strstr(missao, "vermelha")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelha") == 0) return 0;
        }
        return 1;
    }

    if (strstr(missao, "10 tropas")) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 10) return 1;
        }
    }

    if (strstr(missao, "Azuis")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) return 0;
        }
        return 1;
    }

    if (strstr(missao, "20 tropas totais")) {
        int total = 0;
        for (int i = 0; i < tamanho; i++) {
            total += mapa[i].tropas;
        }
        if (total >= 20) return 1;
    }

    return 0;
}

void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
}

int main() {
    srand(time(NULL));

    int qtd;
    printf("Quantidade de territorios: ");
    scanf("%d", &qtd);
    limparBuffer();

    Territorio* mapa = calloc(qtd, sizeof(Territorio));

    cadastrarTerritorios(mapa, qtd);

    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor Vermelha",
        "Conquistar qualquer territorio com mais de 10 tropas",
        "Dominar todos os territorios Azuis",
        "Ficar com pelo menos 20 tropas totais"
    };

    char* missaoJogador = malloc(200);
    atribuirMissao(missaoJogador, missoes, 5);

    printf("\n=== SUA MISSAO ===\n%s\n\n", missaoJogador);

    while (1) {
        exibirMapa(mapa, qtd);

        int atk, def;
        printf("\nTerritorio atacante: ");
        scanf("%d", &atk);
        limparBuffer();
        atk--;

        printf("Territorio defensor: ");
        scanf("%d", &def);
        limparBuffer();
        def--;

        if (strcmp(mapa[atk].cor, mapa[def].cor) == 0) {
            printf("Nao pode atacar a mesma cor!\n");
            continue;
        }

        atacar(&mapa[atk], &mapa[def]);

        if (verificarMissao(missaoJogador, mapa, qtd)) {
            printf("\n=== MISSAO CUMPRIDA! VOCÊ VENCEU! ===\n");
            break;
        }
    }

    liberarMemoria(mapa, missaoJogador);
    return 0;
}