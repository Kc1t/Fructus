#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void adicionarProdutos()
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("./servidor/fructus.db", &db);

    // Tenta abrir o banco de dados, se falhar exibe uma mensagem de erro e encerra o programa.
    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Não foi possível abrir o banco de dados: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    // SQL para criar a tabela "produtos" caso não exista.
    const char *sql_create_table =
        "CREATE TABLE IF NOT EXISTS produtos("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nome TEXT, "
        "preco_kg REAL, "
        "preco_granel REAL, "
        "quantidade_estoque INTEGER);";

    // Executa o SQL de criação da tabela, se falhar exibe uma mensagem de erro e encerra.
    rc = sqlite3_exec(db, sql_create_table, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Erro ao criar tabela: %s", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    // Declaração de variáveis para armazenar os dados do novo produto.
    char nome[100];
    float preco_kg, preco_granel;
    int quantidade_estoque;

    // Limpa a tela e configura a posição para o formulário de inserção de produto.
    clear();

    int largura_formulario = 50;
    int linha_inicio = (LINES - 10) / 2;
    int coluna_inicio = (COLS - largura_formulario) / 2;

    // Solicita ao usuário o nome do produto.
    mvprintw(linha_inicio, coluna_inicio, "Digite o nome do produto: ");
    move(linha_inicio, coluna_inicio + 26); // Move o cursor para a posição correta.
    echo();
    getnstr(nome, sizeof(nome) - 1); // Lê o nome do produto.
    noecho();
    nome[strcspn(nome, "\n")] = '\0'; // Remove o '\n' no final do nome.

    // Solicita o preço por kg do produto.
    mvprintw(linha_inicio + 2, coluna_inicio, "Digite o preço por kg: ");
    move(linha_inicio + 2, coluna_inicio + 24); // Move o cursor para a posição correta.
    echo();
    scanw("%f", &preco_kg); // Lê o preço por kg.
    noecho();

    // Solicita o preço a granel do produto.
    mvprintw(linha_inicio + 4, coluna_inicio, "Digite o preço a granel: ");
    move(linha_inicio + 4, coluna_inicio + 26); // Move o cursor para a posição correta.
    echo();
    scanw("%f", &preco_granel); // Lê o preço a granel.
    noecho();

    // Solicita a quantidade em estoque do produto.
    mvprintw(linha_inicio + 6, coluna_inicio, "Digite a quantidade em estoque: ");
    move(linha_inicio + 6, coluna_inicio + 33); // Move o cursor para a posição correta.
    echo();
    scanw("%d", &quantidade_estoque); // Lê a quantidade em estoque.
    noecho();

    // Limpa a tela após a coleta de informações do produto.
    clear();

    // Prepara a string SQL para inserir o novo produto na tabela "produtos".
    char sql_insert[512];
    snprintf(sql_insert, sizeof(sql_insert),
             "INSERT INTO produtos(nome, preco_kg, preco_granel, quantidade_estoque) VALUES('%s', %.2f, %.2f, %d);",
             nome, preco_kg, preco_granel, quantidade_estoque);

    // Executa o SQL para inserir os dados do novo produto no banco de dados.
    rc = sqlite3_exec(db, sql_insert, 0, 0, &err_msg);

    // Se a inserção falhar, exibe a mensagem de erro.
    if (rc != SQLITE_OK)
    {
        mvprintw(linha_inicio, (COLS - strlen("Erro ao adicionar produto")) / 2, "Erro ao adicionar produto: %s", err_msg);
        sqlite3_free(err_msg); // Libera o espaço ocupado pela mensagem de erro.
    }
    else
    {
        // Caso a inserção seja bem-sucedida, exibe uma mensagem de sucesso.
        mvprintw(linha_inicio, (COLS - strlen("Produto adicionado com sucesso!")) / 2, "Produto adicionado com sucesso!");
    }

    // Fecha o banco de dados após a operação.
    sqlite3_close(db);

    // Exibe uma mensagem solicitando ao usuário que pressione uma tecla para continuar.
    mvprintw(linha_inicio + 2, (COLS - strlen("Pressione qualquer tecla para continuar...")) / 2, "Pressione qualquer tecla para continuar...");
    getch(); // Aguarda a tecla pressionada para continuar.
}

