#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definições e protótipos de pac.h
#define HEROI 'H'
#define FANTASMA 'F'
#define PILULA 'P'
#define PAREDE_VERTICAL '|'
#define PAREDE_HORIZONTAL '-'
#define VAZIO ' '
#define ESQUERDA 'a'
#define DIREITA 'd'
#define CIMA 'w'
#define BAIXO 's'
#define BOMBA 'h'

typedef struct {
    int x;
    int y;
} POSICAO;

typedef struct {
    int linhas;
    int colunas;
    char** matriz;
} MAPA;

// Variáveis globais
MAPA m;
POSICAO heroi;
int tempilula = 0;

// "Desenhos" para a UI - podem ser adaptados ou melhorados
char desenhofantasma[4][7] = {" F ", " F ", " F ", " F "};
char desenhoheroi[4][7] = {" H ", " H ", " H ", " H "};
char desehopilula[4][7] = {" P ", " P ", " P ", " P "};
char desenhoparede[4][7] = {"|", "|", "|", "|"};
char desenhovazio[4][7] = {" ", " ", " ", " "};

// ---------------------------------
// Funções de UI (imprimir mapa)
// ---------------------------------
void imprimeparte(char desenho[4][7], int parte){
    printf("%s", desenho[parte]);
}

void imprimemapa(MAPA* m){
    for(int i = 0; i < m->linhas; i++) {
        for(int parte = 0; parte < 4; parte++){
            for(int j = 0; j < m->colunas; j++){
                switch (m->matriz[i][j]){
                    case FANTASMA:
                        imprimeparte(desenhofantasma, parte);
                        break;
                    case HEROI:
                        imprimeparte(desenhoheroi, parte);
                        break;
                    case PILULA:
                        imprimeparte(desehopilula, parte);
                        break;
                    case PAREDE_VERTICAL:
                    case PAREDE_HORIZONTAL:
                        imprimeparte(desenhoparede, parte);
                        break;  // corrige o switch "cair" pro VAZIO
                    case VAZIO:
                        imprimeparte(desenhovazio, parte);
                        break;
                }
            }
            printf("\n");
        }
    }
}

// ---------------------------------
// Funções de mapa
// ---------------------------------
void alocamapa(MAPA* m){
    // corrigido: ponteiro pra ponteiro
    m->matriz = (char**)malloc(sizeof(char*) * m->linhas);
    for(int i = 0; i < m->linhas; i++){
        m->matriz[i] = (char*)malloc(sizeof(char) * (m->colunas + 1));
    }
}

void lemapa(MAPA* m){
    FILE* f = fopen("mapa.txt", "r");
    if(f == NULL){
        printf("Erro na leitura do mapa\n");
        exit(1);
    }

    fscanf(f, "%d %d", &(m->linhas), &(m->colunas));
    printf("Linhas %d Colunas %d\n", m->linhas, m->colunas);

    alocamapa(m);

    for(int i = 0; i < m->linhas; i++){
        fscanf(f, "%s", m->matriz[i]);
    }

    fclose(f);
}

void liberamapa(MAPA* m){
    for(int i = 0; i < m->linhas; i++){
        free(m->matriz[i]);
    }
    free(m->matriz);
}

void copiamapa(MAPA* origem, MAPA* destino){
    destino->linhas = origem->linhas;
    destino->colunas = origem->colunas;
    alocamapa(destino);
    for(int i = 0; i < origem->linhas; i++){
        strcpy(destino->matriz[i], origem->matriz[i]);
    }
}

void andanomapa(MAPA* m, int xorigem, int yorigem, int xdestino, int ydestino){
    char personagem = m->matriz[xorigem][yorigem];
    m->matriz[xdestino][ydestino] = personagem;
    m->matriz[xorigem][yorigem] = VAZIO;
}

int ehvalida(MAPA* m, int x, int y){
    return x >= 0 && x < m->linhas && y >= 0 && y < m->colunas;
}

int ehvazia(MAPA* m, int x, int y){
    return m->matriz[x][y] == VAZIO;
}

int ehparede(MAPA* m, int x, int y){
    return m->matriz[x][y] == PAREDE_VERTICAL || m->matriz[x][y] == PAREDE_HORIZONTAL;
}

