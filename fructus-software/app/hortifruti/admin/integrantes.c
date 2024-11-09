#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void exibirMenu()
{
    int opcao = 0;
    int ch;

    while (1)
    {
        clear();

        int largura_caixa = 30;
        int altura_caixa = 8;
        int inicio_x = (COLS - largura_caixa) / 2;
        int inicio_y = (LINES - altura_caixa) / 2;

        mvprintw(inicio_y - 2, (COLS - strlen("Desenvolvedores")) / 2, "Desenvolvedores");
        mvprintw(inicio_y - 1, inicio_x + 2, "Use as setas para navegar");

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

        char *opcoes[] = {
            "1. Gustavo Correa",
            "2. João Paulo",
            "3. Kaique Tabboni",
            "4. Kauã Miguel",
            "5. Wagner Fortes",
            "Voltar"};
        int n_opcoes = sizeof(opcoes) / sizeof(char *);

        for (int i = 0; i < n_opcoes; i++)
        {
            if (i == opcao)
            {
                attron(A_REVERSE);
            }
            mvprintw(inicio_y + i + 1, inicio_x + (largura_caixa - strlen(opcoes[i])) / 2, "%s", opcoes[i]);
            if (i == opcao)
            {
                attroff(A_REVERSE);
            }
        }

        refresh();

        ch = getch();
        switch (ch)
        {
        case KEY_UP:
            opcao = (opcao - 1 + n_opcoes) % n_opcoes;
            break;
        case KEY_DOWN:
            opcao = (opcao + 1) % n_opcoes;
            break;
        case 10: // Enter
            switch (opcao)
            {
            case 6:
                endwin();
                return;
            }
            break;
        }
    }
}

int main()
{
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    exibirMenu();

    endwin();
    return 0;
}