void visualizarProdutos()
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("./servidor/fructus.db", &db);

    // Tenta abrir o banco de dados. Se falhar, exibe uma mensagem de erro e encerra o programa.
    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Não foi possível abrir o banco de dados: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    // Define a consulta SQL para selecionar todos os produtos.
    const char *sql_select = "SELECT id, nome, preco_kg, preco_granel, quantidade_estoque FROM produtos;";
    sqlite3_stmt *stmt;

    // Prepara a consulta SQL para execução. Se falhar, exibe mensagem de erro e encerra.
    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        mvprintw(0, 0, "Erro ao preparar a consulta: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    clear();

    // Configurações iniciais para exibir os dados dos produtos de maneira organizada.
    int largura_tabela = 70;
    int coluna_inicio = (COLS - largura_tabela) / 2;
    int linha_inicio = (LINES - 12) / 2;

    // Exibe mensagem para o usuário informando que pode apagar um produto pressionando Enter.
    mvprintw(linha_inicio, coluna_inicio, "Aperte 'Enter' para apagar um produto!");

    mvprintw(linha_inicio + 1, coluna_inicio, "Produtos:");
    mvprintw(linha_inicio + 2, coluna_inicio, "Nome                         Preço/kg  Preço Granel  Quantidade");
    mvprintw(linha_inicio + 3, coluna_inicio, "-------------------------------------------------------------");

    // Variáveis para armazenar dados dos produtos e controle de navegação.
    int linha_atual = linha_inicio + 3;
    int selecionado = 0;
    int num_produtos = 0;

    // Estrutura para armazenar os dados dos produtos que serão exibidos.
    struct
    {
        int id;
        char nome[100];
        float preco_kg;
        float preco_granel;
        int quantidade_estoque;
    } produtos[100];

    // Lê todos os produtos do banco de dados e armazena na estrutura 'produtos'.
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

    // Se não houver produtos no banco, exibe uma mensagem e encerra a função.
    if (num_produtos == 0)
    {
        mvprintw(linha_atual, coluna_inicio, "Nenhum produto no estoque.");
        mvprintw(linha_atual + 2, (COLS - strlen("Pressione qualquer tecla para continuar...")) / 2, "Pressione qualquer tecla para continuar...");
        getch();
        sqlite3_close(db);
        return;
    }

    // Configuração para exibir a opção de voltar ao menu.
    char *opcao_voltar = "Voltar";
    int opcao_voltar_idx = num_produtos;

    int ch;
    while (1)
    {
        // Exibe a lista de produtos com formatação.
        for (int i = 0; i < num_produtos; i++)
        {
            if (i == selecionado)
            {
                attron(A_REVERSE); // Destaca o produto selecionado.
            }

            mvprintw(linha_atual + i, coluna_inicio, "%-30s %10.2f %12.2f %12d",
                     produtos[i].nome, produtos[i].preco_kg, produtos[i].preco_granel, produtos[i].quantidade_estoque);

            if (i == selecionado)
            {
                attroff(A_REVERSE); // Remove o destaque do produto.
            }
        }

        // Exibe a opção de voltar ao menu, destacando se estiver selecionada.
        if (selecionado == opcao_voltar_idx)
        {
            attron(A_REVERSE);
        }
        mvprintw(linha_atual + num_produtos, coluna_inicio, "%s", opcao_voltar);
        if (selecionado == opcao_voltar_idx)
        {
            attroff(A_REVERSE);
        }

        refresh();    // Atualiza a tela.
        ch = getch(); // Aguarda a entrada do usuário.

        // Trata a navegação entre os produtos e a opção de voltar.
        switch (ch)
        {
        case KEY_UP:
            selecionado = (selecionado - 1 + (num_produtos + 1)) % (num_produtos + 1);
            break;
        case KEY_DOWN:
            selecionado = (selecionado + 1) % (num_produtos + 1);
            break;
        case 10: // Se Enter for pressionado.
            if (selecionado == opcao_voltar_idx)
            {
                sqlite3_close(db); // Fecha o banco de dados e retorna ao menu.
                return;
            }
            else
            {
                // Prepara e executa a SQL para apagar o produto selecionado.
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
                sqlite3_close(db); // Fecha o banco de dados após a operação.
                return;
            }
        }
    }

    sqlite3_close(db); // Fecha o banco de dados caso o loop seja interrompido.
}

void exibirMenu(sqlite3 *db)
{
    int opcao = 0; // Variável para armazenar a opção selecionada no menu.
    int ch;        // Variável para armazenar a entrada do usuário.

    while (1)
    {
        clear();

        // Define as dimensões da caixa onde as opções do menu serão exibidas.
        int largura_caixa = 30;
        int altura_caixa = 7;
        int inicio_x = (COLS - largura_caixa) / 2; 
        int inicio_y = (LINES - altura_caixa) / 2; 

        // Exibe o título do menu e a instrução para navegação.
        mvprintw(inicio_y - 2, (COLS - strlen("Menu de Produtos")) / 2, "Menu de Produtos");
        mvprintw(inicio_y - 1, inicio_x + 2, "Use as setas para navegar");

        // Desenha a borda da caixa do menu.
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

        // Define as opções do menu.
        char *opcoes[] = {
            "Adicionar Produto",
            "Visualizar Produtos",
            "Sair"};
        int n_opcoes = sizeof(opcoes) / sizeof(char *); 

        // Exibe as opções no menu, destacando a opção selecionada.
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

        ch = getch(); // Aguarda a entrada do usuário.

        // Trata as entradas do usuário para navegação e seleção de opções.
        switch (ch)
        {
        case KEY_UP: // Tecla para mover a seleção para cima.
            opcao = (opcao - 1 + n_opcoes) % n_opcoes;
            break;
        case KEY_DOWN:
            opcao = (opcao + 1) % n_opcoes;
            break;
        case 10: // Tecla Enter para selecionar a opção.
            switch (opcao)
            {
            case 0: // Se a opção "Adicionar Produto" for selecionada, chama a função 'adicionarProdutos'.
                adicionarProdutos(db);
                break;
            case 1: // Se a opção "Visualizar Produtos" for selecionada, chama a função 'visualizarProdutos'.
                visualizarProdutos(db);
                break;
            case 2: // Se a opção "Sair" for selecionada, encerra a aplicação.
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

    // Abre o banco de dados. Se falhar, exibe uma mensagem de erro e encerra o programa.
    if (sqlite3_open("./servidor/fructus.db", &db))
    {
        perror("Erro ao abrir banco de dados");
        return EXIT_FAILURE;
    }

    // Inicializa a biblioteca ncurses para interface com o usuário.
    initscr();
    noecho();            
    curs_set(FALSE);     
    keypad(stdscr, TRUE); 

    // Exibe o menu para o usuário interagir.
    exibirMenu(db);

    sqlite3_close(db); // Fecha o banco de dados.
    endwin();          // Encerra a interface ncurses.

    return 0;
}
