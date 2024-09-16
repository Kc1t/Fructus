#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void adicionarProdutos()
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

void visualizarProdutos()
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

    const char *sql_select = "SELECT id, nome, preco_kg, preco_granel, quantidade_estoque FROM produtos;";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Erro ao preparar a consulta: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    clear();

    int largura_tabela = 70;
    int coluna_inicio = (COLS - largura_tabela) / 2;
    int linha_inicio = (LINES - 12) / 2;

    mvprintw(linha_inicio, coluna_inicio, "Aperte 'Enter' para apagar um produto!");

    mvprintw(linha_inicio + 1, coluna_inicio, "Produtos:");
    mvprintw(linha_inicio + 2, coluna_inicio, "Nome                         Preço/kg  Preço Granel  Quantidade");
    mvprintw(linha_inicio + 3, coluna_inicio, "-------------------------------------------------------------");

    int linha_atual = linha_inicio + 3;
    int selecionado = 0;
    int num_produtos = 0;

    struct
    {
        int id;
        char nome[100];
        float preco_kg;
        float preco_granel;
        int quantidade_estoque;
    } produtos[100];

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        produtos[num_produtos].id = sqlite3_column_int(stmt, 0);
        strncpy(produtos[num_produtos].nome, (const char *)sqlite3_column_text(stmt, 1), sizeof(produtos[num_produtos].nome) - 1);
        produtos[num_produtos].preco_kg = (float)sqlite3_column_double(stmt, 2);
        produtos[num_produtos].preco_granel = (float)sqlite3_column_double(stmt, 3);
        produtos[num_produtos].quantidade_estoque = sqlite3_column_int(stmt, 4);

        num_produtos++;
    }

    sqlite3_finalize(stmt);

    if (num_produtos == 0)
    {
        mvprintw(linha_atual, coluna_inicio, "Nenhum produto no estoque.");
        mvprintw(linha_atual + 2, (COLS - strlen("Pressione qualquer tecla para continuar...")) / 2, "Pressione qualquer tecla para continuar...");
        getch();
        sqlite3_close(db);
        return;
    }

    char *opcao_voltar = "Voltar";
    int opcao_voltar_idx = num_produtos;

    int ch;
    while (1)
    {
        for (int i = 0; i < num_produtos; i++)
        {
            if (i == selecionado)
            {
                attron(A_REVERSE);
            }

            mvprintw(linha_atual + i, coluna_inicio, "%-30s %10.2f %12.2f %12d",
                     produtos[i].nome, produtos[i].preco_kg, produtos[i].preco_granel, produtos[i].quantidade_estoque);

            if (i == selecionado)
            {
                attroff(A_REVERSE);
            }
        }

        if (selecionado == opcao_voltar_idx)
        {
            attron(A_REVERSE);
        }
        mvprintw(linha_atual + num_produtos, coluna_inicio, "%s", opcao_voltar);
        if (selecionado == opcao_voltar_idx)
        {
            attroff(A_REVERSE);
        }

        refresh();
        ch = getch();

        switch (ch)
        {
        case KEY_UP:
            selecionado = (selecionado - 1 + (num_produtos + 1)) % (num_produtos + 1);
            break;
        case KEY_DOWN:
            selecionado = (selecionado + 1) % (num_produtos + 1);
            break;
        case 10:
            if (selecionado == opcao_voltar_idx)
            {
                sqlite3_close(db);
                return;
            }
            else
            {
                char sql_delete[256];
                snprintf(sql_delete, sizeof(sql_delete), "DELETE FROM produtos WHERE id = %d;", produtos[selecionado].id);
                rc = sqlite3_exec(db, sql_delete, 0, 0, &err_msg);

                if (rc != SQLITE_OK)
                {
                    mvprintw(linha_atual + num_produtos + 2, coluna_inicio, "Erro ao apagar produto: %s", err_msg);
                    sqlite3_free(err_msg);
                }
                else
                {
                    mvprintw(linha_atual + num_produtos + 2, coluna_inicio, "Produto apagado com sucesso!");
                }

                mvprintw(linha_atual + num_produtos + 4, coluna_inicio, "Pressione qualquer tecla para continuar...");
                getch();
                sqlite3_close(db);
                return;
            }
        }
    }

    sqlite3_close(db);
}

void exibirMenu(sqlite3 *db)
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

        mvprintw(inicio_y - 2, (COLS - strlen("Menu de Produtos")) / 2, "Menu de Produtos");
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
            "Adicionar Produto",
            "Visualizar Produtos",
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
                adicionarProdutos(db);
                break;
            case 1:
                visualizarProdutos(db);
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
    sqlite3 *db;
    if (sqlite3_open("./Servidor/fructus.db", &db))
    {
        perror("Erro ao abrir banco de dados");
        return EXIT_FAILURE;
    }

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    exibirMenu(db);

    sqlite3_close(db);
    endwin();
    return 0;
}
