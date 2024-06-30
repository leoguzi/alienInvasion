#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

enum PosicaoCanhao
{
    ZERO_GRAUS = 0,
    QUARENTA_CINCO_GRAUS = 45,
    NOVENTA_GRAUS = 90,
    CENTO_E_TRINTA_CINCO_GRAUS = 135,
    CENTO_E_OITENTA_GRAUS = 180
};

int origemCanhao = 53;

// Fun��o gotoxy
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void explode_bomba(int x, int y)
{
    gotoxy(x, y);
    printf("*");
    Sleep(50);
    printf(" ");
    gotoxy(x, y - 1);
    printf("O");
    gotoxy(x - 1, y);
    printf("O O");
    gotoxy(x, y + 1);
    printf("O");
    Sleep(50);
    gotoxy(x, y - 1);
    printf(" ");
    gotoxy(x - 1, y);
    printf("   ");
    gotoxy(x, y + 1);
    printf(" ");

    gotoxy(x, y - 2);
    printf("o");
    gotoxy(x - 2, y);
    printf("o   o");
    gotoxy(x, y + 2);
    printf("o");
    Sleep(50);
    gotoxy(x, y - 2);
    printf(" ");
    gotoxy(x - 2, y);
    printf("     ");
    gotoxy(x, y + 2);
    printf(" ");
}

void desenhaNave(int x, int y)
{
    gotoxy(x, y);
    printf("     _     \n");
    gotoxy(x, y + 1);
    printf("   _( )_   \n");
    gotoxy(x, y + 2);
    printf("  /     \\  \n");
    gotoxy(x, y + 3);
    printf(" /_______\\ \n");
    gotoxy(x, y + 4);
    printf("   |   |   \n");
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

void canhaoAtira(int x, int y)
{
    int posicao;
    time_t t;
    gotoxy(x, y + 1);
    printf(" ___+--+___");
    gotoxy(x, y + 2);
    printf("/          \\");
    gotoxy(x, y + 3);
    printf("\\          /");
    gotoxy(x, y + 4);
    printf(" -O-O--O-O-");
    gotoxy(x, y);
    srand((unsigned)time(&t));
    for (int foguete = 3; foguete > 0; foguete--)
    {
        posicao = (rand() % 3) - 1;
        gotoxy(0, 28);
        printf("%6d  %6d\n", foguete, posicao);
        gotoxy(x, y);
        switch (posicao)
        {
        case -1:
            printf("     \\\\\n");
            break;
        case 0:
            printf("     ||");
            break;
        case 1:
            printf("     //");
        }
        bomba(x + 5, y - 1, posicao);
    }
}

int pressionouDireita(int posicaoAtual){
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

int pressionouEsquerda(int posicaoAtual){
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

int main()
{
    enum PosicaoCanhao posicao = NOVENTA_GRAUS;
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
                case 75:                                  // Seta para a esquerda
                    posicao = pressionouEsquerda(posicao); 
                    break;
                case 77:                            // Seta para a direita
                    posicao = pressionouDireita(posicao); 
                    break;
                case 72: // Seta para cima
                    posicao = NOVENTA_GRAUS;
                    break;
                }
                break;
            case 'q':
            case 'Q':
                return 0; // Encerra o programa ao pressionar 'q' ou 'Q'
            }
        }

        Sleep(200); // Pausa de 100ms entre cada atualização de tela (ajuste conforme necessário)
    }

    // canhaoAtira(origemCanhao, 20);      // Canhao Dispara

    // for (int i = 0; i < 7; i++)
    // { // Bombas explodem no ar
    //     explode_bomba((rand() % 70) + 10, (rand() % 10) + 4);
    //     Sleep(1000);
    // }

    while (numeroNaves)
    {
        int coluna = rand() % -87;
        for (linha = 0; linha < 19; linha++)
        {

            desenhaNave(coluna, linha);
            Sleep(200);
            apagaNave(coluna, linha);
        }
        numeroNaves--;
    }
    getche();
}
