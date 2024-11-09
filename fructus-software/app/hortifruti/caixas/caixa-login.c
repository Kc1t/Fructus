#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// Ação de Login
// Inicialização da interface gráfica ncurses
// Configura o terminal para o modo ncurses, desativa o echo e oculta o cursor.
// Essas funções são cruciais para criar uma interface interativa e responsiva no terminal.
void inicializarTela()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr, FALSE);
    curs_set(0);
    refresh();
}

// Finalização da interface ncurses
// Restaura o cursor e encerra o modo ncurses.
void finalizarTela()
{
    curs_set(1);
    endwin();
}

// Obtém o tamanho atual do terminal
// Função usada para ajustar o layout e centralizar textos e animações na tela.
void get_terminal_size(int *cols, int *rows)
{
    getmaxyx(stdscr, *rows, *cols);
}

// Animação de carregamento
// Exibe uma barra de progresso com texto centralizado para indicar o carregamento.
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

// Função de login
// Solicita a senha do usuário, verifica sua validade e executa uma animação de carregamento
// em caso de sucesso. Se a senha estiver incorreta, o usuário deve tentar novamente.
void login()
{
    char password[20];
    int cols, rows;
    get_terminal_size(&cols, &rows);

    while (1) // Loop até que a senha correta seja inserida
    {
        clear();
        mvprintw(rows / 2 - 1, (cols - 20) / 2, "Digite a senha:");

        curs_set(1);
        echo();
        mvgetnstr(rows / 2, (cols - 20) / 2, password, 20);
        curs_set(0);
        noecho();

        if (strcmp(password, "cashier") == 0) // Verificação da senha
        {
            clear();
            mvprintw(rows / 2, (cols - 24) / 2, "Login bem-sucedido!");
            refresh();
            usleep(1000000);

            // Executa a animação de carregamento
            for (int i = 0; i <= 10; i++)
            {
                clear();
                print_loading_animation(i, cols);
                usleep(500000);
            }

            finalizarTela();
            system("./caixas/caixa-menu"); // Inicia o menu do caixa
            break;
        }
        else
        {
            // Mensagem de erro se a senha estiver incorreta
            clear();
            mvprintw(rows / 2, (cols - 27) / 2, "Senha invalida. Tente novamente.");
            refresh();
            usleep(1500000);
        }
    }
}

// Função principal
// Inicializa a tela e chama a função de login.
int main()
{
    inicializarTela();
    login();
    return 0;
}
