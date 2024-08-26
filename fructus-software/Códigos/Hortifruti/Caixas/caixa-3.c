#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#define NUM_PRODUTOS 20
#define LARGURA_TABELA 35 // Aumentado para garantir que a borda direita não fique cortada

typedef struct
{
    char nome[20];
    float preco_por_kg;
} Produto;

void mostrar_menu(Produto produtos[], int selecionado)
{
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max); // Obtém o tamanho atual da tela (y_max, x_max)

    int start_y = (y_max - (NUM_PRODUTOS + 5)) / 2; // Calcula a linha inicial para centralizar verticalmente
    int start_x = (x_max - LARGURA_TABELA) / 2;     // Calcula a coluna inicial para centralizar horizontalmente

    // Desenha a borda ao redor da tabela
    for (int i = start_y; i <= start_y + NUM_PRODUTOS + 1; i++)
    {
        mvprintw(i, start_x - 1, "|");
        mvprintw(i, start_x + LARGURA_TABELA - 1, "|");
    }
    mvprintw(start_y - 1, start_x - 1, "+");
    mvprintw(start_y - 1, start_x + LARGURA_TABELA - 1, "+");
    mvprintw(start_y + NUM_PRODUTOS + 1, start_x - 1, "+");
    mvprintw(start_y + NUM_PRODUTOS + 1, start_x + LARGURA_TABELA - 1, "+");
    for (int i = start_x; i < start_x + LARGURA_TABELA - 1; i++)
    {
        mvprintw(start_y - 1, i, "-");
        mvprintw(start_y + NUM_PRODUTOS + 1, i, "-");
    }

    for (int i = 0; i < NUM_PRODUTOS; i++)
    {
        if (i == selecionado)
        {
            attron(A_REVERSE); // Inverte as cores para destacar o item selecionado
        }
        mvprintw(start_y + i, start_x, "%-20s R$ %.2f/kg", produtos[i].nome, produtos[i].preco_por_kg);
        if (i == selecionado)
        {
            attroff(A_REVERSE);
        }
    }

    // Centraliza o botão "Voltar" abaixo dos produtos
    if (selecionado == NUM_PRODUTOS)
    {
        attron(A_REVERSE); // Destaca o botão "Voltar" quando selecionado
        mvprintw(start_y + NUM_PRODUTOS + 2, start_x, "Voltar");
        attroff(A_REVERSE);
    }
    else
    {
        mvprintw(start_y + NUM_PRODUTOS + 2, start_x, "Voltar");
    }

    refresh();
}

void registrar_compra(const char *nome_produto, float peso, float preco_total)
{
    FILE *arquivo;
    arquivo = fopen("caixa-3.txt", "a");
    if (arquivo == NULL)
    {
        endwin(); // Encerra o ncurses
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
    fprintf(arquivo, "Produto: %-15s | Peso: %6.2f kg | Preço Total: R$ %6.2f\n",
            nome_produto, peso, preco_total);
    fclose(arquivo);
}

void voltar()
{
    system("./cashier-menu");
    exit(0);
}

int main()
{
    Produto produtos[NUM_PRODUTOS] = {
        {"Laranja", 14.55},
        {"Maca", 10.30},
        {"Abacaxi", 8.70},
        {"Banana", 5.60},
        {"Uva", 12.90},
        {"Melancia", 18.20},
        {"Manga", 7.50},
        {"Pera", 9.80},
        {"Cenoura", 4.30},
        {"Tomate", 6.20},
        {"Batata", 3.50},
        {"Alface", 2.80},
        {"Brocolis", 6.90},
        {"Couve", 4.50},
        {"Pepino", 5.00},
        {"Pimentao", 7.20},
        {"Cebola", 3.80},
        {"Alho", 15.00},
        {"Beterraba", 4.40},
        {"Coco", 8.00}};

    int selecionado = 0;
    float peso;
    float preco_total;

    initscr();            // Inicializa a tela
    noecho();             // Não mostra os caracteres digitados
    curs_set(FALSE);      // Oculta o cursor
    keypad(stdscr, TRUE); // Habilita o uso de teclas como setas

    while (1)
    {
        clear();
        int start_y, start_x;
        getmaxyx(stdscr, start_y, start_x); // Obtém o tamanho da tela

        start_y = (start_y - (NUM_PRODUTOS + 6)) / 2; // Calcula a linha inicial para centralizar verticalmente
        start_x = (start_x - LARGURA_TABELA) / 2;     // Calcula a coluna inicial para centralizar horizontalmente

        // Títulos
        mvprintw(start_y - 3, (COLS - strlen("-Caixa 3-")) / 2, "-Caixa 3-");
        mvprintw(start_y - 2, (COLS - strlen("Para calcular o valor da compra")) / 2, "Para calcular o valor da compra");
        mvprintw(start_y - 1, (COLS - strlen("Escolha o produto:")) / 2, "Escolha o produto:");

        // Mostrar o menu de produtos
        mostrar_menu(produtos, selecionado);

        int tecla = getch(); // Captura a tecla pressionada

        switch (tecla)
        {
        case KEY_UP:
            if (selecionado > 0)
            {
                selecionado--;
            }
            break;
        case KEY_DOWN:
            if (selecionado < NUM_PRODUTOS)
            {
                selecionado++;
            }
            break;
        case '\n':                           // Tecla Enter
            if (selecionado == NUM_PRODUTOS) // Verifica se o botão "Voltar" foi selecionado
            {
                voltar();
                break;
            }
            clear();
            mvprintw(2, (COLS - strlen("Produto selecionado:")) / 2, "Produto selecionado: %s", produtos[selecionado].nome);
            mvprintw(4, (COLS - strlen("Digite o peso (kg):")) / 2, "Digite o peso (kg): ");
            echo();
            curs_set(TRUE);
            scanw("%f", &peso);
            noecho();
            curs_set(FALSE);

            preco_total = produtos[selecionado].preco_por_kg * peso;
            mvprintw(6, (COLS - strlen("Preço total:")) / 2, "Preço total: R$ %.2f", preco_total);

            // Registra a compra em um arquivo de texto
            registrar_compra(produtos[selecionado].nome, peso, preco_total);

            mvprintw(8, (COLS - strlen("Pressione qualquer tecla para continuar")) / 2, "Pressione qualquer tecla para continuar");
            refresh();
            getch();
            break;
        }

        // Atualiza o menu de produtos
        clear();
        mvprintw(start_y - 3, (COLS - strlen("-Caixa 3-")) / 2, "-Caixa 3-");
        mvprintw(start_y - 2, (COLS - strlen("Para calcular o valor da compra")) / 2, "Para calcular o valor da compra");
        mvprintw(start_y - 1, (COLS - strlen("Escolha o produto:")) / 2, "Escolha o produto:");
        mostrar_menu(produtos, selecionado);
    }

    endwin(); // Encerra o ncurses
    return 0;
}
