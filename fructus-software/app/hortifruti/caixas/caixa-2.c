#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sqlite3.h>

#define NUM_PRODUTOS 20
#define LARGURA_TABELA 35
#define NUM_OPCOES 2

typedef struct
{
    char nome[20];
    float preco_por_kg;
    float preco_granel;
} Produto;

typedef struct
{
    char nome[20];
    float peso;
    float preco_total;
    char tipo_compra[10];
} ItemCompra;

void criar_tabela_vendas(sqlite3 *db)
{
    char *err_msg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS vendas_caixa2 ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "nome_produto TEXT NOT NULL, "
                      "peso REAL NOT NULL, "
                      "preco_total REAL NOT NULL, "
                      "tipo_compra TEXT NOT NULL);";

    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao criar tabela: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }
}

void mostrar_menu(Produto produtos[], int selecionado, int num_produtos)
{
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);

    int start_y = (y_max - (num_produtos + 5)) / 2;
    int start_x = (x_max - LARGURA_TABELA) / 2;

    for (int i = start_y; i <= start_y + num_produtos + 1; i++)
    {
        mvprintw(i, start_x - 1, "|");
        mvprintw(i, start_x + LARGURA_TABELA - 1, "|");
    }
    mvprintw(start_y - 1, start_x - 1, "+");
    mvprintw(start_y - 1, start_x + LARGURA_TABELA - 1, "+");
    mvprintw(start_y + num_produtos + 1, start_x - 1, "+");
    mvprintw(start_y + num_produtos + 1, start_x + LARGURA_TABELA - 1, "+");
    for (int i = start_x; i < start_x + LARGURA_TABELA - 1; i++)
    {
        mvprintw(start_y - 1, i, "-");
        mvprintw(start_y + num_produtos + 1, i, "-");
    }

    for (int i = 0; i < num_produtos; i++)
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

    if (selecionado == num_produtos)
    {
        attron(A_REVERSE);
        mvprintw(start_y + num_produtos + 2, start_x, "Voltar");
        attroff(A_REVERSE);
    }
    else
    {
        mvprintw(start_y + num_produtos + 2, start_x, "Voltar");
    }

    refresh();
}

void mostrar_opcoes_calculo(int selecionado)
{
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);

    int start_y = (y_max - 4) / 2;
    int start_x = (x_max - 30) / 2;

    const char *opcoes[NUM_OPCOES] = {"Calcular por Peso", "Calcular por Granel"};

    for (int i = 0; i < NUM_OPCOES; i++)
    {
        if (i == selecionado)
        {
            attron(A_REVERSE);
        }
        mvprintw(start_y + i, start_x, "%s", opcoes[i]);
        if (i == selecionado)
        {
            attroff(A_REVERSE);
        }
    }

    refresh();
}

