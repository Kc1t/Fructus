#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

void inicializarTela()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    curs_set(0);
    refresh();
}

void finalizarTela()
{
    endwin();
    exit(0);
}

void adminLogin()
{
    system("./Admin/admin-login");
}

void cashierLogin()
{
    system("./Caixas/cashier-login");
}

void exibirArte()
{
    const char *arte_ascii[] = {
        "     _____ ____  _   _  ____ _____ _   _ ____        ",
        "    |  ___|  _ \\| | | |/ ___|_   _| | | / ___|       ",
        "    | |_  | |_) | | | | |     | | | | | \\___ \\       ",
        "    |  _| |  _ <| |_| | |___  | | | |_| |___) |      ",
        " ___|_| __|_|_\\_\\___/_\\____| |_| _\\___/|____/ _____ ",
        "/ ___| / _ \\|  ___|_   _\\ \\      / / \\  |  _ \\| ____|",
        "\\___ \\| | | | |_    | |  \\ \\ /\\ / / _ \\ | |_) |  _|  ",
        " ___) | |_| |  _|   | |   \\ V  V / ___ \\|  _ <| |___ ",
        "|____/ \\___/|_|     |_|    \\_/\\_/_/   \\_\\_| \\_\\_____|",
    };

    clear();

    int ART_HEIGHT = sizeof(arte_ascii) / sizeof(arte_ascii[0]);
    int ART_WIDTH = strlen(arte_ascii[0]);

    int start_y = (LINES - ART_HEIGHT) / 2 - 2;
    int start_x = (COLS - ART_WIDTH) / 2;

    for (int i = 0; i < ART_HEIGHT; ++i)
    {
        mvprintw(start_y + i, start_x, "%s", arte_ascii[i]);
    }

    refresh();
}

void exibirOpcoes(int selecionado)
{
    const char *opcoes[] = {
        "Entrar como Administrador",
        "Entrar como Caixa",
        "Encerrar Programa",
    };

    int num_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    int start_y = (LINES + sizeof(opcoes) / sizeof(opcoes[0])) / 2 + 3;

    for (int i = 0; i < num_opcoes; ++i)
    {
        if (i == selecionado)
        {
            attron(A_REVERSE);
        }
        mvprintw(start_y + i, (COLS - strlen(opcoes[i])) / 2, "%s", opcoes[i]);
        if (i == selecionado)
        {
            attroff(A_REVERSE);
        }
    }

    refresh();
}

int main()
{
    inicializarTela();
    exibirArte();

    int selecionado = 0;
    int ch;

    exibirOpcoes(selecionado);

    while ((ch = getch()) != 'q')
    {
        switch (ch)
        {
        case KEY_UP:
            selecionado = (selecionado == 0) ? 2 : selecionado - 1;
            break;
        case KEY_DOWN:
            selecionado = (selecionado == 2) ? 0 : selecionado + 1;
            break;
        case 10: // Tecla Enter
            if (selecionado == 0)
            {
                adminLogin();
            }
            else if (selecionado == 1)
            {
                cashierLogin();
            }
            else if (selecionado == 2)
            {
                finalizarTela();
                exit(0);
            }
            break;
        default:
            break;
        }

        exibirOpcoes(selecionado);
    }

    finalizarTela();
    return 0;
}
