#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#define NUM_PRODUTOS 20
#define LARGURA_TABELA 35

typedef struct
{
    char nome[20];
    float preco_por_kg;
} Produto;

typedef struct
{
    char nome[20];
    float peso;
    float preco_total;
} ItemCompra;

void mostrar_menu(Produto produtos[], int selecionado)
{
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);

    int start_y = (y_max - (NUM_PRODUTOS + 5)) / 2;
    int start_x = (x_max - LARGURA_TABELA) / 2;

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
            attron(A_REVERSE);
        }
        mvprintw(start_y + i, start_x, "%-20s R$ %.2f/kg", produtos[i].nome, produtos[i].preco_por_kg);
        if (i == selecionado)
        {
            attroff(A_REVERSE);
        }
    }

    if (selecionado == NUM_PRODUTOS)
    {
        attron(A_REVERSE);
        mvprintw(start_y + NUM_PRODUTOS + 2, start_x, "Voltar");
        attroff(A_REVERSE);
    }
    else
    {
        mvprintw(start_y + NUM_PRODUTOS + 2, start_x, "Voltar");
    }

    refresh();
}

void registrar_compra(ItemCompra itens[], int num_itens, float total)
{
    FILE *arquivo = fopen("caixa-1.txt", "a");
    if (arquivo == NULL)
    {
        endwin();
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    fprintf(arquivo, "Compra: ");
    for (int i = 0; i < num_itens; i++)
    {
        fprintf(arquivo, "%s: %.2f kg = R$ %.2f", itens[i].nome, itens[i].peso, itens[i].preco_total);
        if (i < num_itens - 1)
        {
            fprintf(arquivo, " ");
        }
    }
    fprintf(arquivo, " Total: R$ %.2f\n", total);
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
    float valor_total = 0;
    char resposta;

    ItemCompra itens[NUM_PRODUTOS];
    int num_itens = 0;

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    while (1)
    {
        clear();
        int start_y, start_x;
        getmaxyx(stdscr, start_y, start_x);

        start_y = (start_y - (NUM_PRODUTOS + 6)) / 2;
        start_x = (start_x - LARGURA_TABELA) / 2;

        mvprintw(start_y - 3, (COLS - strlen("-Caixa-")) / 2, "-Caixa-");
        mvprintw(start_y - 2, (COLS - strlen("Para calcular o valor da compra")) / 2, "Para calcular o valor da compra");
        mvprintw(start_y - 1, (COLS - strlen("Escolha o produto:")) / 2, "Escolha o produto:");

        mostrar_menu(produtos, selecionado);

        int tecla = getch();

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
        case '\n':
            if (selecionado == NUM_PRODUTOS)
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

            itens[num_itens].peso = peso;
            itens[num_itens].preco_total = preco_total;
            strcpy(itens[num_itens].nome, produtos[selecionado].nome);
            num_itens++;
            valor_total += preco_total;

            mvprintw(8, (COLS - strlen("Deseja adicionar mais itens? (S/N)")) / 2, "Deseja adicionar mais itens? (S/N)");
            refresh();

            resposta = getch();
            if (resposta == 'N' || resposta == 'n')
            {
                clear();
                mvprintw(2, (COLS - strlen("Valor total da compra:")) / 2, "Valor total da compra: R$ %.2f", valor_total);
                registrar_compra(itens, num_itens, valor_total);
                mvprintw(4, (COLS - strlen("Pressione qualquer tecla para sair")) / 2, "Pressione qualquer tecla para sair");
                refresh();
                getch();
                endwin();
                system("./caixa");
                return 0;
            }
            break;
        }
    }

    endwin();
    return 0;
}