void registrar_compra(sqlite3 *db, ItemCompra itens[], int num_itens, float total)
{
    char *err_msg = 0;
    sqlite3_stmt *stmt;

    const char *sql = "INSERT INTO vendas_caixa2(nome_produto, peso, preco_total, tipo_compra) VALUES(?, ?, ?, ?)";

    for (int i = 0; i < num_itens; i++)
    {
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
        {
            endwin();
            printf("Falha ao preparar a inserção: %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_bind_text(stmt, 1, itens[i].nome, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, itens[i].peso);
        sqlite3_bind_double(stmt, 3, itens[i].preco_total);
        sqlite3_bind_text(stmt, 4, itens[i].tipo_compra, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            endwin();
            printf("Falha ao executar a inserção: %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_finalize(stmt);
    }

    clear();
    mvprintw((LINES - 1) / 2, (COLS - strlen("Compra registrada com sucesso!")) / 2, "Compra registrada com sucesso!");
    refresh();
    napms(2000);
}

void voltar()
{
    system("./caixas/caixa-menu");
    exit(0);
}

int carregar_produtos(sqlite3 *db, Produto produtos[])
{
    int num_produtos = 0;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT nome, preco_kg, preco_granel FROM produtos";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        endwin();
        printf("Falha ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        strcpy(produtos[num_produtos].nome, (const char *)sqlite3_column_text(stmt, 0));
        produtos[num_produtos].preco_por_kg = sqlite3_column_double(stmt, 1);
        produtos[num_produtos].preco_granel = sqlite3_column_double(stmt, 2);
        num_produtos++;
    }

    sqlite3_finalize(stmt);
    return num_produtos;
}

int main()
{
    Produto produtos[NUM_PRODUTOS];
    ItemCompra itens[NUM_PRODUTOS];
    int selecionado = 0;
    float peso, preco_total, valor_total = 0;
    char resposta;
    int num_itens = 0;
    int opcao_calculo = 0;

    sqlite3 *db;
    if (sqlite3_open("./servidor/fructus.db", &db))
    {
        printf("Não foi possível conectar ao banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Cria a tabela vendas_caixa2 se não existir
    criar_tabela_vendas(db);

    int num_produtos = carregar_produtos(db, produtos);

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    while (1)
    {
        clear();
        int start_y, start_x;
        getmaxyx(stdscr, start_y, start_x);

        start_y = (start_y - (num_produtos + 6)) / 2;
        start_x = (start_x - LARGURA_TABELA) / 2;

        mvprintw(start_y - 3, (COLS - strlen("-Caixa 2-")) / 2, "-Caixa 2-");
        mvprintw(start_y - 2, (COLS - strlen("Para calcular o valor da compra")) / 2, "Para calcular o valor da compra");
        mvprintw(start_y - 1, (COLS - strlen("Escolha o produto:")) / 2, "Escolha o produto:");

        mostrar_menu(produtos, selecionado, num_produtos);

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
            if (selecionado < num_produtos)
            {
                selecionado++;
            }
            break;
        case '\n':
            if (selecionado == num_produtos)
            {
                voltar();
            }

            clear();
            opcao_calculo = -1;

            // Escolha o tipo de cálculo
            int opcao_selecionada = 0;
            while (1)
            {
                clear();
                mvprintw(2, (COLS - strlen("Escolha a forma de cálculo:")) / 2, "Escolha a forma de cálculo:");
                mostrar_opcoes_calculo(opcao_selecionada);

                tecla = getch();

                switch (tecla)
                {
                case KEY_UP:
                    if (opcao_selecionada > 0)
                    {
                        opcao_selecionada--;
                    }
                    break;
                case KEY_DOWN:
                    if (opcao_selecionada < NUM_OPCOES - 1)
                    {
                        opcao_selecionada++;
                    }
                    break;
                case '\n':
                    opcao_calculo = opcao_selecionada;
                    goto sair_menu_opcoes;
                }
            }
        sair_menu_opcoes:

            clear();
            if (opcao_calculo == 0) // Peso
            {
                mvprintw(2, (COLS - strlen("Digite o peso (kg):")) / 2, "Digite o peso (kg): ");
                echo();
                curs_set(TRUE);
                scanw("%f", &peso);
                noecho();
                curs_set(FALSE);

                preco_total = produtos[selecionado].preco_por_kg * peso;
                strcpy(itens[num_itens].tipo_compra, "Peso"); // Tipo de compra
            }
            else // Granel
            {
                mvprintw(2, (COLS - strlen("Digite o peso (kg) para cálculo por granel:")) / 2, "Digite o peso (kg) para cálculo por granel: ");
                echo();
                curs_set(TRUE);
                scanw("%f", &peso);
                noecho();
                curs_set(FALSE);

                preco_total = produtos[selecionado].preco_granel * peso;
                strcpy(itens[num_itens].tipo_compra, "Granel"); // Tipo de compra
            }

            mvprintw(4, (COLS - strlen("Preço total:")) / 2, "Preço total: R$ %.2f", preco_total);

            // Registrar o item na compra
            itens[num_itens].peso = peso;
            itens[num_itens].preco_total = preco_total;
            strcpy(itens[num_itens].nome, produtos[selecionado].nome);
            num_itens++;
            valor_total += preco_total;

            mvprintw(6, (COLS - strlen("Deseja adicionar outro item (s/n)?")) / 2, "Deseja adicionar outro item (s/n)? ");
            resposta = getch();

            if (resposta == 'n' || resposta == 'N')
            {
                registrar_compra(db, itens, num_itens, valor_total);
                break;
            }
            else if (resposta == 's' || resposta == 'S')
            {
                clear();
                mvprintw(2, (COLS - strlen("Escolha o produto:")) / 2, "Escolha o produto:");
                mostrar_menu(produtos, selecionado, num_produtos);
            }
            break;
        }
    }

    sqlite3_close(db);
    endwin();

    return 0;
}
