# rainbow-game-of-life

### Integrantes:

- Eduardo de Oliveira Motta 148881
- Gabriel Henrique Silva 156514

### Descrição do projeto:

O projeto consiste em uma implementação do jogo da vida de Conway utilizando MPI para paralelizar o processamento do jogo.

### Como compilar:

Para compilar o projeto, basta executar o comando `make` na pasta raiz do projeto.

### Como executar:

Para executar o projeto, basta executar o comando `make run` na pasta raiz do projeto.

### Configurações:

O Makefile possui algumas configurações que podem ser alteradas para modificar o comportamento do programa:

- `CC`: Compilador utilizado para compilar o programa.
- `CFLAGS`: Flags utilizadas para compilar o programa.
- `TARGET`: Nome do executável gerado.
- `ARGS`: `<Número de iterações> <Número de linhas da Matriz>`

### Observações:

- O programa foi testado utilizando o compilador `mpicc` e as flags `-g -Wall` no sistema operacional EndeavourOS.
- Kernel: x86_64 Linux 6.6.4-arch1-1
- CPU: AMD Ryzen 5 3500U with Radeon Vega Mobile Gfx @ 8x 2.1GHz
- RAM: 9883MiB.
