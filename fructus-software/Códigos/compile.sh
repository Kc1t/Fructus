#!/bin/bash

# Cria a estrutura de diretórios dentro de builds
mkdir -p ./builds/Hortifruti/Admin/Vendas
mkdir -p ./builds/Hortifruti/Caixas
mkdir -p ./builds/Hortifruti/Servidor

# Bem-vindo
gcc bem-vindo.c -lncurses -o ./builds/bem-vindo
echo "Compilado bem-vindo.c"

# Admin
gcc Hortifruti/Admin/admin-login.c -lncurses -o ./builds/Hortifruti/Admin/admin-login
echo "Compilado admin-login.c"

gcc Hortifruti/Admin/admin-menu.c -lncurses -o ./builds/Hortifruti/Admin/admin-menu
echo "Compilado admin-menu.c"

# Cashier
gcc Hortifruti/Caixas/cashier-login.c -lncurses -o ./builds/Hortifruti/Caixas/cashier-login
echo "Compilado cashier-login.c"

gcc Hortifruti/Caixas/cashier-menu.c -lncurses -o ./builds/Hortifruti/Caixas/cashier-menu
echo "Compilado cashier-menu.c"

# Pages
gcc Hortifruti/Admin/estoque.c -lncurses -o ./builds/Hortifruti/Admin/estoque
echo "Compilado estoque.c"

gcc Hortifruti/Admin/integrantes.c -lncurses -o ./builds/Hortifruti/Admin/integrantes
echo "Compilado integrantes.c"

# Caixas
gcc Hortifruti/Caixas/caixa.c -lncurses -o ./builds/Hortifruti/Caixas/caixa
echo "Compilado caixa.c"

gcc Hortifruti/Caixas/caixa-2.c -lncurses -o ./builds/Hortifruti/Caixas/caixa-2
echo "Compilado caixa-2.c"

gcc Hortifruti/Caixas/caixa-3.c -lncurses -o ./builds/Hortifruti/Caixas/caixa-3
echo "Compilado caixa-3.c"

# Vendas
gcc Hortifruti/Admin/Vendas/vendas-menu.c -lncurses -o ./builds/Hortifruti/Admin/Vendas/vendas-menu
echo "Compilado vendas-menu.c"

gcc Hortifruti/Admin/Vendas/historico-caixa.c -lncurses -o ./builds/Hortifruti/Admin/Vendas/historico-caixa
echo "Compilado historico-caixa.c"

gcc Hortifruti/Admin/Vendas/historico-caixa-2.c -lncurses -o ./builds/Hortifruti/Admin/Vendas/historico-caixa-2
echo "Compilado historico-caixa-2.c"

gcc Hortifruti/Admin/Vendas/historico-caixa-3.c -lncurses -o ./builds/Hortifruti/Admin/Vendas/historico-caixa-3
echo "Compilado historico-caixa-3.c"
