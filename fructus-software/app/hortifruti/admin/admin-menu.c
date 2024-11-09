#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

// Função que inicia o estoque
void iniciarEstoque()
{
    system("./admin/ver-produtos"); // Executa o programa para visualizar/gerenciar produtos
}

// Função que inicia a visualização de integrantes
void iniciarIntegrantes()
{
    system("./admin/integrantes"); // Executa o programa para visualizar/gerenciar integrantes
}

// Função que inicia o menu de vendas
void IniciarMenuVendas()
{
    system("./admin/vendas/vendas-menu"); // Executa o programa de menu de vendas
    exit(0);                              // Finaliza o programa após a execução do menu
}

// Função que inicia o menu inicial
void IniciarMenu()
{
    system("./bem-vindo"); // Executa o programa de boas-vindas
    exit(0);               // Finaliza o programa
}

// Função que exibe o menu principal
void mostrarMenu()
{
    // Definição das opções de menu
    char *opcoes[] = {
        "Visualizar Vendas",
        "Visualizar Produtos",
        "Desenvolvedores",
        "Sair",
    };

    int escolha = 0;
    int n_opcoes = sizeof(opcoes) / sizeof(char *); // Número de opções no menu
    int ch;                                         // Variável para armazenar a tecla pressionada

    // Inicializa a tela ncurses para interação
    initscr();
    noecho();             // Não exibe os caracteres digitados
    curs_set(FALSE);      // Oculta o cursor
    keypad(stdscr, TRUE); // Permite o uso das teclas especiais (setas, etc.)

    while (1) // Loop principal do menu
    {
        clear();

        // Configurações para desenhar a caixa em volta do menu
        int largura_caixa = 30;
        int altura_caixa = n_opcoes + 4;
        int inicio_x = (COLS - largura_caixa) / 2;
        int inicio_y = (LINES - altura_caixa) / 2;

        // Exibe uma mensagem de boas-vindas no topo da tela
        mvprintw(inicio_y - 2, (COLS - strlen("Bem-vindo admin do Fructus!")) / 2, "Bem-vindo admin do Fructus!");
        mvprintw(inicio_y - 1, inicio_x + 2, "Use as setas para navegar.");

        // Exibe as opções do menu, destacando a opção selecionada
        for (int i = 0; i < n_opcoes; i++)
        {
            if (i == escolha) // Destaca a opção selecionada
            {
                attron(A_REVERSE);
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

        // Preenche a borda superior e inferior da caixa
        for (int i = 1; i < largura_caixa - 1; i++)
        {
            mvprintw(inicio_y, inicio_x + i, "-");
            mvprintw(inicio_y + altura_caixa - 1, inicio_x + i, "-");
        }

        // Preenche as bordas laterais da caixa
        for (int i = 1; i < altura_caixa - 1; i++)
        {
            mvprintw(inicio_y + i, inicio_x, "|");
            mvprintw(inicio_y + i, inicio_x + largura_caixa - 1, "|");
        }

        refresh();

        ch = getch(); // Captura a tecla pressionada
        switch (ch)
        {
        case KEY_UP: // Move a seleção para cima
            escolha = (escolha - 1 + n_opcoes) % n_opcoes;
            break;
        case KEY_DOWN:
            escolha = (escolha + 1) % n_opcoes;
            break;
        case 10:
            endwin(); // Finaliza o modo ncurses
            switch (escolha)
            {
            case 0:
                IniciarMenuVendas(); // Inicia o menu de vendas
                break;
            case 1:
                iniciarEstoque(); // Inicia o sistema de estoque
                break;
            case 2:
                iniciarIntegrantes(); // Inicia a visualização de integrantes
                break;
            case 3:
                IniciarMenu(); // Retorna ao menu inicial
                exit(0);
                break;
            }

            // Re-inicia a tela ncurses após a execução da ação
            initscr();
            noecho();
            curs_set(FALSE);
            keypad(stdscr, TRUE);
            break;
        }
    }
}

// Função principal
// Inicia o menu principal chamando a função mostrarMenu.
int main()
{
    mostrarMenu();
    return 0;
}
