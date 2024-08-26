#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void inicializarTela()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr, FALSE); // Evita o scroll da tela
    curs_set(0);             // Oculta o cursor
    refresh();
}

void finalizarTela()
{
    curs_set(1); // Restaura o cursor antes de finalizar
    endwin();
}

void get_terminal_size(int *cols, int *rows)
{
    getmaxyx(stdscr, *rows, *cols);
}

void print_loading_animation(int progress, int terminal_width)
{
    const char *loading_text = "Carregando";
    int text_length = strlen(loading_text);
    int bar_width = 10;
    int center_position = terminal_width / 2;
    int text_position = center_position - (text_length + bar_width + 5) / 2;

    mvprintw(LINES / 2, text_position, "[");
    for (int i = 0; i < bar_width; i++)
    {
        if (i < progress)
        {
            printw("=");
        }
        else
        {
            printw(" ");
        }
    }
    printw("] %d%% %s", progress * 10, loading_text);

    refresh();
}

void login()
{
    char password[20];
    int cols, rows;
    get_terminal_size(&cols, &rows);

    while (1) // Loop para pedir a senha até que esteja correta
    {
        clear();

        // Mensagem para digitar a senha
        mvprintw(rows / 2 - 1, (cols - 20) / 2, "Digite a senha:");

        // Ativa o cursor e oculta o texto digitado (para a senha)
        curs_set(1);
        echo(); // Mostra os caracteres digitados

        // Captura a senha do usuário
        mvgetnstr(rows / 2, (cols - 20) / 2, password, 20);

        // Desativa o cursor e oculta o texto digitado (volta ao padrão)
        curs_set(0);
        noecho(); // Oculta os caracteres digitados

        // Verifica a senha
        if (strcmp(password, "cashier") == 0)
        {
            clear();
            mvprintw(rows / 2, (cols - 24) / 2, "Login bem-sucedido!");
            refresh();
            usleep(1000000);

            // Desativa a interação com a tela
            noecho();
            keypad(stdscr, FALSE);   // Desativa as teclas de função
            scrollok(stdscr, FALSE); // Desativa o scroll
            curs_set(0);             // Oculta o cursor

            // Animação de carregamento
            for (int i = 0; i <= 10; i++)
            {
                clear();
                print_loading_animation(i, cols);
                usleep(500000);
            }

            finalizarTela();
            system("./cashier-menu"); 
            break;
        }
        else
        {
            clear();
            mvprintw(rows / 2, (cols - 27) / 2, "Senha invalida. Tente novamente.");
            refresh();
            usleep(1500000);
        }
    }
}

int main()
{
    inicializarTela();

    login(); 

    return 0;
}