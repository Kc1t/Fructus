#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void main()
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("./Servidor/fructus.db", &db);

    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Não foi possível abrir o banco de dados: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    const char *sql_create_table =
        "CREATE TABLE IF NOT EXISTS produtos("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nome TEXT, "
        "preco_kg REAL, "
        "preco_granel REAL, "
        "quantidade_estoque INTEGER);";

    rc = sqlite3_exec(db, sql_create_table, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Erro ao criar tabela: %s", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    char nome[100];
    float preco_kg, preco_granel;
    int quantidade_estoque;

    clear();

    int largura_formulario = 50;
    int linha_inicio = (LINES - 10) / 2;
    int coluna_inicio = (COLS - largura_formulario) / 2;

    mvprintw(linha_inicio, coluna_inicio, "Digite o nome do produto: ");
    move(linha_inicio, coluna_inicio + 26); // Move cursor para a direita do prompt
    echo();
    getnstr(nome, sizeof(nome) - 1);
    noecho();
    nome[strcspn(nome, "\n")] = '\0';

    mvprintw(linha_inicio + 2, coluna_inicio, "Digite o preço por kg: ");
    move(linha_inicio + 2, coluna_inicio + 24); // Move cursor para a direita do prompt
    echo();
    scanw("%f", &preco_kg);
    noecho();

    mvprintw(linha_inicio + 4, coluna_inicio, "Digite o preço a granel: ");
    move(linha_inicio + 4, coluna_inicio + 26); // Move cursor para a direita do prompt
    echo();
    scanw("%f", &preco_granel);
    noecho();

    mvprintw(linha_inicio + 6, coluna_inicio, "Digite a quantidade em estoque: ");
    move(linha_inicio + 6, coluna_inicio + 33); // Move cursor para a direita do prompt
    echo();
    scanw("%d", &quantidade_estoque);
    noecho();

    clear();

    char sql_insert[512];
    snprintf(sql_insert, sizeof(sql_insert),
             "INSERT INTO produtos(nome, preco_kg, preco_granel, quantidade_estoque) VALUES('%s', %.2f, %.2f, %d);",
             nome, preco_kg, preco_granel, quantidade_estoque);

    rc = sqlite3_exec(db, sql_insert, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        mvprintw(linha_inicio, (COLS - strlen("Erro ao adicionar produto")) / 2, "Erro ao adicionar produto: %s", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        mvprintw(linha_inicio, (COLS - strlen("Produto adicionado com sucesso!")) / 2, "Produto adicionado com sucesso!");
    }

    sqlite3_close(db);

    mvprintw(linha_inicio + 2, (COLS - strlen("Pressione qualquer tecla para continuar...")) / 2, "Pressione qualquer tecla para continuar...");
    getch();
}
