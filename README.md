# C_Quicksort


# Programação Paralela e Concorrente

Este repositório contém os arquivos desenvolvidos como parte do curso de Programação Paralela e Concorrente da Universidade Federal de Sergipe.

## Quicksort.v1.c

O arquivo `Quicksort.v1.c` implementa o algoritmo de ordenação Quicksort usando threads. Ele divide a lista em partes e cada parte é ordenada em uma thread separada. No final, os resultados são mesclados para obter a lista ordenada completa.

## Quicksort.v2.c

O arquivo `Quicksort.v2.c` é uma versão aprimorada do algoritmo Quicksort paralelo. Ele usa um número fixo de threads para dividir e ordenar a lista em paralelo.

## Quicksort.v3.c

O arquivo `Quicksort.v3.c` apresenta outra implementação paralela do algoritmo Quicksort. Ele usa threads para dividir a lista em partes menores e, em seguida, utiliza recursão para ordenar essas partes em paralelo.

## Compilação e Execução

Para compilar e executar esses programas, você precisará de um compilador C e da biblioteca pthread. Certifique-se de ter essas dependências instaladas em seu sistema.

Use o seguinte comando para compilar os arquivos:

```shell
gcc -o Quicksort.v1 Quicksort.v1.c -pthread
gcc -o Quicksort.v2 Quicksort.v2.c -pthread
gcc -o Quicksort.v3 Quicksort.v3.c -pthread
```

Em seguida, você pode executar os programas gerados:

```shell
./Quicksort.v1
./Quicksort.v2
./Quicksort.v3
```
