#!/bin/bash

# Criação da estrutura de diretórios dentro do diretório "builds"
# Cria diretórios para os diferentes módulos do projeto dentro de "builds/hortifruti"
mkdir -p ./builds/hortifruti/admin/vendas
mkdir -p ./builds/hortifruti/admin/produtos
mkdir -p ./builds/hortifruti/caixas
mkdir -p ./builds/hortifruti/servidor

# Seção de compilação do programa "bem-vindo"
# Compila o código C localizado em "hortifruti/bem-vindo.c", com a biblioteca ncurses
# O arquivo compilado será salvo em "./builds/hortifruti/bem-vindo"
gcc hortifruti/bem-vindo.c -lncurses -o ./builds/hortifruti/bem-vindo
echo "Compilado bem-vindo.c"

# Seção de compilação de arquivos para a administração (admin)
# Compila o código C para login do administrador
gcc hortifruti/admin/admin-login.c -lncurses -o ./builds/hortifruti/admin/admin-login
echo "Compilado admin-login.c"

# Compila o código C para o menu de administração
gcc hortifruti/admin/admin-menu.c -lncurses -o ./builds/hortifruti/admin/admin-menu
echo "Compilado admin-menu.c"

# Seção de compilação de arquivos para o módulo de caixas
# Compila o código C para login de caixa
gcc hortifruti/caixas/caixa-login.c -lncurses -o ./builds/hortifruti/caixas/caixa-login
echo "Compilado caixa-login.c"

# Compila o código C para o menu de caixa
gcc hortifruti/caixas/caixa-menu.c -lncurses -o ./builds/hortifruti/caixas/caixa-menu
echo "Compilado caixa-menu.c"

# Seção de compilação de páginas para o administrador
# Compila o código C para visualização de produtos
gcc hortifruti/admin/ver-produtos.c -lncurses -lsqlite3 -o ./builds/hortifruti/admin/ver-produtos
echo "Compilado produtos.c"

# Compila o código C para gerenciamento de integrantes
gcc hortifruti/admin/integrantes.c -lncurses -o ./builds/hortifruti/admin/integrantes
echo "Compilado integrantes.c"

# Seção de compilação de caixas (módulo de caixa)
# Compila o código C para o módulo de caixa principal
gcc hortifruti/caixas/caixa.c -lncurses -lsqlite3 -o ./builds/hortifruti/caixas/caixa
echo "Compilado caixa.c"

# Compila o código C para a segunda versão do caixa
gcc hortifruti/caixas/caixa-2.c -lncurses -lsqlite3 -o ./builds/hortifruti/caixas/caixa-2
echo "Compilado caixa-2.c"

# Compila o código C para a terceira versão do caixa
gcc hortifruti/caixas/caixa-3.c -lncurses -lsqlite3 -o ./builds/hortifruti/caixas/caixa-3
echo "Compilado caixa-3.c"

# Seção de compilação de vendas para administração
# Compila o código C para o menu de vendas do administrador
gcc hortifruti/admin/vendas/vendas-menu.c -lncurses -o ./builds/hortifruti/admin/vendas/vendas-menu
echo "Compilado vendas-menu.c"

# Compila o código C para histórico de vendas no caixa
gcc hortifruti/admin/vendas/historico-caixa.c -lncurses -lsqlite3 -o ./builds/hortifruti/admin/vendas/historico-caixa
echo "Compilado historico-caixa.c"

# Compila o código C para o segundo histórico de vendas no caixa
gcc hortifruti/admin/vendas/historico-caixa-2.c -lncurses -lsqlite3 -o ./builds/hortifruti/admin/vendas/historico-caixa-2
echo "Compilado historico-caixa-2.c"

# Compila o código C para o terceiro histórico de vendas no caixa
gcc hortifruti/admin/vendas/historico-caixa-3.c -lncurses -lsqlite3 -o ./builds/hortifruti/admin/vendas/historico-caixa-3
echo "Compilado historico-caixa-3.c"
