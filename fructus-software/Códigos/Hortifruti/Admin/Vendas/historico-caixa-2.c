#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define MAX_LINHA 256
#define MAX_COMPRAS 100

typedef struct
{
    char descricao[MAX_LINHA];
} Compra;

void centralizar_tabela(Compra compras[], int num_compras)
{
    int y_max, x_max;
    int largura_tabela = 0;
    int altura_tabela = num_compras + 2;

    for (int i = 0; i < num_compras; i++)
    {
        int largura_linha = strlen(compras[i].descricao);
        if (largura_linha > largura_tabela)
        {
            largura_tabela = largura_linha;
        }
    }

    getmaxyx(stdscr, y_max, x_max);

    int start_y = (y_max - altura_tabela) / 2;
    int start_x = (x_max - largura_tabela) / 2;

    // Desenha a borda ao redor da tabela
    for (int i = start_y; i <= start_y + altura_tabela; i++)
    {
        mvprintw(i, start_x - 1, "|");
        mvprintw(i, start_x + largura_tabela, "|");
    }
    mvprintw(start_y - 1, start_x - 1, "+");
    mvprintw(start_y - 1, start_x + largura_tabela, "+");
    mvprintw(start_y + altura_tabela, start_x - 1, "+");
    mvprintw(start_y + altura_tabela, start_x + largura_tabela, "+");

    for (int i = start_x; i < start_x + largura_tabela; i++)
    {
        mvprintw(start_y - 1, i, "-");
        mvprintw(start_y + altura_tabela, i, "-");
    }

    // Imprime as compras na tela centralizada
    for (int i = 0; i < num_compras; i++)
    {
        mvprintw(start_y + i + 1, start_x, "%s", compras[i].descricao);
    }

    refresh();
}

int main()
{
    FILE *arquivo;
    Compra compras[MAX_COMPRAS];
    int num_compras = 0;

    arquivo = fopen("caixa-1.txt", "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lê o arquivo linha por linha e armazena em um array de compras
    while (fgets(compras[num_compras].descricao, MAX_LINHA, arquivo) != NULL && num_compras < MAX_COMPRAS)
    {
        compras[num_compras].descricao[strcspn(compras[num_compras].descricao, "\n")] = 0; // Remove o newline
        num_compras++;
    }
    fclose(arquivo);

    initscr();
    noecho();
    curs_set(FALSE);

    clear();
    centralizar_tabela(compras, num_compras);

    mvprintw(LINES - 2, (COLS - strlen("Pressione Enter para sair...")) / 2, "Pressione Enter para sair...");
    refresh();
    getch();

    endwin(); 

    system("./vendas-menu");
    return 0;
}
