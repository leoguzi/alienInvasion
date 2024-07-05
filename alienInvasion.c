#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <pthread.h>
#include <stdbool.h>

enum PosicaoCanhao
{
    ZERO_GRAUS = 0,
    QUARENTA_CINCO_GRAUS = 45,
    NOVENTA_GRAUS = 90,
    CENTO_E_TRINTA_CINCO_GRAUS = 135,
    CENTO_E_OITENTA_GRAUS = 180
};

#define MAX_NAVES 10
#define MAX_BOMBAS 5
#define TEMPO_RECARGA 4
#define CANHAO_X 59
#define CANHAO_Y 25

int bombasDisponiveis = 0;

typedef struct
{
    int x, y;
    bool ativo;
} Objeto;

Objeto naves[MAX_NAVES];
Objeto bombas[MAX_BOMBAS];

int navesChegaram, navesDestruidas = 0;

enum PosicaoCanhao posicaoCanhao = NOVENTA_GRAUS;

pthread_mutex_t mutexGotoxy;
pthread_mutex_t mutexNaves;
pthread_mutex_t mutexBombas;
pthread_mutex_t mutexColisao;

// Funcao gotoxy
void gotoxy(int x, int y)
{

    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void desenhaNave(int x, int y)
{
    gotoxy(x, y);
    printf("    _    \n");
    gotoxy(x, y + 1);
    printf("  _( )_  \n");
    gotoxy(x, y + 2);
    printf(" /     \\ \n");
    gotoxy(x, y + 3);
    printf("/_______\\\n");
    gotoxy(x, y + 4);
    printf("  |   |  \n");
}

void apagaNave(int x, int y)
{
    gotoxy(x, y);
    printf("           \n");
    gotoxy(x, y + 1);
    printf("           \n");
    gotoxy(x, y + 2);
    printf("           \n");
    gotoxy(x, y + 3);
    printf("           \n");
    gotoxy(x, y + 4);
    printf("           \n");
}

void desenhaCanhao(int x, int y, enum PosicaoCanhao posicao)
{
    gotoxy(x, y);
    switch (posicao)
    {
    case ZERO_GRAUS:
        printf(">");
        break;
    case QUARENTA_CINCO_GRAUS:
        printf("/");
        break;
    case NOVENTA_GRAUS:
        printf("|");
        break;
    case CENTO_E_TRINTA_CINCO_GRAUS:;
        printf("\\");
        break;
    case CENTO_E_OITENTA_GRAUS:
        printf("<");
        break;
    }
}

void apagaCanhao(int x, int y)
{
    gotoxy(x, y);
    printf("       \n");
}

int pressionouDireita(int posicaoAtual)
{
    switch (posicaoAtual)
    {
    case ZERO_GRAUS:
        return ZERO_GRAUS;
    case QUARENTA_CINCO_GRAUS:
        return ZERO_GRAUS;
    case NOVENTA_GRAUS:
        return QUARENTA_CINCO_GRAUS;
    case CENTO_E_TRINTA_CINCO_GRAUS:
        return NOVENTA_GRAUS;
    case CENTO_E_OITENTA_GRAUS:
        return CENTO_E_TRINTA_CINCO_GRAUS;
    }
}

int pressionouEsquerda(int posicaoAtual)
{
    switch (posicaoAtual)
    {
    case ZERO_GRAUS:
        return QUARENTA_CINCO_GRAUS;
    case QUARENTA_CINCO_GRAUS:
        return NOVENTA_GRAUS;
    case NOVENTA_GRAUS:
        return CENTO_E_TRINTA_CINCO_GRAUS;
    case CENTO_E_TRINTA_CINCO_GRAUS:
        return CENTO_E_OITENTA_GRAUS;
    case CENTO_E_OITENTA_GRAUS:
        return CENTO_E_OITENTA_GRAUS;
    }
}

void *atualizaContatores()
{
    while (1)
    {
        pthread_mutex_lock(&mutexGotoxy);
        gotoxy(0, 27);
        printf("Naves chegaram: %d\n", navesChegaram);
        printf("Naves destruidas: %d", navesDestruidas);
        pthread_mutex_unlock(&mutexGotoxy);
        pthread_mutex_lock(&mutexGotoxy);
        gotoxy(20, 27);
        printf("Bombas disponiveis: %d", bombasDisponiveis);
        pthread_mutex_unlock(&mutexGotoxy);
        Sleep(100);
    }
}

void finalizaJogoPerdeu()
{
    pthread_mutex_lock(&mutexGotoxy);
    gotoxy(0, 29);
    printf("Fim de jogo! Voce perdeu!\n");
    pthread_mutex_unlock(&mutexGotoxy);
    exit(0);
}

void *threadNave(void *arg)
{
    int *idx = (int *)arg;
    srand(time(NULL));
    int x = rand() % 109; // Posição X aleatória
    int y = 0;

    while (1)
    {
        pthread_mutex_lock(&mutexColisao);
        if (!naves[*idx].ativo)
        {
            pthread_mutex_unlock(&mutexColisao);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&mutexColisao);
        pthread_mutex_lock(&mutexGotoxy);
        apagaNave(x, y);
        pthread_mutex_unlock(&mutexGotoxy);

        y += 1;

        if (y > 20)
        {
            pthread_mutex_unlock(&mutexGotoxy);
            apagaNave(x, y);
            pthread_mutex_unlock(&mutexGotoxy);
            pthread_mutex_lock(&mutexNaves);
            navesChegaram++;
            pthread_mutex_unlock(&mutexNaves);
            if (navesChegaram >= MAX_NAVES / 2)
            {
                finalizaJogoPerdeu();
            }
            pthread_exit(NULL);
            break;
        }
        pthread_mutex_lock(&mutexGotoxy);
        desenhaNave(x, y);
        pthread_mutex_unlock(&mutexGotoxy);
        pthread_mutex_lock(&mutexColisao);
        naves[*idx].x = x;
        naves[*idx].y = y;
        pthread_mutex_unlock(&mutexColisao);

        Sleep(500);
    }
    pthread_exit(NULL);
}

void apagaBomba(int x, int y)
{
    gotoxy(x, y);
    printf(" ");
}

void desenhaBomba(int x, int y)
{
    gotoxy(x, y);
    printf("o");
}

void recarregaCanhao()
{
    for (int i = 0; i < TEMPO_RECARGA; i++)
    {
        pthread_mutex_lock(&mutexGotoxy);
        gotoxy(20, 28);
        printf("Sem bombas disponiveis! Pronto em %d segundos...", TEMPO_RECARGA - i);
        pthread_mutex_unlock(&mutexGotoxy);
        Sleep(1000);
    }
    pthread_mutex_lock(&mutexBombas);
    bombasDisponiveis = MAX_BOMBAS;
    pthread_mutex_unlock(&mutexBombas);
    pthread_mutex_lock(&mutexGotoxy);
    gotoxy(20, 28);
    printf("                                                 \n");
    pthread_mutex_unlock(&mutexGotoxy);
}

void *movimentaBomba(void *arg)
{
    int *idx = (int *)arg;
    int x = CANHAO_X - 1;
    int y = CANHAO_Y;
    const int posicaoCanhaoThread = posicaoCanhao;

    pthread_mutex_lock(&mutexBombas);
    bombasDisponiveis--;
    pthread_mutex_unlock(&mutexBombas);
    if (bombasDisponiveis == 0)
    {
        recarregaCanhao();
    };
    while (1)
    {
        pthread_mutex_lock(&mutexColisao);
        if (!bombas[*idx].ativo)
        {
            pthread_mutex_unlock(&mutexColisao);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&mutexColisao);
        pthread_mutex_lock(&mutexGotoxy);
        apagaBomba(x, y);
        pthread_mutex_unlock(&mutexGotoxy);
        switch (posicaoCanhaoThread)
        {
        case ZERO_GRAUS:
            x++;
            break;
        case QUARENTA_CINCO_GRAUS:
            x++;
            y--;
            break;
        case NOVENTA_GRAUS:
            y--;
            break;
        case CENTO_E_TRINTA_CINCO_GRAUS:
            x--;
            y--;
            break;
        case CENTO_E_OITENTA_GRAUS:
            x--;
            break;
        }

        if (x < 0 || x >= 120 || y < 0)
        {
            break;
        }

        pthread_mutex_lock(&mutexGotoxy);
        desenhaBomba(x, y);
        pthread_mutex_unlock(&mutexGotoxy);

        pthread_mutex_lock(&mutexColisao);
        bombas[*idx].x = x;
        bombas[*idx].y = y;
        pthread_mutex_unlock(&mutexColisao);

        Sleep(30);
    }
    pthread_exit(NULL);
}

void disparaBomba()
{
    pthread_t threadBomba;
    int *idx = malloc(sizeof(int));
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (!bombas[i].ativo)
        {
            *idx = i;
            bombas[i].ativo = true;
            break;
        }
    }
    pthread_create(&threadBomba, NULL, movimentaBomba, idx);
    pthread_detach(threadBomba);
}

