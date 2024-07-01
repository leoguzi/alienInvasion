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

struct Nave
{
    int x;
    int y;
    pthread_mutex_t mutex;
};

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

void bomba(int x, int y, int posicao)
{
    for (; y > 0; y--)
    {
        gotoxy(x, y);
        printf("o\n");
        Sleep(20);
        gotoxy(x, y);
        printf(" \n");
        switch (posicao)
        {
        case -1:
            x--;
            break;
        case 0:
            x;
            break;
        case 1:
            x++;
        }
    }
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

void *threadNave(void *arg)
{
    struct Nave *disco = (struct Nave *)arg;
    while (1)
    {

        pthread_mutex_lock(&disco->mutex);
        apagaNave(disco->x, disco->y);
        disco->y += 1;
        if (disco->y > 23)
        {
            apagaNave(disco->x, disco->y);
            pthread_mutex_unlock(&disco->mutex);
            break;
        }
        desenhaNave(disco->x, disco->y);
        pthread_mutex_unlock(&disco->mutex);
        Sleep(200);
    }
    return NULL;
}

void iniciaCanhao(int x, int y)
{
    enum PosicaoCanhao posicao = NOVENTA_GRAUS;
    while (1)
    {
        apagaCanhao(x, y);
        desenhaCanhao(x, y, posicao);
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
                    posicao = pressionouEsquerda(posicao);
                    break;
                case 77: // Seta para a direita
                    posicao = pressionouDireita(posicao);
                    break;
                case 72: // Seta para cima
                    posicao = NOVENTA_GRAUS;
                    break;
                }
                break;
            case 'q':
            case 'Q':
                break;
            }
        }
        Sleep(100); // Pausa de 100ms entre cada atualização de tela (ajuste conforme necessário)
    }
}

int main()
{
    int x = 59; // Posição inicial x do canhão
    int y = 27; // Posição inicial y do canhao
    int linha, coluna = 0;
    int numeroFoguetes = 5;
    int numeroNaves = 5;

    system("cls");

    gotoxy(0, 28);
    for (coluna = 0; coluna < 120; coluna++)
    {
        printf("^"); // chão
    }

    //iniciaCanhao(x, y); // deve ser uma thread.

    //thread para as bombas

    struct Nave naves[numeroNaves];
    pthread_t threads[numeroNaves];
    bool colunasOcupadas[109] = {false};

    srand(time(NULL));

    for (int i = 0; i < numeroNaves; i++)
    {
        int x;
        do
        {
            x = rand() % 109;
        } while (colunasOcupadas[x] || colunasOcupadas[x + 1] || colunasOcupadas[x + 2] ||
                 colunasOcupadas[x + 3] || colunasOcupadas[x + 4] || colunasOcupadas[x + 5] ||
                 colunasOcupadas[x + 6] || colunasOcupadas[x + 7] || colunasOcupadas[x + 8]);
        naves[i].x = x;
        naves[i].y = 0;
        pthread_mutex_init(&naves[i].mutex, NULL);
        pthread_create(&threads[i], NULL, threadNave, (void *)&naves[i]);
        Sleep(1500);
    }

    getche();
}
