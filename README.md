# Fructus - Simulador de Hortifruti

<div align="center">
  
 ![image](https://github.com/user-attachments/assets/02c78dfe-b975-4a30-80be-7ebc574e8f63)
  <h3 align="center">PIM II - Fructus</h3>
  Este projeto foi desenvolvido como parte de um trabalho acadêmico e tem como objetivo simular a gestão de um hortifruti utilizando a linguagem de programação C.
  <br/ >
  
</div>

## Preview
https://github.com/user-attachments/assets/d19ec083-ecbe-4931-a432-2cad80e76716

## Descrição

O **Fructus** é um sistema simples que permite gerenciar um hortifruti, incluindo funcionalidades como:
- Cadastro de produtos (frutas, verduras e legumes)
- Controle de estoque
- Registro de vendas

O projeto foi desenvolvido para reforçar conceitos fundamentais da linguagem C, como manipulação de arquivos, estruturas de dados e interações básicas com o usuário via terminal.

## Tecnologias Utilizadas

- Linguagem C
- Manipulação de arquivos (para armazenamento de dados)
- Estruturas e ponteiros
- Entrada e saída via terminal
- WSL (Windows Subsystem for Linux)
- Ncurses

## Requisitos

Para compilar e rodar o projeto, você precisa de um compilador C instalado, como:
- GCC (Linux/macOS): `sudo apt install gcc` ou `brew install gcc`
- MinGW (Windows): Baixe e instale via [MinGW-w64](https://www.mingw-w64.org/)

O sistema também utiliza as bibliotecas `ncurses`, `sqlite` e `sqlite3`, que devem estar instaladas para garantir o funcionamento adequado.

## Como Executar

### Instalação Automática

1. Clone o repositório ou baixe o pacote do site:
   ```sh
   git clone https://github.com/Kc1t/Fructus.git
   cd Fructus/fructus-software/app
   ```
2. Execute o sistema:
   - No Linux: `./fructus`
   - No Windows (WSL recomendado): `./fructus`

O sistema verificará e baixará automaticamente todas as dependências, compilará o código e iniciará o menu principal.

### Instalação Manual

1. Baixe o repositório do GitHub.
2. Instale as dependências:
   ```sh
   sudo apt install gcc libncurses5-dev sqlite3 libsqlite3-dev
   ```
3. Compile o código:
   ```sh
   cd fructus-software/app
   ./compile.sh
   ```
4. Execute o sistema:
   ```sh
   ./builds/bem-vindo
   ```

## Estrutura do Projeto

``` c
.
├── README.md
├── fructus-software
│   ├── Prints
│   ├── Videos
│   │   └── Prototipo0.mp4
│   └── codigos
│       ├── Hortifruti
│       │   ├── Admin
│       │   │   ├── Vendas
│       │   │   ├── admin-login.c
│       │   │   ├── admin-menu.c
│       │   │   ├── estoque.c
│       │   │   └── integrantes.c
│       │   ├── Caixas
│       │   │   ├── caixa.c
│       │   │   ├── cashier-login.c
│       │   │   └── cashier-menu.c
│       │   ├── Servidor
│       │   └── bem-vindo.c
│       ├── builds
│       │   ├── Admin
│       │   ├── Caixas
│       │   ├── Servidor
│       │   └── bem-vindo
│       ├── compile.sh
│       └── fructus.ico
└── fructus-website
```

## Contribuição

Este projeto foi desenvolvido para fins educacionais, mas melhorias são sempre bem-vindas! Se quiser contribuir, faça um fork do repositório e abra um pull request.

## Licença

Este projeto não possui uma licença específica. Uso livre para fins acadêmicos.
