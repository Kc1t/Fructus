#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sqlite3.h>

// Definindo constantes para o número de produtos e largura da tabela
#define NUM_PRODUTOS 20
#define LARGURA_TABELA 35
#define NUM_OPCOES 2

// Estrutura para armazenar informações sobre um produto
typedef struct
{
    char nome[20];
    float preco_por_kg;
    float preco_granel;
} Produto;

// Estrutura para armazenar informações de itens comprados
typedef struct
{
    char nome[20];
    float peso;
    float preco_total;
    char tipo_compra[10];
} ItemCompra;

// Função para criar tabelas caso n exista no banco sqlite
void criar_tabela_vendas(sqlite3 *db)
{
    char *err_msg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS vendas_caixa1 ("
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

// Função de mostrar menu
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

// Função para mostrar as opções a serem calculadas
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

// Função para registrar compra e item no banco de dados
void registrar_compra(sqlite3 *db, ItemCompra itens[], int num_itens, float total)
{
    char *err_msg = 0;  // Variável para armazenar mensagens de erro do SQLite
    sqlite3_stmt *stmt; // Ponteiro para armazenar o comando SQL preparado

    // Comando SQL para inserir dados na tabela 'vendas_caixa1'
    const char *sql = "INSERT INTO vendas_caixa1(nome_produto, peso, preco_total, tipo_compra) VALUES(?, ?, ?, ?)";

    // Laço para percorrer todos os itens da compra
    for (int i = 0; i < num_itens; i++)
    {
        // Prepara o comando SQL para execução, substituindo os valores dos placeholders (?)
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
        {
            // Se a preparação falhar, imprime a mensagem de erro e encerra o programa
            endwin();
            printf("Falha ao preparar a inserção: %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        // Vincula os dados do item ao comando SQL
        sqlite3_bind_text(stmt, 1, itens[i].nome, -1, SQLITE_STATIC);        // Substitui o primeiro '?' com o nome do produto
        sqlite3_bind_double(stmt, 2, itens[i].peso);                         // Substitui o segundo '?' com o peso do item
        sqlite3_bind_double(stmt, 3, itens[i].preco_total);                  // Substitui o terceiro '?' com o preço total do item
        sqlite3_bind_text(stmt, 4, itens[i].tipo_compra, -1, SQLITE_STATIC); // Substitui o quarto '?' com o tipo de compra (Peso ou Granel)

        // Executa o comando SQL para inserir os dados
        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            // Se a execução falhar, imprime a mensagem de erro e encerra o programa
            endwin();
            printf("Falha ao executar a inserção: %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        // Finaliza o uso do comando SQL e libera recursos
        sqlite3_finalize(stmt);
    }

    // Após inserir todos os itens, limpa a tela
    clear();

    // Exibe a mensagem de sucesso no centro da tela
    mvprintw((LINES - 1) / 2, (COLS - strlen("Compra registrada com sucesso!")) / 2, "Compra registrada com sucesso!");

    // Atualiza a tela para mostrar a mensagem
    refresh();

    // Espera 2 segundos para a mensagem ser visível
    napms(2000);
}

// Função para voltar ao menu principal
void voltar()
{
    system("./caixas/caixa-menu");
    exit(0);
}

// Função de carregar produtos do banco de dados para o array de produtos.
// Essa função consulta o banco de dados e preenche o array 'produtos' com os dados retornados.
int carregar_produtos(sqlite3 *db, Produto produtos[])
{
    int num_produtos = 0;                                                  // Inicializa o contador de produtos
    sqlite3_stmt *stmt;                                                    // Ponteiro para o comando SQL preparado
    const char *sql = "SELECT nome, preco_kg, preco_granel FROM produtos"; // Consulta SQL para obter o nome e preços dos produtos

    // Prepara o comando SQL para ser executado
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        // Caso falhe ao preparar o comando, exibe a mensagem de erro e encerra o programa
        endwin();
        printf("Falha ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    // Laço que percorre os resultados da consulta e preenche o array 'produtos'
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Copia os dados da linha do resultado da consulta para o array de produtos
        strcpy(produtos[num_produtos].nome, (const char *)sqlite3_column_text(stmt, 0)); // Nome do produto
        produtos[num_produtos].preco_por_kg = sqlite3_column_double(stmt, 1);            // Preço por kg
        produtos[num_produtos].preco_granel = sqlite3_column_double(stmt, 2);            // Preço por granel
        num_produtos++;                                                                  // Incrementa o número de produtos carregados
    }

    // Finaliza a consulta e libera os recursos alocados
    sqlite3_finalize(stmt);

    // Retorna o número de produtos carregados
    return num_produtos;
}

// Função principal que gerencia o fluxo de vendas no caixa.
// A função conecta ao banco de dados, carrega os produtos, e permite ao usuário registrar compras interativas.

int main()
{
    Produto produtos[NUM_PRODUTOS];           // Array que irá armazenar os produtos carregados do banco de dados
    ItemCompra itens[NUM_PRODUTOS];           // Array para armazenar os itens da compra
    int selecionado = 0;                      // Índice do produto selecionado no menu
    float peso, preco_total, valor_total = 0; // Variáveis para armazenar o peso, preço total do item e o valor total da compra
    char resposta;                            // Resposta para saber se o usuário quer adicionar mais itens
    int num_itens = 0;                        // Contador de itens adicionados à compra
    int opcao_calculo = 0;                    // Tipo de cálculo selecionado (peso ou granel)

    sqlite3 *db;                                    // Ponteiro para o banco de dados SQLite
    if (sqlite3_open("./servidor/fructus.db", &db)) // Conecta ao banco de dados
    {
        printf("Não foi possível conectar ao banco de dados: %s\n", sqlite3_errmsg(db)); // Caso haja erro na conexão
        return 1;                                                                        // Encerra o programa com erro
    }

    // Cria a tabela de vendas se ela não existir
    criar_tabela_vendas(db);

    // Carrega os produtos do banco para o array 'produtos'
    int num_produtos = carregar_produtos(db, produtos);

    initscr();            // Inicializa o modo ncurses para interação em terminal
    noecho();             // Desativa a exibição de caracteres digitados
    curs_set(FALSE);      // Desativa o cursor
    keypad(stdscr, TRUE); // Habilita o uso de teclas especiais (setas)

    // Loop principal da interface interativa de vendas
    while (1)
    {
        clear();
        int start_y, start_x;
        getmaxyx(stdscr, start_y, start_x);

        // Calcula o posicionamento do conteúdo para centralizar na tela
        start_y = (start_y - (num_produtos + 6)) / 2;
        start_x = (start_x - LARGURA_TABELA) / 2;

        // Exibe o título e instruções para o usuário
        mvprintw(start_y - 3, (COLS - strlen("-Caixa-")) / 2, "-Caixa-");
        mvprintw(start_y - 2, (COLS - strlen("Para calcular o valor da compra")) / 2, "Para calcular o valor da compra");
        mvprintw(start_y - 1, (COLS - strlen("Escolha o produto:")) / 2, "Escolha o produto:");

        // Exibe o menu com a lista de produtos
        mostrar_menu(produtos, selecionado, num_produtos);

        // Espera a entrada do usuário (tecla pressionada)
        int tecla = getch();

        // Processa a tecla pressionada para navegação ou seleção
        switch (tecla)
        {
        case KEY_UP:             // Setas para cima
            if (selecionado > 0) // Previne a seleção de um índice inválido
            {
                selecionado--; // Move para o item acima
            }
            break;
        case KEY_DOWN:                      // Setas para baixo
            if (selecionado < num_produtos) // Previne a seleção de um índice inválido
            {
                selecionado++; // Move para o item abaixo
            }
            break;
        case '\n':                           // Enter (confirmação de seleção)
            if (selecionado == num_produtos) // Se selecionado a opção "voltar"
            {
                voltar(); // Função que faz o usuário voltar ao menu anterior
            }

            clear();            // Limpa a tela
            opcao_calculo = -1; // Reset do tipo de cálculo

            // Inicia o menu para escolher o tipo de cálculo (peso ou granel)
            int opcao_selecionada = 0;
            while (1)
            {
                clear(); // Limpa a tela
                mvprintw(2, (COLS - strlen("Escolha a forma de cálculo:")) / 2, "Escolha a forma de cálculo:");
                mostrar_opcoes_calculo(opcao_selecionada); // Exibe opções de cálculo

                // Espera a entrada do usuário
                tecla = getch();

                // Processa a navegação pelas opções de cálculo
                switch (tecla)
                {
                case KEY_UP: // Move para cima nas opções
                    if (opcao_selecionada > 0)
                    {
                        opcao_selecionada--;
                    }
                    break;
                case KEY_DOWN: // Move para baixo nas opções
                    if (opcao_selecionada < NUM_OPCOES - 1)
                    {
                        opcao_selecionada++;
                    }
                    break;
                case '\n':                             // Seleção de uma opção
                    opcao_calculo = opcao_selecionada; // Salva a opção escolhida
                    goto sair_menu_opcoes;             // Sai do loop de opções de cálculo
                }
            }
        sair_menu_opcoes:

            clear(); // Limpa a tela

            // Se o cálculo for baseado em peso
            if (opcao_calculo == 0)
            {
                mvprintw(2, (COLS - strlen("Digite o peso (kg):")) / 2, "Digite o peso (kg): ");
                echo();             // Permite que o usuário digite
                curs_set(TRUE);     // Exibe o cursor
                scanw("%f", &peso); // Lê o peso do usuário
                noecho();           // Desativa a exibição dos caracteres digitados
                curs_set(FALSE);    // Oculta o cursor

                preco_total = produtos[selecionado].preco_por_kg * peso; // Calcula o preço total pelo peso
                strcpy(itens[num_itens].tipo_compra, "Peso");            // Define o tipo de compra como "Peso"
            }
            else // Se o cálculo for por granel
            {
                mvprintw(2, (COLS - strlen("Digite o peso (kg) para cálculo por granel:")) / 2, "Digite o peso (kg) para cálculo por granel: ");
                echo();             // Permite que o usuário digite
                curs_set(TRUE);     // Exibe o cursor
                scanw("%f", &peso); // Lê o peso do usuário
                noecho();           // Desativa a exibição dos caracteres digitados
                curs_set(FALSE);    // Oculta o cursor

                preco_total = produtos[selecionado].preco_granel * peso; // Calcula o preço total por granel
                strcpy(itens[num_itens].tipo_compra, "Granel");          // Define o tipo de compra como "Granel"
            }

            mvprintw(4, (COLS - strlen("Preço total:")) / 2, "Preço total: R$ %.2f", preco_total); // Exibe o preço total do item

            // Registra o item na compra
            itens[num_itens].peso = peso;
            itens[num_itens].preco_total = preco_total;
            strcpy(itens[num_itens].nome, produtos[selecionado].nome); // Armazena o nome do produto no item
            num_itens++;                                               // Incrementa o contador de itens
            valor_total += preco_total;                                // Atualiza o valor total da compra

            mvprintw(6, (COLS - strlen("Deseja adicionar outro item (s/n)?")) / 2, "Deseja adicionar outro item (s/n)? ");
            resposta = getch(); // Pergunta ao usuário se ele deseja adicionar outro item

            if (resposta == 'n' || resposta == 'N') // Se a resposta for 'não'
            {
                registrar_compra(db, itens, num_itens, valor_total); // Registra a compra no banco de dados
                break;                                               // Encerra a compra
            }
            else if (resposta == 's' || resposta == 'S') // Se a resposta for 'sim'
            {
                clear();                                                                      // Limpa a tela
                mvprintw(2, (COLS - strlen("Escolha o produto:")) / 2, "Escolha o produto:"); // Solicita novo item
                mostrar_menu(produtos, selecionado, num_produtos);                            // Exibe novamente o menu de produtos
            }
            break;
        }
    }

    sqlite3_close(db); // Fecha a conexão com o banco de dados
    endwin();          // Finaliza o modo ncurses

    return 0; // Encerra o programa
}
