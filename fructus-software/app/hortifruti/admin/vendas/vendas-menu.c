#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

// Funções que são chamadas quando o usuário seleciona uma opção
void IniciarAdmin()
{
    system("./admin/admin-menu"); // Chama o script de menu de administração
    exit(0);
}

void VendasCaixa1()
{
    system("./admin/vendas/historico-caixa");
    exit(0);
}

void VendasCaixa2()
{
    system("./admin/vendas/historico-caixa-2");
    exit(0);
}

void VendasCaixa3()
{
    system("./admin/vendas/historico-caixa-3");
    exit(0);
}

// Função que exibe o menu de opções e navega entre elas
void mostrarMenu()
{
    char *opcoes[] = {
        "Vendas - Caixa 1",
        "Vendas - Caixa 2",
        "Vendas - Caixa 3",
        "Sair",
    };

    int escolha = 0;                                // Índice da opção selecionada
    int n_opcoes = sizeof(opcoes) / sizeof(char *); 
    int ch;                                         

    // Inicialização da interface ncurses
    initscr();
    noecho();             // Desativa a exibição de caracteres digitados
    curs_set(FALSE);      
    keypad(stdscr, TRUE); 

    while (1) // Loop que mantém o menu exibido até o usuário selecionar uma opção
    {
        clear(); // Limpa a tela a cada iteração

        // Calcula as posições para centralizar a caixa do menu na tela
        int largura_caixa = 30;
        int altura_caixa = n_opcoes + 4;
        int inicio_x = (COLS - largura_caixa) / 2;
        int inicio_y = (LINES - altura_caixa) / 2;

        // Exibe o título e as instruções na tela
        mvprintw(inicio_y - 2, (COLS - strlen("Acompanhe as Vendas")) / 2, "Acompanhe as Vendas");
        mvprintw(inicio_y - 1, inicio_x + 2, "Use as setas para navegar.");

        // Exibe as opções no menu
        for (int i = 0; i < n_opcoes; i++)
        {
            if (i == escolha)
            {
                attron(A_REVERSE); // Inverte as cores (destaque da opção selecionada)
            }
            mvprintw(inicio_y + i + 1, inicio_x + (largura_caixa - strlen(opcoes[i])) / 2, "%s", opcoes[i]);
            if (i == escolha)
            {
                attroff(A_REVERSE); // Desativa o destaque da opção
            }
        }

        // Desenha a borda ao redor do menu
        mvprintw(inicio_y, inicio_x, "+");
        mvprintw(inicio_y + altura_caixa - 1, inicio_x, "+");
        mvprintw(inicio_y, inicio_x + largura_caixa - 1, "+");
        mvprintw(inicio_y + altura_caixa - 1, inicio_x + largura_caixa - 1, "+");

        // Preenche as bordas laterais e superior/inferior
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

        refresh(); // Atualiza a tela

        // Aguarda a tecla pressionada pelo usuário
        ch = getch();
        switch (ch)
        {
        case KEY_UP:
            escolha = (escolha - 1 + n_opcoes) % n_opcoes; // Navega para cima
            break;
        case KEY_DOWN:
            escolha = (escolha + 1) % n_opcoes; // Navega para baixo
            break;
        case 10:             // Enter
            endwin();        // Finaliza o ncurses para poder rodar outros programas
            switch (escolha) // Ação de acordo com a opção escolhida
            {
            case 0:
                VendasCaixa1();
                break;
            case 1:
                VendasCaixa2();
                break;
            case 2:
                VendasCaixa3();
                break;
            case 3:
                IniciarAdmin(); // Sai do menu e chama a função de administração
                exit(0);
                break;
            }

            // Reinicia o ncurses após a seleção
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
    mostrarMenu(); // Chama a função que exibe o menu
    return 0;
}
