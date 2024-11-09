#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sqlite3.h>

#define MAX_LINHA 256
#define MAX_COMPRAS 100

// Estrutura que representa uma compra
typedef struct
{
    int id;
    char nome_produto[MAX_LINHA];
    float peso;
    float preco_total;
} Compra;

// Função que centraliza e exibe a tabela de compras na tela
void centralizar_tabela(Compra compras[], int num_compras)
{
    int y_max, x_max;
    // Larguras das colunas
    int largura_id = 5;
    int largura_nome = 30;
    int largura_peso = 10;
    int largura_preco = 12;
    int largura_tabela = largura_id + largura_nome + largura_peso + largura_preco + 8; // Espaços entre colunas e bordas
    int altura_tabela = num_compras + 4;                                               // Altura inclui cabeçalho e bordas

    // Obtém o tamanho da tela
    getmaxyx(stdscr, y_max, x_max);

    // Calcula a posição para centralizar a tabela na tela
    int start_y = (y_max - altura_tabela) / 2;
    int start_x = (x_max - largura_tabela) / 2;

    // Desenha as bordas da tabela
    for (int i = start_y; i <= start_y + altura_tabela; i++)
    {
        mvprintw(i, start_x - 1, "|");
        mvprintw(i, start_x + largura_tabela, "|");
    }
    mvprintw(start_y - 1, start_x - 1, "+");
    mvprintw(start_y - 1, start_x + largura_tabela, "+");
    mvprintw(start_y + altura_tabela, start_x - 1, "+");
    mvprintw(start_y + altura_tabela, start_x + largura_tabela, "+");

    // Desenha as linhas horizontais de separação
    for (int i = start_x; i < start_x + largura_tabela; i++)
    {
        mvprintw(start_y - 1, i, "-");
        mvprintw(start_y + altura_tabela, i, "-");
    }

    // Imprime o cabeçalho da tabela
    mvprintw(start_y, start_x, "ID   Nome Produto                  Peso      Preço Total");
    mvprintw(start_y + 1, start_x, "-------------------------------------------------------------");

    // Imprime os dados das compras
    for (int i = 0; i < num_compras; i++)
    {
        mvprintw(start_y + i + 2, start_x, "%-5d %-30s %-10.2f %-12.2f", compras[i].id, compras[i].nome_produto, compras[i].peso, compras[i].preco_total);
    }

    refresh();
}

// Função que carrega as compras do banco de dados SQLite
int carregar_compras(sqlite3 *db, Compra compras[])
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nome_produto, peso, preco_total FROM vendas_caixa1";  // Consulta SQL
    int num_compras = 0;

    // Prepara a consulta SQL
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        endwin();
        printf("Falha ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    // Executa a consulta e armazena os resultados na estrutura de compras
    while (sqlite3_step(stmt) == SQLITE_ROW && num_compras < MAX_COMPRAS)
    {
        compras[num_compras].id = sqlite3_column_int(stmt, 0);
        const char *nome_produto = (const char *)sqlite3_column_text(stmt, 1);
        compras[num_compras].peso = sqlite3_column_double(stmt, 2);
        compras[num_compras].preco_total = sqlite3_column_double(stmt, 3);

        strncpy(compras[num_compras].nome_produto, nome_produto, MAX_LINHA - 1);
        compras[num_compras].nome_produto[MAX_LINHA - 1] = '\0';  // Garante a terminação do nome do produto
        num_compras++;
    }

    sqlite3_finalize(stmt);  // Finaliza a consulta SQL
    return num_compras;  // Retorna o número de compras carregadas
}

int main()
{
    Compra compras[MAX_COMPRAS];
    int num_compras;

    sqlite3 *db;
    // Abre a conexão com o banco de dados SQLite
    if (sqlite3_open("./servidor/fructus.db", &db))
    {
        printf("Não foi possível conectar ao banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Carrega as compras do banco de dados
    num_compras = carregar_compras(db, compras);
    sqlite3_close(db);  // Fecha a conexão com o banco de dados

    // Inicializa a interface ncurses
    initscr();
    noecho();
    curs_set(FALSE);

    clear();  // Limpa a tela
    // Chama a função para centralizar e exibir a tabela de compras
    centralizar_tabela(compras, num_compras);

    // Exibe a mensagem para o usuário
    mvprintw(LINES - 2, (COLS - strlen("Pressione Enter para sair...")) / 2, "Pressione Enter para sair...");
    refresh();
    getch();  // Aguarda o usuário pressionar uma tecla

    endwin();  // Finaliza o ncurses

    // Chama o script de vendas
    system("./admin/vendas/vendas-menu");
    return 0;
}
