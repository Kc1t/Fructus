#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void main()
{
    sqlite3 *db;  // Declara um ponteiro para o banco de dados SQLite.
    char *err_msg = 0;  // Variável para armazenar mensagens de erro.
    
    // Abre o banco de dados SQLite. Se não conseguir, exibe mensagem de erro e sai.
    int rc = sqlite3_open("./servidor/fructus.db", &db);
    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Não foi possível abrir o banco de dados: %s", sqlite3_errmsg(db));
        sqlite3_close(db);  // Fecha o banco de dados antes de sair.
        exit(EXIT_FAILURE);  // Sai do programa em caso de erro ao abrir o banco.
    }

    // SQL para criar a tabela 'produtos' se ela não existir.
    const char *sql_create_table =
        "CREATE TABLE IF NOT EXISTS produtos("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nome TEXT, "
        "preco_kg REAL, "
        "preco_granel REAL, "
        "quantidade_estoque INTEGER);";

    // Executa o comando SQL para criar a tabela.
    rc = sqlite3_exec(db, sql_create_table, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Erro ao criar tabela: %s", err_msg);  // Exibe mensagem de erro, se houver.
        sqlite3_free(err_msg);  // Libera memória usada pela mensagem de erro.
        sqlite3_close(db);  // Fecha o banco de dados e sai do programa.
        exit(EXIT_FAILURE);
    }

    // Declara variáveis para armazenar as informações do produto inserido.
    char nome[100];
    float preco_kg, preco_granel;
    int quantidade_estoque;

    clear();  // Limpa a tela do terminal.

    // Calcula a posição inicial para centralizar o formulário na tela.
    int largura_formulario = 50;
    int linha_inicio = (LINES - 10) / 2;
    int coluna_inicio = (COLS - largura_formulario) / 2;

    // Exibe prompt para o nome do produto e lê a entrada do usuário.
    mvprintw(linha_inicio, coluna_inicio, "Digite o nome do produto: ");
    move(linha_inicio, coluna_inicio + 26);  // Move o cursor para a direita do prompt.
    echo();  // Habilita a exibição dos caracteres digitados.
    getnstr(nome, sizeof(nome) - 1);  // Lê a string do nome do produto.
    noecho();  // Desabilita a exibição dos caracteres digitados.

    // Exibe prompt para o preço por kg e lê a entrada do usuário.
    mvprintw(linha_inicio + 2, coluna_inicio, "Digite o preço por kg: ");
    move(linha_inicio + 2, coluna_inicio + 24);  // Move o cursor para a direita do prompt.
    echo();
    scanw("%f", &preco_kg);  // Lê o preço por kg.
    noecho();

    // Exibe prompt para o preço a granel e lê a entrada do usuário.
    mvprintw(linha_inicio + 4, coluna_inicio, "Digite o preço a granel: ");
    move(linha_inicio + 4, coluna_inicio + 26);  // Move o cursor para a direita do prompt.
    echo();
    scanw("%f", &preco_granel);  // Lê o preço a granel.
    noecho();

    // Exibe prompt para a quantidade em estoque e lê a entrada do usuário.
    mvprintw(linha_inicio + 6, coluna_inicio, "Digite a quantidade em estoque: ");
    move(linha_inicio + 6, coluna_inicio + 33);  // Move o cursor para a direita do prompt.
    echo();
    scanw("%d", &quantidade_estoque);  // Lê a quantidade em estoque.
    noecho();

    clear();  // Limpa a tela para exibir o resultado da operação.

    // Cria a string SQL para inserir o produto no banco de dados.
    char sql_insert[512];
    snprintf(sql_insert, sizeof(sql_insert),
             "INSERT INTO produtos(nome, preco_kg, preco_granel, quantidade_estoque) VALUES('%s', %.2f, %.2f, %d);",
             nome, preco_kg, preco_granel, quantidade_estoque);

    // Executa o comando SQL para inserir o produto.
    rc = sqlite3_exec(db, sql_insert, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        mvprintw(linha_inicio, (COLS - strlen("Erro ao adicionar produto")) / 2, "Erro ao adicionar produto: %s", err_msg);
        sqlite3_free(err_msg);  // Libera a memória da mensagem de erro.
    }
    else
    {
        mvprintw(linha_inicio, (COLS - strlen("Produto adicionado com sucesso!")) / 2, "Produto adicionado com sucesso!");
    }

    sqlite3_close(db);  // Fecha o banco de dados.

    // Exibe mensagem final de sucesso e aguarda o pressionamento de qualquer tecla para continuar.
    mvprintw(linha_inicio + 2, (COLS - strlen("Pressione qualquer tecla para continuar...")) / 2, "Pressione qualquer tecla para continuar...");
    getch();  // Espera uma tecla ser pressionada para finalizar.
}
