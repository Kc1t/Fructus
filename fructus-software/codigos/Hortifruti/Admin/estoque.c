#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void adicionarEstoque()
{
    FILE *file = fopen("./Servidor/estoque.txt", "a");
    if (file == NULL)
    {
        perror("Não foi possível abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char nome[100];
    int quantia;
    float peso;

    clear();

    // Centralizar o formulário de entrada
    int largura_formulario = 50;        // Ajuste conforme a largura desejada
    int linha_inicio = (LINES - 6) / 2; // 6 linhas de formulário
    int coluna_inicio = (COLS - largura_formulario) / 2;

    mvprintw(linha_inicio, coluna_inicio, "Digite o nome do item: ");
    echo();
    mvgetnstr(linha_inicio, coluna_inicio + 22, nome, sizeof(nome) - 1);
    noecho();
    nome[strcspn(nome, "\n")] = '\0';

    mvprintw(linha_inicio + 2, coluna_inicio, "Digite a quantia: ");
    echo();
    scanw("%d", &quantia);
    noecho();

    mvprintw(linha_inicio + 4, coluna_inicio, "Digite o peso (em kg): ");
    echo();
    scanw("%f", &peso);
    noecho();

    clear();
    fprintf(file, "%s %d %.2f\n", nome, quantia, peso);
    fclose(file);

    mvprintw(linha_inicio, (COLS - strlen("Item adicionado com sucesso!")) / 2, "Item adicionado com sucesso!");
    mvprintw(linha_inicio + 2, (COLS - strlen("Pressione qualquer tecla para continuar...")) / 2, "Pressione qualquer tecla para continuar...");
    getch();
}

void visualizarEstoque()
{
    FILE *file = fopen("./Servidor/estoque.txt", "r");
    if (file == NULL)
    {
        perror("Não foi possível abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char nome[100];
    int quantia;
    float peso;

    clear();

    // Centralizar a tabela inteira
    int largura_tabela = 50;
    int altura_tabela = 10;
    int coluna_inicio = (COLS - largura_tabela) / 2;
    int linha_inicio = (LINES - altura_tabela) / 2;

    mvprintw(linha_inicio, coluna_inicio, "Estoque:");
    mvprintw(linha_inicio + 1, coluna_inicio, "Nome                         Quantia  Peso (kg)");
    mvprintw(linha_inicio + 2, coluna_inicio, "---------------------------------------------------");

    int linha_atual = linha_inicio + 3;
    while (fscanf(file, "%99s %d %f", nome, &quantia, &peso) == 3)
    {
        mvprintw(linha_atual, coluna_inicio, "%-30s %7d %10.2f", nome, quantia, peso);
        linha_atual++;
    }

    fclose(file);
    mvprintw(linha_atual + 1, (COLS - strlen("Pressione qualquer tecla para continuar...")) / 2, "Pressione qualquer tecla para continuar...");
    getch();
}

void exibirMenu()
{
    int opcao = 0;
    int ch;

    while (1)
    {
        clear();

        int largura_caixa = 30;
        int altura_caixa = 7;
        int inicio_x = (COLS - largura_caixa) / 2;
        int inicio_y = (LINES - altura_caixa) / 2;

        mvprintw(inicio_y - 2, (COLS - strlen("Menu de Estoque")) / 2, "Menu de Estoque");
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
            "Adicionar ao estoque",
            "Visualizar estoque",
            "Sair"};
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
        case 10:
            switch (opcao)
            {
            case 0:
                adicionarEstoque();
                break;
            case 1:
                visualizarEstoque();
                break;
            case 2:
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
