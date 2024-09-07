#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void adicionarEstoque()
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

    // Cria a tabela se ela não existir
    const char *sql_create_table =
        "CREATE TABLE IF NOT EXISTS estoque("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nome TEXT, "
        "quantia INTEGER, "
        "peso REAL, "
        "preco_kg REAL, "
        "preco_granel REAL);";

    rc = sqlite3_exec(db, sql_create_table, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Erro ao criar tabela: %s", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    char nome[100];
    int quantia;
    float peso, preco_kg, preco_granel;

    clear();

    int largura_formulario = 50;
    int linha_inicio = (LINES - 8) / 2;
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

    mvprintw(linha_inicio + 6, coluna_inicio, "Digite o preço por kg: ");
    echo();
    scanw("%f", &preco_kg);
    noecho();

    mvprintw(linha_inicio + 8, coluna_inicio, "Digite o preço a granel: ");
    echo();
    scanw("%f", &preco_granel);
    noecho();

    clear();

    // Insere o novo item no banco de dados
    char sql_insert[512];
    snprintf(sql_insert, sizeof(sql_insert),
             "INSERT INTO estoque(nome, quantia, peso, preco_kg, preco_granel) VALUES('%s', %d, %.2f, %.2f, %.2f);",
             nome, quantia, peso, preco_kg, preco_granel);

    rc = sqlite3_exec(db, sql_insert, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        mvprintw(linha_inicio, (COLS - strlen("Erro ao adicionar item ao estoque")) / 2, "Erro ao adicionar item ao estoque: %s", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        mvprintw(linha_inicio, (COLS - strlen("Item adicionado com sucesso!")) / 2, "Item adicionado com sucesso!");
    }

    sqlite3_close(db);

    mvprintw(linha_inicio + 2, (COLS - strlen("Pressione qualquer tecla para continuar...")) / 2, "Pressione qualquer tecla para continuar...");
    getch();
}

void visualizarEstoque()
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

    const char *sql_select = "SELECT id, nome, quantia, peso, preco_kg, preco_granel FROM estoque;";
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

    mvprintw(linha_inicio, coluna_inicio, "Aperte 'Enter' para apagar um item!");

    mvprintw(linha_inicio + 1, coluna_inicio, "Estoque:");
    mvprintw(linha_inicio + 2, coluna_inicio, "Nome                         Quantia  Peso (kg)  Preço/kg  Preço Granel");
    mvprintw(linha_inicio + 3, coluna_inicio, "-------------------------------------------------------------------");

    int linha_atual = linha_inicio + 3;
    int selecionado = 0;
    int num_itens = 0;

    struct
    {
        int id;
        char nome[100];
        int quantia;
        float peso;
        float preco_kg;
        float preco_granel;
    } itens[100];

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        itens[num_itens].id = sqlite3_column_int(stmt, 0);
        strncpy(itens[num_itens].nome, (const char *)sqlite3_column_text(stmt, 1), sizeof(itens[num_itens].nome) - 1);
        itens[num_itens].quantia = sqlite3_column_int(stmt, 2);
        itens[num_itens].peso = (float)sqlite3_column_double(stmt, 3);
        itens[num_itens].preco_kg = (float)sqlite3_column_double(stmt, 4);
        itens[num_itens].preco_granel = (float)sqlite3_column_double(stmt, 5);

        num_itens++;
    }

    sqlite3_finalize(stmt);

    if (num_itens == 0)
    {
        mvprintw(linha_atual, coluna_inicio, "Nenhum item no estoque.");
        mvprintw(linha_atual + 2, (COLS - strlen("Pressione qualquer tecla para continuar...")) / 2, "Pressione qualquer tecla para continuar...");
        getch();
        sqlite3_close(db);
        return;
    }

    // Adiciona a opção de voltar ao final da lista
    char *opcao_voltar = "Voltar";
    int opcao_voltar_idx = num_itens;

    int ch;
    while (1)
    {
        for (int i = 0; i < num_itens; i++)
        {
            if (i == selecionado)
            {
                attron(A_REVERSE); // Inverte as cores do item selecionado
            }

            mvprintw(linha_atual + i, coluna_inicio, "%-30s %7d %10.2f %8.2f %12.2f", itens[i].nome, itens[i].quantia, itens[i].peso, itens[i].preco_kg, itens[i].preco_granel);

            if (i == selecionado)
            {
                attroff(A_REVERSE); // Reverte as cores para normal
            }
        }

        // Exibe a opção de voltar
        if (selecionado == opcao_voltar_idx)
        {
            attron(A_REVERSE);
        }
        mvprintw(linha_atual + num_itens, coluna_inicio, "%s", opcao_voltar);
        if (selecionado == opcao_voltar_idx)
        {
            attroff(A_REVERSE);
        }

        refresh();
        ch = getch();

        switch (ch)
        {
        case KEY_UP:
            selecionado = (selecionado - 1 + (num_itens + 1)) % (num_itens + 1);
            break;
        case KEY_DOWN:
            selecionado = (selecionado + 1) % (num_itens + 1);
            break;
        case 10: // Enter
            if (selecionado == opcao_voltar_idx)
            {
                sqlite3_close(db);
                return; // Volta para o menu principal
            }
            else
            {
                // Apaga o item selecionado
                char sql_delete[256];
                snprintf(sql_delete, sizeof(sql_delete), "DELETE FROM estoque WHERE id = %d;", itens[selecionado].id);
                rc = sqlite3_exec(db, sql_delete, 0, 0, &err_msg);

                if (rc != SQLITE_OK)
                {
                    mvprintw(linha_atual + num_itens + 2, coluna_inicio, "Erro ao deletar item: %s", err_msg);
                    sqlite3_free(err_msg);
                }
                else
                {
                    mvprintw(linha_atual + num_itens + 2, coluna_inicio, "Item deletado com sucesso!");
                    getch();
                    visualizarEstoque();
                    sqlite3_close(db);
                    return;
                }
            }
            break;
        case 27:
            sqlite3_close(db);
            return;
        }
    }
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
                adicionarEstoque(db);
                break;
            case 1:
                visualizarEstoque(db);
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