int ehpersonagem(MAPA* m, char personagem, int x, int y){
    return m->matriz[x][y] == personagem;
}

int podeandar(MAPA* m, char personagem, int x, int y){
    return ehvalida(m, x, y) && !ehparede(m, x, y) && !ehpersonagem(m, personagem, x, y);
}

int encontramapa(MAPA* m, POSICAO* p, char c){
    for(int i = 0; i < m->linhas; i++){
        for(int j = 0; j < m->colunas; j++){
            if(m->matriz[i][j] == c){
                p->x = i;
                p->y = j;
                return 1;
            }
        }
    }
    return 0;
}

// ---------------------------------
// Funções de lógica do jogo
// ---------------------------------
int praondefantasmavai(int xatual, int yatual, int* xdestino, int* ydestino){
    int opcoes[4][2] = {
        {xatual, yatual + 1},
        {xatual + 1, yatual},
        {xatual, yatual - 1},
        {xatual - 1, yatual}
    };

    for(int i = 0; i < 10; i++){
        int posicao = rand() % 4;
        if(podeandar(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1])){
            *xdestino = opcoes[posicao][0];
            *ydestino = opcoes[posicao][1];
            return 1;
        }
    }
    return 0;
}

void fantasmas(){
    MAPA copia;
    copiamapa(&m, &copia);

    for(int i = 0; i < m.linhas; i++){
        for(int j = 0; j < m.colunas; j++){
            if(copia.matriz[i][j] == FANTASMA){
                int xdestino, ydestino;
                if(praondefantasmavai(i, j, &xdestino, &ydestino)){
                    andanomapa(&m, i, j, xdestino, ydestino);
                }
            }
        }
    }
    liberamapa(&copia);
}

int acabou(){
    POSICAO pos;
    return !encontramapa(&m, &pos, HEROI);
}

int ehdirecao(char direcao){
    if(direcao == 'h'){
        printf("SIM, HENRIQUE ZAQUEU TA FAZENDO ESSE PROJETO.\n");
    }
    return (direcao == 'a' || direcao == 'w' || direcao == 's' || direcao =='d' || direcao == 'h');
}

void move(char direcao){
    if(!ehdirecao(direcao)) return;

    int proximox = heroi.x;
    int proximoy = heroi.y;

    switch(direcao){
        case ESQUERDA: proximoy--; break;
        case CIMA: proximox--; break;
        case BAIXO: proximox++; break;
        case DIREITA: proximoy++; break;
    }

    if(!podeandar(&m, HEROI, proximox, proximoy)) return;

    if(ehpersonagem(&m, PILULA, proximox, proximoy)) tempilula = 1;

    andanomapa(&m, heroi.x, heroi.y, proximox, proximoy);
    heroi.x = proximox;
    heroi.y = proximoy;
}

void explodepilula2(int x, int y, int somax, int somay, int qtd){
    if(qtd == 0) return;
    int novox = x + somax;
    int novoy = y + somay;

    if(!ehvalida(&m, novox, novoy)) return;
    if(ehparede(&m, novox, novoy)) return;

    m.matriz[novox][novoy] = VAZIO;
    explodepilula2(novox, novoy, somax, somay, qtd - 1);
}

void explodepilula(){
    if(!tempilula) return;
    explodepilula2(heroi.x, heroi.y, 0, 1, 3);
    explodepilula2(heroi.x, heroi.y, 0, -1, 3);
    explodepilula2(heroi.x, heroi.y, 1, 0, 3);
    explodepilula2(heroi.x, heroi.y, -1, 0, 3);
    tempilula = 0;
}

// ---------------------------------
// Função principal
// ---------------------------------
int main(){
    srand(time(0)); // semente do rand apenas uma vez

    lemapa(&m);
    encontramapa(&m, &heroi, HEROI);

    do {
        printf("Tem pilula %s\n", (tempilula ? "SIM" : "NAO"));
        imprimemapa(&m);

        char comando;
        scanf(" %c", &comando);
        move(comando);

        if(comando == BOMBA) explodepilula();
        fantasmas();

    } while(!acabou());

    liberamapa(&m);
    return 0;
}
