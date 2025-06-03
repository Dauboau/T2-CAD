#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <stdbool.h>
#include <math.h>

#define DEBUG 0
#define TIMING 1

#define uint unsigned int
#define maxLines 1000000

// Variável do Timer
double t_task, t_for, t_sequential;

// Números de entrada
uint input[maxLines];

// Quantidade de números primos encontrados
uint output_task, output_for, output_sequential = 0;

/**
 * @brief Verifica se um número inteiro positivo é primo!
 */
bool isPrime(uint n) {
    if(n <= 1){
        return false;
    }
    if(n == 2 || n == 3){
        return true;
    }
    if(n % 2 == 0 || n % 3 == 0){
        return false;
    }
    uint range = (uint)sqrt(n);
    for(uint i = 5; i <= range; i += 6) {
        if(n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

int main(void) {

    printf("Trabalho 2 - CAD\n");
    printf("Threads disponíveis: %d\n", omp_get_num_procs());

    // Leitura da entrada
    uint n = 0;
    while (scanf("%u", &input[n]) == 1 && n < maxLines) {
        n++;
    }

    t_sequential = omp_get_wtime();
    for (uint i = 0; i < maxLines; i++) {
        if (isPrime(input[i])) {
            output_sequential++;
        }
    }
    t_sequential = omp_get_wtime() - t_sequential;

    #pragma omp parallel 
    {

        printf("Thread(%d) criada\n", omp_get_thread_num());

        #pragma omp single
        {
            t_task = omp_get_wtime();
        }

        #pragma omp single
        {

            // grainsize(100)
            // Ajusta a granulação
            // Cada tarefa processa pelo menos 100 iterações, mas menos que o dobro disso.

            // Tarefa de Redução Utilizando Taskloop Reduction
            #pragma omp taskloop reduction(+:output_task)
            for (uint i = 0; i < maxLines; i++) {
                if (isPrime(input[i])) {
                    output_task++;
                }
            }
        }

        #pragma omp single
        {
            t_task = omp_get_wtime() - t_task;
        }

        #pragma omp single
        {
            t_for = omp_get_wtime();
        }

        // Tarefa de Redução Utilizando For Loop Reduction
        #pragma omp for reduction(+:output_for) schedule(auto)
        for (uint i = 0; i < maxLines; i++) {
            if (isPrime(input[i])) {
                output_for++;
            }
        }

        #pragma omp single
        {
            t_for = omp_get_wtime() - t_for;
        }

    }

    printf("Quantidade de primos (Sequential): %d\n", output_sequential);
    printf("Quantidade de primos (Task Reduction): %d\n", output_task);
    printf("Quantidade de primos (For Loop Reduction): %d\n", output_for);

    if(TIMING){
        printf("Tempo Utilizando Sequential: %.3f ms\n", t_sequential * 1000);
        printf("Tempo Utilizando Task Reduction: %.3f ms\n", t_task * 1000);
        printf("Tempo Utilizando For Loop Reduction: %.3f ms\n", t_for * 1000);
    }

    return 0;

}
