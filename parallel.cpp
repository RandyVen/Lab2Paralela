#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define UMBRAL 1000

void par_qsort(int *data, int lo, int hi) // Funcion del archivo qsort.c
{
    if (lo > hi)
        return;
    int l = lo;
    int h = hi;
    int p = data[(hi + lo) / 2];

    while (l <= h)
    {
        while ((data[l] - p) < 0)
            l++;
        while ((data[h] - p) > 0)
            h--;
        if (l <= h)
        {
            int tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            l++;
            h--;
        }
    }
// recursive call
#pragma omp task shared(data) final(l - lo < UMBRAL)
    par_qsort(data, lo, h);
#pragma omp task shared(data) final(hi - l < UMBRAL)
    par_qsort(data, l, hi);
}

int main(int argc, char *argv[])
{
    double start, end;
    start = omp_get_wtime();

    long limit = pow(10, 2);
    if (argc == 1)
    {
        printf("Corto OpenMP\n");
        exit(1);
    }
    int N = atoi(argv[1]);
    int T = atoi(argv[2]);

    // Generando la lista de numeros
    int *randArray = new int[N];
    // #pragma omp parallel
    //     {
    // #pragma omp single
    //         {
    //             printf("Numero de hilos: %d \n", omp_get_num_threads());
    //         }
    //         srand(time(NULL) + omp_get_thread_num());
    // #pragma omp for
    //         for (int i = 0; i < N; i++)
    //             randArray[i] = rand() % 32769;
    //     }

    for (int i = 0; i < N; i++)
        randArray[i] = rand() % 100000; // Generando numeros random

    // Escribiendo los datos en un archivo
    ofstream myfile("data.txt");
    if (myfile.is_open())
    {
        for (int count = 0; count < N; count++)
        {
            {
                myfile << randArray[count] << ",";
            }
        }
        myfile.close();
    }
    else
    {
        cout << "Error generando el archivo";
    }

    delete[] randArray;
    printf("archivo generado");
    // Leyendo los datos de un archivo
    std::ifstream inFile("data.txt");
    std::vector<int> numbers;
    std::string number_as_string;
    while (std::getline(inFile, number_as_string, ','))
    {
        numbers.push_back(std::stoi(number_as_string));
    }

    std::cout << "n : " << numbers.size() << "\n";
    int n = numbers.size();
    int *a = &numbers[0];

#pragma omp parallel num_threads(T)
    {
#pragma omp single
        {
            par_qsort(a, 0, N - 1);
            // #pragma omp taskwait
        }
    }

    printf("Primeros Elementos: %d, %d, %d\n", a[0], a[1], a[2]);
    printf("Medios Elementos: %d, %d, %d\n", a[N / 4], a[N / 4 + 1], a[N / 4 + 2]);
    printf("Ultimos Elementos: %d, %d, %d\n", a[N - 3], a[N - 2], a[N - 1]);

    // Ahora agregamos los numeros ordenados al archivo
    ofstream myOutputfile("dataSorted.txt");
    if (myOutputfile.is_open())
    {
        // #pragma omp parallel for ordered
        for (int count = 0; count < N; count++)
        {
            // #pragma omp ordered
            myOutputfile << a[count] << ",";
        }
        myOutputfile.close();
    }
    else
        cout << "Error generando el archivo";

    delete[] a;
    end = omp_get_wtime();
    printf("Tiempo de ejecucion: %f\n", end - start);
    return 0;
}