void *iniciaCanhao()
{
    int continuar = 1;
    bombasDisponiveis = MAX_BOMBAS;
    while (continuar)
    {
        pthread_mutex_lock(&mutexGotoxy);
        apagaCanhao(CANHAO_X, CANHAO_Y);
        desenhaCanhao(CANHAO_X, CANHAO_Y, posicaoCanhao);
        pthread_mutex_unlock(&mutexGotoxy);
        if (kbhit())
        {
            // Captura a tecla pressionada
            int tecla = getch();
            switch (tecla)
            {
            case 224: // Tecla especial para setas no conio.h
                tecla = getch();
                switch (tecla)
                {
                case 75: // Seta para a esquerda
                    posicaoCanhao = pressionouEsquerda(posicaoCanhao);
                    break;
                case 77: // Seta para a direita
                    posicaoCanhao = pressionouDireita(posicaoCanhao);
                    break;
                case 72: // Seta para cima
                    posicaoCanhao = NOVENTA_GRAUS;
                    break;
                }
                break;
            case ' ':
                if (bombasDisponiveis > 0)
                {
                    disparaBomba();
                }
                break;
            case 'q':
            case 'Q':
                continuar = 0;
                break;
            }
        }

        Sleep(50);
    }
}

void *verificaColisao()
{
    while (1)
    {

        pthread_mutex_lock(&mutexColisao);
        for (int i = 0; i < MAX_NAVES; i++)
        {
            if (naves[i].ativo)
            {
                for (int j = 0; j < MAX_BOMBAS; j++)
                {
                    if (bombas[j].ativo)
                    {
                        if (bombas[j].x == naves[i].x || bombas[j].x == naves[i].x ||
                            bombas[j].y == naves[i].y || bombas[j].y == naves[i].y)
                        {
                            naves[i].ativo = false;
                            bombas[j].ativo = false;
                            pthread_mutex_lock(&mutexNaves);
                            navesDestruidas++;
                            pthread_mutex_unlock(&mutexNaves);
                            pthread_mutex_lock(&mutexGotoxy);
                            apagaNave(naves[i].x, naves[i].y);
                            apagaBomba(bombas[j].x, bombas[j].y);
                            pthread_mutex_unlock(&mutexGotoxy);
                        }
                    }
                }
            }
        }

        pthread_mutex_unlock(&mutexColisao);
        Sleep(150); // 50 ms
    }
}

int main()
{
    system("cls");
    pthread_mutex_lock(&mutexGotoxy);
    gotoxy(0, 26);
    pthread_mutex_unlock(&mutexGotoxy);

    for (int coluna = 0; coluna < 120; coluna++)
    {
        printf("^"); // chão
    }

    pthread_t threadContadores;
    pthread_create(&threadContadores, NULL, atualizaContatores, NULL);

    pthread_t threadCanhao;
    pthread_create(&threadCanhao, NULL, iniciaCanhao, NULL);

    pthread_t threadColisao;
    pthread_create(&threadColisao, NULL, verificaColisao, NULL);

    // thread para as bombas

    int *idx = malloc(sizeof(int));
    for (int i = 0; i < MAX_NAVES; i++)
    {
        pthread_t novaNave;
        *idx = i;
        naves[i].ativo = true;
        pthread_create(&novaNave, NULL, threadNave, idx);
        Sleep(3000);
    }

    getche();
}
