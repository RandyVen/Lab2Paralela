#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
 
using namespace std;

void par_qsort(int *data, int lo, int hi) //Funcion del archivo qsort.c
{
    if(lo > hi) return;
    int l = lo;
    int h = hi;
    int p = data[(hi + lo)/2];

    while(l <= h){
        while((data[l] - p) < 0) l++;
        while((data[h] - p) > 0) h--;
        if(l<=h){
        int tmp = data[l];
        data[l] = data[h];
        data[h] = tmp;
        l++; h--;
        }
    }
    //recursive call
    par_qsort(data, lo, h);
    par_qsort(data, l, hi);
}

int main (int argc, char *argv[]){

    long limit = pow(10,2);
    if (argc == 1)
        {
        printf("Corto OpenMP\n");
        exit (1);
        }
    int N = atoi(argv[1]);
    int T = atoi(argv[2]);

    // Generando la lista de numeros
    int randArray[N];
    for(int i=0;i<N;i++)
        randArray[i]=rand()%100;  //Generando numeros random

    // Escribiendo los datos en un archivo
    ofstream myfile ("data.txt");
    if (myfile.is_open())
    {
        for(int count = 0; count < N; count ++){
        myfile << randArray[count] << "," ;
        }
        myfile.close();
    }
    else cout << "Error generando el archivo";

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
    int* a = &numbers[0];
    
    par_qsort(a, 0, N-1);

    printf("Primeros Elementos: %d, %d, %d\n", a[0], a[1], a[2]);
    printf("Medios Elementos: %d, %d, %d\n", a[N/4], a[N/4 +1], a[N/4 +2]);
    printf("Ultimos Elementos: %d, %d, %d\n", a[N-3], a[N-2], a[N-1]);

    // Ahora agregamos los numeros ordenados al archivo
    ofstream myOutputfile ("dataSorted.txt");
    if (myOutputfile.is_open())
        {
        for(int count = 0; count < N; count ++){
            myOutputfile << a[count] << "," ;
        }
        myOutputfile.close();
        }
    else cout << "Error generando el archivo";
    return 0;
}