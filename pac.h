//mapa.c

#define HEROI '@'
#define FANTASMA 'F'
#define VAZIO '.'
#define PAREDE_VERTICAL '|'
#define PAREDE_HORIZONTAL '-'
#define PILULA 'P'
#define BOMBA 'b'

struct mapa{
    char** matriz;
    int linhas;
    int colunas;
};

typedef struct mapa MAPA;

struct posicao{
    int x;
    int y;
};

typedef struct posicao POSICAO;


void liberamapa(MAPA* m);
void alocamapa(MAPA* m);
void lemapa(MAPA* m);
int encontramapa(MAPA* m, POSICAO* p, char c);
int ehvalida(MAPA* m, int x, int y);
int ehvazia(MAPA* m, int x, int y);
void copiamapa(MAPA* origem, MAPA* destino);
int podeandar(MAPA* m,char personagem,  int x, int y);
int ehparede(MAPA* m,int x, int y);
int ehpersonagem(MAPA* m,char personagem, int x, int y);

//pac.c

#define CIMA 'w'
#define BAIXO 's'
#define ESQUERDA 'a'
#define DIREITA 'd'

void move(char direcao);
int acabou();
int ehdirecao(char direcao);
void fantasmas();
int praondefantasmavai(int xatual, int yatual, int* xdestino, int* ydestino);
void explodepilula();
void explodepilula2(int x, int y, int somax, int somay, int qtd);

//ui.h
void imprimemapa(MAPA* m);
void imprimeparte(char desenho[4][7], int parte);

char desenhoparede[4][7] = {
    {"......" },
    {"......" },
    {"......" },
    {"......" },
};

char desenhofantasma[4][7] = {
    {" .-.  " },
    {"| Oo| " },
    {"|   | " },
    {"'^^^' " }
};

char desenhoheroi[4][7] = {
    {" .--. "  },
    {"/ _.-'"  },
    {"\\  '-." },
    {" '--' "  }
};

char desenhopilula[4][7] = {
    {"      "},
    {" .-.  "},
    {" '-'  "},
    {"      "}
};

char desenhovazio[4][7] = {
    {"      "},
    {"      "},
    {"      "},
    {"      "}
};