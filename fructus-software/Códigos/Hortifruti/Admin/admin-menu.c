#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void iniciarEstoque()
{
    system("./estoque");
}

void iniciarIntegrantes()
{
    system("./integrantes");
}

void IniciarMenuVendas()
{
    system("./vendas-menu");
    exit(0);
}

void IniciarMenu()
{
    system("./bem-vindo");
    exit(0);
}

void mostrarMenu()
{
    char *opcoes[] = {
        "Visualizar Vendas",
        "Abrir Estoque",
        "Ajuda",
        "Desenvolvedores",
        "Sair",
    };

    int escolha = 0;
    int n_opcoes = sizeof(opcoes) / sizeof(char *);
    int ch;

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    while (1)
    {
        clear();

        // Caixa
        int largura_caixa = 30;
        int altura_caixa = n_opcoes + 4;
        int inicio_x = (COLS - largura_caixa) / 2;
        int inicio_y = (LINES - altura_caixa) / 2;

        mvprintw(inicio_y - 2, (COLS - strlen("Bem-vindo Admin do Fructus!")) / 2, "Bem-vindo Admin do Fructus!");
        mvprintw(inicio_y - 1, inicio_x + 2, "Use as setas para navegar.");

        for (int i = 0; i < n_opcoes; i++)
        {
            if (i == escolha)
            {
                attron(A_REVERSE);
            }
            mvprintw(inicio_y + i + 1, inicio_x + (largura_caixa - strlen(opcoes[i])) / 2, "%s", opcoes[i]);
            if (i == escolha)
            {
                attroff(A_REVERSE);
            }
        }

        // Desenha a borda
        mvprintw(inicio_y, inicio_x, "+");
        mvprintw(inicio_y + altura_caixa - 1, inicio_x, "+");
        mvprintw(inicio_y, inicio_x + largura_caixa - 1, "+");
        mvprintw(inicio_y + altura_caixa - 1, inicio_x + largura_caixa - 1, "+");

        for (int i = 1; i < largura_caixa - 1; i++)
        {
            mvprintw(inicio_y, inicio_x + i, "-");
            mvprintw(inicio_y + altura_caixa - 1, inicio_x + i, "-");
        }

        for (int i = 1; i < altura_caixa - 1; i++)
        {
            mvprintw(inicio_y + i, inicio_x, "|");
            mvprintw(inicio_y + i, inicio_x + largura_caixa - 1, "|");
        }

        refresh();

        ch = getch();
        switch (ch)
        {
        case KEY_UP:
            escolha = (escolha - 1 + n_opcoes) % n_opcoes;
            break;
        case KEY_DOWN:
            escolha = (escolha + 1) % n_opcoes;
            break;
        case 10:
            endwin();
            switch (escolha)
            {
            case 0:
                IniciarMenuVendas();
                break;
            case 1:
                iniciarEstoque();
                break;
            case 2:
                break;
            case 3:
                iniciarIntegrantes();
                break;
            case 4:
                IniciarMenu();
                exit(0);
                break;
            }

            initscr();
            noecho();
            curs_set(FALSE);
            keypad(stdscr, TRUE);
            break;
        }
    }
}

int main()
{
    mostrarMenu();
    return 0;
}
