#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

// Funções para iniciar os diferentes caixas
// Cada função executa um comando do sistema para iniciar o respectivo caixa e encerra o programa.
void IniciarMenu()
{
    system("./bem-vindo");
    exit(0);
}

// Existem 3 caixas, cada script inicia o código com o respectivo caixa.
void IniciarCaixa()
{
    system("./caixas/caixa");
    exit(0);
}

void IniciarCaixa2()
{
    system("./caixas/caixa-2");
    exit(0);
}

void IniciarCaixa3()
{
    system("./caixas/caixa-3");
    exit(0);
}

// Função para mostrar o menu principal
// Cria uma interface gráfica simples usando ncurses, com opções para iniciar diferentes caixas ou sair.
void mostrarMenu()
{
    char *opcoes[] = {
        "Iniciar Caixa 1",
        "Iniciar Caixa 2",
        "Iniciar Caixa 3",
        "Sair",
    };

    int escolha = 0;
    int n_opcoes = sizeof(opcoes) / sizeof(char *);
    int ch;

    // Inicializa o modo ncurses
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    // Loop para exibir o menu e capturar a escolha do usuário
    while (1)
    {
        clear();

        // Definindo dimensões e posição da caixa do menu
        int largura_caixa = 30;
        int altura_caixa = n_opcoes + 4;
        int inicio_x = (COLS - largura_caixa) / 2;
        int inicio_y = (LINES - altura_caixa) / 2;

        // Exibindo o título e instruções
        mvprintw(inicio_y - 2, (COLS - strlen("Bem-vindo ao Caixa Fructus!")) / 2, "Bem-vindo ao Caixa Fructus!");
        mvprintw(inicio_y - 1, inicio_x + 2, "Use as setas para operar.");

        // Exibindo as opções do menu
        for (int i = 0; i < n_opcoes; i++)
        {
            if (i == escolha)
            {
                attron(A_REVERSE); // Destaca a opção selecionada
            }
            mvprintw(inicio_y + i + 1, inicio_x + (largura_caixa - strlen(opcoes[i])) / 2, "%s", opcoes[i]);
            if (i == escolha)
            {
                attroff(A_REVERSE);
            }
        }

        // Desenha a borda da caixa
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

        // Captura a entrada do usuário (setas para navegar e Enter para selecionar)
        ch = getch();
        switch (ch)
        {
        case KEY_UP:
            escolha = (escolha - 1 + n_opcoes) % n_opcoes; // Move para cima, com rotação
            break;
        case KEY_DOWN:
            escolha = (escolha + 1) % n_opcoes; // Move para baixo, com rotação
            break;
        case 10: // Enter
            endwin();
            switch (escolha)
            {
                // Escolhe o caixa a ser iniciado com base na opção selecionada
            case 0:
                IniciarCaixa();
                break;
            case 1:
                IniciarCaixa2();
                break;
            case 2:
                IniciarCaixa3();
                break;
            case 3:
                IniciarMenu(); // Volta para o menu principal
                exit(0);
                break;
            }

            // Re-inicializa ncurses após sair de um submenu
            initscr();
            noecho();
            curs_set(FALSE);
            keypad(stdscr, TRUE);
            break;
        }
    }
}

// Função principal que inicia o programa e exibe o menu
int main()
{
    mostrarMenu();
    exit(0);
    return 0;
}
