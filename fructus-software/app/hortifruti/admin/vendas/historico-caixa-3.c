#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sqlite3.h>

#define MAX_LINHA 256
#define MAX_COMPRAS 100

typedef struct
{
    int id;
    char nome_produto[MAX_LINHA];
    float peso;
    float preco_total;
} Compra;

void centralizar_tabela(Compra compras[], int num_compras)
{
    int y_max, x_max;
    // Larguras
    int largura_id = 5;
    int largura_nome = 30;
    int largura_peso = 10;
    int largura_preco = 12;
    int largura_tabela = largura_id + largura_nome + largura_peso + largura_preco + 8; // Espaços entre colunas e bordas
    int altura_tabela = num_compras + 4;                                               // Adiciona altura para cabeçalho e bordas

    getmaxyx(stdscr, y_max, x_max);

    int start_y = (y_max - altura_tabela) / 2;
    int start_x = (x_max - largura_tabela) / 2;

    // Desenha a borda ao redor da tabela
    for (int i = start_y; i <= start_y + altura_tabela; i++)
    {
        mvprintw(i, start_x - 1, "|");
        mvprintw(i, start_x + largura_tabela, "|");
    }
    mvprintw(start_y - 1, start_x - 1, "+");
    mvprintw(start_y - 1, start_x + largura_tabela, "+");
    mvprintw(start_y + altura_tabela, start_x - 1, "+");
    mvprintw(start_y + altura_tabela, start_x + largura_tabela, "+");

    for (int i = start_x; i < start_x + largura_tabela; i++)
    {
        mvprintw(start_y - 1, i, "-");
        mvprintw(start_y + altura_tabela, i, "-");
    }

    // Imprime o cabeçalho
    mvprintw(start_y, start_x, "ID   Nome Produto                  Peso      Preço Total");
    mvprintw(start_y + 1, start_x, "-------------------------------------------------------------");

    // Imprime as compras na tela centralizada
    for (int i = 0; i < num_compras; i++)
    {
        mvprintw(start_y + i + 2, start_x, "%-5d %-30s %-10.2f %-12.2f", compras[i].id, compras[i].nome_produto, compras[i].peso, compras[i].preco_total);
    }

    refresh();
}

int carregar_compras(sqlite3 *db, Compra compras[])
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nome_produto, peso, preco_total FROM vendas_caixa3";
    int num_compras = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        endwin();
        printf("Falha ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW && num_compras < MAX_COMPRAS)
    {
        compras[num_compras].id = sqlite3_column_int(stmt, 0);
        const char *nome_produto = (const char *)sqlite3_column_text(stmt, 1);
        compras[num_compras].peso = sqlite3_column_double(stmt, 2);
        compras[num_compras].preco_total = sqlite3_column_double(stmt, 3);

        strncpy(compras[num_compras].nome_produto, nome_produto, MAX_LINHA - 1);
        compras[num_compras].nome_produto[MAX_LINHA - 1] = '\0';
        num_compras++;
    }

    sqlite3_finalize(stmt);
    return num_compras;
}

int main()
{
    Compra compras[MAX_COMPRAS];
    int num_compras;

    sqlite3 *db;
    if (sqlite3_open("./servidor/fructus.db", &db))
    {
        printf("Não foi possível conectar ao banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    num_compras = carregar_compras(db, compras);
    sqlite3_close(db);

    initscr();
    noecho();
    curs_set(FALSE);

    clear();
    centralizar_tabela(compras, num_compras);

    mvprintw(LINES - 2, (COLS - strlen("Pressione Enter para sair...")) / 2, "Pressione Enter para sair...");
    refresh();
    getch();

    endwin();

    system("./admin/vendas/vendas-menu");
    return 0;
}
